/**************************************************************************
Copyright (c) 1997 Stefano Fornari
Permission to use, copy, modify, and distribute this software for any
purpose is granted without fee, provided that the above copyright notice
and the following paragraph appear in all copies of this software
and supporting documentation.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY, even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  In no event shall the author be
liable to any party for direct, indirect, special, incidental, or
consequential damages arising out of the use of this software and its
documentation.

Modifications to this software may be copyrighted by their authors
and need not follow the licensing terms described here, provided that
the new terms are clearly indicated on the first page of each file where
they apply.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
MODIFICATIONS.
**************************************************************************/

/*
 *
 * Compile with : icc /Ge- /B"/NOE" TKFrame.c TKFrame.def
 *
 */

#define INCL_WIN
#define INCL_GPI

#include <os2.h>
#include <stdlib.h>

#include "TkFrame.h"

#define WINDOWWORDS (4L)

#define DEF_BGCOLOR RGB2L(0xff, 0xff, 0xff)

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


static BYTE ShadeRGBByte(BYTE brgb)
{
  #define SHADER   ( (BYTE) 0x50)

  if (brgb > SHADER)
     return (brgb - SHADER);
  else
     return (0);
}


static BYTE LightRGBByte(BYTE brgb)
{

  #define LIGHTER  ( (BYTE) 0x80)

  if (brgb < (0xff - LIGHTER) )
     return (brgb + LIGHTER);
  else
     return (0xff);
}


/*
 * QueryUserPP
 * -------------
 *
 * Query a presentation parameter
 *
 * Inputs
 *   hwnd      window handle
 *   ulID1     Presentation Parameter 1 ID
 *   ulID2     Presentation Parameter 2 ID
 *   lDefault  Default PP value
 *   lOptions  query options
 *
 * Returns
 *   The specified presentation parameters if exist, otherwise the
 *   default value will be returned.
 */
LONG QueryPP(HWND hwnd, ULONG ulID1, ULONG ulID2, LONG lDefault, LONG lOptions) {
  ULONG ulID;
  LONG  l;

  if (WinQueryPresParam(hwnd, ulID1, ulID2, &ulID, 4UL, (PVOID)&l, lOptions | QPF_NOINHERIT) )
    return l;
  else
    return lDefault;
}


/*
 *--------------------------------------------------------------
 *
 * Draw3DFrame --
 *
 *      This procedure draws frame's border according to the style
 *      specifiedc in the frame internal data.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */
void Draw3DFrame(HPS hps, PRECTL prectUpdate,
                          int x, int y,
                          int width, int height, int borderWidth,
                          LONG color, LONG l_color, LONG d_color,
                          int relief) {
        RECTL   rect;
        POINTL  points[6], pnt;
        POLYGON poly;
        LONG    topBrush, bottomBrush;

        poly.ulPoints = 5;
        poly.aPointl = &points[1];
        if ((width < (borderWidth<<1)) ||
            (height < (borderWidth<<1))) return;

        /*
         * Handle grooves and ridges with recursive calls.
         */

        if ((relief == TKFS_GROOVE) || (relief == TKFS_RIDGE)) {
                int halfWidth, insideOffset;

                halfWidth = borderWidth>>1;
                insideOffset = borderWidth - halfWidth;
                Draw3DFrame(hps, prectUpdate,
                            x, y, width, height, halfWidth,
                            color, l_color, d_color,
                            (relief == TKFS_GROOVE) ? TKFS_SUNKEN : TKFS_RAISED);
                Draw3DFrame(hps, prectUpdate,
                            insideOffset, insideOffset,
                            width - (insideOffset<<1), height - (insideOffset<<1),
                            halfWidth,
                            color, l_color, d_color,
                            (relief == TKFS_GROOVE) ? TKFS_RAISED : TKFS_SUNKEN);
                return;
        }

        if (relief == TKFS_RAISED) {
                topBrush = l_color;
                bottomBrush = d_color;
        } else if (relief == TKFS_SUNKEN) {
                topBrush = d_color;
                bottomBrush = l_color;
        } else {
                topBrush = bottomBrush = color;
        }

        GpiSetColor(hps, topBrush);

        rect.xLeft = x; rect.xRight = x+width;
        rect.yBottom = y; rect.yTop = y+borderWidth;
        WinFillRect(hps, &rect, bottomBrush);

        rect.xLeft = x+width-borderWidth; rect.xRight = x+width;
        rect.yBottom = y+borderWidth; rect.yTop = y+height;
        WinFillRect(hps, &rect, bottomBrush);

        points[0].x = points[1].x = x;
        points[0].y = y;
        points[1].y = points[2].y = y+height;
        points[2].x = x+width;
        points[3].x = x+width-borderWidth;
        points[3].y = points[4].y = y+height-borderWidth;
        points[4].x = points[5].x = x+borderWidth;
        points[5].y = y+borderWidth;

        GpiSetCurrentPosition(hps, &points[0]);
        GpiPolygons(hps, 1, &poly, BA_BOUNDARY, POLYGON_INCL);
}


/*
 *----------------------------------------------------------------------
 *
 * DisplayFrame --
 *
 * This procedure is invoked to display a frame widget.
 *
 * Inputs:
 *  hps           control presentation space
 *  prectUpdate   update rectangle
 *
 * Results:
 * None.
 *
 * Side effects:
 * Commands are output to X to display the frame in its
 * current mode.
 *
 *----------------------------------------------------------------------
 */
static void DisplayFrame(HWND hwnd, HPS hps, PRECTL prectUpdate) {
    ULONG color = QueryPP(hwnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, DEF_BGCOLOR,  QPF_ID2COLORINDEX | QPF_PURERGBCOLOR);
    ULONG style = QueryPP(hwnd, PP_TKFSTYLE, PP_TKFSTYLE, TKFS_FLAT, 0);

    /* erase frame's background */
    GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0 , NULL); /* switch color system in RGB mode */
    WinFillRect (hps, prectUpdate, color);

    if (style != TKFS_FLAT) {
       SWP swp;

       WinQueryWindowPos(hwnd, &swp);
       Draw3DFrame(hps, prectUpdate,
                   0, 0,
                   swp.cx, swp.cy,
                   QueryPP(hwnd, PP_TKFWIDTH, PP_TKFWIDTH, 2, 0),
                   color,
                   RGB2L(LightRGBByte((color & 0x00ff0000)>>16),
                         LightRGBByte((color & 0x0000ff00)>>8),
                         LightRGBByte(color & 0x000000ff)),
                   RGB2L(ShadeRGBByte((color & 0x00ff0000)>>16),
                         ShadeRGBByte((color & 0x0000ff00)>>8),
                         ShadeRGBByte(color & 0x000000ff)),
                  style);
    }
}


/*
 *----------------------------------------------------------------------
 *
 * TkFrameWndProc --
 *
 * Window procedure for a frame window.
 *
 * Input/Output
 *
 * A standard window procedure.
 *
 */
MRESULT EXPENTRY TkFrameWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
  switch (msg) {
     case WM_CREATE:
     {
        PCREATESTRUCT pcs = (PCREATESTRUCT)mp2;

        if (pcs->flStyle) {
           WinSetPresParam(hwnd, PP_TKFSTYLE, sizeof(pcs->flStyle), &pcs->flStyle);
        }

        return 0;
     }; break;

     case WM_PAINT:
     {
        RECTL rectUpdate;
        HPS   hps;

        hps = WinBeginPaint(hwnd, NULLHANDLE, &rectUpdate);
        DisplayFrame(hwnd, hps, &rectUpdate);
        WinEndPaint(hps);

        return 0L;
     }

     case WM_PRESPARAMCHANGED:
     case WM_SYSCOLORCHANGE:
        WinInvalidateRect(hwnd, NULL, TRUE);
        return (MPARAM)0;
  }

  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


/*
<A NAME=_DLL_InitTerm>_DLL_InitTerm</A>
*/
ULONG APIENTRY _DLL_InitTerm(ULONG ulModuleHandle, ULONG ulFlag) {
  if (ulFlag == 0) {
     CLASSINFO clInfo;

    /* DLL initializzation fails if a public class with name TKFRAME already exists */
    if (WinQueryClassInfo(NULLHANDLE, TKFRAME, &clInfo)) return 0;

    /*** IMPORTANT ***/
    /*
     * If you don't want that TKFrame.dll will be loaded at startup time
     * don't add the CS_PUBLIC attribute and add this line to the
     * IMPORTS section of 'yourprog.def':
     *  TKFrame._DLL_InitTerm
     *
     */
    if (!WinRegisterClass(NULLHANDLE, TKFRAME, TkFrameWndProc, CS_SIZEREDRAW /*| CS_PUBLIC*/, WINDOWWORDS)) {
       return 0;
    }
  }

  return 1;
}

