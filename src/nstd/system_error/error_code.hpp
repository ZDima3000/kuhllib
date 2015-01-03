// nstd/system_error/error_code.hpp                                   -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_SYSTEM_ERROR_ERROR_CODE
#define INCLUDED_NSTD_SYSTEM_ERROR_ERROR_CODE

#include "nstd/system_error/error_category.hpp"
#include "nstd/system_error/error_condition.hpp"
#include "nstd/system_error/is_error_code_enum.hpp"
#include "nstd/string/string_fwd.hpp"
#include "nstd/type_traits/enable_if.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    class error_code;
}

// ----------------------------------------------------------------------------

class nstd::error_code {
public:
    error_code() noexcept(true);
    error_code(int, ::nstd::error_category const&) noexcept(true);
    template <typename ErrorCode,
              typename = ::nstd::type_traits::enable_if_t< ::nstd::is_error_code_enum<ErrorCode>::value> >
    error_code(ErrorCode) noexcept(true);

    auto assign(int, ::nstd::error_category const&) noexcept(true) -> void;
    template <typename ErrorCode,
              typename = ::nstd::type_traits::enable_if_t< ::nstd::is_error_code_enum<ErrorCode>::value> >
    auto operator= (ErrorCode) noexcept(true) -> error_code&;
    auto clear() noexcept(true) -> void;

    auto value() const noexcept(true) -> int;
    auto category() const noexcept(true) -> ::nstd::error_category const&;
    auto default_error_condition() const noexcept(true) -> ::nstd::error_condition;
    auto message() const -> ::nstd::string;
    explicit operator bool() const noexcept(true);
};

// ----------------------------------------------------------------------------

#endif
