/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef KNN_HH
#define KNN_HH

#include <QList>
#include <GEARSystem/Types/types.hh>
#include <entity/contromodule/basecoach.h>

class kNN {
private:
    // Compare function (for qsort())
    static int kNN_compare(const void *a, const void *b);

    // Teams access
    static MRCTeam *_ourTeam;
    static MRCTeam *_theirTeam;
public:
    // Initialization
    static void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam) { kNN::_ourTeam = ourTeam; kNN::_theirTeam = theirTeam; }

    // Element definition
    typedef struct {
        qint8 id;
        qint8 team;
        float value;
    } element;

    // Get kNN players to origin
    static QList<element> getKNN(int k, const Position &origin);
};

#endif // KNN_HH
