// decimal-literals.hpp                                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
//                                                                       
//  Permission is hereby granted, free of charge, to any person          
//  obtaining a copy of this software and associated documentation       
//  files (the "Software"), to deal in the Software without restriction, 
//  including without limitation the rights to use, copy, modify,        
//  merge, publish, distribute, sublicense, and/or sell copies of        
//  the Software, and to permit persons to whom the Software is          
//  furnished to do so, subject to the following conditions:             
//                                                                       
//  The above copyright notice and this permission notice shall be       
//  included in all copies or substantial portions of the Software.      
//                                                                       
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING         
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR        
//  OTHER DEALINGS IN THE SOFTWARE. 
// ----------------------------------------------------------------------------

#ifndef INCLUDED_DECIMAL_LITERALS
#define INCLUDED_DECIMAL_LITERALS

#include "decimal.hpp"

// ----------------------------------------------------------------------------

namespace kuhllib
{
    template <char...> constexpr decimal<32>  operator"" _df();
    template <char...> constexpr decimal<32>  operator"" _DF();
    template <char...> constexpr decimal<64>  operator"" _dd();
    template <char...> constexpr decimal<64>  operator"" _DD();
    //-dk:TODO template <char...> constexpr decimal<128> operator"" _dl();
    //-dk:TODO template <char...> constexpr decimal<128> operator"" _DL();

    namespace detail
    {
        template <int Bits>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               bool negative,
                                               int  exponent) {
            return decimal<Bits>(false, significand, negative? -exponent: exponent);
        }
        template <int Bits, char Digit, char...C>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               bool negative,
                                               int  exponent) {
            return parse_exponent<Bits, C...>(significand, negative, exponent * 10 + Digit - '0');
        }

        template <int Bits>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               int exponent) {
            return decimal<Bits>(false, significand, exponent);
        }
        template <int Bits, char Digit, char...C>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               int exponent) {
            return Digit == '-' || Digit == '+'
                ? parse_exponent<Bits, C...>(significand, Digit == '-', exponent)
                : parse_exponent<Bits, C...>(significand, false, exponent * 10 + Digit - '0');
        }

        template <int Bits>
        constexpr decimal<Bits> parse_decimal(typename decimal_config<Bits>::rep_type significand) {
            return decimal<Bits>(false, significand, 0);
        }
        template <int Bits, char Digit, char... C>
        constexpr decimal<Bits> parse_decimal(typename decimal_config<Bits>::rep_type significand) {
            return Digit == 'E'
                ? parse_exponent<Bits, C...>(significand, 0)
                : parse_decimal<Bits, C...>(significand * 10u + Digit - '0');
        }

        template <int Bits, char... C>
        constexpr decimal<Bits> parse_decimal() {
            return parse_decimal<Bits, C...>(typename decimal_config<Bits>::rep_type());
        }
    }
}

// ----------------------------------------------------------------------------

template <char... C>
constexpr kuhllib::decimal<32>
kuhllib::operator"" _df() {
    return kuhllib::detail::parse_decimal<32, C...>();
}
template <char... C>
constexpr kuhllib::decimal<32>
kuhllib::operator"" _DF() {
    return kuhllib::detail::parse_decimal<32, C...>();
}

template <char... C>
constexpr kuhllib::decimal<64>
kuhllib::operator"" _dd() {
    return kuhllib::detail::parse_decimal<64, C...>();
}
template <char... C>
constexpr kuhllib::decimal<64>
kuhllib::operator"" _DD() {
    return kuhllib::detail::parse_decimal<64, C...>();
}

#if 0
//-dk:TODO decimal<128> literals
template <char... C>
constexpr kuhllib::decimal<128>
kuhllib::operator"" _dl() {
    return kuhllib::detail::parse_decimal<128, C...>();
}
template <char... C>
constexpr kuhllib::decimal<128>
kuhllib::operator"" _DL() {
    return kuhllib::detail::parse_decimal<128, C...>();
}
#endif

// ----------------------------------------------------------------------------

#endif
