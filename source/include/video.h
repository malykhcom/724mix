#ifndef __VIDEO_H__
#define __VIDEO_H__

extern ULONG VideoX;
extern ULONG VideoY;

APIRET VideoInit(VOID);
VOID VideoCls(VOID);
ULONG VideoWriteStr(PCSZ, ULONG, ULONG, CHAR);
VOID VideoWriteChar(CHAR, ULONG, ULONG, ULONG, CHAR);
VOID VideoRefresh(VOID);
VOID VideoRefreshLine(ULONG);
VOID VideoHideCursor(VOID);
VOID VideoShowCursor(VOID);
VOID VideoSetCursorPos(ULONG, ULONG);

#endif
