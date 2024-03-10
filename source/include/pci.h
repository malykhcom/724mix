#ifndef __PCI_H__
#define __PCI_H__

extern UCHAR PCIVersionMajor;
extern UCHAR PCIVersionMinor;

APIRET PCIOpen(VOID);
APIRET PCIFindDevice(UCHAR, UCHAR, UCHAR, UCHAR);
APIRET PCIReadConfigB(UCHAR, PUCHAR);
APIRET PCIReadConfigW(UCHAR, PUSHORT);
APIRET PCIReadConfigL(UCHAR, PULONG);
APIRET PCIWriteConfigB(UCHAR, UCHAR);
VOID PCIClose(VOID);

#endif
