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

#ifndef VERTEX_HH
#define VERTEX_HH

#include <QList>
#include <utils/graph/basegraph.hh>

class Vertex {
public:
    Vertex();

    // Add edge
    void addEdge(Edge *edge);

    // Getters
    int id() const { return _id; }
    int numEdges() const { return _edges.size(); }
    QList<Edge*>* edges() { return &_edges; }
    bool connectedTo(Vertex *vertex) const;

private:
    QList<Edge*> _edges;
    int _id;

    // Global vertex id
    static int _vertexId;
};

#endif // VERTEX_HH
