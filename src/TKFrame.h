/*
<A NAME=TK_FRAME>
*/
#ifndef INCL_TKFRAME
  #define INCL_TKFRAME

  #define TKFRAME "TK_FRAME"

  /* RGB to LONG conversions */
  #define RGB2L(r,g,b) (ULONG)((BYTE)(b) + (USHORT)((g)<<8) + (ULONG)((r)<<16))
  #define RGB22L(rgb) (ULONG)((BYTE)((rgb)->bBlue) + (USHORT)((rgb)->bGreen<<8) + (ULONG)((rgb)->bRed<<16))

  /* TKFrame control styles */
  #define TKFS_FLAT    0x01
  #define TKFS_GROOVE  0x02
  #define TKFS_RAISED  0x03
  #define TKFS_RIDGE   0x04
  #define TKFS_SUNKEN  0x05

  /* TKFrame Presentation Parameters */
  #define PP_TKFSTYLE  PP_USER + 1
  #define PP_TKFWIDTH  PP_USER + 2

  /* Helpers */
  #define TKFSetStyle(hwnd, style) (WinSetPresParam(hwnd, PP_TKFSTYLE, sizeof(style), &style))
  #define TKFSetBorderWidth(hwnd, width)  (WinSetPresParam(hwnd, PP_TKFWIDTH, sizeof(width), &width))
  #define TKFSetColor(hwnd, color) (WinSetPresParam(hwnd, PP_BACKGROUNDCOLOR, sizeof(color), &color))
#endif
