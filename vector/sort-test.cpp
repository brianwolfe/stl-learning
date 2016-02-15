// TODO replace this with platform-agnostic version at 
// http://nadeausoftware.com/articles/2012/04/c_c_tip_how_measure_elapsed_real_time_benchmarking
// or similar
#include <sys/time.h>

// Need malloc and rand
#include <cstdlib>
// Need for ostream
#include <iostream>
#include <list>

// This is what I'm using to compare to
#include <vector>

// Use widget to test with a self-defined class
#include "widget.hpp"
// This is my test file
#include "sl-sort.hpp"

constexpr size_t MAX_VECTOR_SIZE = 5000000;
static int random_numbers[MAX_VECTOR_SIZE];
static struct timeval start_time;

/*
 * Convenience function for setting the current time
 */
static void init_start_time(void)
{
    gettimeofday(&start_time, NULL);
}

/*
 * Convenience function for getting the time since the last call to
 * init_start_time
 */
static double get_time(void)
{
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    double diff = (double)(end_time.tv_sec - start_time.tv_sec)
                + (double)(end_time.tv_usec - start_time.tv_usec) / 1e6;

    return diff;
}

/*
 * Initialize the random numbers used in the rest of the testing
 */
static void initialize_random_numbers(void)
{
    for (size_t i = 0; i < MAX_VECTOR_SIZE; i++)
    {
        random_numbers[i] = (int)rand() % MAX_VECTOR_SIZE;
    }
}

struct TimeResult
{
    public:
        std::string tag;
        double time;
        TimeResult(std::string tag, double time)
        {
            this->tag = tag;
            this->time = time;
        }
};

class ResultList : public std::list<TimeResult>
{
};

std::ostream& operator<< (std::ostream & o, ResultList r)
{
    for (auto tr : r)
    {
        o << tr.tag << ": " << tr.time << std::endl;
    }
    return o;
}

template <size_t N>
ResultList test_sort()
{
    ResultList resultlist;

    for (size_t i = 0; i < 3; i++)
    {
        std::vector<int> vec1(random_numbers, random_numbers + N);
        std::vector<int> vec2(random_numbers, random_numbers + N);

        init_start_time();
        std::sort(vec2.begin(), vec2.end());
        resultlist.emplace_back("QSort time", get_time());

        init_start_time();
        sll::heap_sort(vec1.begin(), vec1.end());
        resultlist.emplace_back("Heapify time", get_time());
    }

    return resultlist;
}


int main()
{
    initialize_random_numbers();

    ResultList heapify_results = test_sort<MAX_VECTOR_SIZE>();

    std::cout << MAX_VECTOR_SIZE << " sample times" << std::endl;

    std::cout << heapify_results;

    return 0;
}
