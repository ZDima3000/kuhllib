// nstd/execution/parallel_policy.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_PARALLEL_POLICY
#define INCLUDED_NSTD_EXECUTION_PARALLEL_POLICY

#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/execution/sequenced_policy.hpp"
#include "nstd/base/for_each.hpp"
#include "nstd/algorithm/distance.hpp"
#include "nstd/execution/thread_pool_executor.hpp"
#include "nstd/iterator/random_access.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include <vector>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        struct parallel_policy {
            unsigned size;
            parallel_policy operator()(unsigned s) const;
        };
        constexpr parallel_policy par{65536u};

        template <typename MultiPass, typename EndPoint, typename Callable>
        ::nstd::type_traits::enable_if_t<!::nstd::iterator::is_random_access<EndPoint>::value>
        map(::nstd::execution::parallel_policy const&,
            MultiPass begin, EndPoint end, Callable fun);
        template <typename MultiPass, typename EndPoint, typename Callable>
        ::nstd::type_traits::enable_if_t<::nstd::iterator::is_random_access<EndPoint>::value>
        map(::nstd::execution::parallel_policy const&,
            MultiPass begin, EndPoint end, Callable fun);

        template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
        auto reduce(::nstd::execution::parallel_policy const&,
                    FwdIt it, EndPoint end, Init init, Reduce op)
            -> ::nstd::type_traits::enable_if_t<!::nstd::iterator::is_random_access<EndPoint>::value,
                                                decltype(op(*it, *it))>;
        template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
        auto reduce(::nstd::execution::parallel_policy const&,
                    FwdIt it, EndPoint end, Init init, Reduce op)
            -> ::nstd::type_traits::enable_if_t<::nstd::iterator::is_random_access<EndPoint>::value,
                                                decltype(op(*it, *it))>;

        template <typename RndIt, typename EndPoint, typename Compare>
        auto sort(::nstd::execution::parallel_policy const&,
                    RndIt it, EndPoint end, Compare compare)
            -> void;
    }

    template <>
    struct is_execution_policy<::nstd::execution::parallel_policy>
        : public ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
::nstd::type_traits::enable_if_t<!::nstd::iterator::is_random_access<EndPoint>::value>
nstd::execution::map(::nstd::execution::parallel_policy const&,
                     MultiPass cur, EndPoint end, Callable fun) {
    //-dk:TODO support parallel version for non-random access
    ::nstd::base::for_each(cur, end, fun);
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
::nstd::type_traits::enable_if_t<::nstd::iterator::is_random_access<EndPoint>::value>
nstd::execution::map(::nstd::execution::parallel_policy const& policy,
                     MultiPass cur, EndPoint end, Callable fun) {
    ::nstd::execution::thread_pool_executor executor; //-dk:TODO use the argument?
    auto size(policy.size);
    for (size = ::nstd::algorithm::distance(cur, end);
         policy.size <= size; size -= policy.size) {
        auto tmp(cur + policy.size);
        executor.add([=](){ ::nstd::base::for_each(cur, tmp, fun); });
        cur = tmp;
    }
    if (cur != end) {
        executor.add([=](){ ::nstd::base::for_each(cur, end, fun); });
    }
    executor.process();
}

// ----------------------------------------------------------------------------

template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
auto ::nstd::execution::reduce(::nstd::execution::parallel_policy const&,
                               FwdIt it, EndPoint end, Init init, Reduce op)
    -> ::nstd::type_traits::enable_if_t<!::nstd::iterator::is_random_access<EndPoint>::value,
                                        decltype(op(*it, *it))> {
    //-dk:TODO parallel version of reduce with non-random access
    return reduce(::nstd::execution::seq, it, end, init, op);
}

template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
auto ::nstd::execution::reduce(::nstd::execution::parallel_policy const& policy,
                               FwdIt it, EndPoint end, Init init, Reduce op)
    -> ::nstd::type_traits::enable_if_t<::nstd::iterator::is_random_access<EndPoint>::value,
                                        decltype(op(*it, *it))> {
    if (it == end) {
        return init;
    }
    using result_type = decltype(op(*it, *it));
    auto size(end - it);
    auto chunk_size(policy.size);
    auto chunks(size / chunk_size + bool(size % chunk_size));
    std::vector<result_type> range;
    {
        ::nstd::execution::thread_pool_executor executor; //-dk:TODO use the argument?
        int no_threads(executor.number_of_threads());
        if (int(chunks) < no_threads && 1 < size / (no_threads * 4)) {
            chunks = no_threads * 4;
            chunk_size = size / chunks;
            chunks += bool(size / chunk_size);
        }
        range.resize(chunks, init);

        for (typename std::vector<result_type>::size_type i(0); i != range.size() - 1u; ++i) {
            executor.add([&result = range[i], it, chunk_size, init, op]() {
                    result = reduce(::nstd::execution::seq, it, it + chunk_size, init, op);
                });
            it += chunk_size;
        }
        executor.add([&result = range.back(), it, end, init, op]() {
                result = reduce(::nstd::execution::seq, it, end, init, op);
            });
        executor.process();
    }
    return reduce(::nstd::execution::seq, range.begin(), range.end(), init, op);
}

// ----------------------------------------------------------------------------

inline
::nstd::execution::parallel_policy
nstd::execution::parallel_policy::operator()(unsigned s) const {
    return ::nstd::execution::parallel_policy{s};
}

// ----------------------------------------------------------------------------

template <typename RndIt, typename EndPoint, typename Compare>
auto nstd::execution::sort(::nstd::execution::parallel_policy const&,
                           RndIt it, EndPoint end, Compare compare)
    -> void {
    ::nstd::execution::sort(::nstd::execution::seq, it, end, compare);
}
// ----------------------------------------------------------------------------

#endif
