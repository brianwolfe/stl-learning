
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
#include "sl-vector.hpp"

constexpr size_t MAX_VECTOR_SIZE = 100000000;
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
        random_numbers[i] = (int)rand();
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

/*
 * Test the vector with integers by pushing a N integers onto the list, popping them
 * off, then pushing them back on again.
 */
template<typename T, size_t N> ResultList test_vector_int()
{
    init_start_time();
    ResultList resultlist;

    T vec;
    // Initialize by pushing N elements onto the vector
    init_start_time();
    for (size_t i = 0; i < N; i++)
    {
        vec.emplace_back(random_numbers[i]);
    }
    resultlist.emplace_back("Push time", get_time());

    // Pop everything off of the vector
    init_start_time();
    for (size_t i = 0; i < N; i++)
    {
        vec.pop_back();
    }
    resultlist.emplace_back("Pop time", get_time());

    // Push everything back on again
    init_start_time();
    for (size_t i = 0; i < N; i++)
    {
        vec.emplace_back(random_numbers[i]);
    }
    resultlist.emplace_back("Repush time", get_time());

    // Multiply everything by 913
    init_start_time();
    for (size_t i = 0; i < N; i++)
    {
        vec[i] = vec[i] * 913;
    }
    resultlist.emplace_back("Multiply time", get_time());

    return resultlist;
}


int main()
{
    initialize_random_numbers();

    ResultList tvi = test_vector_int<std::vector<int>, MAX_VECTOR_SIZE>();
    std::cout << "STD vector results" << std::endl;

    std::cout << tvi;

    ResultList tsm = test_vector_int<stll::vector<int>, MAX_VECTOR_SIZE>();
    std::cout << "Mini-SL vector results" << std::endl;

    std::cout << tsm;
}
