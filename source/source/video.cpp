#include "global.h"
#include "video.h"

ULONG VideoX;
ULONG VideoY;

static USHORT Len;
static PCHAR Screen;

static  PCHAR Offset(ULONG x, ULONG y)
{
    return &Screen[(y*VideoX+x)*2];
}

APIRET VideoInit(VOID)
{
    APIRET rc;
    {
        VIOMODEINFO mode;
        mode.cb=sizeof(mode);
        rc=VioGetMode(&mode, 0);
        if (rc)
            return rc;
        VideoX=mode.col;
        VideoY=mode.row;
    }
    {
        ULONG addr;
        rc=VioGetBuf(&addr, &Len, 0);
        if (rc)
            return rc;
        Screen=(PCHAR)(((addr>>3) & 0xffff0000) | (addr & 0xffff));
    }
    return NO_ERROR;
}

VOID VideoCls(VOID)
{
    ULONG t;
    ULONG len=Len/2;
    for(t=0;t<len;t++)
    {
        Screen[t*2]=' ';
        Screen[t*2+1]=0x7;
    }
}

ULONG VideoWriteStr(PCSZ str, ULONG x, ULONG y, CHAR attr)
{
    ULONG t;
    ULONG len=strlen(str);
    PCHAR tmp;
    if (x>=VideoX || y>=VideoY)
        return 0;
    len=min(len, VideoX-x);
    tmp=Offset(x, y);
    for(t=0;t<len;t++)
    {
        tmp[0]=str[t];
        tmp[1]=attr;
        tmp+=2;
    }
    return len;
}

VOID VideoWriteChar(CHAR chr, ULONG len, ULONG x, ULONG y, CHAR attr)
{
    ULONG t;
    PCHAR tmp;
    if (x>=VideoX || y>=VideoY)
        return;
    len=min(len, VideoX-x);
    tmp=Offset(x, y);
    for(t=0;t<len;t++)
    {
        tmp[0]=chr;
        tmp[1]=attr;
        tmp+=2;
    }
}

VOID VideoRefresh(VOID)
{
    VioShowBuf(0, Len, 0);
}

VOID VideoRefreshLine(ULONG y)
{
    VioShowBuf((USHORT)(y*VideoX*2), (USHORT)(VideoX*2), 0);
}

VOID VideoHideCursor(VOID)
{
    VIOCURSORINFO cursor;
    VioGetCurType(&cursor, 0);
    cursor.attr=-1;
    VioSetCurType(&cursor, 0);
}

VOID VideoShowCursor(VOID)
{
    VIOCURSORINFO cursor;
    VioGetCurType(&cursor, 0);
    cursor.attr=0;
    VioSetCurType(&cursor, 0);
}

VOID VideoSetCursorPos(ULONG x, ULONG y)
{
    VioSetCurPos(x, y, 0);
}
