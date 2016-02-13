
#include <iostream>

// STL learning namespace
namespace stll
{
    template<typename T> class vector {
        constexpr static size_t SCALE_FACTOR = 2;
        typedef T*    iterator;

        protected:
            size_t m_size;
            size_t m_capacity;
            T * m_data;

        public:
        /*
         * Default constructor for the vector
         */
        vector() : m_size(0), m_capacity(0), m_data(nullptr)
        {
        }

        vector(size_t count, const T & val) : vector()
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

        /*
         * Destructor
         */
        ~vector()
        {
            while(this->m_size > 0)
            {
                this->pop_back();
            }
            operator delete(this->m_data);
            this->m_data = nullptr;
            this->m_capacity = 0;
        }

        void grow()
        {
            size_t new_capacity = (size_t)(this->m_size * SCALE_FACTOR);
            if (new_capacity == 0)
            {
                new_capacity = 1;
            }

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

        /*
         * Trying to implement an emplace back
         */
        template<class ...Args>
        void emplace_back(Args&&... args)
        {
            if (this->m_size == this->m_capacity)
            {
                this->grow();
            }

            // this->m_data[this->m_size] = T(args...);
            // Use perfect forwarding to avoid move issues when using lvalues
            new(this->m_data + this->m_size) T(std::forward<Args...>(args...));
            this->m_size ++;
        }

        /*
         * Trying to implement a pop_back.
         */
        void pop_back(void)
        {
            // Need to guard this in debug mode
            -- this->m_size;
            this->m_data[this->m_size].~T();
        }

        /*
         */
        bool empty(void)
        {
            return this->m_size == 0;
        }

        /*
         * Get and set operators
         */
        T & operator [](size_t index)
        {
            // Need to guard this in debug mode
            return this->m_data[index];
        }

        /*
         */
    };
}
