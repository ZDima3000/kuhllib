// kuhl/test/context.cpp                                              -*-C++-*-
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

#include "kuhl/test/context.hpp"

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

kuhl::test::context_sbuf::context_sbuf()
    : KM::streambuf() {
    this->reset();
}

auto kuhl::test::context_sbuf::reset() -> void {
    this->setp(buffer, buffer + sizeof(buffer) - 1);
}

auto kuhl::test::context_sbuf::empty() const -> bool {
    return this->pbase() == this->pptr();
}

auto kuhl::test::context_sbuf::c_str() const -> char const* {
    *this->pptr() = 0;
    return this->pbase();
}

// ----------------------------------------------------------------------------

kuhl::test::context::context()
    : kuhl::test::context_sbuf()
    , KM::ios(this)
    , KM::ostream(this) {
}

kuhl::test::context::~context() {
}

auto kuhl::test::context::reset() -> void {
    this->kuhl::test::context_sbuf::reset();
}

auto kuhl::test::context::empty() const -> bool {
    return this->kuhl::test::context_sbuf::empty();
}

auto kuhl::test::context::c_str() const -> const char* {
    return this->kuhl::test::context_sbuf::c_str();
}
