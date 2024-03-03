#include "fastq.h"

class Tester {
public:
    bool testArrayBufferConstructorError(int capacity);
    bool testArrayBufferConstructorEdge(int capacity);
    bool testArrayBufferConstructorNormal(int capacity);

    bool testArrayBufferClear();
};

bool Tester::testArrayBufferConstructorError(int capacity) {
    //call constructor with invalid parameter
    ArrayBuffer array(capacity);

    if (array.m_buffer == nullptr && array.m_capacity == 0 && array.m_count == 0 && array.m_start == 0 &&
        array.m_end == 0 && array.m_next == nullptr) {
        //test passes if an empty object has been created
        return true;
    }
    return false;
}

bool Tester::testArrayBufferConstructorEdge(int capacity) {
    ArrayBuffer array(capacity);

    if (array.m_buffer == nullptr && array.m_capacity == 0 && array.m_count == 0 && array.m_start == 0 &&
        array.m_end == 0 && array.m_next == nullptr) {
        //test passes if an empty object has been created
        return true;
    }
    return false;
}

int main() {
    Tester tester;

    //constructor tests
    cout << "Testing Constructor (error case) - ____:" << endl;
    if (tester.testArrayBufferConstructorError(-9)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\tConstructor failed!" << endl;
    }
}