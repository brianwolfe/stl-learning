/*
 * Experimentation with different sorting algorithms, using the STL library
 */
#include <iterator>

namespace sll
{
/**
 * Sort elements using a heap. This sorts items using the binary operator
 * c. The element e such that either c(f, e) == false or c(e, f) == true
 * for every other element f in the collection will be first. For example,
 * if the less comparison is used, the minimal element will be first, the
 * order for elements for which c(f, e) == c(e, f) is undefined.
 *
 * This is not a stable sort.
 */
template <class RandomAccessIterator, class Compare>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end, Compare c);

/**
 * Sort the elements according to the default less operator.
 */
template <class RandomAccessIterator>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end);

/**
 * Create a heap from the elements starting at start, ending at end.
 * start and end must be random access iterators.
 *
 * This initializes the underlying container so that the binary heap
 * property is satisfied for all nodes in the heap.
 *
 * The nodes are stored at locations indicative of their location in
 * the complete binary tree.
 *
 * left_child(n) = (n - start) * 2 + 1.
 * right_child(n) = (n - start) * 2 + 2.
 *
 * and thus the parent of any node is
 *
 * parent(n) = (n - 1) / 2
 *
 * using integer rounding towards 0
 *
 * The heap property requires that every node except for the root satisfies
 *
 * c(*parent(n), *n) == true, so that if we use the comparison operator
 * less (<), we are guaranteed that every node is greater than its parent,
 * so the root node is the minimum node in the heap.
 */
template <class RandomAccessIterator, class Compare>
void heapify(RandomAccessIterator start, RandomAccessIterator end, Compare c);

/**
 * Propagate the random access iterator cur towards the root of the heap at
 * start until the c(cur, parent(cur)) == false. If the comparison function
 * c is, for example, less, this will propagate until cur is greater than it's
 * parent.
 */
template <class RandomAccessIterator, class Compare>
void heapify_up(RandomAccessIterator start, RandomAccessIterator cur, Compare c);

/**
 * Propagate cur downwards towards the leaves of the heap until cur is a
 * leaf node or until the c(cur, child(cur)) == true for both children.
 * This maintains the heap property by swapping with the left child if
 * c(cur, left_child(cur)) == false and c(left_child(cur), right_child(cur)) == true
 */
template <class RandomAccessIterator, class Compare>
void heapify_down(RandomAccessIterator start, RandomAccessIterator end,
        RandomAccessIterator cur, Compare c);

/**
 * Get the parent of the node cur.
 */
template <class RandomAccessIterator>
RandomAccessIterator heapify_parent(RandomAccessIterator start,
        RandomAccessIterator cur);

/**
 * Get the left child for the node cur.
 *
 * Warning! this doesn't check the validity of the iterator cur, you must check
 * if it is beyond the bounds of the container!
 */
template <class RandomAccessIterator>
RandomAccessIterator heapify_left_child(RandomAccessIterator start,
        RandomAccessIterator cur);

/**
 * Get the right child for the node cur.
 *
 * Warning! this doesn't check the validity of the iterator cur, you must check
 * if it is beyond the bounds of the container!
 */
template <class RandomAccessIterator>
RandomAccessIterator heapify_right_child(RandomAccessIterator start,
        RandomAccessIterator cur);

/**
 * Pop the minimum node from the heap and place it at the location marked end.
 */
template <class RandomAccessIterator, class Compare>
void heapify_end_pop(RandomAccessIterator start, RandomAccessIterator end, Compare);

/**
 * Pop all elements off the heap and place them in reverse order in the container
 */
template <class RandomAccessIterator, class Compare>
void heapify_inplace_sort(RandomAccessIterator start, RandomAccessIterator end, Compare);


template <class RandomAccessIterator>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end)
{
    heap_sort(start, end, std::less<typename RandomAccessIterator::value_type>());
}

template <class RandomAccessIterator, class Compare>
void heap_sort(RandomAccessIterator start, RandomAccessIterator end, Compare c)
{
    typedef typename RandomAccessIterator::value_type  T;
    auto lambda_compare = [c](T t1, T t2) {return !c(t1, t2);};
    heapify(start, end, lambda_compare);
    heapify_inplace_sort(start, end, lambda_compare);
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
    // if child < parent, swap them and check the parent's parent
    if (c(*cur, *parent) && cur != start)
    {
        std::iter_swap(parent, cur);
        heapify_up(start, parent, c);
    }
}

template <class RandomAccessIterator, class Compare>
void heapify_down(RandomAccessIterator start, RandomAccessIterator end,
        RandomAccessIterator cur, Compare c)
{
    auto left_child = heapify_left_child(start, cur);
    auto right_child = heapify_right_child(start, cur);
    auto min_child = left_child;
    // Get the minimum child
    if (right_child < end)
    {
        if (c(*right_child, *left_child))
        {
            min_child = right_child;
        }
    }
    if (min_child < end)
    {
        // if child < parent, swap them and check the child's children
        if (min_child != cur)
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
        RandomAccessIterator cur)
{
    auto ind = std::distance(start, cur);
    return start + (ind * 2) + 1;
}

template <class RandomAccessIterator>
RandomAccessIterator heapify_right_child(RandomAccessIterator start,
        RandomAccessIterator cur)
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
