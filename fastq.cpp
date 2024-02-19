// CMSC 341 - Spring 2024 - Project 1
#include "fastq.h"

ArrayBuffer::ArrayBuffer(int capacity){

}

void ArrayBuffer::clear(){

}

ArrayBuffer::~ArrayBuffer(){

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