// src/nstd/net.hpp                                                   -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_NET
#define INCLUDED_SRC_NSTD_NET

// ----------------------------------------------------------------------------
// NetworkingTS [convenience.hdr.synop]

// #include "nstd/executor.hpp"
#include "nstd/net/io_context.hpp"
// #include "nstd/timer.hpp"
#include "nstd/buffer.hpp"
#include "nstd/socket.hpp"
#include "nstd/internet.hpp"
#include "nstd/net/async_accept.hpp"
#include "nstd/net/async_connect.hpp"
#include "nstd/net/async_read_some.hpp"
#include "nstd/net/async_receive.hpp"
#include "nstd/net/async_receive_from.hpp"
#include "nstd/net/async_send.hpp"
#include "nstd/net/async_send_to.hpp"
#include "nstd/net/async_write.hpp"
#include "nstd/net/async_write_some.hpp"
#include "nstd/net/scope.hpp"
#include "nstd/net/basic_datagram_socket.hpp"
#include "nstd/net/basic_socket.hpp"
#include "nstd/net/basic_stream_socket.hpp"

#include "nstd/net/ip/address.hpp"
#include "nstd/net/ip/address_v4.hpp"
#include "nstd/net/ip/address_v6.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/make_address_v4.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/net/ip/udp.hpp"
#include "nstd/net/ip/types.hpp"
#include "nstd/net/ip/v4_mapped.hpp"

// ----------------------------------------------------------------------------

#endif
