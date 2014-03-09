#ifndef _HEAP_CPP
#define _HEAP_CPP

/******************************************
 * Heap Function Definitions
 * Sharang Phadke
 * DSA 2 Fall 2012
 *****************************************/

#include <iostream>
#include <vector>
#include <string>
#include "hash.h"
#include "heap.h"

using namespace std;

// constructor of node will initalize values
heap::node::node()
{
    id = "";
    key = 0;
    pData = NULL;
}

//
// heap - The constructor allocates space for the nodes of the heap
// and the mapping (hash table) based on the specified capacity
//
heap::heap(int capacity)
{
    mapping = new hashTable(capacity*2);
    data.resize(capacity+1);
    heap::capacity = capacity;
    heap::filled = 0;
}

//
// insert - Inserts a new node into the binary heap
//
// Inserts a node with the specified id string, key,
// and optionally a pointer.  They key is used to
// determine the final position of the new node.
//
// Returns:
//   0 on success
//   1 if the heap is already filled to capacity
//   2 if a node with the given id already exists (but the heap
//     is not filled to capacity)
//
int heap::insert(const std::string &id, int key, void *pv /*= NULL*/)
{
    // If insertion conditions are satisfied, increment filled and
    // insert the node into data[filled], then percolateUp(filled)
    // to satisfy the heap order property

    if(filled == capacity)
        return 1;
    else if(mapping->contains(id))
        return 2;

    ++filled;

    data[filled].id = id;
    data[filled].key = key;
    data[filled].pData = pv;

    mapping->insert(id,&data[filled]);
    percolateUp(filled);

    return 0;
}

//
// setKey - set the key of the specified node to the specified value
//
// I have decided that the class should provide this member function
// instead of two separate increaseKey and decreaseKey functions.
//
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
//
int heap::setKey(const std::string &id, int key)
{
    // Check if a node with the given the id exists
    bool exists;
    node * Node = (static_cast<node*> (mapping->getPointer(id,&exists)) );

    if(!exists)
        return 1;

    // percolate up or down based on whether the key is being
    // incremented or decremented
    if(key == Node->key)
        return 0;
    else if(key > Node->key)
    {
        Node->key = key;
        percolateDown(getPos(Node));
    }
    else
    {
        Node->key = key;
        percolateUp(getPos(Node));
    }

    return 0;
}

//
// deleteMin - return the data associated with the smallest key
//             and delete that node from the binary heap
//
// If pId is supplied (i.e., it is not NULL), write to that address
// the id of the node being deleted. If pKey is supplied, write to
// that address the key of the node being deleted. If ppData is
// supplied, write to that address the associated void pointer.
//
// Returns:
//   0 on success
//   1 if the heap is empty
//
int heap::deleteMin(std::string *pId /*= NULL*/, int *pKey /*= NULL*/, void *ppData /*= NULL*/)
{
    if(filled == 0)
        return 1;
    else
    {
        if(pId != NULL)
            *pId = data[1].id;
        if(pKey != NULL)
            *pKey = data[1].key;
        if(ppData != NULL)
            *(static_cast<void **> (ppData)) = data[1].pData;
    }

    // remove the id from the hash table
    mapping->remove(data[1].id);

    // copy the last node into the root and percolate
    data[1] = data[filled];
    --filled;
    //mapping->setPointer(data[1].id,&data[1]);
    percolateDown(1);

    return 0;
}

//
// remove - delete the node with the specified id from the binary heap
//
// If pKey is supplied, write to that address the key of the node
// being deleted. If ppData is supplied, write to that address the
// associated void pointer.
//
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
//
int heap::remove(const std::string &id, int *pKey /*= NULL*/, void *ppData /*= NULL*/)
{
    // find the node with the given id
    bool exists;
    node * Node = (static_cast<node*> (mapping->getPointer(id,&exists)) );
    
    if(!exists)
        return 1;
    else
    {
        if(pKey != NULL)
            *pKey = Node->key;
        if(ppData != NULL)
            *(static_cast<void **> (ppData)) = Node->pData;
    }

    // remove the id from the hashTable
    mapping->remove(id);
    
    // copy the last node into the hole, update the hashTable and percolate
    *Node = data[filled];
    --filled;
    int pos = getPos(Node);

    // redundant check of whether getPos could find the node 
    if(pos <= 0)
        return 1;
    // based on the kew of the new node in the hole, percolate
    // up or down to satisfy the heap order property
    if(pos == 1)
        percolateDown(pos);
    else if(data[pos].key < data[pos/2].key)
        percolateUp(pos);
    else
        percolateDown(pos);

    return 0;
}

// Returns how much of the heap is filled
int heap::getSize()
{
    return filled;
}

//
// Percolate-up algorithm
//
// Move a node up in the tree until the heap order property is
// satisfied. Update the hashTable with each swap
//
void heap::percolateUp(int posCur)
{
    int hole = posCur;
    node upNode = data[hole];
    
    while(hole > 1)
    {
        if(upNode.key < data[hole/2].key) //check the heap order property
        {
            data[hole] = data[hole/2];
            mapping->setPointer(data[hole].id,&data[hole]);
            hole /= 2;
        }
        else //the location for upNode has been found
        {
            data[hole] = upNode;
            mapping->setPointer(data[hole].id,&data[hole]);
            return;
        }
    }

    // upNode belongs at the root
    if(hole == 1)
    {
        data[hole] = upNode;
        mapping->setPointer(data[hole].id,&data[hole]);
    }
}

//
// Percolate-down algorithm
//
// Move a node down in the tree until the heap order property is
// satisfied. Update the hashTable with each swap
//
void heap::percolateDown(int posCur)
{
    int hole = posCur;
    int child;
    node downNode = data[hole];
    while(hole*2 <= filled)
    {
        child = hole*2;

        // choose a child to swap with according to the heap order property
        if( (child != filled) && (data[child].key > data[child+1].key) )
            ++child;

        if(downNode.key > data[child].key)
        {
            // make the swap and continue percolating
            data[hole] = data[child];
            mapping->setPointer(data[hole].id,&data[hole]);
            hole = child;
        }
        else //the location for downNode has been found
        {
            data[hole] = downNode;
            mapping->setPointer(data[hole].id,&data[hole]);
            return;
        }
    }

    //the location for downNode is in the bottom row of the tree
    data[hole] = downNode;
    mapping->setPointer(data[hole].id,&data[hole]);
}

//
// returns position of node with node * pn in data
//
int heap::getPos(node *pn)
{
    // subtract pointers to find pn's position in data
    int pos = pn - &data[0];
    return pos;
}

#endif

