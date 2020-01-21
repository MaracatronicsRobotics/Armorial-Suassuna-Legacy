#include "coachutils.h"
#define KNN_ALL 100

CoachUtils::CoachUtils(MRCTeam *team) {
    _team = team;
}

QList<kNN::element> CoachUtils::getKNN(int k, const Position &origin) {
    // kNN algorithm
    return kNN::getKNN(k, origin);
}

QList<kNN::element> CoachUtils::getTeammateKNN(int k, const Position &origin) {
    // kNN algorithm
    QList<kNN::element> knn = kNN::getKNN(KNN_ALL, origin);
    // Exclude opponent team
    for(int i=0; i<knn.size(); i++) {
        kNN::element element = knn.at(i);
        if(element.team==_team->opTeam()->teamId()) {
            knn.removeAt(i);
            i--;
        }
    }
    // Get 'k'
    QList<kNN::element> retn;
    if(k>=knn.size())
        retn = knn;
    else {
        for(int i=0; i<k; i++)
            retn.push_back(knn.at(i));
    }
    // Return list
    return retn;
}

QList<kNN::element> CoachUtils::getOpponentKNN(int k, const Position &origin) {
    // kNN algorithm
    QList<kNN::element> knn = kNN::getKNN(KNN_ALL, origin);
    // Exclude our team
    for(int i=0; i<knn.size(); i++) {
        kNN::element element = knn.at(i);
        if(element.team==_team->teamId()) {
            knn.removeAt(i);
            i--;
        }
    }
    // Get 'k'
    QList<kNN::element> retn;
    if(k>=knn.size())
        retn = knn;
    else {
        for(int i=0; i<k; i++)
            retn.push_back(knn.at(i));
    }
    // Return list
    return retn;
}

QList<kNN::element> CoachUtils::getAllKNN(const Position &origin) {
    // kNN algorithm
    return kNN::getKNN(KNN_ALL, origin);
}
