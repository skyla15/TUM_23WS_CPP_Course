#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>


template <typename T>
class Vector {
public:
    Vector(): _size{0}, _capacity{0}{} 

    /**
     * Creates a vector of size n with values default_val.
     */
    Vector(size_t n, const T& default_val): _size{n}, _data{std::make_unique<T[]>(_size)}                                    // https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/memory/unique_ptr/make_unique.html
    {
        _capacity = _size;
        std::fill(_data.get(), _data.get() + _size, default_val);
    }

    /**
     * Creates a vector containing the elements in l.
     */
    Vector(std::initializer_list<T> l): _size{l.size()}, _data{std::make_unique<T[]>(_size)}                                 // * std::initializer_list objects are typically used to initialize container classes, such as std::vector, std::set, and std::map, and other types of objects that can be initialized with a list of values.An initializer_list is a lightweight object that only holds a pointer to the underlying array and its size. It does not own the memory and do not copy the element. It's useful for uniform initialization, for example, it can be used to construct a vector, list, set and etc.
    {
        _capacity = _size;
        std::move(l.begin(), l.end(), _data.get());
    }

    // copy constructor 
    Vector(const Vector& copy) : _size{copy._size}, _capacity{copy._capacity}, _data{std::make_unique<T[]>(_capacity)}
    {
        std::copy(copy._data.get(), copy._data.get() + _size, _data.get()); 
    }

    // move constructor 
    Vector(Vector&& move) : _size{move._size}, _capacity{move._capacity}, _data{std::move(move._data)}{}

    /**
     * Replaces the contents of the vector. - copy assignment
     */
    Vector& operator=(const Vector& copy) 
    {
        if( this == &copy ) 
            return *this;

        _size = copy._size;
        _capacity = copy._capacity;

        _data = std::make_unique<T[]>(_capacity);
        std::copy(copy._data.get(), copy._data.get() + copy._size, _data.get());

        return *this;
    }

    /**
     * Replaces the contents of the vector. - move assignment
     */
    Vector& operator=(Vector&& move) noexcept 
    {
        if(this == &move)
            return *this;

        _data = std::move(move._data);
        _size = move._size;
        _capacity = move._capacity;

        // reset the values from the moved obejct
        move._size = 0;
        move._capacity = 0;
        move._data = nullptr;
        
        return *this;
    }

    size_t size() const noexcept 
    { 
        return _size; 
    }

    size_t capacity() const noexcept 
    { 
        return _capacity; 
    }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(const T& value) 
    {
        size_t new_capacity = calculate_capacity(_size + 1);
        resize(new_capacity);
        _data[_size++] = value;
    }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(T&& value) 
    { 
        size_t new_capacity = calculate_capacity(_size + 1);
        resize(new_capacity);
        _data[_size++] = std::move(value);  
    }

    /**
     * Removes the last element of the vector.
     */
    void pop_back() {
        _data[--_size] = T();
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos) const { // when called by const vector object 
        // bounds checks 
        if( pos >= _size )
        {
            throw std::out_of_range("Index out of range");
        }
        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos) {
        if( pos >= _size )
        {
            throw std::out_of_range("Index out of range");
        }
        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index) const { // when called by const vector object 
        return _data[index];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index) {
        return _data[index];
    }

    friend std::ostream& operator<<(std::ostream& o, Vector<T> v) {
        o << "Size: " << v._size << ", Capacity: " << v._capacity << std::endl;
        for (size_t i = 0; i < v._size; ++i) {
            if (i > 0)
                o << ", ";
            o << v._data[i];
        }
        o << std::endl;
        return o;
    }

    ~Vector() = default;

private:
    // Defines how the `_capacity` is increased.
    size_t growth_factor = 2;
    size_t _size;
    size_t _capacity;

    // Holds vector's data.
    std::unique_ptr<T[]> _data;

    /**
     * Calculates the necessary capacity for new_size.
     * If necessary, double `_capacity` using `growth_factor`.
     */
    size_t calculate_capacity(size_t new_size) {
        if ( _capacity == 0)
            return new_size;
        else if ( new_size <= _capacity)
            return _capacity;
        return _capacity * growth_factor;
    }

    /**
    * Resizes the vector to new_capacity.
    * If the current capacity is less than new_capacity
    * the vector moves all elements to a new array.
    */
    void resize(size_t new_capacity) {
        if(_capacity != new_capacity){
            auto new_data = std::make_unique<T[]>(new_capacity);
            std::move_backward(_data.get(), _data.get() + _size, new_data.get() + _size);
            // for( size_t i{0} ; i < _size ; ++i )
                // new_data[i] = std::move(_data[i]);
            _data = std::move(new_data);
            _capacity = new_capacity;
        }
    }
};


// auto 관련 참고 자료 : https://lakanto.tistory.com/23