#include "fastq.h"
#include <random>
#include <vector>

const int MINDATA = 1000;
const int MAXDATA = 9999;

class Random {
public:
    Random(int min, int max) {
        //UNIFORMINT-type distribution and seed
        m_dist = std::uniform_int_distribution<>(min, max);
        m_randGenerator = std::mt19937(10); //fixed seed value of 10
    }

    int getRandInt() {
        //generate a random integer between the min and max values
        return m_dist(m_randGenerator);
    }
private:
    std::uniform_int_distribution<> m_dist;
    std::mt19937 m_randGenerator;
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

    bool testArrayBufferCopyConstructorEdge(ArrayBuffer &original, ArrayBuffer &copy);
    bool testArrayBufferCopyConstructorNormal(ArrayBuffer &original, ArrayBuffer &copy);

    bool testArrayBufferAssignmentOperatorEdge(ArrayBuffer &original, ArrayBuffer &copy);
    bool testArrayBufferAssignmentOperatorNormal(ArrayBuffer &original, ArrayBuffer &copy);

    bool testListBufferConstructorDefault(int minBufCapacity);
    bool testListBufferConstructorEdge(int minBufCapacity);
    bool testListBufferConstructorNormal(int minBufCapacity);

    bool testListBufferClear();

    bool testListBufferEnqueueAndDequeueNormal();
    bool testListBufferDequeueEmpty();

    bool testListBufferCopyConstructorEdge();
    bool testListBufferCopyConstructorNormal();

    bool testListBufferAssignmentOperatorEdge();
    bool testListBufferAssignmentOperatorNormal();
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

bool Tester::testArrayBufferCopyConstructorEdge(ArrayBuffer &original, ArrayBuffer &copy) {
    //original is an empty object
    if (copy.m_buffer == nullptr && copy.m_capacity == original.m_capacity && copy.m_count == original.m_count &&
        copy.m_start == original.m_start && copy.m_end == original.m_end && copy.m_next == nullptr) {
        //test passes if copy is empty
        return true;
    }
    return false;
}

bool Tester::testArrayBufferCopyConstructorNormal(ArrayBuffer &original, ArrayBuffer &copy) {
    if (copy.m_buffer == original.m_buffer || copy.m_capacity != original.m_capacity ||
        copy.m_count != original.m_count || copy.m_start != original.m_start || copy.m_end != original.m_end) {
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

bool Tester::testArrayBufferAssignmentOperatorEdge(ArrayBuffer &original, ArrayBuffer &copy) {
    //assign empty object (original) to normal object (copy)
    copy = original;

    if (copy.m_buffer == nullptr && copy.m_capacity == original.m_capacity && copy.m_count == original.m_count &&
        copy.m_start == original.m_start && copy.m_end == original.m_end && copy.m_next == nullptr) {
        //test passes if copy is empty
        return true;
    }
    return false;
}

bool Tester::testArrayBufferAssignmentOperatorNormal(ArrayBuffer &original, ArrayBuffer &copy) {
    //assign normal object (original) to normal object (copy)
    copy = original;

    if (copy.m_buffer == original.m_buffer || copy.m_capacity != original.m_capacity ||
        copy.m_count != original.m_count || copy.m_start != original.m_start || copy.m_end != original.m_end) {
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


bool Tester::testListBufferConstructorDefault(int minBufCapacity) {
    //call constructor with invalid parameter
    ListBuffer list(minBufCapacity);

    if (list.m_cursor != nullptr && list.m_minBufCapacity == DEFAULT_MIN_CAPACITY &&
        list.m_cursor->m_next == list.m_cursor && list.m_listSize == 1) {
        //test passes if default-size linked list constructed
        return true;
    }
    return false;
}

bool Tester::testListBufferConstructorEdge(int minBufCapacity) {
    //call constructor with valid parameter
    ListBuffer list(minBufCapacity);

    if (list.m_cursor != nullptr && list.m_minBufCapacity == minBufCapacity &&
        list.m_cursor->m_next == list.m_cursor && list.m_listSize == 1) {
        //test passes if linked list constructed and m_minBufCapacity matches passed-in value
        return true;
    }
    return false;
}

bool Tester::testListBufferConstructorNormal(int minBufCapacity) {
    //call constructor with valid parameter
    ListBuffer list(minBufCapacity);

    if (list.m_cursor != nullptr && list.m_minBufCapacity == minBufCapacity &&
        list.m_cursor->m_next == list.m_cursor && list.m_listSize == 1) {
        //test passes if linked list constructed and m_minBufCapacity matches passed-in value
        return true;
    }
    return false;
}

bool Tester::testListBufferClear() {
    ListBuffer list(18);

    //fill linked list buffers with a large (in this case, 10000) random data set
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < 10000; i++) {
        list.enqueue(randObject.getRandInt());
    }

    list.clear();

    if (list.m_cursor == nullptr && list.m_listSize == 0) {
        return true;
    }
    return false;
}

bool Tester::testListBufferEnqueueAndDequeueNormal() {
    ListBuffer list(21);

    vector<int> insertedDataStorage;

    //fill the array buffer with a certain number (in this case, 10000) of random data
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < 10000; i++) {
        int dataToInsert = randObject.getRandInt();
        list.enqueue(dataToInsert);

        //store all inserted data in a vector to check against later
        insertedDataStorage.push_back(dataToInsert);
    }

    //check whether we can remove the same sequence of data without a problem
    for (int i = 0; i < 10000; i++) {
        if (insertedDataStorage.front() != list.dequeue()) {
            //test fails if data is not removed in first-in-first-out order
            return false;
        }
        //remove the first data value after each check (moves the target to match what should be dequeued)
        insertedDataStorage.erase(insertedDataStorage.begin());
    }

    //if insertion and removal happen properly, test passes
    return true;
}

bool Tester::testListBufferDequeueEmpty() {
    ListBuffer list(17);

    try {
        list.dequeue();
    } catch (underflow_error &e) {
        //test passes if appropriate error is thrown
        return true;
    }
    return false;
}

bool Tester::testListBufferCopyConstructorEdge() {
    ListBuffer original(8);

    //fill only the first buffer
    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < original.m_minBufCapacity; i++) {
        original.enqueue(randObject.getRandInt());
    }

    ListBuffer copy(original);

    if (original.m_cursor == copy.m_cursor || original.m_listSize != copy.m_listSize ||
        original.m_minBufCapacity != copy.m_minBufCapacity) {
        //test fails if the array pointers are the same or the member variables do not match
        return false;
    }

    if (!testArrayBufferCopyConstructorNormal(*original.m_cursor, *copy.m_cursor)) {
        //test fails if the array data is different
        return false;
    }

    return true;
}

bool Tester::testListBufferCopyConstructorNormal() {
    ListBuffer original(8);

    Random randObject(MINDATA, MAXDATA);
    for (int i = 0; i < 10000; i++) {
        original.enqueue(randObject.getRandInt());
    }

    ListBuffer copy(original);

    if (original.m_listSize != copy.m_listSize || original.m_minBufCapacity != copy.m_minBufCapacity) {
        //test fails if member variables do not match
        return false;
    }

    //pointers to the first array in each list
    ArrayBuffer *originalCurr = original.m_cursor;
    ArrayBuffer *copyCurr = copy.m_cursor;

    for (int i = 0; i < original.m_listSize; i++) {
        if (originalCurr == copyCurr) {
            //test fails if any of the array pointers are the same
            return false;
        }

        if (!testArrayBufferCopyConstructorNormal(*originalCurr, *copyCurr)) {
            //test fails if the array data is different for any of the array buffers
            return false;
        }

        //advance the array pointers
        originalCurr = originalCurr->m_next;
        copyCurr = copyCurr->m_next;
    }

    return true;
}

bool Tester::testListBufferAssignmentOperatorEdge() {
    return false;
}

bool Tester::testListBufferAssignmentOperatorNormal() {
    return false;
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
    cout << "\nTesting ArrayBuffer Clear (edge case) - ____:" << endl;
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
    ArrayBuffer original1(-8);
    ArrayBuffer copy1(original1);
    cout << "\nTesting ArrayBuffer Copy Constructor (edge case) - ____:" << endl;
    if (tester.testArrayBufferCopyConstructorEdge(original1, copy1)) {
        cout << "\tCopy Constructor passed!" << endl;
    } else {
        cout << "\t***Copy Constructor failed!***" << endl;
    }

    ArrayBuffer original2(12);
    Random randObject2(MINDATA, MAXDATA);
    for (int i = 0; i < 12; i++) {
        original2.enqueue(randObject2.getRandInt());
    }
    ArrayBuffer copy2(original2);
    cout << "Testing ArrayBuffer Copy Constructor (normal case) - ____:" << endl;
    if (tester.testArrayBufferCopyConstructorNormal(original2, copy2)) {
        cout << "\tCopy Constructor passed!" << endl;
    } else {
        cout << "\t***Copy Constructor failed!***" << endl;
    }

    //ArrayBuffer assignment operator tests
    ArrayBuffer original3(-8);
    ArrayBuffer copy3(6);
    cout << "\nTesting ArrayBuffer Assignment Operator (edge case) - ____:" << endl;
    if (tester.testArrayBufferAssignmentOperatorEdge(original3, copy3)) {
        cout << "\tAssignment Operator passed!" << endl;
    } else {
        cout << "\t***Assignment Operator failed!***" << endl;
    }

    ArrayBuffer original4(2);
    Random randObject4(MINDATA, MAXDATA);
    for (int i = 0; i < 2; i++) {
        original4.enqueue(randObject4.getRandInt());
    }
    ArrayBuffer copy4(7);
    cout << "Testing ArrayBuffer Assignment Operator (normal case) - ____:" << endl;
    if (tester.testArrayBufferAssignmentOperatorNormal(original4, copy4)) {
        cout << "\tAssignment Operator passed!" << endl;
    } else {
        cout << "\t***Assignment Operator failed!***" << endl;
    }


    //ListBuffer constructor tests
    cout << "\n\nTesting ListBuffer Constructor (edge case) - allocates memory with default values:" << endl;
    if (tester.testListBufferConstructorDefault(-9)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }
    cout << "Testing ListBuffer Constructor (edge case) - allocates memory, initializes all member variables to the proper values:" << endl;
    if (tester.testListBufferConstructorEdge(1)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }
    cout << "Testing ListBuffer Constructor (normal case) - allocates memory, initializes all member variables to the proper values:" << endl;
    if (tester.testListBufferConstructorNormal(9)) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }

    //ListBuffer clear test
    cout << "\nTesting ListBuffer Clear - ____:" << endl;
    if (tester.testListBufferClear()) {
        cout << "\tClear passed!" << endl;
    } else {
        cout << "\t***Clear failed!***" << endl;
    }

    //ListBuffer enqueue and dequeue tests
    cout << "\nTesting ListBuffer Enqueue & Dequeue (normal case) - ___:" << endl;
    if (tester.testListBufferEnqueueAndDequeueNormal()) {
        cout << "\tEnqueue & Dequeue passed!" << endl;
    } else {
        cout << "\t***Enqueue/Dequeue failed!***" << endl;
    }
    cout << "Testing ListBuffer Dequeue (exception case) - ___:" << endl;
    if (tester.testListBufferDequeueEmpty()) {
        cout << "\tDequeue passed!" << endl;
    } else {
        cout << "\t***Dequeue failed!***" << endl;
    }

    //ListBuffer copy constructor tests
    cout << "\nTesting ListBuffer Copy Constructor (edge case) - ____:" << endl;
    if (tester.testListBufferCopyConstructorEdge()) {
        cout << "\tCopy Constructor passed!" << endl;
    } else {
        cout << "\t***Copy Constructor failed!***" << endl;
    }
    cout << "Testing ListBuffer Copy Constructor (normal case) - ____:" << endl;
    if (tester.testListBufferCopyConstructorNormal()) {
        cout << "\tCopy Constructor passed!" << endl;
    } else {
        cout << "\t***Copy Constructor failed!***" << endl;
    }

    //ListBuffer assignment operator tests
    cout << "\nTesting ListBuffer Assignment Operator (edge case) - ____:" << endl;
    if (tester.testListBufferAssignmentOperatorEdge()) {
        cout << "\tAssignment Operator passed!" << endl;
    } else {
        cout << "\t***Assignment Operator failed!***" << endl;
    }
    cout << "Testing ListBuffer Assignment Operator (normal case) - ____:" << endl;
    if (tester.testListBufferAssignmentOperatorNormal()) {
        cout << "\tAssignment Operator passed!" << endl;
    } else {
        cout << "\t***Assignment Operator failed!***" << endl;
    }
}