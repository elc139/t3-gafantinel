#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>

int chunksize;

class SharedArray
{
private:
    char *array;
    int index;
    int size;
    bool usemutex;

public:
    SharedArray(int n, bool use) : size(n), index(0), usemutex(use)
    {
        array = new char[size];
        std::fill(array, array + size, '-');
    }
    ~SharedArray()
    {
        delete[] array;
    }
    void addChar(char c)
    {
        switch (usemutex)
        {
            case true:
                #pragma omp ordered
                {
                    array[index] = c;
                    spendSomeTime();
                    index++;
                }
                break;
            default:
                array[index] = c;
                spendSomeTime();
                index++;
        }
    }
    int countOccurrences(char c)
    {
        return std::count(array, array + size, c);
    }
    std::string toString()
    {
        return std::string(array, size);
    }

private:
    void spendSomeTime()
    {
        for (int i = 0; i < 10000; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                // These loops shouldn't be removed by the compiler
            }
        }
    }
};

class ArrayFiller
{
private:
    static const int nThreads = 3;
    static const int nTimes = 20;
    int totalSize = 100;
    SharedArray *array;

public:
    ArrayFiller(bool usemutex)
    {
        array = new SharedArray(totalSize, usemutex);
    }
    void fillArrayConcurrently()
    {
        omp_set_num_threads(nThreads);
        #pragma omp critical
        {
            #pragma omp parallel for schedule(runtime) ordered
            for (int i = 0; i < totalSize; ++i)
            {
                int threadNum = omp_get_thread_num();
                array->addChar('A' + threadNum);
            }

        }
    }
    void printStats()
    {
        std::cout << array->toString() << std::endl;
        for (int i = 0; i < nThreads; ++i)
            std::cout << (char)('A' + i) << "="
                      << array->countOccurrences('A' + i) << " ";
        std::cout << std::endl;
    }
    ~ArrayFiller()
    {
        delete array;
    }
};

int main()
{

    // static
    std::cout << "\n1: static sem chunksize" << std::endl;
    ArrayFiller m1(true);
    omp_set_schedule(omp_sched_static, 0);
    m1.fillArrayConcurrently();
    m1.printStats();

    // static com chunksize
    std::cout << "\n2: static com chunksize 4" << std::endl;
    ArrayFiller m2(true);
    omp_set_schedule(omp_sched_static, 4);
    m2.fillArrayConcurrently();
    m2.printStats();

    // dynamic
    std::cout << "\n3: dynamic sem chunksize" << std::endl;
    ArrayFiller m3(true);
    omp_set_schedule(omp_sched_dynamic, 0);
    m3.fillArrayConcurrently();
    m3.printStats();

    // dynamic com chunksize
    std::cout << "\n4: dynamic com chunksize 4" << std::endl;
    ArrayFiller m4(true);
    omp_set_schedule(omp_sched_dynamic, 4);
    m4.fillArrayConcurrently();
    m4.printStats();

    // guided
    std::cout << "\n5: guided sem chunksize" << std::endl;
    ArrayFiller m5(true);
    omp_set_schedule(omp_sched_guided, 0);
    m5.fillArrayConcurrently();
    m5.printStats();

    // guided com chunksize
    std::cout << "\n6: guided com chunksize 4" << std::endl;
    ArrayFiller m6(true);
    omp_set_schedule(omp_sched_guided, 4);
    m6.fillArrayConcurrently();
    m6.printStats();

    // runtime
    std::cout << "\n7: runtime" << std::endl;
    ArrayFiller m7(true);
    m7.fillArrayConcurrently();
    m7.printStats();

    // auto
    std::cout << "\n8: auto" << std::endl;
    ArrayFiller m8(true);
    omp_set_schedule(omp_sched_auto, 0);
    m8.fillArrayConcurrently();
    m8.printStats();

    // dynamic sem mutex
    std::cout << "\n9: dynamic sem mutex" << std::endl;
    ArrayFiller m9(false);
    omp_set_schedule(omp_sched_dynamic, 0);
    m9.fillArrayConcurrently();
    m9.printStats();

}
