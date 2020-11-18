NetBee -- An Advanced Library for Packet Processing (patched)
=============================================================

This repository contains patches to the
[original NetBee library (Politecnico di Torino)](https://github.com/netgroup-polito/netbee)

Overview
--------

NetBee is a library intended for network packet processing. It (not only) features the
NetBee Packet Decoder, which utilizes the NetPDL language and decodes and parses various
packet protocols and formats.

Changelog
---------

The following changes were made to the original library:

 - Ported to Visual Studio 2019 and 64 bit architectures.
 - Modernized the build system (now uses CMake 3.13).
 - Added support for out-of-tree builds.
 - Simplified the build process on windows by directly integrating
   [winflexbison](https://github.com/lexxmark/winflexbison) into the
   build system.
 - Made the PCAP dependency optional.

Please note that not all parts of this project have been modernized. Only
the core library was updated, while some tools or samples were not
and might fail to build now or have been removed.

Usage example
-------------

To use the netbee library in your own projects built with CMake, we recommend to have a look
at the [example program](samples/) and [its README](samples/README.md).

Essentially, you will need to follow these three steps:

1. Copy [the `dependencies/netbee` directory](samples/nbee/packetdecoder/dependencies/netbee)
   into your project.
2. Add `add_subdirectory(dependencies/netbee)` to your root `CMakeLists.txt` file.
3. Make your target depend on `netbee` e.g. using `target_link_libraries(your_target PUBLIC netbee)`.

An example `CMakeLists.txt` can be found [here](samples/nbee/packetdecoder/CMakeLists.txt).

Build Instructions
------------------

In case you want to build the library alone, see [Instructions for compiling the library](readme-compile.md)

Documentation
-------------

The documentation can be generated using the `createdoc.sh` script. You need to have Doxygen installed
for this.

License
-------

The code in this repository can be redistributed and used under the terms of the
[GNU Lesser General Public License version 2.1](http://www.gnu.org/licenses/lgpl-2.1.html).

For details, see [license.txt](license.txt) and [the license text](lgpl-2.1.txt)

