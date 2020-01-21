#ifndef COACHUTILS_H
#define COACHUTILS_H

#include <utils/knn/knn.hh>
#include <entity/contromodule/mrcteam.h>

class CoachUtils {
private:
    MRCTeam *_team;
public:
    CoachUtils(MRCTeam *team);
    // kNN
    QList<kNN::element> getKNN(int k, const Position &origin);
    QList<kNN::element> getAllKNN(const Position &origin);
    QList<kNN::element> getTeammateKNN(int k, const Position &origin);
    QList<kNN::element> getOpponentKNN(int k, const Position &origin);
};

#endif // COACHUTILS_H
