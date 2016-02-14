
#include <iostream>

// STL learning namespace
namespace stll
{

template <typename T> T min(const T & a, const T & b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}


template<typename T> class vector {
    typedef T*    iterator;

    protected:
        constexpr static size_t SCALE_FACTOR = 2;
        T * m_data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
        void grow();
        void _delete();
        template<class ...Args> void unsafe_emplace_back(Args&&... args);

    public:
        /**
         * Use a default constructor
         */
        vector(void) = default;

        /**
         * Create a vector with count elements, initilized to val
         */
        vector(size_t count, const T & val);

        /**
         * Copy constructor
         */
        vector(const vector<T> & other);

        /**
         * Move constructor
         */
        vector(vector<T> && other) noexcept;

        /**
         * Copy assignment operator
         */
        vector<T> & operator=(const vector<T> & other);

        /**
         * Move assignment operator
         */
        vector<T> & operator=(vector<T> && other) noexcept;

        /**
         * Free the constructor
         */
        ~vector();

        void assign(typename vector<T>::iterator b, typename vector<T>::iterator e);

        /**
         * Construct a new element T with the provided arguments
         */
        template<class ...Args> void emplace_back(Args&&... args);

        /**
         * Access a reference to the element stored at the provided index
         */
        T & operator [](size_t index) const noexcept;

        /**
         *
         */
        void pop_back(void) noexcept;
        
        /**
         */
        bool empty(void) const noexcept;

        /**
         *
         */
        void ensure_capacity(size_t capacity);

        iterator begin(void) const noexcept;

        iterator end(void) const noexcept;

        size_t size(void) const noexcept;
};

template<typename T>
vector<T>::vector(size_t count, const T & val) : vector()
{
    std::cout << "Constructing vec" << std::endl;
    // Keep valid empty data
    if (count == 0)
    {
        return;
    }

    this->m_data = operator new(sizeof(T) * count);
    this->m_size = 0;
    this->m_capacity = count;

    for (size_t i = 0; i < count; i++)
    {
        new(this->m_data + i) T(val);
    }
    this->m_size = count;
}

template<typename T>
vector<T>::vector(const vector<T> & other)
{
    this->ensure_capacity(other.m_size);
    for (auto a : other)
    {
        this->unsafe_emplace_back(a);
    }
}

template<typename T>
vector<T>::vector(vector<T> && other) noexcept :
    m_data{other.m_data}, m_size{other.m_size}, m_capacity{other.m_capacity}
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template<typename T>
vector<T> & vector<T>::operator=(const vector<T> & other)
{
    if (this == &other) return *this;

    this->ensure_capacity(other.size());
    size_t n = min<size_t>(other.m_size, this->m_size);
    for (size_t i = 0; i < n; i++)
    {
        this->m_data[i] = other.m_data[i];
    }

    for (size_t i = n; i < other.m_size; i++)
    {
        this->unsafe_emplace_back(other.m_data[i]);
    }

    while(this->m_size > other.m_size)
    {
        this->pop_back();
    }

    return *this;
}

template<typename T>
void vector<T>::assign(vector<T>::iterator b, vector<T>::iterator e)
{
    ensure_capacity(e - b);
    
    size_t n_other = e - b;
    size_t n = min<size_t>(e - b, this->size());
    size_t i = 0;


    auto t_it = this->begin();
    auto o_it = b;
    auto t_end = this->begin() + n;
    while (t_it < t_end)
    {
        *t_it = *o_it;
        ++t_it;
        ++o_it;
    }
    i += n;

    while (i < n_other)
    {
        this->unsafe_emplace_back(o_it);
        ++ i;
        ++ o_it;
    }

    while (i < this->m_size)
    {
        this->pop_back();
    }
}

template<typename T>
vector<T> & vector<T>::operator=(vector<T> && other) noexcept
{
    if (this == &other) return *this;

    this->_delete();

    this->m_data = other.m_data;
    this->m_size = other.m_size;
    this->m_capacity = other.m_capacity;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;

    return *this;
}


template<typename T>
vector<T>::~vector()
{
    this->_delete();
}

/**
 * Trying to implement an emplace back
 */
template<typename T>
template<class ...Args>
void vector<T>::emplace_back(Args&&... args)
{
    if (this->m_size == this->m_capacity)
    {
        this->grow();
    }

    // this->m_data[this->m_size] = T(args...);
    // Use perfect forwarding to avoid move issues when using lvalues
    unsafe_emplace_back(std::forward<Args...>(args...));
}

template<typename T>
template<class ...Args>
void vector<T>::unsafe_emplace_back(Args&&... args)
{
    new(this->m_data + this->m_size) T(std::forward<Args...>(args...));
    this->m_size ++;
}

/**
 * Delete the last element in the vector
 */
template<typename T>
void vector<T>::pop_back(void) noexcept
{
    // Need to guard this in debug mode
    -- this->m_size;
    this->m_data[this->m_size].~T();
}

/**
 * Test if the vector is empty
 */
template<typename T>
bool vector<T>::empty(void) const noexcept
{
    return this->m_size == 0;
}

/**
 * Get and set operators
 */
template<typename T>
T & vector<T>::operator [](size_t index) const noexcept
{
    // Need to guard this in debug mode
    return this->m_data[index];
}

/*
 */
template<typename T>
void vector<T>::grow()
{
    size_t new_capacity = (size_t)(this->m_size * SCALE_FACTOR);
    if (new_capacity == 0)
    {
        new_capacity = 1;
    }

    ensure_capacity(new_capacity);
}

template<typename T>
void vector<T>::ensure_capacity(size_t new_capacity)
{
    if (new_capacity < this->m_capacity) return;

    // Check that new_size > m_size?
    T * tmp_data = (T*) (operator new(sizeof(T) * new_capacity));
    for (size_t i = 0; i < this->m_size; i++)
    {
        tmp_data[i] = std::move(this->m_data[i]);
    }
    operator delete(this->m_data);
    this->m_data = tmp_data;
    this->m_capacity = new_capacity;
}

template<typename T>
void vector<T>::_delete(void)
{
    while (!this->empty())
    {
        this->pop_back();
    }
    operator delete(this->m_data);

    this->m_capacity = 0;
    this->m_size = 0;
    this->m_data = nullptr;
}

template<typename T>
typename vector<T>::iterator vector<T>::begin(void) const noexcept
{
    return this->m_data;
}

template<typename T>
typename vector<T>::iterator vector<T>::end(void) const noexcept
{
    return this->m_data + this->m_size;
}

template<typename T>
size_t vector<T>::size(void) const noexcept
{
    return this->m_size;
}

}
