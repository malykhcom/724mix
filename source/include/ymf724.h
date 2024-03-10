#ifndef __YMF724_H__
#define __YMF724_H__

extern USHORT YMF724Vendor;
extern USHORT YMF724Device;
extern ULONG YMF724BaseAddr;

APIRET YMF724Find(VOID);
PCSZ YMF724QueryDeviceName(VOID);
APIRET YMF724MapMemory(VOID);
VOID YMF724ResetAC(VOID);
USHORT YMF724ReadAC(ULONG);
VOID YMF724WriteAC(ULONG, USHORT);
                    
#endif
