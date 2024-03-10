#ifndef __PMVIO_H__
#define __PMVIO_H__

extern HINI APIENTRY (*PmOpenProfile)(HAB hab, PCSZ pszFileName);
extern BOOL APIENTRY (*PmQueryProfileSize)(HINI hini,
                                           PCSZ pszApp,
                                           PCSZ pszKey,
                                           PULONG pulReqLen);
extern BOOL APIENTRY (*PmQueryProfileData)(HINI hini,
                                           PCSZ pszApp,
                                           PCSZ pszKey,
                                           PVOID pBuffer,
                                           PULONG pulBuffLen);
extern BOOL APIENTRY (*PmWriteProfileData)(HINI hini,
                                           PCSZ pszApp,
                                           PCSZ pszKey,
                                           PVOID pData,
                                           ULONG cchDataLen);
extern BOOL APIENTRY (*PmCloseProfile)(HINI hini);
extern HAB PmHab;

APIRET PmInit(VOID);
VOID PmTerm(VOID);

#endif
