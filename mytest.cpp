#include "fastq.h"
#include <random>
#include <vector>

const int MINDATA = 1000;
const int MAXDATA = 9999;

class Random {
public:
    Random(int min, int max) {
        //UNIFORMINT-type seed and distribution
        m_randGenerator = std::mt19937(10); //fixed seed value of 10
        m_dist = std::uniform_int_distribution<>(min, max);
    }

    int getRandInt() {
        //generate a random integer between the min and max values
        return m_dist(m_randGenerator);
    }
private:
    std::mt19937 m_randGenerator;
    std::uniform_int_distribution<> m_dist;
};

class Tester {
public:
    bool testArrayBufferConstructorError(int capacity);
    bool testArrayBufferConstructorEdge(int capacity);
    bool testArrayBufferConstructorNormal(int capacity);

    bool testArrayBufferClearEmpty();
    bool testArrayBufferClearNormal();

    bool testArrayBufferIsEmpty();
    bool testArrayBufferNotEmpty();

    bool testArrayBufferEnqueueAndDequeueNormal();
    bool testArrayBufferEnqueueAndDequeueOneItem();
    bool testArrayBufferEnqueueAndDequeueBufferSize();
    bool testArrayBufferEnqueueFull();
    bool testArrayBufferDequeueEmpty();

    bool testArrayBufferCopyConstructorEdge();
    bool testArrayBufferCopyConstructorNormal();

    bool testArrayBufferAssignmentOperatorEdge();
    bool testArrayBufferAssignmentOperatorNormal();
};

bool Tester::testArrayBufferConstructorError(int capacity) {
    //call constructor with invalid parameter
    ArrayBuffer emptyArray(capacity);

    if (emptyArray.m_buffer == nullptr && emptyArray.m_capacity == 0 && emptyArray.m_count == 0 &&
        emptyArray.m_start == 0 && emptyArray.m_end == 0 && emptyArray.m_next == nullptr) {
        //test passes if empty array buffer constructed
        return true;
    }
    return false;
}

bool Tester::testArrayBufferConstructorEdge(int capacity) {
    //call constructor with edge parameter (1)
    ArrayBuffer array(capacity);

    if (array.m_buffer != nullptr && array.m_capacity == capacity && array.m_count == 0 && array.m_start == 0 &&
        array.m_end == 0 && array.m_next == nullptr) {
        //test passes if array buffer constructed and capacity matches passed-in value
        return true;
    }
    return false;
}

bool Tester::testArrayBufferConstructorNormal(int capacity) {
    //call constructor with normal parameter
    ArrayBuffer array(capacity);

    if (array.m_buffer != nullptr && array.m_capacity == capacity && array.m_count == 0 && array.m_start == 0 &&
        array.m_end == 0 && array.m_next == nullptr) {
        //test passes if array buffer constructed and capacity matches passed-in value
        return true;
    }
    return false;
}


bool Tester::testArrayBufferClearEmpty() {
    //construct empty array buffer
    ArrayBuffer emptyArray(-5);

    emptyArray.clear();

    if (emptyArray.m_buffer == nullptr && emptyArray.m_capacity == 0 && emptyArray.m_count == 0 &&
        emptyArray.m_start == 0 && emptyArray.m_end == 0 && emptyArray.m_next == nullptr) {
        //test passes if empty array buffer is still empty
        return true;
    }
    return false;
}

bool Tester::testArrayBufferClearNormal() {
    //construct normal array buffer
    ArrayBuffer filledArray(3);

    //fill buffer with random data
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < filledArray.m_capacity; i++) {
        filledArray.enqueue(randObject.getRandInt());
    }

    filledArray.clear();

    if (filledArray.m_buffer == nullptr && filledArray.m_capacity == 0 && filledArray.m_count == 0 &&
        filledArray.m_start == 0 && filledArray.m_end == 0 && filledArray.m_next == nullptr) {
        //test passes if array buffer has been emptied
        return true;
    }
    return false;
}


bool Tester::testArrayBufferIsEmpty() {
    ArrayBuffer emptyArray(6);

    if (emptyArray.empty()) {
        return true;
    }
    return false;
}

bool Tester::testArrayBufferNotEmpty() {
    ArrayBuffer filledArray(6);

    //fill buffer with random data
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < filledArray.m_capacity; i++) {
        filledArray.enqueue(randObject.getRandInt());
    }

    if (!filledArray.empty()) {
        return true;
    }
    return false;
}


bool Tester::testArrayBufferEnqueueAndDequeueNormal() {
    ArrayBuffer array(8);

    vector<int> insertedDataStorage;

    //fill the array buffer with a certain number (in this case, 3) of random data
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < 3; i++) {
        int dataToInsert = randObject.getRandInt();
        array.enqueue(dataToInsert);

        //store all inserted data in a vector to check against later
        insertedDataStorage.push_back(dataToInsert);
    }

    //check whether we can remove the same sequence of data without a problem
    for (int i = 0; i < 3; i++) {
        if (insertedDataStorage.front() != array.dequeue()) {
            //test fails if data is not removed in first-in-first-out order
            return false;
        }
        //remove the first data value after each check (moves the target to match what should be dequeued)
        insertedDataStorage.erase(insertedDataStorage.begin());
    }

    //if insertion and removal happen properly, test passes
    return true;
}

bool Tester::testArrayBufferEnqueueAndDequeueOneItem() {
    ArrayBuffer array(10);

    //insert one random data into array buffer
    Random randObject(MINDATA, MAXDATA);
    int dataToInsert = randObject.getRandInt();
    array.enqueue(dataToInsert);

    //check whether we can remove the data value without a problem
    if (dataToInsert != array.dequeue()) {
        return false;
    }
    //if insertion and removal happen properly, test passes
    return true;
}

bool Tester::testArrayBufferEnqueueAndDequeueBufferSize() {
    ArrayBuffer array(11);

    vector<int> insertedDataStorage;

    //completely fill the array buffer with random data (to capacity)
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < array.m_capacity; i++) {
        int dataToInsert = randObject.getRandInt();
        array.enqueue(dataToInsert);

        //store data in a vector to check against later
        insertedDataStorage.push_back(dataToInsert);
    }

    //check whether we can remove all the data without a problem
    for (int i = 0; i < array.m_capacity; i++) {
        if (insertedDataStorage.front() != array.dequeue()) {
            //test fails if data is not removed in first-in-first-out order
            return false;
        }
        //remove the first data value after each check (moves the target to match what should be dequeued)
        insertedDataStorage.erase(insertedDataStorage.begin());
    }

    //if insertion and removal happen properly, test passes
    return true;
}

bool Tester::testArrayBufferEnqueueFull() {
    ArrayBuffer filledArray(7);

    //fully fill buffer with random data
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < filledArray.m_capacity; i++) {
        filledArray.enqueue(randObject.getRandInt());
    }

    try {
        filledArray.enqueue(randObject.getRandInt());
    } catch (overflow_error &e) {
        //test passes if appropriate error is thrown
        return true;
    }
    return false;
}

bool Tester::testArrayBufferDequeueEmpty() {
    ArrayBuffer filledArray(13);

    try {
        filledArray.dequeue();
    } catch (underflow_error &e) {
        //test passes if appropriate error is thrown
        return true;
    }
    return false;
}

bool Tester::testArrayBufferCopyConstructorEdge() {
    //original is an empty object
    ArrayBuffer original(-8);

    ArrayBuffer copy(original);

    if (copy.m_buffer == nullptr && copy.m_capacity == original.m_capacity && copy.m_count == original.m_count &&
        copy.m_start == original.m_start && copy.m_end == original.m_end && copy.m_next == nullptr) {
        //test passes if copy is empty
        return true;
    }
    return false;
}

bool Tester::testArrayBufferCopyConstructorNormal() {
    ArrayBuffer original(12);

    ArrayBuffer copy(original);

    if (copy.m_buffer == original.m_buffer || copy.m_capacity != original.m_capacity ||
        copy.m_count != original.m_count || copy.m_start != original.m_start || copy.m_end != original.m_end ||
        copy.m_next != nullptr) {
        //test fails if the objects point to the same memory location or other member variables do not match
        return false;
    }

    for (int i = 0; i < copy.m_capacity; i++) {
        if (copy.m_buffer[i] != original.m_buffer[i]) {
            //test fails if any of the array data is different
            return false;
        }
    }
    return true;
}

bool Tester::testArrayBufferAssignmentOperatorEdge() {
    ArrayBuffer original(-8);
    ArrayBuffer copy(6);

    //assign empty object (original) to normal object (copy)
    copy = original;

    if (copy.m_buffer == nullptr && copy.m_capacity == original.m_capacity && copy.m_count == original.m_count &&
        copy.m_start == original.m_start && copy.m_end == original.m_end && copy.m_next == nullptr) {
        //test passes if copy is empty
        return true;
    }
    return false;
}

bool Tester::testArrayBufferAssignmentOperatorNormal() {
    ArrayBuffer original(2);

    //fill original with random data
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < original.m_capacity; i++) {
        original.enqueue(randObject.getRandInt());
    }

    ArrayBuffer copy(7);

    //assign normal object (original) to normal object (copy)
    copy = original;

    if (copy.m_buffer == original.m_buffer || copy.m_capacity != original.m_capacity ||
        copy.m_count != original.m_count || copy.m_start != original.m_start || copy.m_end != original.m_end ||
        copy.m_next != nullptr) {
        //test fails if the objects point to the same memory location or other member variables do not match
        return false;
    }

    for (int i = 0; i < copy.m_capacity; i++) {
        if (copy.m_buffer[i] != original.m_buffer[i]) {
            //test fails if any of the array data is different
            return false;
        }
    }
    return true;
}


int main() {
    Tester tester;

    //ArrayBuffer constructor tests
    cout << "Testing ArrayBuffer Constructor (error case) - creates an empty object:" << endl;
    if (tester.testArrayBufferConstructorError(-9)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Constructor (edge case) - allocates memory, initializes all member variables to the proper values:" << endl;
    if (tester.testArrayBufferConstructorEdge(1)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Constructor (normal case) - allocates memory, initializes all member variables to the proper values:" << endl;
    if (tester.testArrayBufferConstructorNormal(9)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }

    //ArrayBuffer clear tests
    cout << "\nTesting ArrayBuffer Clear (edge case) - empties the current object:" << endl;
    if (tester.testArrayBufferClearEmpty()) {
        cout << "\tClear passed!" << endl;
    } else {
        cout << "\t***Clear failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Clear (normal case) - empties the current object:" << endl;
    if (tester.testArrayBufferClearNormal()) {
        cout << "\tClear passed!" << endl;
    } else {
        cout << "\t***Clear failed!***" << endl;
    }

    //ArrayBuffer empty tests
    cout << "\nTesting ArrayBuffer Empty (true case) - ____:" << endl;
    if (tester.testArrayBufferIsEmpty()) {
        cout << "\tEmpty passed!" << endl;
    } else {
        cout << "\t***Empty failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Empty (false case) - ____:" << endl;
    if (tester.testArrayBufferNotEmpty()) {
        cout << "\tEmpty passed!" << endl;
    } else {
        cout << "\t***Empty failed!***" << endl;
    }

    //ArrayBuffer enqueue and dequeue tests
    cout << "\nTesting ArrayBuffer Enqueue & Dequeue (normal case) - ensures queue functionality for the same number of inserted and removed data:" << endl;
    if (tester.testArrayBufferEnqueueAndDequeueNormal()) {
        cout << "\tEnqueue & Dequeue passed!" << endl;
    } else {
        cout << "\t***Enqueue/Dequeue failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Enqueue & Dequeue (edge case) - ensures queue functionality for one data value:" << endl;
    if (tester.testArrayBufferEnqueueAndDequeueOneItem()) {
        cout << "\tEnqueue & Dequeue passed!" << endl;
    } else {
        cout << "\t***Enqueue/Dequeue failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Enqueue & Dequeue (edge case) - ensures queue functionality for inserting and removing data with the same size as the buffer size:" << endl;
    if (tester.testArrayBufferEnqueueAndDequeueBufferSize()) {
        cout << "\tEnqueue & Dequeue passed!" << endl;
    } else {
        cout << "\t***Enqueue/Dequeue failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Enqueue (exception case) - ___:" << endl;
    if (tester.testArrayBufferEnqueueFull()) {
        cout << "\tEnqueue passed!" << endl;
    } else {
        cout << "\t***Enqueue failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Dequeue (exception case) - ___:" << endl;
    if (tester.testArrayBufferDequeueEmpty()) {
        cout << "\tDequeue passed!" << endl;
    } else {
        cout << "\t***Dequeue failed!***" << endl;
    }

    //ArrayBuffer copy constructor tests
    cout << "\nTesting ArrayBuffer Copy Constructor (edge case) - ____:" << endl;
    if (tester.testArrayBufferCopyConstructorEdge()) {
        cout << "\tCopy Constructor passed!" << endl;
    } else {
        cout << "\t***Copy Constructor failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Copy Constructor (normal case) - ____:" << endl;
    if (tester.testArrayBufferCopyConstructorNormal()) {
        cout << "\tCopy Constructor passed!" << endl;
    } else {
        cout << "\t***Copy Constructor failed!***" << endl;
    }

    //ArrayBuffer assignment operator tests
    cout << "\nTesting ArrayBuffer Assignment Operator (edge case) - ____:" << endl;
    if (tester.testArrayBufferAssignmentOperatorEdge()) {
        cout << "\tAssignment Operator passed!" << endl;
    } else {
        cout << "\t***Assignment Operator failed!***" << endl;
    }
    cout << "Testing ArrayBuffer Assignment Operator (normal case) - ____:" << endl;
    if (tester.testArrayBufferAssignmentOperatorNormal()) {
        cout << "\tAssignment Operator passed!" << endl;
    } else {
        cout << "\t***Assignment Operator failed!***" << endl;
    }


    //ListBuffer constructor tests


    //ListBuffer clear tests


    //ListBuffer enqueue and dequeue tests


    //ListBuffer copy constructor tests


    //ListBuffer assignment operator tests
}