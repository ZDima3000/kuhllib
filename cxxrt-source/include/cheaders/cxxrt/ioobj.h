// -*-C++-*- cxxrt/ioobj.h
// -------------------------------------------------------------------------- 
// Copyright (c) 2002 Dietmar Kuehl

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------------------- 
// Author:  Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:   Declaration of the standard stream objects
// Version: $Id: ioobj.h,v 1.1.1.1 2002/06/05 01:02:06 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IOOBJ_H__)
#define _CXXRT_IOOBJ_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

#if !defined(_CXXRT_IOSFWD_H__)
#  include <cxxrt/iosfwd.h>
#endif
#if !defined(_CXXRT_ISTREAM_H__)
#  include <cxxrt/istream.h>
#endif
#if !defined(_CXXRT_OSTREAM_H__)
#  include <cxxrt/ostream.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------

extern istream cin;
extern ostream cout;
extern ostream cerr;
extern ostream clog;

extern wistream wcin;
extern wostream wcout;
extern wostream wcerr;
extern wostream wclog;

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_IOOBJ_H__ */
