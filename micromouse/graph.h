#ifndef _GRAPH_H
#define _GRAPH_H

/*******************************************
 * Graph Class Declaration
 * Sharang Phadke
 * DSA II Fall 2012
 ******************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "hash.h"
#include "heap.h"

using namespace std;

// graph class
// 
// Describes a graph using a heap and an adjacency list
// Contains private class vertex and nested class edge
//
class graph{

public:

    // constructor
    graph();

    // vertexExists
    // checks whether a vertex exists in the heap
    // by looking up the id in the vertex hashTable
    bool containsVertex(string n);

    // insert vertex
    // create a new vertex, initiate the id using the
    // constructor, and return a pointer to the vertex
    void insertVertex(string Id);

    int length();

    // insert edge
    // inserts an edge from v1 to v2, creating the vertecies
    // if they dont already exist. Adds the edge to the
    // adjacency list of v1
    void insertEdge(string v1, string v2, int cost);

    // Dijkstra's Algorithm
    // find the shortest path to each vertex from the
    // source vertex S using a heap to efficiently find
    // the vertex with the shortest temporary pathCost.
    void applyDijkstra(string S);

    // print paths
    // print shortest path to each edge and associated cost
    // iterate through the vertexList and print the path to
    // each vertex, and the associated cost of the path
    void printPaths(ofstream &output);

private:
    
    // vertex class
    // Each vertex contains a string id, an integer pathCost,
    // the cost of the lowest cost path from the source, and
    // a pointer to the previous vertex in the path
    class vertex{
    public:

        // edge class
        // edges will be members of the adjacency lists of each
        // vertex. They each contain an edge cost and a pointer
        // to the destination vertex
        class edge{
        public:
            int cost;
            vertex * dest;
        };

        vertex();   //constructor
        string id;
        list<edge> adjList;
        int pathCost;
        vertex * prev;
    };

    // store all vertecies in a list
    list<vertex> vertexList;

    // use a hash table to find vertecies quickly by id
    hashTable * vertexTable;

};

#endif

