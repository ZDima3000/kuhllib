// include/execution.hpp                                              -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CXXRT_EXECUTION
#define INCLUDED_CXXRT_EXECUTION

#include <execution/connect.hpp>
#include <execution/operation_state.hpp>
#include <execution/sender.hpp>
#include <execution/sender_traits.hpp>
#include <execution/set_done.hpp>
#include <execution/set_error.hpp>
#include <execution/set_value.hpp>
#include <execution/start.hpp>

#include <stdexcept>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    struct receiver_invocation_error;

    //-dk:TODO using invocable_archetype = unspecified;

    namespace customization
    {
        // --------------------------------------------------------------------

        void execute();
        struct execute_t
        {
        };
        inline constexpr execute_t execute_cp{};

        // --------------------------------------------------------------------

        void submit();
        struct submit_t
        {
        };
        inline constexpr submit_t submit_cp{};

        // --------------------------------------------------------------------

        void schedule();
        struct schedule_t
        {
        };
        inline constexpr schedule_t schedule_cp{};

        // --------------------------------------------------------------------

        void bulk_execute();
        struct bulk_execute_t
        {
        };
        inline constexpr bulk_execute_t bulk_execute_cp{};

        // --------------------------------------------------------------------

    }
    inline namespace customization_points
    {
        inline constexpr auto execute      = customization::execute_cp;
        inline constexpr auto submit       = customization::submit_cp;
        inline constexpr auto schedule     = customization::schedule_cp;
        inline constexpr auto bulk_execute = customization::bulk_execute_cp;
    }

    //-dk:TODO template<class S, class R>
    //-dk:TODO using connect_result_t = invoke_result_t<decltype(connect), S, R>;

#if 0
    template<class T, class E = exception_ptr>
    concept receiver = see-below;

    template<class T, class... An>
    concept receiver_of = see-below;

    template<class R, class... An>
    inline constexpr bool is_nothrow_receiver_of_v =
        receiver_of<R, An...> &&
        is_nothrow_invocable_v<decltype(set_value), R, An...>;

    template<class S>
    concept typed_sender = see-below;

    template<class S, class R>
    concept sender_to = see-below;

    template<class S>
    concept scheduler = see-below;

    template<class E>
    concept executor = see-below;

    template<class E, class F>
    concept executor_of = see-below;
#endif

    namespace execution_detail { struct sender_base {}; }
    using execution_detail::sender_base;

    struct context_t {};
    constexpr context_t context;

    struct blocking_t {};
    constexpr blocking_t blocking;

    struct blocking_adaptation_t {};
    constexpr blocking_adaptation_t blocking_adaptation;

    struct relationship_t {};
    constexpr relationship_t relationship;

    struct outstanding_work_t {};
    constexpr outstanding_work_t outstanding_work;

    struct bulk_guarantee_t {};
    constexpr bulk_guarantee_t bulk_guarantee;

    struct mapping_t {};
    constexpr mapping_t mapping;

    template <typename ProtoAllocator>
    struct allocator_t {};
    constexpr allocator_t<void> allocator;

    template<class Executor> struct executor_shape;
    template<class Executor> struct executor_index;

    template<class Executor> using executor_shape_t = typename executor_shape<Executor>::type;
    template<class Executor> using executor_index_t = typename executor_index<Executor>::type;

    class bad_executor;
    
    template <class... SupportableProperties> class any_executor;

    template<class Property> struct prefer_only;

} // namespace cxxrt::execution

// ----------------------------------------------------------------------------

struct cxxrt::execution::receiver_invocation_error
    : std::runtime_error
    , std::nested_exception
{
    receiver_invocation_error() noexcept
        : std::runtime_error("invocation error")
        , std::nested_exception()
    {
    }
};

// ----------------------------------------------------------------------------


#endif
