#ifndef _HEAP_H
#define _HEAP_H

/******************************************
 * Heap Class Declaration
 * Sharang Phadke
 * DSA 2 Fall 2012
 *****************************************/

#include <vector>
#include <string>
#include "hash.h"

using namespace std;

class heap {

 public:

  //
  // heap - The constructor allocates space for the nodes of the heap
  // and the mapping (hash table) based on the specified capacity
  //
  heap(int capacity);

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
  int insert(const std::string &id, int key, void *pv = NULL);

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
  int setKey(const std::string &id, int key);

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
  int deleteMin(std::string *pId = NULL, int *pKey = NULL, void *ppData = NULL);

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
  int remove(const std::string &id, int *pKey = NULL, void *ppData = NULL);

  // Returns how much of the heap is filled
  int getSize();

 private:

  // An inner class within heap
  class node {
    public:
      node(); // The node constructor
      std::string id; // The id of this node
      int key; // The key of this node
      void *pData; // A pointer to the actual data
  };

  int capacity; // The current capacity of the hash table.
  int filled;  // Number of occupied items in the table.

  std::vector<node> data; // The actual binary heap.
  hashTable *mapping; // maps ids to node pointers

  //
  // Percolate-up algorithm
  //
  // Move a node up in the tree until the heap order property is
  // satisfied. Update the hashTable with each swap
  //
  void percolateUp(int posCur);

  //
  // Percolate-down algorithm
  //
  // Move a node down in the tree until the heap order property is
  // satisfied. Update the hashTable with each swap
  //
  void percolateDown(int posCur);

  //
  // returns position of node with node * pn
  //
  int getPos(node *pn);

};

#endif

