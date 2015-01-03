// nstd/iostream/io_types.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_IOSTREAM_IO_TYPES
#define INCLUDED_NSTD_IOSTREAM_IO_TYPES

#include "nstd/system_error/is_error_code_enum.hpp"
#include "nstd/system_error/error_category.hpp"
#include "nstd/system_error/error_code.hpp"
#include "nstd/system_error/error_condition.hpp"
#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    using streamoff  = signed long long;
    using streamsize = signed long long;

    enum class io_errc { stream = 1 };
    auto make_error_code(::nstd::io_errc) noexcept(true) -> ::nstd::error_code;
    auto make_error_condition(::nstd::io_errc) noexcept(true) -> ::nstd::error_condition;
    auto iostream_category() noexcept(true) -> ::nstd::error_category const&;

    template <> struct is_error_code_enum< ::nstd::io_errc>;
}

// ----------------------------------------------------------------------------

template <>
struct nstd::is_error_code_enum< ::nstd::io_errc>
    : ::nstd::type_traits::true_type {
};

// ----------------------------------------------------------------------------

#endif
