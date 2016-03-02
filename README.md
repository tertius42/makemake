# makemake
This is a (tiny) utility that generates C and C++ Makefiles.

Installation:
- make
- sudo make install

Changelog:
0.1.1
- Added support for C.
- Added extensibility for more programming languages.
- Improved parameter handling:
 - added -l=[language].
 - bad parameters now cancel execution
- Improved dependencies()


0.1 - Initial "release"
- Added support for C++ files.
- Sends a usable Makefile to stdout
- Has x parameters:
 - added -c=[compiler] to replace the default compiler.
 - added -p=[prefix] to set the installation directory
 - added -f=[compiler flags]
 - added -o=[name] to set the executable's name to something other than a.out
- dependencies() uses through a cpp to find headers, then recursively looks through the header to find more headers.
- dependencies() uses #define and #ifndef to prevent include loops.
- Added clean, install, and uninstall rules.
