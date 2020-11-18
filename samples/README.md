Usage example
=============

This directory contains the [packetdecoder](nbee/packetdecoder) sample that was ported from
the [original netbee library](https://github.com/netgroup-polito/netbee/tree/master/samples/nbee/packetdecoder).

It has been simplified by providing hard-coded message blobs instead of reading them
from a PCAP file.

Other examples have not yet been ported over to the new build system. Porting them,
or writing your own application, should be straightforward, by just utilizing
the `CMakeLists.txt` file provided.

Building
--------

Enter the _packetdecoder_ directory in your bash shell:

```
cd samples/nbee/packetdecoder
```

For an out-of-tree-build, create a separate build directory and run `cmake` from there:

```
mkdir build
cd build
cmake ..
```

On Linux, you can build using `make`.

On Windows, `cmake --open .` opens the project in Visual Studio where you can choose _Build_ / _Build solution_.

The resulting binary, together with all required dynamic libraries will be located under
`build/Debug/packetdecoder.exe` on Windows or `build/packetdecoder` on Linux.

Running
-------

Prior to running the example, you will have to generate a `netpdl.xml` file. To do so, run the `netpdl_creator.sh`
script in the [`languages/misc` directory](../languages/misc) from a bash shell:

```
cd /path/to/repo/root
cd languages/misc
./netpdl_creator.sh
```

This will generate the file into the `bin/` directory.

You can now run the packet decoder example from the `build/` directory using:

```
cd /path/to/build
./Debug/packetdecoder.exe -netpdl ../../../../bin/netpdl.xml       # on windows or
./packetdecoder -netpdl ../../../../bin/netpdl.xml                 # on linux
```

This will generate the files `pdmlout.xml` and `psmlout.xml`, which you can inspect with your text editor
in the current working directory.

Example of `pdmlout.xml`:

```xml
<?xml version="1.0" encoding="ISO-8859-1" ?>
<pdml version="0" creator="NetBee Library" date="26-10-20">
<packet num="1" len="42" caplen="42" timestamp="42.000000" >
<proto name="ethernet" longname="Ethernet 802.3" pos="0" size="14">
<field name="dst" longname="MAC Destination" size="6" pos="0" value="FFFFFFFFFFFF" showvalue="FFFFFF-FFFFFF" showdtl="FFFFFF-FFFFFF (Broadcast address)" showmap="Broadcast address" />
<field name="src" longname="MAC Source" size="6" pos="6" value="0080C7CB439A" showvalue="0080C7-CB439A" showdtl="0080C7-CB439A (Unicast address, vendor Xircom, Inc.)" showmap="Xircom, Inc." />
<field name="type" longname="Ethertype - Length" size="2" pos="12" value="0806" showvalue="2054" showdtl="0x0806 (Ethertype)" />
</proto>
<proto name="arp" longname="ARP (Address Resolution Protocol)" pos="14" size="28">
<field name="hType" longname="Hardware type" size="2" pos="14" value="0001" showvalue="1" />
<field name="pType" longname="Protocol type" size="2" pos="16" value="0800" showvalue="0x0800" />
<field name="hLen" longname="Hardware address length" size="1" pos="18" value="06" showvalue="6" />
<field name="pLen" longname="Protocol address length" size="1" pos="19" value="04" showvalue="4" />
<field name="op" longname="Operation code" size="2" pos="20" value="0001" showvalue="1" showmap="Request" />
<field name="sHwAddr" longname="Sender MAC address" size="6" pos="22" value="0080C7CB439A" showvalue="0080C7-CB439A" showdtl="0080C7-CB439A (Unicast address, vendor Xircom, Inc.)" showmap="Xircom, Inc." />
<field name="sIPAddr" longname="Sender IP address" size="4" pos="28" value="82C01051" showvalue="130.192.16.81" />
<field name="dHwAddr" longname="Destination MAC address" size="6" pos="32" value="000000000000" showvalue="000000-000000" showdtl="000000-000000 (Unicast address, vendor code not available)" showmap="code not available" />
<field name="dIPAddr" longname="Destination IP address" size="4" pos="38" value="82C01011" showvalue="130.192.16.17" />
</proto>
</packet>
<packet num="2" len="60" caplen="60" timestamp="42.000000" >
...
```
