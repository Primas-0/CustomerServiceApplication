// CMSC 341 - Spring 2024 - Project 1
#include "fastq.h"

ArrayBuffer::ArrayBuffer(int capacity) {
    //allocate memory for array if capacity >= 1
    if (capacity < 1) {
        m_buffer = nullptr;
        m_capacity = 0;
    } else {
        m_buffer = new int[capacity];
        m_capacity = capacity;
    }
    //initialize all other member variables
    m_count = 0;
    m_start = 0;
    m_end = 0;
    m_next = nullptr;
}

void ArrayBuffer::clear() {
    //deallocate all memory
    delete[] m_buffer;

    //reinitialize all member variables
    m_buffer = nullptr;
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
    m_next = nullptr;
}

ArrayBuffer::~ArrayBuffer() {
    clear();
}

bool ArrayBuffer::empty() const {
    //return true if array buffer is empty, otherwise return false
    if (m_count == 0) {
        return true;
    }
    return false;
}

void ArrayBuffer::enqueue(int data) {
    if (m_count == m_capacity) {
        throw overflow_error("Buffer is full");
    }

    //insert data at end of array
    m_buffer[m_end] = data;

    //increment m_end and m_count, moving m_end back to beginning if it goes past capacity
    m_end = (m_end + 1) % m_capacity;
    m_count++;
}

int ArrayBuffer::dequeue() {
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

ArrayBuffer::ArrayBuffer(const ArrayBuffer &rhs) {
    //construct current object with same dimensions as rhs
    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_next = nullptr;

    if (rhs.m_buffer == nullptr) {
        m_buffer = nullptr;
    } else {
        m_buffer = new int[rhs.m_capacity];
    }

    //make current object a deep copy of rhs
    for (int i = 0; i < rhs.m_capacity; i++) {
        m_buffer[i] = rhs.m_buffer[i];
    }
}

const ArrayBuffer &ArrayBuffer::operator=(const ArrayBuffer &rhs) {
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
    m_next = nullptr;

    if (rhs.m_buffer == nullptr) {
        m_buffer = nullptr;
    } else {
        m_buffer = new int[rhs.m_capacity];
    }

    //make current object a deep copy of rhs
    for (int i = 0; i < rhs.m_capacity; i++) {
        m_buffer[i] = rhs.m_buffer[i];
    }

    return *this;
}

void ArrayBuffer::dump() {
    int start = m_start;
    //int end = m_end;
    int counter = 0;
    cout << "ArrayBuffer size: " << m_capacity << " : ";
    if (!empty()) {
        while (counter < m_count) {
            cout << m_buffer[start] << "[" << start << "]" << " ";
            start = (start + 1) % m_capacity;
            counter++;
        }
        cout << endl;
    } else cout << "ArrayBuffer is empty!" << endl;
}

ListBuffer::ListBuffer(int minBufCapacity) {
    //create ListBuffer object which contains the first ArrayBuffer object
    if (minBufCapacity < 1) {
        m_cursor = new ArrayBuffer(DEFAULT_MIN_CAPACITY);
        m_minBufCapacity = DEFAULT_MIN_CAPACITY;
    } else {
        m_cursor = new ArrayBuffer(minBufCapacity);
        m_minBufCapacity = minBufCapacity;
    }

    //make sure linked list is always circular
    m_cursor->m_next = m_cursor;

    //update size to reflect new node
    m_listSize = 1;
}

ListBuffer::~ListBuffer() {
    clear();
}

void ListBuffer::clear() {
    ArrayBuffer *curr = m_cursor;

    if (curr == nullptr) {
        return;
    }

    //loop through the linked list and remove each array buffer, until it circles back to the beginning
    do {
        ArrayBuffer *next = curr->m_next;
        curr->clear();
        curr = next;
    } while (curr != m_cursor);

    //reset all other member variables
    m_cursor = nullptr;
    m_listSize = 0;
}

void ListBuffer::enqueue(const int &data) {
    try {
        m_cursor->enqueue(data);
    } catch (overflow_error &e) {
        ArrayBuffer *newBuffer = nullptr;

        //once the buffer is full, insert a new node (ArrayBuffer object) into the linked list
        if (m_cursor->m_capacity < MAX_FACTOR * m_minBufCapacity) {
            //new buffer is double the size if max size has not been reached
            newBuffer = new ArrayBuffer(m_cursor->m_capacity * INCREASE_FACTOR);
        } else {
            //otherwise, new buffer is created with the minimum size
            newBuffer = new ArrayBuffer(m_minBufCapacity);
        }
        m_listSize++;

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
    } catch (underflow_error &e) {
        //if there is only one node in the linked list and its buffer is empty, do not remove the node (throw exception)
        if (m_listSize == 1) {
            throw underflow_error("Must have at least one node in the linked list");
        }

        //remove the node containing the empty buffer from the linked list
        m_cursor->m_next = oldestBuffer->m_next;
        delete oldestBuffer;
        m_listSize--;

        //remove data from the next node
        oldestBuffer = m_cursor->m_next;
        return oldestBuffer->dequeue();
    }
}

ListBuffer::ListBuffer(const ListBuffer &rhs) {
    //mirror member variables
    m_cursor = nullptr;
    m_listSize = rhs.m_listSize;
    m_minBufCapacity = rhs.m_minBufCapacity;

    ArrayBuffer *rhsCurr = rhs.m_cursor;
    ArrayBuffer *prevBuffer = nullptr;

    do {
        //create a new node copy using the ArrayBuffer copy constructor
        ArrayBuffer *newBuffer = new ArrayBuffer(*rhsCurr);

        //first node copied is stored as m_cursor, rest are linked to their immediate predecessor
        if (m_cursor == nullptr) {
            m_cursor = newBuffer;
        } else {
            prevBuffer->m_next = newBuffer;
        }

        //advance previous node pointer
        prevBuffer = newBuffer;

        //advance target node in rhs
        rhsCurr = rhsCurr->m_next;
    } while (rhsCurr != rhs.m_cursor);

    //sets next of current node (final node copied) to the oldest node (first node copied), completing the circle
    prevBuffer->m_next = m_cursor;
}

const ListBuffer &ListBuffer::operator=(const ListBuffer &rhs) {
    //if self-assignment, return current object without any changes
    if (this == &rhs) {
        return *this;
    }

    //otherwise, destroy current object
    clear();

    //create a deep copy of rhs (identical to copy constructor)
    m_listSize = rhs.m_listSize;
    m_minBufCapacity = rhs.m_minBufCapacity;
    ArrayBuffer *rhsCurr = rhs.m_cursor;
    ArrayBuffer *prevBuffer = nullptr;
    do {
        ArrayBuffer *newBuffer = new ArrayBuffer(*rhsCurr);
        if (m_cursor == nullptr) {
            m_cursor = newBuffer;
        } else {
            prevBuffer->m_next = newBuffer;
        }
        prevBuffer = newBuffer;
        rhsCurr = rhsCurr->m_next;
    } while (rhsCurr != rhs.m_cursor);
    prevBuffer->m_next = m_cursor;

    return *this;
}

void ListBuffer::dump() {
    ArrayBuffer *temp = m_cursor->m_next;
    for (int i = 0; i < m_listSize; i++) {
        temp->dump();
        temp = temp->m_next;
        cout << endl;
    }
    cout << endl;
}