# LIB-PM-TKFrame

TKFrame v0.1 - a custom Presentation Manager frame control for OS/2.

## What it is

TKFrame is a simple custom control to create nice frames, inspired by the
frame widget in the Tcl/Tk toolkit. It uses presentation parameters to manage
its shape and color.

Styles: `TKFS_FLAT`, `TKFS_GROOVE`, `TKFS_RAISED`, `TKFS_RIDGE`, `TKFS_SUNKEN`.

Presentation parameters: `PP_TKFSTYLE`, `PP_TKFWIDTH`, and background color.

## Project structure

```
dll\      Built DLL
lib\      Import library (if generated)
h\        Public header (TKFrame.h)
src\      Library source code
sample\   Test application
doc\      Documentation (tkframe.html)
```

## Build

Set `WATCOM` to the OpenWatcom root, then run:

```
compile_wat.cmd
```

or

```
wmake -f Makefile.wat
```

## Usage

TKFrame can be used as a local control (linked as a DLL) or installed as a
public system control via `install.cmd`.

## License

BSD 3-Clause License (see LICENSE).
