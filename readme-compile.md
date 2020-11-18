# How to compile NetBee

This section is useful only for the programmers that want to modify and 
recompile the NetBee sources.

Please note that this patched version of NetBee is not intended to be compiled
as stand-alone library, but instead to be used as dependency within a CMake
project. Read [the readme](README.md) and have a look at [the example](samples/)
for more information.

## Compiling NetBee on Windows

First, get the NetBee sources:

```
git clone https://github.com/barbara-moser/netbee
```

### Required tools

In order to compile NetBee in Windows, you need the following tools:

* **Microsoft Visual Studio** (2019 works, but any other version should do)

* [**CMake**](https://www.cmake.org) >=3.13	
	The building environment is created by CMake; hence you need this tool
	in order to create the project files (Visual Studio makefiles). We suggest
	that you add the directory containing `cmake.exe` to the `PATH` environment
	variable.

### Compiling NetBee

You are now ready to compile NetBee on Windows.

1. First, create the Visual Studio solution, e.g. by typing the following in
   your terminal (e.g. `cmd.exe` or MINGW bash):

   ```
   cd path\to\netbee
   mkdir build
   cd build
   cmake ..
   ```

2. Open the solution in Visual Studio, e.g. via `cmake --open .` (while still
   in the `build` directory).

3. Choose _Build_ -> _Build solution_.

4. Get a valid NetPDL database.	
   You can use the NetPDL database provided with the netbee
   library. Details on how to use this database can be found
   [here](languages/misc/README.md).


## Compiling NetBee on Unix-like systems

Currently we compiled NetBee on Linux Ubuntu 12.04, Mac OS X Lion and 
FreeBSD 8.3.
Since NetBee is cross platform, we do not expect problems in compiling the 
library on other platforms.

### Required tools

* [**CMake**](https://www.cmake.org) >=3.13.
	The building process is managed through CMake. Therefore you need this tool in order to create 
	the project files. Please install this package using the tools provided by your Unix 
	distribution.
	Note: version >=3.13 is required. You can check the version installed in your system by issuing the 
	command `cmake --version`		
	from your favourite shell. If your distribution does not provide the correct version through the 
	package manager, please download the tarball from the CMake site and install it manually. 
	On Ubuntu Linux you can use `sudo apt-get install cmake`.
	On Mac OS X you can get the Universal .dmg package from the [CMake site](https://www.cmake.org), or use 
	the Darwin Ports (http://darwinports.com/) tool: `sudo port install cmake`

* **GNU GCC**.	
	If missing on your Ubuntu Linux you can use `sudo apt-get install gcc build-essential`

* **Libpcap** (optional).
	The library used to capture traffic from your network interfaces and to parse capture files. 
	Although the binaries should be unstalled by default by the operating system, the system headers 
	may not.
	On Ubuntu Linux you can use apt-get to install the appropriate package (binaries and development
        files):
		
		sudo apt-get install libpcap libpcap-dev
	
	On Mac OS X:
		
		sudo port install libpcap
	
	Please note that latest netbee revisions require libpcap 0.9.8 for correctly compiling, so make 
	sure your system is up to date. For instance, on Ubuntu 7.04 the libpcap0.8 package contains the 
	version 0.9.5 of the library, while on Ubuntu 7.10 you will find the version 0.9.7 of libpcap.

* **libsqlite3** (optional).
	This library is optional; it may be required if some compilation flags are turned on in order to 
	dump some data on a SQLite3 database. Often it is already present on your systen; in case it is 
	missing, you can use the package manager available in your Unix distribution.
	On Ubuntu Linux:
	
		sudo apt-get install libsqlite3 libsqlite3-dev
	
	On Mac OS X you can retrieve it through Darwin Ports (http://darwinports.com/):
	
		sudo port install sqlite3

* A recent version of **flex and bison**.
	These tools are needed in order to build the parsers of the several compilers included in the 
	NetBee library. You should install them using the package manager available in your Unix 
	distribution.
	On Ubuntu Linux you can use apt-get:
	
		sudo apt-get install flex bison
	
	On Mac OS X you can use the Darwin Ports tool (http://darwinports.com/):
	
		sudo port install flex
		sudo port install bison

### Compiling NetBee

1.	In order to compile the NetBee library and the tools you need first to create the build system 
	using cmake. So you need to type into a shell:

	```	
	cd /path/to/netbee
	mkdir build
	cd build
	cmake ..
	```
	
	Then you can build the NetBee library by issuing the make command:
	
	```
	make
	```
	
	The build process should proceed smoothly, although some warnings may be present (please don't 
	care about them). At the end you should find a set of libraries and of course the NetBee shared 
	library in the netbee/bin folder.

2. 	Finally, you will need to get a valid NetPDL database for protocol definitions.
	You can use the NetPDL database provided with the netbee library. Details on how
	to use this database can be found [here](languages/misc/README.md).


# Possible problems

**Note**: This section is left here for reference, but might be outdated.

## CMake returns the "LINK : fatal error LNK1104: cannot open file 'user32.lib'"

There is nothing we can do with this error. It is related to CMake, not to our environment. A 
suggestion can be found 
[here](http://people.mech.kuleuven.be/~tdelaet/bfl_doc/installation_guide/node15.html).

However, what we have notices is that this error is more likely to happen if you have Visual Studio 
C++, instead of Visual Studio .NET 2005. So, in case you do not have Visual .NET, please install that 
compiler and see is now works.

## CMake returns the "CMAKE_CXX_COMPILER-NOTFOUND error"

This means that the user has not installed g++ or another C++ compiler that cmake knows about. On an 
Ubuntu system, it is easy to install gcc and not realize g++ did not come bundled with it.

## Some module fails to compile

This can be due to the impossibility to generate the grammar files with Flex/Bison. In this case 
please make sure that Flex and Bison are properly installed (in Windows, they are part of the CygWin 
package) and are in your path.

## In DEBUG mode, I get tons of strange messages on screen

This is due to the "libpcre" module, which creates tons of debug messages when compiled in DEBUG mode. 
This issue can be solved by compiling this module in RELEASE mode.


# Enabling Native Backends

The NetVM can geneate native code for several platforms (Intel x86, Intel x64, Cavium Octeon, 
Xelerated X11). The former two are supported through Just-In-Time techniques, while the latters are 
supported through Ahead-Of-Time code generation (i.e., the output must be further processed by the 
native compiler for that platform in order to get the actual code).

The first backend is the most widely tested, while the second is still work in progress. The latters 
are really experimental.

When you want to deal with the generation of native code, you may have to take into account the 
following steps:

* Enable the compilation of the backend in the makefile: by default, only the NetVM interpreter 
  is turned on. You can select the additional native backends you want by editing the 
  "src/nbnetvm/CMakeLists.txt" file and turn the proper option on.
  
* Disable the Execution Disable bit on your Intel platform: on latest operating systems (e.g., 
  Linux), the Execution Disable bit available on the newest Intel hardware is turned on by 
  default. This prevents memory pages that are marked as 'data' to be executed. However, this is
  what we use to do in dynamic code gneration: we create dynamically the code in memory and then 
  we execute it. The NetBee library on Linux has some additional code that is able to avoid this 
  problem, but in general other operating systems (e.g. MacOS X) may not be able to execute the 
  dynamically generated code. In case you experience a crash each time you try to execute native
  code (most tools and examples have the '-jit' switch, which turns the execution of native code 
  on), please check this on your platform and try disabling the Execute Disable bit in your 
  operating system.
