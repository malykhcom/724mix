/* Using USL Generic I/O Driver [Copyright (c) 1999 Vadim Rumyantsev]. */

#include "global.h"
#include "memio.h"

#pragma pack(1)

#define IOCTL_CAT_USER        0x80
#define GENIO_MAP             0x02

struct USLMAP
{
    /* Out */
    ULONG  UserVirtual;
    /* In */
    ULONG  Physical;
    USHORT Size;
    USHORT Reserved;
};

#pragma pack()

PCHAR memioPointer;

APIRET MemioMakePointer(ULONG baseaddr, USHORT size)
{
    APIRET rc;
    HFILE uslgio;
    ULONG action;
    struct USLMAP param;
    ULONG paramlen;
    rc=DosOpen("\\DEV\\USLGIO$", &uslgio, &action, 0, 0, FILE_OPEN,
               OPEN_SHARE_DENYNONE, NULL);
    if (!rc)
    {
        param.Size=size;
        param.Physical=baseaddr;
        paramlen=sizeof(param);
        rc=DosDevIOCtl(uslgio, IOCTL_CAT_USER, GENIO_MAP,
                       &param, paramlen, &paramlen, NULL, 0, NULL);
        if (!rc)
            memioPointer=(PCHAR)param.UserVirtual;
        DosClose(uslgio);
    }
    return rc;
}
