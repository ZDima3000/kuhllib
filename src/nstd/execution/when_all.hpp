// nstd/execution/when_all.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_EXECUTION_WHEN_ALL
#define INCLUDED_NSTD_EXECUTION_WHEN_ALL

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/value_types_of_t.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include "nstd/utility/forward.hpp"
#include <atomic>
#include <exception>
#include <optional>
#include <tuple>
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::when_all {
    template <typename> struct receiver;
    struct state_base;
    template <::nstd::execution::sender> struct inner_state_args;
    template <::nstd::execution::sender> struct inner_state;
    template <::nstd::execution::receiver, ::nstd::execution::sender...> struct state;
    template <::nstd::execution::sender...> struct sender;

    template <typename> struct set_value_from_tuple_impl;
    template <typename... T>
    struct set_value_from_tuple_impl<::std::tuple<T...>> {
        using type = ::nstd::execution::set_value_t(T...);
    };
    template <typename T>
        using set_value_from_tuple = typename ::nstd::hidden_names::when_all::set_value_from_tuple_impl<T>::type;

    struct cpo {
        template <::nstd::execution::sender... Sender>
        friend auto tag_invoke(cpo, Sender&&... s) -> ::nstd::hidden_names::when_all::sender<Sender...> {
            return  { { ::nstd::utility::forward<Sender>(s)... } };
        }

        template <::nstd::execution::sender... Sender>
            requires (0ul != sizeof...(Sender))
                  && requires(cpo const& c, Sender&&... s) {
                    { ::nstd::tag_invoke(c, ::nstd::utility::forward<Sender>(s)...) } -> ::nstd::execution::sender;
                  }
        auto operator()(Sender&&... sender) const {
            return ::nstd::tag_invoke(*this, ::nstd::utility::forward<Sender>(sender)...);
        }
    };
}

// ----------------------------------------------------------------------------

namespace nstd::execution {
    using when_all_t = ::nstd::hidden_names::when_all::cpo;
    inline constexpr ::nstd::execution::when_all_t when_all{};

#if 0
    inline constexpr struct when_all_t
    {
        template <::nstd::execution::receiver Receiver>
        struct common {
            ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
            ::std::atomic<::std::size_t>                  d_count;
            auto complete() -> void {
                if (--this->d_count == 0u) {
                    ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver));
                }
            }
        };
        template <::nstd::execution::receiver Receiver>
        struct receiver {
            common<Receiver>* d_common;
            friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept {
                return ::nstd::execution::get_env(self.d_common->d_receiver);
            }
            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, auto&&...)
                noexcept -> void {
                self.d_common->complete();
            }
            friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, auto&&)
                noexcept -> void {
                self.d_common->complete();
            }
            friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self)
                noexcept -> void {
                self.d_common->complete();
            }
        };
        template <::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        struct nested_state_builder {
            Sender            d_sender;
            common<Receiver>* d_common;
        };
        template <::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        struct nested_state {
            decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                ::nstd::type_traits::declval<receiver<Receiver>>())) d_state;
            nested_state(nested_state_builder<Sender, Receiver>&& b)
                : d_state(::nstd::execution::connect(::nstd::utility::move(b.d_sender),
                                                     receiver<Receiver>{b.d_common}))
            {
            }
            auto start() noexcept -> void { ::nstd::execution::start(this->d_state); }
        };
        template <::nstd::execution::receiver Receiver, ::nstd::execution::sender... Sender>
        struct state {
            common<Receiver>                                d_common;
            ::std::tuple<nested_state<Sender, Receiver>...> d_state;

            template <::nstd::execution::receiver R>
            state(R&& r, Sender&&... s)
                : d_common{::nstd::utility::forward<R>(r), sizeof...(Sender)}
                , d_state(nested_state_builder<Sender, Receiver>{::nstd::utility::move(s), &this->d_common}...)
            {
            }
            state(state&&) = delete;
            state(state const&) = delete;
            friend auto tag_invoke(::nstd::execution::start_t, state& s)
                noexcept -> void {
                ::std::apply([](auto&... st){ (st.start(), ...); }, s.d_state);
            }
        };
        template <::nstd::execution::sender... Sender>
        struct sender {
            using completion_signatures = ::nstd::execution::completion_signatures<
                    //-dk:TODO fix when_all completion_signals
                    ::nstd::execution::set_value_t(),
                    ::nstd::execution::set_stopped_t()
                >;

            ::std::tuple<::nstd::type_traits::remove_cvref_t<Sender>...> d_sender;
            template <::nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t,
                                   sender&& s,
                                   Receiver&& r)
                noexcept -> state<Receiver, Sender...> {
                return ::std::apply([&r](auto&&... s){
                    return state<Receiver, Sender...>(
                        ::nstd::utility::forward<Receiver>(r),
                        ::nstd::utility::move(s)...);
                    }, ::nstd::utility::move(s.d_sender));
            }
        };
        template <::nstd::execution::sender... Sender>
        auto operator()(Sender&&... s) const {
            return sender<Sender...>{ { ::nstd::utility::forward<Sender>(s)... } };
        }
        auto operator()() const { // deal with the odd case of no senders
            return ::nstd::execution::just();
        }
    } when_all;
#endif
}

// ----------------------------------------------------------------------------

struct nstd::hidden_names::when_all::state_base {
    ::std::atomic<::std::size_t> d_count;

    state_base(::std::size_t count): d_count(count) {}
    state_base(state_base&&) = delete;
    auto complete() -> void {
        ::std::cout << "when_all complete\n";
        if (--this->d_count == 0u) {
            this->do_complete();
        }
        else {
            ::std::cout << "not completing\n";
        }
        ::std::cout << "when_all complete done\n";
    }
    virtual auto do_complete() -> void {}
};

// ----------------------------------------------------------------------------

template <typename Result>
struct nstd::hidden_names::when_all::receiver {
    state_base&              d_state;
    ::std::optional<Result>& d_result;

    friend auto tag_invoke(::nstd::execution::get_env_t, receiver const&) noexcept
        -> int {
        return {};
    }

    template <typename... T>
    friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, T&&... args) noexcept
         -> void {
        ::std::cout << "when_all set_value\n";
        self.d_result.emplace(::nstd::utility::forward<T>(args)...);
        self.d_state.complete();
        ::std::cout << "when_all set_value done\n";
    }
    template <typename E>
    friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, E&&) noexcept
         -> void {
        //-dk:TODO set error
        self.d_state.complete();
    }
    friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept
         -> void {
        self.d_state.complete();
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender>
struct nstd::hidden_names::when_all::inner_state_args {
    ::nstd::hidden_names::when_all::state_base& d_outer_state;
    Sender&                                     d_sender;

    inner_state_args(::nstd::hidden_names::when_all::state_base& outer_state,
                     Sender&                                     sender)
        : d_outer_state(outer_state)
        , d_sender(sender) {
    }
};

template <::nstd::execution::sender Sender>
struct nstd::hidden_names::when_all::inner_state {
    using result_t = ::nstd::execution::value_types_of_t<Sender, int, ::nstd::hidden_names::decayed_tuple, ::nstd::type_traits::type_identity_t>;
    using state_t = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                        ::nstd::type_traits::declval<::nstd::hidden_names::when_all::receiver<result_t>>()));

    ::nstd::hidden_names::when_all::state_base& d_outer_state;
    ::std::optional<result_t> d_result;
    state_t                   d_state;

    inner_state(inner_state_args<Sender> const& args)
        : d_outer_state(args.d_outer_state)
        , d_result()
        , d_state(::nstd::execution::connect(args.d_sender,
                                             ::nstd::hidden_names::when_all::receiver<result_t>{this->d_outer_state, this->d_result}))
    {
    }
    inner_state(inner_state&&) = delete;

    auto start() noexcept -> void {
        ::std::cout << ::std::unitbuf << "inner start\n";
        ::nstd::execution::start(this->d_state);
        ::std::cout << "inner start done\n";
    }
    auto result() -> result_t { return *this->d_result; }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, ::nstd::execution::sender... Sender>
struct nstd::hidden_names::when_all::state
    : ::nstd::hidden_names::when_all::state_base
{
    using state_t = ::std::tuple<::nstd::hidden_names::when_all::inner_state<Sender>...>;
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    state_t                                       d_state;

    friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept
        -> void {
        ::std::cout << "when_all start()\n" << std::flush;
        ::std::apply([](auto&... inner){ (inner.start(), ...); }, self.d_state);
        ::std::cout << "when_all start() done\n" << std::flush;
    }

    template<::nstd::execution::receiver R, typename Tuple>
    state(R&& receiver, Tuple&& sender)
        : ::nstd::hidden_names::when_all::state_base(sizeof...(Sender))
        , d_receiver(::nstd::utility::forward<R>(receiver))
        , d_state(::std::apply([this](auto&&...s){ return ::std::make_tuple(::nstd::hidden_names::when_all::inner_state_args<Sender>(*this, s)...); },
                               ::nstd::utility::forward<Tuple>(sender))) 
    {
        ::std::cout << "when_all(" << sizeof...(Sender) << ") constructed\n";
    }

    auto do_complete() -> void override {
        ::std::cout << "when_all::do_complete!\n" << ::std::flush;
        ::std::apply([this](auto&&... a){ ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), a...); },
                     ::std::apply([](auto&&... t){ return ::std::tuple_cat(t.result()...); }, this->d_state)
                    );
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender... Sender>
struct nstd::hidden_names::when_all::sender
{
    ::std::tuple<Sender...> d_sender;

    template <typename Env>
    friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, sender const&, Env&&)
        -> ::nstd::execution::completion_signatures<
            ::nstd::hidden_names::when_all::set_value_from_tuple<
                decltype(
                    ::std::tuple_cat(
                        ::nstd::type_traits::declval<
                            ::nstd::execution::value_types_of_t<Sender, Env, ::std::tuple, ::nstd::type_traits::type_identity_t>
                        >()
                        ...
                    )
                )
            >
        >
    {
        return {};
    }
    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& receiver)
        -> ::nstd::hidden_names::when_all::state<Receiver, Sender...> {
            return ::nstd::hidden_names::when_all::state<Receiver, Sender...>(
                ::nstd::utility::forward<Receiver>(receiver),
                ::nstd::utility::move(self.d_sender)
            );
    }
};

// ----------------------------------------------------------------------------

#endif
