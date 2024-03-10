#ifndef __MEMIO_H__
#define __MEMIO_H__

/* Using USL Generic I/O Driver. Copyright (c) 1999 Vadim Rumyantsev */

extern PCHAR MemioPointer;

APIRET MemioMakePointer(ULONG, USHORT);
static VOID MemioWriteW(ULONG reg, USHORT data)
{
    PUSHORT tmp=(PUSHORT)(MemioPointer+reg);
    *tmp=data;
}

static USHORT MemioReadW(ULONG reg)
{
    PUSHORT tmp=(PUSHORT)(MemioPointer+reg);
    return *tmp;
}

#endif
