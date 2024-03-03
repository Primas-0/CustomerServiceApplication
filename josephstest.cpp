#include "fastq.h"
#include <queue>
#include <random>

class Random {
public:
    Random(int min, int max) {
        std::random_device randomDevice;
        m_generator = std::mt19937(randomDevice());
        m_uniformIntDistribution = std::uniform_int_distribution(min, max);
    }

    int getRandomInteger() {
        return m_uniformIntDistribution(m_generator);
    }

private:
    std::mt19937 m_generator;
    std::uniform_int_distribution<> m_uniformIntDistribution;
};

class Tester {
public:
    Tester() : m_random(1000, 5000) {}

    static bool canConstructEmptyArrayBufferWithInvalidCapacity();

    static bool canConstructArrayBufferWithValidCapacity();

    static bool cannotDequeueFromEmptyArrayBuffer();

    bool cannotEnqueueToFullArrayBuffer();

    bool canEnqueueAndDequeueArrayBufferNormally();

    bool canEnqueueAndDequeueOneItemInArrayBuffer();

    bool canEnqueueAndDequeueAllItemsInArrayBuffer();

    bool canCopyNormalArrayBuffer();

    static bool canCopyEmptyArrayBuffer();

    bool canUseAssignmentOperatorWithNormalArrayBuffer();

    bool canUseAssignmentOperatorWithEmptyArrayBuffer();

    bool canClearNormalArrayBuffer();

    static bool canClearEmptyArrayBuffer();

    static bool canCheckArrayBufferIsEmpty();

    bool canCheckArrayBufferIsNotEmpty();

    static bool canConstructListBufferWithInvalidMinimumCapacity();

    static bool canConstructListBufferWithValidMinimumCapacity();

    bool canCopyListBufferWithSingleNode();

    bool canCopyListBufferWithMultipleNodes();

    bool canUseAssignmentOperatorWithSingleNodeListBuffer();

    bool canUseAssignmentOperatorWithMultipleNodeListBuffer();

    static bool cannotDequeueFromEmptyListBuffer();

    bool canEnqueueAndDequeueMultipleItemsInListBuffer();

    bool canEnqueueAndDequeueSingleItemInListBuffer();

    bool canClearListBuffer();

private:
    bool canEnqueueAndDequeueArrayBuffer(int dataSize, int bufferSize);

    static bool areArrayBuffersEquivalent(const ArrayBuffer &expected, const ArrayBuffer &actual);

    void fillArrayBuffer(ArrayBuffer &arrayBuffer);

    void populateListBufferWithTestData(ListBuffer &listBuffer);

    static bool areListBuffersEquivalent(const ListBuffer &expected, const ListBuffer &actual);

    bool canEnqueueAndDequeueListBuffer(int dataSize, int minimumBufferCapacity);

    Random m_random;
};

bool Tester::cannotDequeueFromEmptyArrayBuffer() {
    ArrayBuffer arrayBuffer(5);

    try {
        arrayBuffer.dequeue();
    }
    catch (std::underflow_error &error) {
        return true;
    }

    return false;
}

bool Tester::cannotEnqueueToFullArrayBuffer() {
    ArrayBuffer arrayBuffer(1);
    arrayBuffer.enqueue(m_random.getRandomInteger());

    try {
        arrayBuffer.enqueue(m_random.getRandomInteger());
    }
    catch (std::overflow_error &error) {
        return true;
    }

    return false;
}

bool Tester::canEnqueueAndDequeueArrayBufferNormally() {
    return Tester::canEnqueueAndDequeueArrayBuffer(4, 5);
}

bool Tester::canEnqueueAndDequeueArrayBuffer(int dataSize, int bufferSize) {
    ArrayBuffer arrayBuffer(bufferSize);
    std::deque<int> addedItems;

    for (int i = 0; i < dataSize; ++i) {
        int randomData = m_random.getRandomInteger();
        addedItems.push_back(randomData);
        arrayBuffer.enqueue(randomData);
    }

    for (int i = 0; i < dataSize; ++i) {
        if (addedItems.front() != arrayBuffer.dequeue()) {
            return false;
        }
        addedItems.pop_front();
    }

    return true;
}

bool Tester::canEnqueueAndDequeueOneItemInArrayBuffer() {
    return Tester::canEnqueueAndDequeueArrayBuffer(1, 2);
}

bool Tester::canEnqueueAndDequeueAllItemsInArrayBuffer() {
    return Tester::canEnqueueAndDequeueArrayBuffer(5, 5);
}

bool Tester::canConstructEmptyArrayBufferWithInvalidCapacity() {
    ArrayBuffer arrayBuffer(0);

    return arrayBuffer.m_buffer == nullptr && arrayBuffer.m_capacity == 0 && arrayBuffer.m_next == nullptr &&
           arrayBuffer.m_start == 0 && arrayBuffer.m_end == 0 && arrayBuffer.m_count == 0;
}

bool Tester::canConstructArrayBufferWithValidCapacity() {
    ArrayBuffer arrayBuffer(1);

    return arrayBuffer.m_buffer != nullptr && arrayBuffer.m_capacity == 1 && arrayBuffer.m_next == nullptr &&
           arrayBuffer.m_start == 0 && arrayBuffer.m_end == 0 && arrayBuffer.m_count == 0;
}

bool Tester::canCopyNormalArrayBuffer() {
    ArrayBuffer arrayBuffer(5);

    for (int i = 0; i < 5; ++i) {
        int randomData = m_random.getRandomInteger();
        arrayBuffer.enqueue(randomData);
    }

    ArrayBuffer arrayBufferCopy(arrayBuffer);
    return arrayBuffer.m_buffer != arrayBufferCopy.m_buffer && arrayBufferCopy.m_next == nullptr &&
           Tester::areArrayBuffersEquivalent(arrayBuffer, arrayBufferCopy);
}

bool Tester::areArrayBuffersEquivalent(const ArrayBuffer &expected, const ArrayBuffer &actual) {
    // check if member variables copied correctly / were initialized to correct values
    if (actual.m_count != expected.m_count || actual.m_end != expected.m_end || actual.m_start != expected.m_start ||
        actual.m_capacity != expected.m_capacity) {
        return false;
    }

    for (int i = 0; i < expected.m_capacity; ++i) {
        if (actual.m_buffer[i] != expected.m_buffer[i]) {
            return false;
        }
    }

    return true;
}

bool Tester::canCopyEmptyArrayBuffer() {
    ArrayBuffer emptyArrayBuffer(0);
    ArrayBuffer emptyArrayBufferCopy(emptyArrayBuffer);

    return Tester::areArrayBuffersEquivalent(emptyArrayBuffer, emptyArrayBufferCopy) &&
           emptyArrayBufferCopy.m_buffer == nullptr && emptyArrayBufferCopy.m_next == nullptr;
}

bool Tester::canUseAssignmentOperatorWithNormalArrayBuffer() {
    ArrayBuffer normalBuffer(5);
    Tester::fillArrayBuffer(normalBuffer);
    ArrayBuffer secondBuffer(10);
    Tester::fillArrayBuffer(secondBuffer);

    secondBuffer = normalBuffer;

    return normalBuffer.m_buffer != secondBuffer.m_buffer &&
           Tester::areArrayBuffersEquivalent(normalBuffer, secondBuffer);
}

void Tester::fillArrayBuffer(ArrayBuffer &arrayBuffer) {
    for (int i = 0; i < arrayBuffer.m_capacity; ++i) {
        arrayBuffer.enqueue(m_random.getRandomInteger());
    }
}

bool Tester::canUseAssignmentOperatorWithEmptyArrayBuffer() {
    ArrayBuffer emptyBuffer(0);
    ArrayBuffer otherBuffer(10);
    Tester::fillArrayBuffer(otherBuffer);

    otherBuffer = emptyBuffer;

    return otherBuffer.m_buffer == nullptr && Tester::areArrayBuffersEquivalent(emptyBuffer, otherBuffer);
}

bool Tester::canClearNormalArrayBuffer() {
    ArrayBuffer arrayBuffer(5);
    Tester::fillArrayBuffer(arrayBuffer);

    arrayBuffer.clear();

    return arrayBuffer.m_capacity == 0 && arrayBuffer.m_buffer == nullptr && arrayBuffer.m_end == 0 &&
           arrayBuffer.m_start == 0 && arrayBuffer.m_count == 0 && arrayBuffer.m_next == nullptr;
}

bool Tester::canClearEmptyArrayBuffer() {
    ArrayBuffer emptyArrayBuffer(0);
    emptyArrayBuffer.clear();

    return emptyArrayBuffer.m_capacity == 0 && emptyArrayBuffer.m_buffer == nullptr && emptyArrayBuffer.m_end == 0 &&
           emptyArrayBuffer.m_start == 0 && emptyArrayBuffer.m_count == 0 && emptyArrayBuffer.m_next == nullptr;
}

bool Tester::canCheckArrayBufferIsEmpty() {
    ArrayBuffer arrayBuffer(5);

    return arrayBuffer.empty();
}

bool Tester::canCheckArrayBufferIsNotEmpty() {
    ArrayBuffer arrayBuffer(5);
    arrayBuffer.enqueue(m_random.getRandomInteger());

    return !arrayBuffer.empty();
}

bool Tester::canConstructListBufferWithInvalidMinimumCapacity() {
    ListBuffer listBuffer(0);

    return listBuffer.m_listSize == 1 && listBuffer.m_cursor != nullptr &&
           listBuffer.m_minBufCapacity == DEFAULT_MIN_CAPACITY && listBuffer.m_cursor->m_next != nullptr;
}

bool Tester::canConstructListBufferWithValidMinimumCapacity() {
    ListBuffer listBuffer(5);

    return listBuffer.m_listSize == 1 && listBuffer.m_cursor != nullptr && listBuffer.m_minBufCapacity == 5 &&
           listBuffer.m_cursor->m_next != nullptr;
}

bool Tester::canCopyListBufferWithSingleNode() {
    ListBuffer listBuffer(10);
    listBuffer.enqueue(m_random.getRandomInteger());
    ListBuffer listBufferCopy(listBuffer);

    return Tester::areListBuffersEquivalent(listBuffer, listBufferCopy);
}

bool Tester::areListBuffersEquivalent(const ListBuffer &expected, const ListBuffer &actual) {
    if (expected.m_listSize != actual.m_listSize || expected.m_minBufCapacity != actual.m_minBufCapacity) {
        return false;
    }

    ArrayBuffer *currentExpected = expected.m_cursor;
    ArrayBuffer *currentActual = actual.m_cursor;

    do {
        // make sure pointers don't match, to ensure a deep copy
        if (currentActual == currentExpected || !Tester::areArrayBuffersEquivalent(*currentExpected, *currentActual)) {
            return false;
        }

        currentExpected = currentExpected->m_next;
        currentActual = currentActual->m_next;

    } while (currentExpected != expected.m_cursor);

    return true;
}

bool Tester::canCopyListBufferWithMultipleNodes() {
    ListBuffer listBuffer(1);

    Tester::populateListBufferWithTestData(listBuffer);

    ListBuffer listBufferCopy(listBuffer);
    return Tester::areListBuffersEquivalent(listBuffer, listBufferCopy);
}

void Tester::populateListBufferWithTestData(ListBuffer &listBuffer) {
    for (int i = 0; i < 100; ++i) {
        listBuffer.enqueue(m_random.getRandomInteger());
    }
}

bool Tester::canUseAssignmentOperatorWithSingleNodeListBuffer() {
    ListBuffer listBuffer(10);
    listBuffer.enqueue(m_random.getRandomInteger());

    ListBuffer otherListBuffer(5);
    otherListBuffer = listBuffer;

    return Tester::areListBuffersEquivalent(listBuffer, otherListBuffer);
}

bool Tester::canUseAssignmentOperatorWithMultipleNodeListBuffer() {
    ListBuffer listBuffer(1);
    Tester::populateListBufferWithTestData(listBuffer);

    ListBuffer otherListBuffer(5);
    otherListBuffer = listBuffer;

    return Tester::areListBuffersEquivalent(listBuffer, otherListBuffer);
}


bool Tester::cannotDequeueFromEmptyListBuffer() {
    ListBuffer listBuffer(10);

    try {
        listBuffer.dequeue();
    }
    catch (underflow_error &error) {
        return true;
    }

    return false;
}

bool Tester::canEnqueueAndDequeueListBuffer(int dataSize, int minimumBufferCapacity) {
    ListBuffer listBuffer(minimumBufferCapacity);
    std::deque<int> addedItems;

    for (int i = 0; i < dataSize; ++i) {
        int randomData = m_random.getRandomInteger();
        addedItems.push_back(randomData);
        listBuffer.enqueue(randomData);
    }

    for (int i = 0; i < dataSize; ++i) {
        if (addedItems.front() != listBuffer.dequeue()) {
            return false;
        }
        addedItems.pop_front();
    }

    return true;
}

bool Tester::canEnqueueAndDequeueMultipleItemsInListBuffer() {
    return Tester::canEnqueueAndDequeueListBuffer(10000, 50);
}

bool Tester::canEnqueueAndDequeueSingleItemInListBuffer() {
    return Tester::canEnqueueAndDequeueListBuffer(1, 5);
}

bool Tester::canClearListBuffer() {
    ListBuffer listBuffer(10);
    Tester::populateListBufferWithTestData(listBuffer);

    listBuffer.clear();

    return listBuffer.m_cursor == nullptr;
}

void displayTestStatus(const string &testName, bool testPassed);

int main() {
    Tester tester;

    // test ArrayBuffer
    // test constructor
    displayTestStatus("Can Construct Empty Array Buffer with Invalid Capacity",
                      Tester::canConstructEmptyArrayBufferWithInvalidCapacity());
    displayTestStatus("Can Construct Array Buffer with Valid Capacity",
                      Tester::canConstructArrayBufferWithValidCapacity());

    // test copy constructor
    displayTestStatus("Can Copy Normal Array Buffer", tester.canCopyNormalArrayBuffer());
    displayTestStatus("Can Copy Empty Array Buffer", Tester::canCopyEmptyArrayBuffer());

    // test assignment operator
    displayTestStatus("Can Use Assignment Operator with Normal Array Buffer",
                      tester.canUseAssignmentOperatorWithNormalArrayBuffer());
    displayTestStatus("Can Use Assignment Operator with Empty Array Buffer",
                      tester.canUseAssignmentOperatorWithEmptyArrayBuffer());

    // test enqueue and dequeue
    displayTestStatus("Cannot Dequeue from an Empty Array Buffer", Tester::cannotDequeueFromEmptyArrayBuffer());
    displayTestStatus("Cannot Enqueue to a Full Array Buffer", tester.cannotEnqueueToFullArrayBuffer());
    displayTestStatus("Can Enqueue and Dequeue Array Buffer Normally",
                      tester.canEnqueueAndDequeueArrayBufferNormally());
    displayTestStatus("Can Queue and Dequeue One Item in Array Buffer",
                      tester.canEnqueueAndDequeueOneItemInArrayBuffer());
    displayTestStatus("Can Enqueue and Dequeue All Items in Array Buffer",
                      tester.canEnqueueAndDequeueAllItemsInArrayBuffer());

    // test clear method
    //displayTestStatus("Can Clear Normal Array Buffer", tester.canClearNormalArrayBuffer());
    //displayTestStatus("Can Clear Empty Array Buffer", Tester::canClearEmptyArrayBuffer());

    // test empty method (checks that m_count is zero)
    displayTestStatus("Can Check Array Buffer Is Empty", Tester::canCheckArrayBufferIsEmpty());
    displayTestStatus("Can Check Array Buffer Is Not Empty", tester.canCheckArrayBufferIsNotEmpty());

    // tests for ListBuffer
    // test constructor
    displayTestStatus("Can Construct List Buffer with Invalid Minimum Capacity",
                      Tester::canConstructListBufferWithInvalidMinimumCapacity());
    displayTestStatus("Can Construct List Buffer with Valid Minimum Capacity",
                      Tester::canConstructListBufferWithValidMinimumCapacity());

    // Test copy constructor
    displayTestStatus("Can Copy List Buffer with Single Node", tester.canCopyListBufferWithSingleNode());
    displayTestStatus("Can Copy List Buffer with Multiple Nodes", tester.canCopyListBufferWithMultipleNodes());

    // test assignment operator
    displayTestStatus("Can Use Assignment Operator with Single Node List Buffer",
                      tester.canUseAssignmentOperatorWithSingleNodeListBuffer());
    displayTestStatus("Can Use Assignment Operator with Multiple Node List Buffer",
                      tester.canUseAssignmentOperatorWithMultipleNodeListBuffer());


    // test enqueue and dequeue
    displayTestStatus("Cannot Dequeue from Empty List Buffer", Tester::cannotDequeueFromEmptyListBuffer());
    displayTestStatus("Can Enqueue and Dequeue Multiple Items in List Buffer",
                      tester.canEnqueueAndDequeueMultipleItemsInListBuffer());
    displayTestStatus("Can Enqueue and Dequeue Single Item in List Buffer",
                      tester.canEnqueueAndDequeueSingleItemInListBuffer());

    // test clear
    displayTestStatus("Can Clear List Buffer", tester.canClearListBuffer());

    return 0;
}

void displayTestStatus(const string &testName, bool testPassed) {
    testPassed ? cout << "The test '" << testName << "' has passed" << endl
               : cout << "**** The test '" << testName << "' has failed ****" << endl;
}