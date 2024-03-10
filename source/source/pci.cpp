#include "global.h"
#include "pci.h"

#define IOCTL_OEMHLP          0x0080

/* Function */

#define OEMHLP_PCI            0x000B

/* Subfunction */

#define PCIBIOS_QUERY_INFORMATION       0x00
#define PCIBIOS_FIND_CLASS_CODE         0x02
#define PCIBIOS_READ_REGISTER           0x03
#define PCIBIOS_WRITE_REGISTER          0x04

#pragma pack(0)

struct PCIBIOS_PARAM_0
{
    UCHAR SubFunction;
};

struct PCIBIOS_PARAM_2
{
    UCHAR SubFunction;
    UCHAR ProgIF;
    UCHAR SubClass;
    UCHAR Class;
    UCHAR Reserved;
    UCHAR Index;
};

struct PCIBIOS_PARAM_3
{
    UCHAR SubFunction;
    UCHAR Bus;
    UCHAR DevFunc;
    UCHAR Register;
    UCHAR Size;
};

struct PCIBIOS_PARAM_4
{
    UCHAR SubFunction;
    UCHAR Bus;
    UCHAR DevFunc;
    UCHAR Register;
    UCHAR Size;
    ULONG Data;
};

struct PCIBIOS_DATA_0
{
    UCHAR RetCode;
    UCHAR Mechanism;
    UCHAR VersionMajor;
    UCHAR VersionMinor;
    UCHAR LastBus;
};

struct PCIBIOS_DATA_2
{
    UCHAR RetCode;
    UCHAR Bus;
    UCHAR DevFunc;
};

struct PCIBIOS_DATA_3
{
    UCHAR RetCode;
    ULONG Data;
};

struct PCIBIOS_DATA_4
{
    UCHAR RetCode;
};

#pragma pack()

UCHAR PCIVersionMajor;
UCHAR PCIVersionMinor;

static HFILE OEMHelp;

static UCHAR Bus, DevFunc;

static APIRET PCIBIOSPresent(VOID)
{
    APIRET rc;
    struct PCIBIOS_PARAM_0 param;
    struct PCIBIOS_DATA_0 data;
    ULONG paramlen=sizeof(param);
    ULONG datalen=sizeof(data);
    param.SubFunction=PCIBIOS_QUERY_INFORMATION;
    rc=DosDevIOCtl(OEMHelp, IOCTL_OEMHLP, OEMHLP_PCI,
                   &param, paramlen, &paramlen,
                   &data, datalen, &datalen);
    if (rc)
        return rc;
    if (data.RetCode)
        return data.RetCode;
    PCIVersionMajor=data.VersionMajor;
    PCIVersionMinor=data.VersionMinor;
    return NO_ERROR;
}

APIRET PCIOpen(VOID)
{
    APIRET rc;
    ULONG action;
    rc=DosOpen("\\DEV\\OEMHLP$", &OEMHelp, &action, 0, 0, FILE_OPEN,
               OPEN_SHARE_DENYNONE | OPEN_FLAGS_FAIL_ON_ERROR |
               OPEN_ACCESS_READWRITE, NULL);
    if (rc)
        return rc;
    return PCIBIOSPresent();
}

APIRET PCIFindDevice(UCHAR devclass, UCHAR subclass, UCHAR progif, UCHAR index)
{
    APIRET rc;
    struct PCIBIOS_PARAM_2 param;
    struct PCIBIOS_DATA_2 data;
    ULONG paramlen=sizeof(param);
    ULONG datalen=sizeof(data);
    param.SubFunction=PCIBIOS_FIND_CLASS_CODE;
    param.Class=devclass;
    param.SubClass=subclass;
    param.ProgIF=progif;
    param.Reserved=0;
    param.Index=index;
    rc=DosDevIOCtl(OEMHelp, IOCTL_OEMHLP, OEMHLP_PCI,
                   &param, paramlen, &paramlen,
                   &data, datalen, &datalen);
    if (rc)
        return rc;
    if (data.RetCode)
        return data.RetCode;
    Bus=data.Bus;
    DevFunc=data.DevFunc;
    return NO_ERROR;
}

APIRET PCIReadConfigB(UCHAR reg, PUCHAR value)
{
    APIRET rc;
    struct PCIBIOS_PARAM_3 param;
    struct PCIBIOS_DATA_3 data;
    ULONG paramlen=sizeof(param);
    ULONG datalen=sizeof(data);
    param.SubFunction=PCIBIOS_READ_REGISTER;
    param.Bus=Bus;
    param.DevFunc=DevFunc;
    param.Register=reg;
    param.Size=sizeof(*value);
    rc=DosDevIOCtl(OEMHelp, IOCTL_OEMHLP, OEMHLP_PCI,
                   &param, paramlen, &paramlen,
                   &data, datalen, &datalen);
    if (rc)
        return rc;
    if (data.RetCode)
        return data.RetCode;
    *value=(UCHAR)data.Data;
    return NO_ERROR;
}

APIRET PCIReadConfigW(UCHAR reg, PUSHORT value)
{
    APIRET rc;
    struct PCIBIOS_PARAM_3 param;
    struct PCIBIOS_DATA_3 data;
    ULONG paramlen=sizeof(param);
    ULONG datalen=sizeof(data);
    param.SubFunction=PCIBIOS_READ_REGISTER;
    param.Bus=Bus;
    param.DevFunc=DevFunc;
    param.Register=reg;
    param.Size=sizeof(*value);
    rc=DosDevIOCtl(OEMHelp, IOCTL_OEMHLP, OEMHLP_PCI,
                   &param, paramlen, &paramlen,
                   &data, datalen, &datalen);
    if (rc)
        return rc;
    if (data.RetCode)
        return data.RetCode;
    *value=(USHORT)data.Data;
    return NO_ERROR;
}

APIRET PCIReadConfigL(UCHAR reg, PULONG value)
{
    APIRET rc;
    struct PCIBIOS_PARAM_3 param;
    struct PCIBIOS_DATA_3 data;
    ULONG paramlen=sizeof(param);
    ULONG datalen=sizeof(data);
    param.SubFunction=PCIBIOS_READ_REGISTER;
    param.Bus=Bus;
    param.DevFunc=DevFunc;
    param.Register=reg;
    param.Size=sizeof(*value);
    rc=DosDevIOCtl(OEMHelp, IOCTL_OEMHLP, OEMHLP_PCI,
                   &param, paramlen, &paramlen,
                   &data, datalen, &datalen);
    if (rc)
        return rc;
    if (data.RetCode)
        return data.RetCode;
    *value=(ULONG)data.Data;
    return NO_ERROR;
}

APIRET PCIWriteConfigB(UCHAR reg, UCHAR value)
{
    APIRET rc;
    struct PCIBIOS_PARAM_4 param;
    struct PCIBIOS_DATA_4 data;
    ULONG paramlen=sizeof(param);
    ULONG datalen=sizeof(data);
    param.SubFunction=PCIBIOS_WRITE_REGISTER;
    param.Bus=Bus;
    param.DevFunc=DevFunc;
    param.Register=reg;
    param.Size=sizeof(value);
    param.Data=(ULONG)value;
    rc=DosDevIOCtl(OEMHelp, IOCTL_OEMHLP, OEMHLP_PCI,
                   &param, paramlen, &paramlen,
                   &data, datalen, &datalen);
    if (rc)
        return rc;
    if (data.RetCode)
        return data.RetCode;
    return NO_ERROR;
}


VOID PCIClose(VOID)
{
    DosClose(OEMHelp);
}
