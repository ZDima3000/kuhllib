// nstd/net/async_send_to.hpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_NET_ASYNC_SEND_TO
#define INCLUDED_NSTD_NET_ASYNC_SEND_TO

#include "nstd/file/async_io.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/net/get_iovec.hpp"
#include "nstd/hidden_names/message_flags.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/sequence.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/net/io_context.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_send_to {
    template <typename Socket, typename CB> struct operation;
    struct cpo;
}

// ----------------------------------------------------------------------------

template <typename Socket, typename CB>
struct nstd::net::hidden_names::async_send_to::operation {
    using completion_signature = ::nstd::execution::set_value_t(int);
    using buffer_sequence = ::nstd::type_traits::remove_cvref_t<CB>;
    template <typename Env>
    using iovec = decltype(::nstd::net::get_iovec(::nstd::type_traits::declval<Env>(), ::nstd::type_traits::declval<buffer_sequence>()));

    typename Socket::native_handle_type      d_handle;
    ::nstd::type_traits::remove_cvref_t<CB>  d_buffer;
    ::nstd::hidden_names::message_flags      d_flags;
    typename Socket::endpoint_type           d_endpoint;

    template <typename Env>
    struct state {
        ::sockaddr_storage d_address;
        iovec<Env> d_iovec;
        ::msghdr   d_msg;
        state(Env const& env, buffer_sequence const& buffer, typename Socket::endpoint_type& endpoint)
            : d_iovec(::nstd::net::get_iovec(env, buffer)) {
            this->d_msg.msg_name    = &this->d_address;
            this->d_msg.msg_namelen = endpoint.get_address(&this->d_address);
            this->d_msg.msg_iov     = this->d_iovec.data();
            this->d_msg.msg_iovlen  = this->d_iovec.size();
        }
    };
    template <typename Env>
    auto connect(Env const& env) -> state<Env> {
        return state<Env>(env, this->d_buffer, this->d_endpoint);
    }
    template <typename Env>
    auto start(::nstd::net::io_context::scheduler_type scheduler, state<Env>& s, ::nstd::file::context::io_base* cont) -> void{
        scheduler.sendmsg(this->d_handle, &s.d_msg, static_cast<int>(this->d_flags), cont);
    }
    template <::nstd::execution::receiver Receiver, typename Env>
    auto complete(int32_t rc, uint32_t, bool cancelled, state<Env>&, Receiver& receiver) -> void {
        if (cancelled) {
            ::nstd::execution::set_stopped(::nstd::utility::move(receiver));
        }
        else if (rc < 0) {
            ::nstd::execution::set_error(::nstd::utility::move(receiver), ::std::error_code(-rc, std::system_category()));
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(receiver), rc);
        }
    }
};

// ----------------------------------------------------------------------------

struct nstd::net::hidden_names::async_send_to::cpo {
    template <typename Socket, typename CB>
    friend auto tag_invoke(cpo, Socket& socket, CB&& cb, ::nstd::hidden_names::message_flags flags, typename Socket::endpoint_type endpoint) {
        return nstd::file::hidden_names::async_io_sender<::nstd::net::hidden_names::async_send_to::operation<Socket, CB>>(
            socket.native_handle(), ::nstd::utility::forward<CB>(cb), flags, endpoint
            );
    }
    template <typename Socket, typename CB>
    auto operator()(Socket& socket, CB&& cb, ::nstd::hidden_names::message_flags flags, typename Socket::endpoint_type endpoint) const {
        return ::nstd::tag_invoke(*this, socket, ::nstd::utility::forward<CB>(cb), flags, endpoint);
    }
    template <typename Socket, typename CB>
    auto operator()(Socket& socket, CB&& cb, typename Socket::endpoint_type endpoint) const {
        return ::nstd::tag_invoke(*this, socket,
                                  ::nstd::utility::forward<CB>(cb),
                                  ::nstd::hidden_names::message_flags(),
                                  endpoint);
    }
};

// ----------------------------------------------------------------------------

namespace nstd::net::inline customization_points {
    using async_send_to_t = ::nstd::net::hidden_names::async_send_to::cpo;
    inline constexpr async_send_to_t async_send_to;
}


// ----------------------------------------------------------------------------

#endif
