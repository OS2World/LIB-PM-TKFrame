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
 * Test program for TKFrame custom control.
 *
 * Compile with: icc /B"/pmtype:pm" test.c test.def
 *
 */

#define INCL_WIN
#define INCL_GPI

#include <os2.h>

#include "TKFrame.h"

void main(void) {
  HAB hab;
  HMQ hmq;
  int i;
  QMSG qmsg;
  HWND hwndFrame[5], hwndClient[5];
  ULONG ul = FCF_TITLEBAR | FCF_SIZEBORDER | FCF_SYSMENU | FCF_AUTOICON;
  USHORT usStyles[] = {TKFS_FLAT, TKFS_GROOVE, TKFS_RAISED, TKFS_RIDGE, TKFS_SUNKEN};
  USHORT width = 5;
  RGB    colors[] = {{255,0,0}, {0,255,0}, {0,0,255}, {255,255,0}, {255,0,255}};
  PSZ    stTitles[] = {"Flat", "Groove", "Raised", "Ridge", "Sunken"};

  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab, 0);

  for(i=0; i<5; ++i) {
    hwndFrame[i] = WinCreateStdWindow(HWND_DESKTOP, 0, &ul, TKFRAME, stTitles[i],
                                      usStyles[i], NULLHANDLE, 0, &hwndClient[i]);
    if (hwndFrame[i] == NULLHANDLE) exit(-1);
    TKFSetBorderWidth(hwndClient[i], width);
    TKFSetColor(hwndClient[i], colors[i]);
    WinSetWindowPos(hwndFrame[i], HWND_TOP, 10+(i*150), 100, 100, 100, SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ZORDER);
  }

  while(WinGetMsg(hab,&qmsg,NULLHANDLE,0L,0L))
    WinDispatchMsg(hab,&qmsg);

  for(i=0; i<5; ++i) {
    WinDestroyWindow(hwndFrame[i]);
  }

  WinTerminate(hab);
  WinDestroyMsgQueue(hmq);
}
