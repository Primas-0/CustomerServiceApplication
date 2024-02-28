// CMSC 341 - Spring 2024 - Project 1
#include "fastq.h"

ArrayBuffer::ArrayBuffer(int capacity){
    //allocate memory for array if capacity >= 1
    if (capacity < 1) {
        m_buffer = nullptr;
        m_capacity = 0;
    } else {
        m_buffer = new int [capacity];
        m_capacity = capacity;
    }
    //initialize all other member variables
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

void ArrayBuffer::clear(){
    //deallocate all memory
    delete[] m_buffer;

    //reinitialize all member variables
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

ArrayBuffer::~ArrayBuffer(){
    clear();
}

bool ArrayBuffer::empty() const {
    //return true if array buffer is empty, otherwise return false
    if (m_buffer == nullptr) {
        return true;
    }
    return false;
}

void ArrayBuffer::enqueue(int data){
    if (m_count == m_capacity) {
        throw overflow_error("Buffer is full");
    }

    //insert data at end of array
    m_buffer[m_end] = data;

    //increment m_end and m_count, moving m_end back to beginning if it goes past capacity
    m_end = (m_end + 1) % m_capacity;
    m_count++;
}

int ArrayBuffer::dequeue(){
    if (m_count == 0) {
        throw underflow_error("Buffer is empty");
    }

    //store data from start of array to return later
    int dequeuedData = m_buffer[m_start];

    //remove data from array
    m_buffer[m_start] = 0;

    //increment m_start and decrement m_count, moving m_start back to beginning if it goes past capacity
    m_start = (m_start + 1) % m_capacity;
    m_count--;

    return dequeuedData;
}

ArrayBuffer::ArrayBuffer(const ArrayBuffer & rhs){
    //construct current object with same dimensions as rhs
    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_buffer = new int[rhs.m_capacity];

    //make current object a deep copy of rhs
    for (int i = 0; i < rhs.m_capacity; i++) {
        m_buffer[i] = rhs.m_buffer[i];
    }
}

const ArrayBuffer & ArrayBuffer::operator=(const ArrayBuffer & rhs){
    //if self-assignment, return current object without any changes
    if (this == &rhs) {
        return *this;
    }

    //otherwise, destroy current object
    clear();

    //construct current object with same dimensions as rhs
    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_buffer = new int[rhs.m_capacity];

    //make current object a deep copy of rhs
    for (int i = 0; i < rhs.m_capacity; i++) {
        m_buffer[i] = rhs.m_buffer[i];
    }

    return *this;
}

void ArrayBuffer::dump(){
    int start = m_start;
    //int end = m_end;
    int counter = 0;
    cout << "ArrayBuffer size: " << m_capacity << " : ";
    if (!empty()){
        while(counter < m_count){
            cout << m_buffer[start] << "[" << start << "]" << " ";
            start = (start + 1) % m_capacity;
            counter++;
        }
        cout << endl;
    }
    else cout << "ArrayBuffer is empty!" << endl;
}

ListBuffer::ListBuffer(int minBufCapacity){
    //create ListBuffer object which contains the first ArrayBuffer object
    if (minBufCapacity < 1) {
        m_cursor = new ArrayBuffer(DEFAULT_MIN_CAPACITY);
    } else {
        m_cursor = new ArrayBuffer(minBufCapacity);
    }

    //update size to reflect new node
    m_listSize = 1;
}

ListBuffer::~ListBuffer(){
    clear();
}

void ListBuffer::clear(){
    ArrayBuffer *curr = m_cursor;

    //loop through the linked list and remove each array buffer, until it circles back to the beginning
    do {
        ArrayBuffer* next = curr->m_next;
        curr->clear();
        curr = next;
    } while (curr != m_cursor);

    //reset all other member variables
    m_cursor = nullptr;
    m_listSize = 0;
}

void ListBuffer::enqueue(const int& data) {
    try {
        m_cursor->enqueue(data);
    } catch (overflow_error e) {
        //TODO: To limit the size of buffer arrays we use the constant global variable MAX_FACTOR. For example, if the
        // minimum size of buffer is 100, the maximum size of a buffer cannot exceed (MAX_FACTOR x 100). As soon as we
        // reach to the maximum size, the next buffer will be created with the minimum size which is defined by the
        // variable member m_minBufCapacity.

        //once the buffer is full, insert a new node (ArrayBuffer object of double the size) into the linked list
        ArrayBuffer *newBuffer = new ArrayBuffer(m_listSize * INCREASE_FACTOR);
        newBuffer->m_next = m_cursor->m_next;
        m_cursor->m_next = newBuffer;

        //advance the cursor and insert data
        m_cursor = newBuffer;
        m_cursor->enqueue(data);
    }
}

int ListBuffer::dequeue() {
    ArrayBuffer *oldestBuffer = m_cursor->m_next;

    try {
        return oldestBuffer->dequeue();
    } catch (underflow_error e) {
        //if there is only one node in the linked list and its buffer is empty, do not remove the node (throw exception)
        if (m_listSize == 1) {
            throw underflow_error("Must have at least one node in the linked list");
        }

        m_cursor->m_next = oldestBuffer->m_next;
        oldestBuffer->clear();
        oldestBuffer = oldestBuffer->m_next;

        return oldestBuffer->dequeue();
    }
}

ListBuffer::ListBuffer(const ListBuffer & rhs){
    //TODO
}

const ListBuffer & ListBuffer::operator=(const ListBuffer & rhs){
    //TODO
}

void ListBuffer::dump(){
    ArrayBuffer* temp = m_cursor->m_next;
    for(int i=0;i<m_listSize;i++){
        temp->dump();
        temp = temp->m_next;
        cout << endl;
    }
    cout << endl;
}