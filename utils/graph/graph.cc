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

#include "graph.hh"
#include <iostream>
#include <utils/graph/vertex.hh>
#include <utils/graph/edge.hh>

#define GRAPH_INFINITY 999

Graph::Graph() {

}

Graph::~Graph() {
    // Delete vertices
    while(_vertices.empty()==false)
        delete _vertices.takeFirst();

    // Delete edges
    while(_edges.empty()==false)
        delete _edges.takeFirst();
}

void Graph::addVertex(Vertex *vertex) {
    // Add vertex to Graph
    if(_vertices.contains(vertex)==false)
        _vertices.push_back(vertex);
}

void Graph::connect(Vertex *v1, Vertex *v2, float weight) {
    // Check if vertices was added to this graph
    if(_vertices.contains(v1)==false || _vertices.contains(v2)==false) {
        std::cout << "[WARNING] Graph::connect(v1, v2, weight), v1 or v2 wasn't added to this graph, ignoring!\n";
        return;
    }

    // Check if already connected
    if(v1->connectedTo(v2))
        return;

    // Create Edge
    Edge *edge = new Edge(v1, v2, weight);

    // Add edges to vertices
    v1->addEdge(edge);
    v2->addEdge(edge);

    // Store edges to delete later
    _edges.push_back(edge);
}

QLinkedList<Vertex *> Graph::getShortestPath(Vertex *source, Vertex *dest) {
    // Check if vertices was added to this graph
    if(_vertices.contains(source)==false || _vertices.contains(dest)==false) {
        std::cout << "[WARNING] Graph::getShortestPath(source, dest), source or dest wasn't added to this graph, ignoring!\n";
        return QLinkedList<Vertex*>();
    }

    // Map from dist to vertex
    QMap<float,Vertex*> vertices;

    // Initialize
    QHash<Vertex*, float> dist;
    QHash<Vertex*, Vertex*> parent;
    for(int i=0; i<_vertices.size(); i++) {
        Vertex *v = _vertices.at(i);

        // Ignore source vertex
        if(v==source)
            continue;

        // Initialize distance and parent
        dist.insert(v, GRAPH_INFINITY);
        parent.insert(v, NULL);

        vertices.insertMulti(GRAPH_INFINITY, v);
    }

    // Initialize source
    dist.insert(source, 0.0);
    parent.insert(source, NULL);
    vertices.insertMulti(0.0, source);

    // Iterate through all vertices to find shortest path
    while(vertices.empty()==false) {

        // Get nearest
        Vertex *u = vertices.first();
        vertices.erase(vertices.begin());

        // Check dest
        if(u==dest)
            break;

        // Iterate on neighbors
        for(int i=0; i<u->edges()->size(); i++) {
            Edge *e = u->edges()->at(i);
            Vertex *v1 = e->getVertex1();
            Vertex *v2 = e->getVertex2();
            Vertex *v = (u==v1? v2 : v1);

            // Check if 'v' still on map
            if(vertices.values().contains(v)==false)
                continue;

            // Calc distance
            float alt = dist.value(u) + e->getWeight();

            // Update if lower
            if(alt < dist.value(v)) {
                dist.insert(v, alt);
                parent.insert(v, u);

                // Update vertices (new distances)
                QMap<float,Vertex*>::iterator it;
                for(it = vertices.begin(); it!=vertices.end(); it++) {
                    if(it.value()==v) {
                        vertices.erase(it);
                        vertices.insertMulti(alt, v);
                        break;
                    }
                }
            }

        }

    }

    // Get shortest path
    QLinkedList<Vertex*> path;
    Vertex* current = dest;
    while(parent.value(current)!=NULL) {
        path.push_front(current);
        current = parent.value(current);
    }
    path.push_front(source);

    return path;
}
