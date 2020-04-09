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

#include "knn.hh"
#include <iostream>
#include <GEARSystem/worldmap.hh>
#include <utils/utils.hh>
#include <entity/contromodule/mrcteam.h>
#include <entity/player/player.h>

MRCTeam* kNN::_ourTeam = NULL;
MRCTeam* kNN::_theirTeam = NULL;

QList<kNN::element> kNN::getKNN(int k, const Position &origin) {
    const QHash<quint8,Player*> ourPlayers = _ourTeam->avPlayers();
    const QHash<quint8,Player*> opPlayers = _theirTeam->avPlayers();

    const int numPlayersTeam0 = ourPlayers.size();
    const int numPlayersTeam1 = opPlayers.size();
    const int numPlayers = numPlayersTeam0 + numPlayersTeam1;

    element *elements = NULL;

    // Reset elements array
    if(elements!=NULL)
        delete[] elements;
    elements = new element[numPlayers];
    for(int i=0; i<numPlayers; i++) {
        elements[i].id = elements[i].team = -1;
        elements[i].value = 1000.0f;
    }

    // Fill array
    QList<quint8> ids;

    int c=0;
    ids = ourPlayers.keys();
    for(int i=0; i<ids.size(); i++) {
        const quint8 id = ids.at(i);
        const Position pos = ourPlayers.value(id)->position();
        elements[c].id = id;
        elements[c].team = _ourTeam->teamId();
        elements[c].value = WR::Utils::distance(pos, origin);
        c++;
    }

    ids = opPlayers.keys();
    for(int i=0; i<ids.size(); i++) {
        const quint8 id = ids.at(i);
        const Position pos = opPlayers.value(id)->position();
        elements[c].id = id;
        elements[c].team = _theirTeam->teamId();
        elements[c].value = WR::Utils::distance(pos, origin);
        c++;
    }

    // Check empty
    if(c==0)
        return QList<element>();

    // Apply qsort
    qsort(elements, c, sizeof(element), kNN_compare);

    // Return 'k'
    QList<element> retn;
    for(int i=0; i<((k>c)?c:k); i++)
        retn.push_back(elements[i]);

    return retn;
}

int kNN::kNN_compare(const void *a, const void *b) {
    element *A = (element*)a;
    element *B = (element*)b;
    float result = A->value - B->value;
    if(result>0)
        return 1;
    if(result<0)
        return -1;
    return 0;
}
