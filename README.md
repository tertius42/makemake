# makemake
This is a (tiny) utility that generates Makefiles.

Installation:
- make
- sudo make install

Usage:
makemake [parameters] *.* > Makefile
- -c=[compiler]		Change default compiler
- -p=[prefix]		Change installation prefix
- -f=[compiler flags]	Add compiler flags
- -g			Adds the debug compiler flag
- -o=[executable]	Add an executable 
- -l=[language]		Change language - default C++
It is necessary to to have the equals sign before the sub-parameter. Else, the program will see it as a file name, then throw an error. If you need to use spaces in your parameter, use quotes.

Changelog:

0.1.2
- Fixed/added some comments
- Removed -Wall in release Makefile
- Improved parameters
 - Parameters that require an additional parameter now require a third character
- Added usage to README.md

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
- Has 4 parameters:
 - added -c=[compiler] to replace the default compiler.
 - added -p=[prefix] to set the installation directory
 - added -f=[compiler flags]
 - added -o=[name] to set the executable's name to something other than a.out
- dependencies() uses through a cpp to find headers, then recursively looks through the header to find more headers.
- dependencies() uses #define and #ifndef to prevent include loops.
- Added clean, install, and uninstall rules.
