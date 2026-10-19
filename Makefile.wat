# Makefile.wat - TKFrame built with OpenWatcom 2.0 for OS/2

CC     = wcc386
CFLAGS = -bt=os2 -mf -3 -zq -i=h;src;C:\WATCOM\h;C:\WATCOM\h\os2
DLLDIR = dll
LIBDIR = lib
SRCDIR = src
SMPLDIR = sample

all : $(DLLDIR)\TKFrame.dll $(SMPLDIR)\test.exe

# --- DLL ---

$(DLLDIR)\TKFrame.obj : $(SRCDIR)\TKFrame.c h\TKFrame.h
    if not exist $(DLLDIR) md $(DLLDIR)
    $(CC) $(CFLAGS) -fo=$@ src\TKFrame.c

$(DLLDIR)\TKFrame.dll : $(DLLDIR)\TKFrame.obj $(SRCDIR)\tkframe.def
    if not exist $(DLLDIR) md $(DLLDIR)
    if not exist $(LIBDIR) md $(LIBDIR)
    wlink system os2v2 dll name $@ file { $(DLLDIR)\TKFrame.obj } @src\tkframe.def option quiet library os2386.lib library clib3r.lib

# --- sample ---

$(SMPLDIR)\test.exe : $(SMPLDIR)\test.c $(SMPLDIR)\test.def h\TKFrame.h
    if not exist $(SMPLDIR) md $(SMPLDIR)
    $(CC) $(CFLAGS) -i=$(SMPLDIR) -fo=$(SMPLDIR)\test.obj $(SMPLDIR)\test.c
    wlink system os2v2_pm name $@ file { $(SMPLDIR)\test.obj } option quiet

clean : .SYMBOLIC
    -if exist $(DLLDIR)\*.obj del $(DLLDIR)\*.obj
    -if exist $(DLLDIR)\*.dll del $(DLLDIR)\*.dll
    -if exist $(SMPLDIR)\*.obj del $(SMPLDIR)\*.obj
    -if exist $(SMPLDIR)\*.exe del $(SMPLDIR)\*.exe
    -if exist $(SMPLDIR)\*.res del $(SMPLDIR)\*.res
