#pragma once
#include <concepts>

template <int N, std::integral T = int>
struct fibonacci {
    // store Nth factorial 
    static constexpr T value = fibonacci<N-1>::value + fibonacci<N-2>::value;
};

// partial specialization for 1
template <typename T>
requires std::integral<T>
struct fibonacci<0, T>{
    static constexpr T value = 0;
};

template <typename T>
requires std::integral<T>
struct fibonacci<1, T>{
    static constexpr T value = 1;
};




