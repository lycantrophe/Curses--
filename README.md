Curses++
========

C++ Curses widget set.

The what
--------
Curses++ is aiming to be a simple, usable and somewhat flexible widget set and
abstraction layer for C++ over ncurses. We all love text interfaces, but plain
ncurses can occasionally be a drag. Curses++ is here to help you with the house
keeping and to provide C++ native objects behaving like widgets as an
alternative to draw directly on a canvas through an opaque pointer.

The why
-------
Fun, mostly. Hopefully the library will be both useable and used, but the
primary goal is education and experimentation. Curses++ usees plenty of
techniques such as composition, mixins and strict RAII, as well as relying
heavily on C++11 features. This is done for (mainly) three reasons:
* Familiarize myself with these techniques, their applications, strengths,
  weaknesses and implementations.
* See how ineheritance-less code will behave and perform
* Teach myself C++11 features.


The how
-------
Currently I am not supplying any means for either using, packaging or
distributing the code. It is, of course, freely available, but not supported.
Interfaces and models may change without notice. However, if you would like to
try it or use it, here is what you will need:
* A C++11 ready compiler. It is currently tested and built with gcc4.7.3 and
  llvm/clang3.2
* ncurses development libraries

Include your header and build your project as usual. Remember to include the
ncurses link flag.

    g++ -std=c++0x project.cpp curses++.cpp -o project -lncurses

As no binary packages are distrubted (which may never happen. this is C++ after
all :---)), you must also compile in the curses++.cpp file.

A more complete usage manual will be written and distributed with this project
at a later time.


Licence
-------
Curses++ is licenced under the GPLv3: http://www.gnu.org/licenses/gpl.html
