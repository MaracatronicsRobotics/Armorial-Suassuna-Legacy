#include "playbook.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

Playbook::Playbook() {
    _initialized = false;
    _lastNumPlayers = 0;
    _configureEnabled = false;
    _dist = NULL;
}

Playbook::~Playbook() {
    setCurrBehavioursToOld();
    clearOldBehaviours();
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
        setCurrBehavioursToOld();
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
    // This will update assignment table with current desired player-behaviour pairs
    run(_lastNumPlayers);
    // Ensure players on this playbook has behaviours assigned to them
    updatePlayersBehaviours();
}

void Playbook::updatePlayersBehaviours() {
    QHash<quint8,Behaviour*>::const_iterator it;
    for(it=_assignmentTable.constBegin(); it!=_assignmentTable.constEnd(); it++) {
        quint8 id = it.key();
        Behaviour *behaviour = it.value();
        // Check invalid id
        if(id==DIST_INVALID_ID)
            continue;
        // Get list of available players
        QHash<quint8,Player*> avPlayers = _ourTeam->avPlayers();
        Player *player = avPlayers.value(id);
        // Effectivelly set behaviour to player
        player->setBehaviour(behaviour);
        // Warning to inform that some player isn't being used on playbook
        if(behaviour==NULL)
            std::cout << "[WARNING] " << name().toStdString() << ", player #" << (int)id << " has no Behaviour assigned!\n";
    }
}

void Playbook::setCurrBehavioursToOld() {
    while(_behavioursList.empty()==false)
        _oldBehaviours.push_back(_behavioursList.takeFirst());
}

void Playbook::clearOldBehaviours() {
    while(_oldBehaviours.empty()==false)
        delete _oldBehaviours.takeFirst();
}

void Playbook::usesBehaviour(Behaviour *behaviour) {
    // Check if call is inside configure()
    if(_configureEnabled==false) {
        std::cout << "[WARNING] Blocked playbook '" << name().toStdString() << "' setting Behaviour to use outside configure().\n";
        return;
    }
    // Check null pointer
    if(behaviour==NULL) {
        std::cout << "[ERROR] " << name().toStdString() << "::usesBehaviour(NULL), &behaviour==NULL.\n";
        return;
    }
    // Add behaviour
    if(_behavioursList.contains(behaviour)==false)
        _behavioursList.push_back(behaviour);
}

void Playbook::setPlayerBehaviour(quint8 id, Behaviour *behaviour) {
    if(id==200)
        return;
    // Check if player is on this playbook
    if(_players.contains(id)==false) {
        std::cout << "[ERROR] " << name().toStdString() << "::setPlayerBehaviour(" << (int)id << ", " << behaviour->name().toStdString() << "), player #" << (int)id << " wasn't added to this playbook!\n";
        return;
    }
    // Check pointer
    if(behaviour==NULL) {
        std::cout << "[ERROR] " << name().toStdString() << "::setPlayerBehaviour(" << (int)id << ", NULL), &behaviour==NULL!\n";
        return;
    }
    // Check if behaviour is on this playbook
    if(_behavioursList.contains(behaviour)==false) {
        std::cout << "[ERROR] " << name().toStdString() << "::setPlayerBehaviour(" << (int)id << ", " << behaviour->name().toStdString() << "), behaviour " << behaviour->name().toStdString() << " wasn't added to this playbook!\n";
        return;
    }
    // Check if old player still has the behaviour
    if(_assignmentTable.values().contains(behaviour)) {
        quint8 oldID = _assignmentTable.key(behaviour);
        if(_players.contains(oldID))
            _assignmentTable.insert(oldID, NULL);
    }
    // Assign behaviour
    _assignmentTable.insert(id, behaviour);
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
