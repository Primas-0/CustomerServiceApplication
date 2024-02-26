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
    m_next = nullptr;
}

void ArrayBuffer::clear(){
    //reinitialize all member variables
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
    m_next = nullptr;

    //deallocate all memory
    delete[] m_buffer;
}

ArrayBuffer::~ArrayBuffer(){
    delete[] m_buffer;
}

bool ArrayBuffer::empty() const {
    //return true if array buffer is empty, otherwise return false
    if (m_buffer == nullptr) {
        return true;
    }
    return false;
}

void ArrayBuffer::enqueue(int data){
    //This function inserts the data into the array buffer at the proper index. The proper index is the end of the list
    // indicated by the member variable m_end. After every insertion, the function updates the appropriate member
    // variables in the object such as m_end and m_count. If the array is already full, the function throws the
    // exception overflow_error. The exception is defined in the include library <stdexcept>.

    if (m_count == m_capacity) {
        throw overflow_error("Array is full");
    }

    m_buffer[m_end] = data;
    m_end++;
    m_count++;
}

int ArrayBuffer::dequeue(){

}

ArrayBuffer::ArrayBuffer(const ArrayBuffer & rhs){

}

const ArrayBuffer & ArrayBuffer::operator=(const ArrayBuffer & rhs){

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