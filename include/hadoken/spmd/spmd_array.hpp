/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 * Boost Software License - Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*
*/
#ifndef SPMD_ARRAY_HPP
#define SPMD_ARRAY_HPP



#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctgmath>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <string>
#include <type_traits>

#include <hadoken/spmd/spmd_ops.hpp>


namespace hadoken {

namespace spmd {

namespace {

template<typename NumType, std::size_t N, typename Fun>
inline void simd_for_each(const NumType* a, NumType* res, Fun ops = Fun()){
    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
        res[i] = ops(a[i]);
    }
}

template<typename NumType, std::size_t N, typename Fun>
inline void simd_transform(const NumType* a, const NumType* b, NumType* c, Fun ops = Fun()){
    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
        c[i] = ops(a[i], b[i]);
    }
}

template<typename NumType, typename ResType, std::size_t N, typename Fun>
inline void simd_transform2(const NumType* a, const NumType* b, ResType* c, Fun ops = Fun()){
    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
        c[i] = ops(a[i], b[i]);
    }
}


template <typename NumType, std::size_t N, typename Fun >
inline bool simd_all_of(const NumType* a, const Fun & ops = Fun()){
    std::array<bool, N> all_bool;

    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
	all_bool[i] = ops(a[i]);
    }
    return std::all_of(all_bool.begin(), all_bool.end(), [](bool v){ return v;});
}




} // 


// simple boolean aligned array
template<std::size_t N, std::size_t Alignment>
class alignas(Alignment) boolean_array : public std::array<bool, N>{ };



template<typename NumType, std::size_t N, std::size_t Alignment = (sizeof(NumType) * N) >
class spmd_array{
public:
    typedef spmd_array<NumType, N, Alignment> spmd_array_t;
    typedef NumType value_type;
    typedef NumType* iterator;
    typedef const NumType* const_iterator;

    constexpr static std::size_t static_size = N;

    spmd_array() = default;
    spmd_array(const spmd_array_t & ) = default;
    spmd_array& operator = (const spmd_array_t & ) = default;

    HADOKEN_FORCE_INLINE spmd_array(std::initializer_list<NumType> init_list){
        std::copy_n(init_list.begin(), N, begin());
    }



    HADOKEN_FORCE_INLINE iterator begin(){
        return &(_d[0]);
    }

    HADOKEN_FORCE_INLINE const_iterator begin() const{
        return &(_d[0]);
    }

    HADOKEN_FORCE_INLINE iterator end(){
        return begin() + N;
    }

    HADOKEN_FORCE_INLINE const_iterator end() const{
        return begin() + N;
    }


    ///
    /// simple packed / packed sum
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator + (const spmd_array_t & other) const{
        return plus(*this, other);
    }


    ///
    /// simple packed /scalar sum
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator + ( NumType scalar) const{
        return plus(*this, scalar);
    }


    ///
    /// simple packed/packed sub
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator - (const spmd_array_t & other) const{
        return minus(*this, other);
    }

    ///
    /// simple packed/scalar  sub
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator - ( NumType scalar) const{
        return minus(*this, scalar);
    }


    ///
    /// simple packed multiplication
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator * (const spmd_array_t & other) const{
        return mul(*this, other);
    }

    ///
    /// simple packed/scalar mul
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator * ( NumType scalar) const{
        return mul(*this, scalar);
    }


    ///
    /// simple packed division
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator/ (const spmd_array_t & other) const{
        return div(*this, other);
    }

    ///
    /// simple packed/scalar mul
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator / ( NumType scalar) const{
        return div(*this, scalar);
    }


    ///
    ///
    ///
    HADOKEN_FORCE_INLINE bool operator==(const spmd_array_t & other) const{
        spmd_array<bool, N> res;
        simd_transform2<NumType, bool, N, std::equal_to<NumType> >(begin(), other.begin(), res.begin());
        return simd_all_of<bool, N>(res.begin(), [](bool status){ return status;});
    }


    ///
    ///
    ///
    HADOKEN_FORCE_INLINE NumType operator[](std::size_t pos) const {
        return _d[pos];
    }

    ///
    ///
    ///
    HADOKEN_FORCE_INLINE NumType& operator[](std::size_t pos){
        return _d[pos];
    }


    ///
    ///
    ///
    HADOKEN_FORCE_INLINE NumType* data(){
        return &(_d[0]);
    }


    ///
    /// compile time range-checked getter
    ///
    template<std::size_t Pos>
    HADOKEN_FORCE_INLINE NumType get() const{
        static_assert( Pos < N, "get<Pos>: pos should be inferior to array size N ");
        return _d[Pos];
    }

    ///
    /// compile time range-checked setter
    ///
    template<std::size_t Pos>
    HADOKEN_FORCE_INLINE void set(const NumType v) const{
        static_assert( Pos < N, "get<Pos>: pos should be inferior to array size N ");
        _d[Pos] = v;
    }



    ///
    /// get size
    ///
    HADOKEN_FORCE_INLINE constexpr std::size_t size() const{
        return N;
    }


    ///
    /// get size
    ///
    HADOKEN_FORCE_INLINE constexpr std::size_t alignment(){
        return Alignment;
    }


private:

    alignas(Alignment) NumType _d[N];

};



///
/// ostream serlializer
///
template<typename NumType, std::size_t N, std::size_t Alignment>
std::ostream& operator<<(std::ostream& os, const spmd_array<NumType, N, Alignment> & arr)
{
    os << '{';
    bool delimiter = false;
    std::for_each(arr.begin(), arr.end(), [&](const NumType & v){
       os <<  ( (delimiter) ? (",") : ("") );

       if(sizeof(NumType) == sizeof(char)){
            // convert char with a numerical formating
            os << (static_cast<int>(v) & 0xff );
       } else {
            os << (static_cast<int>(v) & 0xff );
       }
       delimiter = true;
    });
    os << '}';

    return os;
}


///
/// to string
///
template<typename NumType, std::size_t N, std::size_t Alignment>
std::string to_string(const spmd_array<NumType, N, Alignment> & arr){
    std::ostringstream res;
    res << arr;
    return res.str();
}






} // spmd


} // hadoken

#endif // SPMD_ARRAY_HPP

