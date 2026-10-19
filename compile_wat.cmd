@echo off
rem TKFrame - OpenWatcom build script
if not exist dll md dll
if not exist lib md lib

set WATCOM=C:\WATCOM
set INCLUDE=C:\WATCOM\h;C:\WATCOM\h\os2
set PATH=C:\WATCOM\binp;%PATH%

wmake -f Makefile.wat clean > compile.log 2>&1
wmake -f Makefile.wat >> compile.log 2>&1

echo === Build finished ===
type compile.log
