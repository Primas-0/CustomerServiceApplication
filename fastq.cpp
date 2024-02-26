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
    m_end = 0; //the one after the end
    m_next = nullptr;
}

void ArrayBuffer::clear(){
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
    m_next = nullptr;

    delete[] m_buffer;
}

ArrayBuffer::~ArrayBuffer(){
    delete[] m_buffer;
}

bool ArrayBuffer::empty() const {

}

void ArrayBuffer::enqueue(int data){

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