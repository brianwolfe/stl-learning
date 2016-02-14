/*
 * Experimentation with different sorting algorithms, using the STL library
 */
#include <iterator>

namespace sll
{
// using std::RandomAccessIterator;
template <class RandomAccessIterator, class Compare>
void heapify(RandomAccessIterator start, RandomAccessIterator end, Compare c);
template <class RandomAccessIterator, class Compare>
void heapify_up(RandomAccessIterator start, RandomAccessIterator cur, Compare c);
template <class RandomAccessIterator, class Compare>
void heapify_down(RandomAccessIterator start, RandomAccessIterator end,
        RandomAccessIterator cur, Compare c);
template <class RandomAccessIterator>
RandomAccessIterator heapify_parent(RandomAccessIterator start,
        RandomAccessIterator cur);
template <class RandomAccessIterator>
RandomAccessIterator heapify_left_child(RandomAccessIterator start,
        RandomAccessIterator end, RandomAccessIterator cur);
template <class RandomAccessIterator>
RandomAccessIterator heapify_right_child(RandomAccessIterator start,
        RandomAccessIterator end, RandomAccessIterator cur);
template <class RandomAccessIterator, class Compare>
void heapify_end_pop(RandomAccessIterator start, RandomAccessIterator end, Compare);
template <class RandomAccessIterator, class Compare>
void heapify_inplace_sort(RandomAccessIterator start, RandomAccessIterator end, Compare);
template <class RandomAccessIterator, class Compare>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end, Compare c);

template <class RandomAccessIterator>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end)
{
    heap_sort(start, end, std::less<typename RandomAccessIterator::value_type>());
}

template <class RandomAccessIterator, class Compare>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end, Compare c)
{
    heapify(start, end, c);
    heapify_inplace_sort(start, end, c);
}

template <class RandomAccessIterator, class Compare>
void heapify_inplace_sort(RandomAccessIterator start, RandomAccessIterator end, Compare c)
{
    for (auto heap_end = end; heap_end != start; heap_end --)
    {
        heapify_end_pop(start, heap_end, c);
    }
}

template <class RandomAccessIterator, class Compare>
void heapify(RandomAccessIterator start, RandomAccessIterator end, Compare c)
{
    for (auto heap_end = start; heap_end != end; heap_end ++)
    {
        heapify_up(start, heap_end, c);
    }
}

template <class RandomAccessIterator, class Compare>
void heapify_up(RandomAccessIterator start, RandomAccessIterator cur, Compare c)
{
    auto parent = heapify_parent(start, cur);
    if (*cur < *parent)
    {
        std::iter_swap(parent, cur);
        heapify_up(start, parent, c);
    }
}

template <class RandomAccessIterator, class Compare>
void heapify_down(RandomAccessIterator start, RandomAccessIterator end,
        RandomAccessIterator cur, Compare c)
{
    auto left_child = heapify_left_child(start, end, cur);
    auto right_child = heapify_right_child(start, end, cur);
    auto min_child = left_child;
    if (right_child < end)
    {
        if (*right_child < *left_child)
        {
            min_child = right_child;
        }
    }
    if (left_child < end)
    {
        if (*min_child < *cur)
        {
            std::iter_swap(min_child, cur);
            heapify_down(start, end, min_child, c);
        }
    }
}

template <class RandomAccessIterator>
RandomAccessIterator heapify_parent(RandomAccessIterator start, RandomAccessIterator cur)
{
    auto ind = std::distance(start, cur);
    return start + (ind - 1) / 2;
}

template <class RandomAccessIterator>
RandomAccessIterator heapify_left_child(RandomAccessIterator start,
        RandomAccessIterator end, RandomAccessIterator cur)
{
    auto ind = std::distance(start, cur);
    return start + (ind * 2) + 1;
}

template <class RandomAccessIterator>
RandomAccessIterator heapify_right_child(RandomAccessIterator start,
        RandomAccessIterator end, RandomAccessIterator cur)
{
    auto ind = std::distance(start, cur);
    return start + (ind * 2) + 2;
}

template <class RandomAccessIterator, class Compare>
void heapify_end_pop(RandomAccessIterator start, RandomAccessIterator end, Compare c)
{
    std::iter_swap(start, end - 1);
    heapify_down(start, end - 1, start, c);
}
}
