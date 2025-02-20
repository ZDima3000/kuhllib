// toy-udp-client.cpp                                                 -*-C++-*-
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

#include "toy-networking.hpp"
#include "toy-task.hpp"
#include <utility>
#include <cerrno>
#include <cstring>

// ----------------------------------------------------------------------------

int main() {
    toy::io_context context;

    toy::socket client{ ::socket(PF_INET, SOCK_DGRAM, 0) };
    toy::address addr(AF_INET, htons(12345), INADDR_ANY);

    context.spawn([](toy::socket& client, auto const& addr)->toy::task<toy::io_context::scheduler> {
        while (true) {
            toy::address tmp{addr};
            char         buffer[16];
            std::size_t  n = co_await toy::async_receive_from(client, toy::buffer(buffer), tmp);
            (std::cout << "received='").write(buffer, n) << "'\n";
        }
    }(client, addr));

    context.spawn([](toy::socket& client, auto const& addr)->toy::task<toy::io_context::scheduler> {
        toy::socket stdin{0};
        while (true) {
            char buffer[16];
            std::size_t n = co_await toy::async_read_some(stdin, buffer, sizeof(buffer));
            std::size_t r = co_await toy::async_send_to(client, toy::buffer(buffer, n), addr);
            (std::cout << "sent='").write(buffer, r) << "'\n";
        }
    }(client, addr));

    context.run();
}
