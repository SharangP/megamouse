#ifndef _GRAPH_CPP
#define _GRAPH_CPP

/*******************************************
 * Graph Class Function Definitions
 * Sharang Phadke
 * DSA II Fall 2012
 ******************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "hash.h"
#include "heap.h"
#include "graph.h"

using namespace std;


// vertex constructor
// initializes tentative path cost to maximum integer size
// and pointer to previous vertex to NULL
graph::vertex::vertex(){
    pathCost = 1000000000;
    prev = NULL;
}

// constructor
graph::graph(){
    vertexTable = new hashTable;
}

// vertexExists
// checks whether a vertex exists in the heap
// by looking up the id in the vertex hashTable
bool graph::containsVertex(string Id){
    return vertexTable->contains(Id);
}

// insert vertex
// create a new vertex, initiate the id using the
// constructor, and return a pointer to the vertex
void graph::insertVertex(string Id)
{
    vertex V;
    V.id = Id;
    vertexList.push_back(V);
    if(vertexTable->insert(Id,&vertexList.back()) == 2)
        cerr << "ERROR rehashing" << endl;
}

// insert edge
// inserts an edge from v1 to v2, creating the vertecies
// if they dont already exist. Adds the edge to the
// adjacency list of v1
void graph::insertEdge(string v1, string v2, int cost){

    // locate verticies with ids v1 and v2,
    // create them if they dont already exist
    bool exists1, exists2;
    vertex * pv1 = (static_cast<vertex*> (vertexTable->getPointer(v1, &exists1)));
    vertex * pv2 = (static_cast<vertex*> (vertexTable->getPointer(v2, &exists2)));
    if(!exists1)
    {
        insertVertex(v1);
        pv1 = &vertexList.back();
    }
    if(!exists2)
    {
        insertVertex(v2);
        pv2 = &vertexList.back();
    }

    // insert an edge from v1 to v2
    vertex::edge e;
    e.cost = cost;
    e.dest = pv2;
    pv1->adjList.push_back(e);
}

// Dijkstra's Algorithm
// find the shortest path to each vertex from the
// source vertex S using a heap to efficiently find
// the vertex with the shortest temporary pathCost.
void graph::applyDijkstra(string S){
    
    vertex * v;

    // initialize a heap and insert all vertecies into it
    heap H(vertexList.size()+1);
    for(list<vertex>::iterator it = vertexList.begin(); it != vertexList.end(); ++it)
        H.insert(it->id,it->pathCost,&(*it));

    // initialize source
    v = (static_cast<vertex*> (vertexTable->getPointer(S)));
    v->pathCost = 0;
    v->prev = NULL;
    H.setKey(v->id,0);

    // get the vertex with the best pathCost so far from
    // the heap, update the pathCosts of its adjacent
    // verticies, and repeat until the heap is empty
    while(!H.deleteMin(NULL,NULL,&v))
    {
        for(list<vertex::edge>::iterator it = v->adjList.begin(); it != v->adjList.end(); ++it)
        {
            if( (v->pathCost + it->cost) < it->dest->pathCost )
            {
                it->dest->pathCost = v->pathCost + it->cost;
                it->dest->prev = v;
                H.setKey(it->dest->id,it->dest->pathCost);
            }
        }
    }
}

// print paths
// print shortest path to each edge and associated cost
// iterate through the vertexList and print the path to
// each vertex, and the associated cost of the path
void graph::printPaths(ofstream &output){

    for(list<vertex>::iterator it = vertexList.begin(); it != vertexList.end(); ++it)
    {
        vertex * pv;
        list<string> path;

        output << it->id << ": ";

        if(it->prev == NULL)
        {
            if(it->pathCost == 0)
                output << it->pathCost << " [" << it->id << "]" << endl;
            else
                output << "NO PATH" << endl;
        }
        else
        {
            // trace the path to the source using prev pointers
            for(pv = it->prev; pv != NULL; pv = pv->prev)
                path.push_front(pv->id);
            
            output << it->pathCost << " [";

            for(list<string>::iterator pit = path.begin(); pit != path.end(); ++pit)
                output << *pit << ", ";

            output << it->id << "]"<< endl;
        }
    }
}

#endif

