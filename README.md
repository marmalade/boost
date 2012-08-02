Introduction
-------------

This is a part of Boost libraries converted to be used with Marmalade SDK.
The libraries were converted in a straitforward way by creating mkf files
with the list of all boost libraries sources. The list of modifications
to Boost source code was minimal and listed below in this file.

This package was created by applying this sequence of commands:
 1. bcp smart_ptr regex format conversion signal system
 2. boost_convert_tests.py
 3. Apply manual corrections

Thus the package includes the full version of the listed libraries 
and their dependancies:
 - smart_ptr
 - regex
 - format
 - conversion
 - signal
 - system

This package is based on boost 1.49.0 release


How to use it
-------------

To start using boost libraries in your project please follow the steps:
 1. Download the sources of the package
 2. Create ``boost`` folder under root folder of your project
 3. Move the downloaded sources to the recently created ``boost`` folder
 3. Add ``subproject boost`` command to your project mkb file


List of manual corrections to boost libraries
---------------------------------------------

 - boost/detail/lightweight_thread.hpp - declare lw_thread_routine as static
 - libs/regex/src/posix_api.cpp - avoid code paths when ``sprintf_s`` is used
 - libs/regex/src/wide_posix_api.cpp - avoid code paths when ``wcscpy_s`` is used


Known limitations
-----------------

 - lexical_cast is not supported because ``swprintf`` function is not implemented
   for ARM builds in SDK 6.0


