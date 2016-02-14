ofxSquash
=========

![exampleVideoGrabberAndGui](https://raw.githubusercontent.com/elliotwoods/ofxSquash/master/exampleVideoGrabberAndGui/screenshot.png)

[Squash](https://quixdb.github.io/) is an open source library for compressing data, and comes with a range of compression algorithms. ofxSquash is a C++ wrapper for that libary for use with openFrameworks and supports both simple and stream API's. 

Comprsesion algorithms
----------------------

Squash ships with plugin implementations for the following algorithms:

* brieflz
* brotli
* bsc
* bzip2
* copy (no compression)
* crush
* csc
* density
* doboz
* fari
* fastlz
* gipfeli
* heatshrink
* libdeflate
* lz4
* lzf
* lzg
* lzham
* lzjb
* lzma
* lzo
* miniz
* ms-compress
* ncompress
* pithy
* quicklz
* snappy
* wflz
* yalz77
* zlib-ng
* zlib
* zling
* zpaq
* zstd

There's a really helpful system at https://quixdb.github.io/squash-benchmark/ which can help you choose which algorithm to pick for your data.

Unsupported algorithms
----------------------

The following plugins do not work on these platforms:

### OSX

* csc
* ncompress
* quicklz

### Windows

* csc
* doboz
* pithy

Compatability
=============

This addon is tested and working with :

* openFrameworks 0.9.0+
* Visual Studio 2015 x64 (Windows 10)
* XCode 7.2 x64 (OSX 10.11) 

Note : On OSX, the `ncompress` plugin doesn't work (causes a crash), so we generally remove it.

License
=======

ofxSquash
---------
> Copyright (c) 2015, Kimchi and Chips
> 
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Squash
------
> Squash is licensed under the MIT License, which is highly permissive and allows Squash to be integrated into any application virtually without restriction. That said, please keep in mind that some plugins use libraries which are subject to more restrictive terms (such as the GNU GPL), and using Squash as an intermediary does not release you from the obligations of those licenses if you choose to actually use plugins which use those libraries. Squash includes an API for programmatically determining the license of a plugin, and it is possible to use it to avoid any GPL'd plugins even if they are installed. Each plugin's page in the C documentation lists the license of the library, or libraries, it uses.
> 
> Several of the GPL libraries used by Squash plugins are also available for integration with proprietary software under commercial licenses.
