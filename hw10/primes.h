#pragma once

#include <cmath>

template<typename T> 
constexpr auto gcd(T a, T b)
{
    while ( b != 0 )
    {
        auto t = b;
        b = a % b;
        a = t;
    }
    return a;
}

template<typename T, typename ... N>
constexpr auto gcd(T a, T b, N ... args)
{
    return gcd(a, gcd(b, args ...));
}



template<typename T>
constexpr auto mcm(T a, T b)
{
    return a * b / gcd(a, b);
}

template<typename T, typename ... N>
constexpr auto mcm(T a, T b, N ... args)
{
    return mcm(a, mcm(b, args ...));
}


template <int base, int exponent, int modulus> 
struct Power{
    static constexpr int value = base * Power<base, exponent - 1, modulus>::value % modulus;
};

template<int base, int modulus> 
struct Power<base, 0, modulus>{
    static constexpr int value = 1 % modulus;
};