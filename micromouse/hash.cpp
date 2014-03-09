#ifndef _HASH_CPP
#define _HASH_CPP

/**************************************************
 * Hash Table Function Definitions
 * Sharang Phadke
 * DSA 2 Fall 2012
 *************************************************/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "hash.h"

using namespace std;

//Vector of precomputed prime numbers
unsigned int Primes[] ={97,193,389,769,1543,3079,6151,12289,24593,49157,98317,196613,393241,786433,1572869,3145739,6291469,12582917,25165843,50331653,100663319,201326611,402653189,805306457,1610612741};

// Constructor of hashItem will initialize all isOccupied to false
hashTable::hashItem::hashItem()
{
    isOccupied = false;
    isDeleted = false;
}

// The constructor initializes the hash table.
// Uses getPrime to choose a prime number at least as large as
// the specified size for the initial size of the hash table.
hashTable::hashTable(int size /*= 0*/)
{
    filled = 0;
    capacity = getPrime(size);

    if(capacity == 0)
    {
        cerr << "Table size too large!";
        exit(EXIT_FAILURE);
    }
    else
        data.resize(capacity);
}

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
// Uses linear probing upon collisions
int hashTable::insert(const string &key, void *pv /*= NULL*/)
{
    //If the table is half filled, rehash
    if(2*filled >= capacity)
        if(!rehash())
            return 2;

    if(contains(key))
        return 1;

    //Hash the key using linear probing upon collision
    int index = hash(key);
    while(data[index].isOccupied && !data[index].isDeleted)
    {
        //Wrap around the end of the vector
        ++index %= capacity;
    }

    //Once an unoccupied or deleted element is found fill it
    data[index].isOccupied = true;
    data[index].isDeleted = false;
    data[index].key = key;
    data[index].pv = pv;
    ++filled;

    return 0;
}

// Check if the specified key is in the hash table.
// If so, return true; otherwise, return false.
bool hashTable::contains(const string &key)
{
    return ( findPos(key) != -1 );
}

// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return NULL.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
void * hashTable::getPointer(const string &key, bool *b /*= NULL*/)
{
    int index = findPos(key);

    if( index == -1 )
    {
        if(b != NULL)
            *b = false;
        return NULL;
    }
    else
    {
        if(b != NULL)
            *b = true;
        return data[index].pv;
    }
}

// Set the pointer associated with the specified key.
// Returns 0 on success,
// 1 if the key does not exist in the hash table.
int hashTable::setPointer(const string &key, void *pv)
{
    int index = findPos(key);

    if( index == -1 )
        return 1;
    else
        data[index].pv = pv;
    
    return 0;
}

// Delete the item with the specified key.
// Returns true on success,
// false if the specified key is not in the hash table.
bool hashTable::remove(const string &key)
{
    int index = findPos(key);

    if( index == -1 )
        return false;
    else
        data[index].isDeleted = true;

    return true;
}

// Return how much of the table is filled
int hashTable::getSize()
{
    return filled;
}


// The hash function.
// Uses simple product and sum
int hashTable::hash(const string &key)
{
    int hashVal = 0;

    for(int i = 0; i < key.length(); ++i)
    { hashVal = 37*hashVal + key[i]; }
    
    hashVal %= capacity;

    if(hashVal < 0)
    { hashVal += capacity; }

    return hashVal;
}

// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const string &key)
{
    // Search for the key assuming linear probing
    // Search until an unoccupied index is found
    // Ensure the hashItem hasnt been lazily deleted
    // If an empty index is reached, the key doesnt exist
    int index = hash(key);
    while(data[index].isOccupied)
    {
        if(data[index].key == key)
        {
            if(data[index].isDeleted)
                return -1;
            else
                return index;
        }

        //Wrap around the end of the vector
        ++index %= capacity;
    }
    
    return -1;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash()
{
    //Make a copy of the old capacity and data
    int oldCapacity = capacity;
    vector<hashItem> oldData = data;

    //Expand the table if it hasnt reached the max size of an int
    capacity = getPrime(2*oldCapacity);
    if(capacity == 0)
    {
        cerr << "Table size too large!";
        exit(EXIT_FAILURE);
    }

    try
    {
        data.clear();
        data.resize(capacity);
    }
    catch(exception &e)
    {
        cerr << "Error: " << e.what();
        return false;
    }
        
    //Rehash each element in oldData into the new data vector
    for(int i = 0; i < oldCapacity; i++)
    {
        if(oldData[i].isOccupied && !oldData[i].isDeleted)
        {
            int index = hash(oldData[i].key);
            while(data[index].isOccupied && !data[index].isDeleted)
            {
                //Wrap around the end of the vector
                ++index %= capacity;
            }
            data[index] = oldData[i];
        }
    }

    return true;
}

// Return a prime number at least as large as size.
// Uses a precomputed sequence of selected prime numbers.
unsigned int hashTable::getPrime(int size)
{
    for(int i = 0; i < (sizeof(Primes) / sizeof(unsigned int)); i++)
    {
        if(Primes[i] >= size)
            return Primes[i];
    }
    return 0;
}

#endif

