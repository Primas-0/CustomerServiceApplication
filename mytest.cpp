#include "fastq.h"
#include <random>
#include <vector>
#include <algorithm>

const int MINDATA = 1000;     // dealer ID
const int MAXDATA = 9999;     // dealer ID

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

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

    bool testArrayBufferEnqueueAndDequeueEdge();
    bool testArrayBufferEnqueueAndDequeueNormal();
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
        filledArray.enqueue(randObject.getRandNum());
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
    ArrayBuffer emptyArray(-6);

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
        filledArray.enqueue(randObject.getRandNum());
    }

    if (!filledArray.empty()) {
        return true;
    }
    return false;
}


bool Tester::testArrayBufferEnqueueAndDequeueEdge() {
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
    cout << "Testing ArrayBuffer Clear (edge case) - empties the current object:" << endl;
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
    cout << "Testing ArrayBuffer Empty (true case) - ____:" << endl;
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


    //ArrayBuffer copy constructor tests


    //ArrayBuffer assignment operator tests



    //ListBuffer constructor tests


    //ListBuffer clear tests


    //ListBuffer enqueue and dequeue tests


    //ListBuffer copy constructor tests


    //ListBuffer assignment operator tests
}