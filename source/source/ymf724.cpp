#include "global.h"
#include "ymf724.h"

#include "pci.h"
#include "memio.h"

/* Base Class Code: Multimedia */

#define YMFPCI_BASE_CLASS     0x04

/* Sub-Class Code: Audio */

#define YMFPCI_SUB_CLASS      0x01

/* PCI Configuration Registers */

#define YMFPCI_VENDOR_REG     0x00
#define YMFPCI_DEVICE_REG     0x02
#define YMFPCI_COMMAND_REG    0x04
#define YMFPCI_BASE_ADDR_REG  0x10
#define YMFPCI_AC_RESET_REG   0x48

/* Yamaha Vendor ID */

#define YMFPCI_VENDOR         0x1073

/* Yamaha PCI Chips IDs */

#define YMFPCI_DEVICE_724D    0x0004
#define YMFPCI_DEVICE_740     0x000A
#define YMFPCI_DEVICE_740C    0x000C
#define YMFPCI_DEVICE_724F    0x000D
#define YMFPCI_DEVICE_744B    0x0010
#define YMFPCI_DEVICE_754     0x0012

/* Memory Mapped Registers Size */

#define YMFPCI_REGS_SIZE      0x8000

/* Sound Card Registers */

#define YMFPCI_AC97DATA       0x60
#define YMFPCI_AC97ADDR       0x62
#define YMFPCI_PRI_DATA       0x64
#define YMFPCI_PRI_ADDR       0x66

USHORT YMF724Vendor;
USHORT YMF724Device;
ULONG YMF724BaseAddr;

APIRET YMF724Find(VOID)
{
    APIRET rc;
    UCHAR t;
    for(t=0;;t++)
    {
        rc=PCIFindDevice(YMFPCI_BASE_CLASS, YMFPCI_SUB_CLASS,
                         0x00, t);
        if (rc)
            return rc;
        if (PCIReadConfigW(YMFPCI_VENDOR_REG, &YMF724Vendor))
            continue;
        if (PCIReadConfigW(YMFPCI_DEVICE_REG, &YMF724Device))
            continue;
        if (YMF724Vendor!=YMFPCI_VENDOR)
            continue;
        switch (YMF724Device)
        {
        case YMFPCI_DEVICE_724D:
        case YMFPCI_DEVICE_740:
        case YMFPCI_DEVICE_740C:
        case YMFPCI_DEVICE_724F:
        case YMFPCI_DEVICE_744B:
        case YMFPCI_DEVICE_754:
            if (!PCIReadConfigL(YMFPCI_BASE_ADDR_REG, &YMF724BaseAddr))
            {
                USHORT status;
                if (!PCIReadConfigW(YMFPCI_COMMAND_REG, &status))
                    if ((status & 0x6)==0x6)
                    {
                        YMF724ResetAC();
                        return NO_ERROR;
                    }
            }
        }
    }
}

PCSZ YMF724QueryDeviceName(VOID)
{
    switch (YMF724Device)
    {
    case YMFPCI_DEVICE_724D:
        return "YMF724D";
    case YMFPCI_DEVICE_740:
        return "YMF740";
    case YMFPCI_DEVICE_740C:
        return "YMF740C";
    case YMFPCI_DEVICE_724F:
        return "YMF724F";
    case YMFPCI_DEVICE_744B:
        return "YMF744B";
    case YMFPCI_DEVICE_754:
        return "YMF754";
    }
    return "Unknown Yamaha PCI chip";
}

APIRET YMF724MapMemory(VOID)
{
    return MemioMakePointer(YMF724BaseAddr, YMFPCI_REGS_SIZE);
}

#define RESET_DELAY 5

VOID YMF724ResetAC(VOID)
{
    UCHAR tmp;
    if (!PCIReadConfigB(YMFPCI_AC_RESET_REG, &tmp))
    {
        if (tmp & 0x03)
        {
            PCIWriteConfigB(YMFPCI_AC_RESET_REG, tmp & 0xFC);
            DosSleep(RESET_DELAY);
            PCIWriteConfigB(YMFPCI_AC_RESET_REG, tmp | 0x03);
            DosSleep(RESET_DELAY);
            PCIWriteConfigB(YMFPCI_AC_RESET_REG, tmp & 0xFC);
            DosSleep(RESET_DELAY);
        }
    }
}

#define TIMEOUT 2000

USHORT YMF724ReadAC(ULONG reg)
{
    ULONG t=0;
    MemioWriteW(YMFPCI_AC97ADDR, 0x8000 | (reg & 0x7f));
    while (MemioReadW(YMFPCI_PRI_ADDR) & 0x8000)
    {
        t++;
        if (t>=TIMEOUT)
            break;
    }
    return MemioReadW(YMFPCI_PRI_DATA);
}

VOID YMF724WriteAC(ULONG reg, USHORT data)
{
    ULONG t=0;
    MemioWriteW(YMFPCI_AC97ADDR, reg & 0x7f);
    MemioWriteW(YMFPCI_AC97DATA, data);
    while (MemioReadW(YMFPCI_PRI_ADDR) & 0x8000)
    {
        t++;
        if (t>=TIMEOUT)
            break;
    }
}
