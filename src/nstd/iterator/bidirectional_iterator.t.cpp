// nstd/iterator/bidirectional_iterator.t.cpp                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

#include "nstd/iterator/bidirectional_iterator.hpp"
#include "nstd/iterator/bidirectional_iterator_tag.hpp"
#include "nstd/iterator/iterator_traits.hpp"
#include <cstddef>
#include "kuhl/test.hpp"

namespace NI = ::nstd::iterator;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("bidirectional_iterator<T> is a bidirectional iterator",
                       [](KT::context& c)->bool{
        return KT::assert_type<NI::iterator_traits<NI::bidirectional_iterator<int*>>::iterator_category,
                               NI::bidirectional_iterator_tag>(c, "iterator category")
            && KT::assert_type<NI::iterator_traits<NI::bidirectional_iterator<int*>>::value_type,
                               int>(c, "value type")
            && KT::assert_type<NI::iterator_traits<NI::bidirectional_iterator<int*>>::pointer,
                               int*>(c, "pointer type")
            && KT::assert_type<NI::iterator_traits<NI::bidirectional_iterator<int*>>::reference,
                               int&>(c, "reference type")
            && KT::assert_type<NI::iterator_traits<NI::bidirectional_iterator<int*>>::difference_type,
                               std::ptrdiff_t>(c, "difference type")
            ;
        }),
    KT::expect_success("bidirectional_iterator<T> operations",
                       [](KT::context& c)->bool{
            int array[] = { 1, 2 };
            NI::bidirectional_iterator<int*> it  = NI::bidirectional_begin(array);
            NI::bidirectional_iterator<int*> begin = NI::bidirectional_begin(array);
            NI::bidirectional_iterator<int*> end = NI::bidirectional_end(array);
            return KT::assert_false(c, "it == end", it == end)
                && KT::assert_true(c,  "it != end", it != end)
                && KT::assert_equal(c, "*it == 1",  *it, 1)
                && KT::assert_equal(c, "&*it == &array[0]",  &*it, &array[0])
                && KT::assert_true(c,  "it++ == begin", it++ == begin)
                && KT::assert_true(c,  "it != end", it != end)
                && KT::assert_equal(c, "*it == 2",  *it, 2)
                && KT::assert_equal(c, "&*it == &array[1]",  &*it, &array[1])
                && KT::assert_true(c,  "++it == end", ++it == end)
                && KT::assert_true(c,  "it-- == end", it-- == end)
                && KT::assert_equal(c, "&*it == &array[1]",  &*it, &array[1])
                && KT::assert_true(c,  "--it == end", --it == begin)
                && KT::assert_equal(c, "&*it == &array[0]",  &*it, &array[0])
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("test bidirectional iterator wrapper", ac, av, ::tests);
}
