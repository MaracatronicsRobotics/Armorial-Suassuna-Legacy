#include "playbook.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

Playbook::Playbook() {
    _initialized = false;
    _lastNumPlayers = 0;
    _configureEnabled = false;
    _dist = NULL;
}

Playbook::~Playbook() {
    setCurrRolesToOld();
    clearOldRoles();
    if(_dist!=NULL)
        delete _dist;
}

Locations* Playbook::loc() const {
    return _ourTeam->loc();
}

void Playbook::initialize(MRCTeam *ourTeam, MRCTeam *opTeam, CoachUtils *utils, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref) {
    _ourTeam = ourTeam;
    _opTeam = opTeam;
    _utils = utils;
    _ref = ref;
    _dist = new PlayersDistribution(_ourTeam, kickerId, lastState, ref);
    _initialized = true;
}

void Playbook::runPlaybook(QString strategyState) {
    // Check if StrategyState set players to this playbook
    if(_players.size()==0) {
        //std::cout << "[WARNING] " << strategyState.toStdString() << "->" << name().toStdString() << " has no players set!\n";
        return;
    }
    // Run configure(numPlayers) if num of players in this playbook changed
    if(_lastNumPlayers != _players.size()) {
        _lastNumPlayers = _players.size();
        setCurrRolesToOld();
        _configureEnabled = true;
        configure(_lastNumPlayers);
        _configureEnabled = false;
    }
    // Ensure assignment table has only assignments of players that are in this playbook
    QList<quint8> ids = _assignmentTable.keys();
    for(int i=0; i<ids.size(); i++) {
        quint8 id = ids.at(i);
        if(_players.contains(id)==false)
            _assignmentTable.remove(id);
    }
    for(int i=0; i<_players.size(); i++) {
        quint8 id = _players.at(i);
        if(_assignmentTable.contains(id)==false)
            _assignmentTable.insert(id, NULL);
    }
    // Check excessive players on playbook, and print warning
    if(_players.size() > maxNumPlayer()) {
        std::cout << "[WARNING] " << strategyState.toStdString() << "->" << name().toStdString() << " has excessive players (num=" << _players.size() << ", max=" << maxNumPlayer() << ").\n";
    }
    // Run playbook (implemented by child)
    // This will update assignment table with current desired player-role pairs
    run(_lastNumPlayers);
    // Ensure players on this playbook has roles assigned to them
    updatePlayersRoles();
}

void Playbook::updatePlayersRoles() {
    QHash<quint8,Role*>::const_iterator it;
    for(it=_assignmentTable.constBegin(); it!=_assignmentTable.constEnd(); it++) {
        quint8 id = it.key();
        Role *role = it.value();
        // Check invalid id
        if(id==DIST_INVALID_ID)
            continue;
        // Get list of available players
        QHash<quint8,Player*> avPlayers = _ourTeam->avPlayers();
        Player *player = avPlayers.value(id);
        // Effectivelly set role to player
        player->setRole(role);
        // Warning to inform that some player isn't being used on playbook
        if(role==NULL)
            std::cout << "[WARNING] " << name().toStdString() << ", player #" << (int)id << " has no Role assigned!\n";
    }
}

void Playbook::setCurrRolesToOld() {
    while(_rolesList.empty()==false)
        _oldRoles.push_back(_rolesList.takeFirst());
}

void Playbook::clearOldRoles() {
    while(_oldRoles.empty()==false)
        delete _oldRoles.takeFirst();
}

void Playbook::usesRole(Role *role) {
    // Check if call is inside configure()
    if(_configureEnabled==false) {
        std::cout << "[WARNING] Blocked playbook '" << name().toStdString() << "' setting Role to use outside configure().\n";
        return;
    }
    // Check null pointer
    if(role==NULL) {
        std::cout << "[ERROR] " << name().toStdString() << "::usesRole(NULL), &role==NULL.\n";
        return;
    }
    // Add role
    if(_rolesList.contains(role)==false)
        _rolesList.push_back(role);
}

void Playbook::setPlayerRole(quint8 id, Role *role) {
    if(id==200)
        return;
    // Check if player is on this playbook
    if(_players.contains(id)==false) {
        std::cout << "[ERROR] " << name().toStdString() << "::setPlayerRole(" << (int)id << ", " << role->name().toStdString() << "), player #" << (int)id << " wasn't added to this playbook!\n";
        return;
    }
    // Check pointer
    if(role==NULL) {
        std::cout << "[ERROR] " << name().toStdString() << "::setPlayerRole(" << (int)id << ", NULL), &role==NULL!\n";
        return;
    }
    // Check if role is on this playbook
    if(_rolesList.contains(role)==false) {
        std::cout << "[ERROR] " << name().toStdString() << "::setPlayerRole(" << (int)id << ", " << role->name().toStdString() << "), role " << role->name().toStdString() << " wasn't added to this playbook!\n";
        return;
    }
    // Check if old player still has the role
    if(_assignmentTable.values().contains(role)) {
        quint8 oldID = _assignmentTable.key(role);
        if(_players.contains(oldID))
            _assignmentTable.insert(oldID, NULL);
    }
    // Assign role
    _assignmentTable.insert(id, role);
}

bool Playbook::hasPlayer(quint8 id) {
    // Check if player is added to playbook
    return _players.contains(id);
}

void Playbook::addPlayer(quint8 id) {
    if(id==200)
        return;
    // Check if player id is on available players hash
    QHash<quint8,Player*> avPlayers = _ourTeam->avPlayers();
    if(avPlayers.contains(id)==false) {
        std::cout << "[ERROR] " << name().toStdString() << "::addPlayer(" << (int)id << "), player #" << (int)id << " doesn't exist!\n";
        return;
    }
    // Add player id to this playbook and to distribution
    if(_players.contains(id)==false) {
        _players.push_back(id);
        _dist->insert(id);
    }
}

void Playbook::addPlayers(const QList<quint8> &ids) {
    if(ids.empty())
        return;
    // Add list of players
    QList<quint8>::const_iterator it;
    for(it=ids.constBegin(); it!=ids.constEnd(); it++) {
        const quint8 id = *it;
        addPlayer(id);
    }
}

void Playbook::clearPlayers() {
    _players.clear();
    _dist->clear();
}

int Playbook::numPlayers() {
    return _players.size();
}

bool Playbook::canKickBall() const {
    return _ref->getGameInfo(_ourTeam->teamColor())->canKickBall();
}
