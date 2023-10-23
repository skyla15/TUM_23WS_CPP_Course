#pragma once

#include <array>

template <typename T, typename U, typename V, typename W> 
struct Quadruple {
public:
    constexpr Quadruple(const T& t, const U& u, const V& v, const W& w): first{t}, second{u}, third{v}, fourth{w}{}
    constexpr ~Quadruple() = default; 

    constexpr T get_first() const { return first; }
    constexpr U get_second() const { return second; }
    constexpr V get_third() const { return third; }
    constexpr W get_fourth() const { return fourth; }

private:
    T first;
    U second;
    V third;
    W fourth;
};

// Partial Specialization 
template <typename T>
struct Quadruple<T, T, T, T>{
public:
    constexpr Quadruple(const T& t, const T& u, const T& v, const T& w)
    {
        members[0] = t;
        members[1] = u;
        members[2] = v;
        members[3] = w;
    }
    constexpr ~Quadruple() = default;

    constexpr T get_first() const { return members[0]; }
    constexpr T get_second() const { return members[1]; }
    constexpr T get_third() const { return members[2]; }
    constexpr T get_fourth() const { return members[3]; }

    std::array<T, 4> members;
};


// template specialization for the 4 int input 
template<>
struct Quadruple<int, int, int, int> { // use the type traits here ! -> non-type template 
public:
    constexpr Quadruple(int t, int u, int v, int w)
    {
        members[0] = t;
        members[1] = u;
        members[2] = v;
        members[3] = w;
    }
    constexpr ~Quadruple() = default;

    constexpr int get_first() const { return members[0]; }
    constexpr int get_second() const { return members[1]; }
    constexpr int get_third() const { return members[2]; }
    constexpr int get_fourth() const { return members[3]; }

    std::array<int, 4> members;
};


// partial specialization for the case where it uses the same type again 
