// CMSC 341 - Spring 2024 - Project 1
#include "fastq.h"

ArrayBuffer::ArrayBuffer(int capacity){
    if (capacity < 1) {
        m_buffer = nullptr;
        m_capacity = 0;
    } else {
        m_buffer = new int [capacity];
        m_capacity = capacity;
    }
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

    m_buffer[m_end] = data;
    m_end = (m_end + 1) % m_capacity;
    m_count++;
}

int ArrayBuffer::dequeue(){
    if (m_count == 0) {
        throw underflow_error("Buffer is empty");
    }

    int dequeuedData = m_buffer[m_start];

    m_buffer[m_start] = 0;
    m_start = (m_start + 1) % m_capacity;
    m_count--;

    return dequeuedData;
}

ArrayBuffer::ArrayBuffer(const ArrayBuffer & rhs){
    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;

    m_buffer = new int[rhs.m_capacity];

    for (int i = 0; i < rhs.m_capacity; i++) {
        m_buffer[i] = rhs.m_buffer[i];
    }
}

const ArrayBuffer & ArrayBuffer::operator=(const ArrayBuffer & rhs){
    if (this == &rhs) {
        return *this;
    }

    clear();

    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_buffer = new int[rhs.m_capacity];

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

}

ListBuffer::~ListBuffer(){

}
void ListBuffer::clear(){

}

void ListBuffer::enqueue(const int& data) {

}

int ListBuffer::dequeue() {

}

ListBuffer::ListBuffer(const ListBuffer & rhs){

}

const ListBuffer & ListBuffer::operator=(const ListBuffer & rhs){

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