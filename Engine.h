#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>   
#include <vector>     
#include "BST.h"      
#include "Record.h"
//add header files as needed

using namespace std;

// Converts a string to lowercase (used for case-insensitive searches)
static inline string toLower(string s) {
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

// ================== Index Engine ==================
// Acts like a small "database engine" that manages records and two BST indexes:
// 1) idIndex: maps student_id → record index (unique key)
// 2) lastIndex: maps lowercase(last_name) → list of record indices (non-unique key)
struct Engine 
{
    vector<Record> heap;                  // the main data store (simulates a heap file)
    BST<int, int> idIndex;                // index by student ID
    BST<string, vector<int>> lastIndex;   // index by last name (can have duplicates)

    // Inserts a new record and updates both indexes.
    // Returns the record ID (RID) in the heap.
    int insertRecord(const Record &recIn) 
    {
        // Appends the record
        heap.push_back(recIn);
        int recordID = heap.size() - 1;

        // Updates idIndex and lastIndex
        idIndex.insert(recIn.id, recordID);

        string key = toLower(recIn.last);
        vector<int>* position = lastIndex.find(key);

        if (position)
        {
            position->push_back(recordID);
        }
        else
        {
            vector<int> temp;
            temp.push_back(recordID);
            lastIndex.insert(key, temp);
        }
        return recordID;
    }

    // Deletes a record logically (marks as deleted and updates indexes)
    // Returns true if deletion succeeded.
    bool deleteById(int id) 
    {
        // Finds id index
        int* recIDptr = idIndex.find(id);
        if (!recIDptr) 
        {
            return false;
        }
        int recordID = *recIDptr;

        // soft deletion
        heap[recordID].deleted = true;

        // removes key
        idIndex.erase(id);

        // Removes recordID from last name vector
        string key = toLower(heap[recordID].last);
        vector<int>* vect = = lastIndex.find(key);

        if (vect) 
        {
            vect->erase(remove(vect->begin(), vect->end(), recordID), vect->end());

            // removes key from lastIndex
            if (vect->empty()) 
            {
                lastIndex.erase();
            }
        }
        return true;
    }

    // Finds a record by student ID.
    // Returns a pointer to the record, or nullptr if not found.
    // Outputs the number of comparisons made in the search.
    const Record *findById(int id, int &cmpOut) 
    {
        // resets comparisons to 0
        idIndex.resetMetrics();

        // searches id index
        int* recIDptr = idIndex.find(id);  

        // num of comparisons
        cmpOut = idIndex.comparisons;

        // if not found
        if(!recIDptr)
        {
            return nullptr;
        }

        int recordID = *recIDptr;

        // soft deletion
        if (heap[recordID].deleted) 
        {
            return nullptr;
        }

        return &heap[recordID];
    }

    // Returns all records with ID in the range [lo, hi].
    // Also reports the number of key comparisons performed.
    vector<const Record *> rangeById(int lo, int hi, int &cmpOut) 
    {
        //TODO
    }

    // Returns all records whose last name begins with a given prefix.
    // Case-insensitive using lowercase comparison.
    vector<const Record *> prefixByLast(const string &prefix, int &cmpOut) 
    {
        //TODO
    }
};

#endif
