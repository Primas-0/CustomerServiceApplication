// CMSC 341 - Spring 2024 - Project 1
#ifndef FASTQUEUE_H
#define FASTQUEUE_H
#include <stdexcept>
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class ListBuffer;//forward declaration
const int DEFAULT_MIN_CAPACITY = 10;
const int MAX_FACTOR = 16;
const int INCREASE_FACTOR = 2;
class ArrayBuffer{
public:
    friend class Grader;
    friend class Tester;
    friend class ListBuffer;
    ArrayBuffer(int capacity);              //constructor
    ~ArrayBuffer();                         //destructor
    ArrayBuffer(const ArrayBuffer & rhs);   //copy constructor
    const ArrayBuffer & operator=(const ArrayBuffer & rhs);// overloaded assignment operator
    void enqueue(int data); // inserts at the end
    int dequeue();          // removes from start
    void clear();           // deallocate memory
    bool empty() const;     // returns true if buffer holds no items
    void dump();            // prints out the contents, for debugging purposes

private:
    int *m_buffer ;         // pointer to dynamically allocated array for buffer
    int m_capacity ;        // length of the allocated space pointed by m_buffer
    int m_count ;           // # of items in the buffer
    int m_start ;           // index of the first (oldest) item in the buffer
    int m_end ;             // index of the last (newest) item in the buffer
    ArrayBuffer* m_next;    // pointer to the next buffer in a linked list

    /******************************************
    * Private function declarations go here! *
    ******************************************/

};
class ListBuffer{
public:
    friend class Grader;
    friend class Tester;
    ListBuffer(int minBufCapacity);
    ~ListBuffer();
    ListBuffer(const ListBuffer & rhs); //copy constructor
    const ListBuffer & operator=(const ListBuffer & rhs);// overloaded assignment operator
    //bool empty() const;	// is list empty?//do we really need this?
    void enqueue(const int & data);// add data
    int dequeue();			// remove data
    void clear();           // clear all data, deallocate all memory
    void dump();            // prints out the contents, for debugging purposes

private:
    ArrayBuffer * m_cursor; //the cursor is the rear of list and its next points to front of list
    int m_listSize;         //this is the size of linked list, i.e. number of nodes in the list
    int m_minBufCapacity;

    /******************************************
    * Private function declarations go here! *
    ******************************************/

};
#endif