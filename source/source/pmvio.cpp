#include "global.h"
#include "pmvio.h"

static HAB APIENTRY (*pInitialize)(ULONG flOptions);
static HMQ APIENTRY (*pCreateMsgQueue)(HAB hab, LONG cmsg);
static BOOL APIENTRY (*pDestroyMsgQueue)(HMQ hmq);
static BOOL APIENTRY (*pTerminate)(HAB hab);
static BOOL APIENTRY (*pSetWindowText)(HWND hwnd, PSZ pszString);
static MRESULT APIENTRY (*pSendMsg)(HWND hwnd, ULONG ulMsgid,
                                    MPARAM mpParam1, MPARAM mpParam2);
static HPOINTER APIENTRY (*pLoadPointer)(HWND hwndDeskTop, HMODULE Resource,
                                         ULONG idPointer);
static HPOINTER APIENTRY (*pQuerySysPointer)(HWND hwndDeskTop,
                                             LONG lIdentifier, BOOL fCopy);
static BOOL APIENTRY (*pInvalidateRect)(HWND hwnd, PRECTL pwrc,
                                        BOOL fIncludeChildren);

HINI APIENTRY (*PmOpenProfile)(HAB hab, PCSZ pszFileName);
BOOL APIENTRY (*PmQueryProfileSize)(HINI hini,
                                    PCSZ pszApp,
                                    PCSZ pszKey,
                                    PULONG pulReqLen);
BOOL APIENTRY (*PmQueryProfileData)(HINI hini,
                                    PCSZ pszApp,
                                    PCSZ pszKey,
                                    PVOID pBuffer,
                                    PULONG pulBuffLen);
BOOL APIENTRY (*PmWriteProfileData)(HINI hini,
                                    PCSZ pszApp,
                                    PCSZ pszKey,
                                    PVOID pData,
                                    ULONG cchDataLen);
BOOL APIENTRY (*PmCloseProfile)(HINI hini);
static ULONG APIENTRY (*pChangeSwitchEntry)(HSWITCH hswitchSwitch,
                                            PSWCNTRL pswctlSwitchData);
static ULONG APIENTRY (*pQuerySwitchEntry)(HSWITCH hswitchSwitch,
                                           PSWCNTRL pswctlSwitchData);
static HSWITCH APIENTRY (*pQuerySwitchHandle)(HWND hwnd,
                                              PID idProcess);

struct ENTRY
{
    ULONG Ordinal;
    PFN* Func;
};

static struct ENTRY PMWin[]=
{
    {ORD_WIN32INITIALIZE,      (PFN*)&pInitialize      },
    {ORD_WIN32CREATEMSGQUEUE,  (PFN*)&pCreateMsgQueue  },
    {ORD_WIN32DESTROYMSGQUEUE, (PFN*)&pDestroyMsgQueue },
    {ORD_WIN32TERMINATE,       (PFN*)&pTerminate       },
    {ORD_WIN32SETWINDOWTEXT,   (PFN*)&pSetWindowText   },
    {ORD_WIN32SENDMSG,         (PFN*)&pSendMsg         },
    {ORD_WIN32LOADPOINTER,     (PFN*)&pLoadPointer     },
    {ORD_WIN32QUERYSYSPOINTER, (PFN*)&pQuerySysPointer },
    {ORD_WIN32INVALIDATERECT,  (PFN*)&pInvalidateRect  },
};

static struct ENTRY PMShapi[]=
{
    {ORD_PRF32OPENPROFILE,       (PFN*)&PmOpenProfile      },
    {ORD_PRF32QUERYPROFILESIZE,  (PFN*)&PmQueryProfileSize },
    {ORD_PRF32QUERYPROFILEDATA,  (PFN*)&PmQueryProfileData },
    {ORD_PRF32WRITEPROFILEDATA,  (PFN*)&PmWriteProfileData },
    {ORD_PRF32CLOSEPROFILE,      (PFN*)&PmCloseProfile     },
    {ORD_WIN32CHANGESWITCHENTRY, (PFN*)&pChangeSwitchEntry },
    {ORD_WIN32QUERYSWITCHENTRY,  (PFN*)&pQuerySwitchEntry  },
    {ORD_WIN32QUERYSWITCHHANDLE, (PFN*)&pQuerySwitchHandle }
};

static BOOL Inited;
static HMODULE Handle1;
static HMODULE Handle2;
static PPIB Ppib;
static ULONG SavedType;
HAB PmHab;
static HMQ Hmq;
static HSWITCH Hsw;
static SWCNTRL SwData;

static APIRET LoadFuncs(HMODULE handle, struct ENTRY* entries, ULONG size)
{
    APIRET rc;
    ULONG t;
    for(t=0;t<size;t++)
    {
        rc=DosQueryProcAddr(handle, entries[t].Ordinal, NULL, entries[t].Func);
        if (rc)
            return rc;
    }
    return NO_ERROR;
}

static APIRET LoadLibrary(HMODULE* handle, PCSZ name)
{
    APIRET rc;
    CHAR err[100];

    /*rc=DosQueryModuleHandle(name, handle);
    if (rc)
        return rc;*/
    rc=DosLoadModule(err, sizeof(err), name, handle);
    return rc;
}

static APIRET SwitchToPM(VOID)
{
    APIRET rc;
    PTIB ptib;
    rc=DosGetInfoBlocks(&ptib, &Ppib);
    if (!rc)
    {
        if (Ppib->pib_ultype!=PROG_WINDOWABLEVIO)
        {
            HMTX hmtx;
            rc=DosOpenMutexSem("\\SEM32\\PMDRAG.SEM", &hmtx);
            if (rc)
                return rc;
            DosCloseMutexSem(hmtx);
        }
        SavedType=Ppib->pib_ultype;
        Ppib->pib_ultype=PROG_PM;
    }
    return rc;
}

static APIRET StartPM(VOID)
{
    PmHab=pInitialize(0);
    if (PmHab)
    {
        Hmq=pCreateMsgQueue(PmHab, 0);
        if (Hmq)
            return NO_ERROR;
        pTerminate(PmHab);
    }
    return 1;
}

static VOID ReinitSwitchEntry(VOID)
{
    Hsw=pQuerySwitchHandle(0, Ppib->pib_ulpid);
    if (Hsw)
    {
        SWCNTRL swdata;
        static CHAR title[]="YMF724 Mixer/2";
        if (!pQuerySwitchEntry(Hsw, &SwData))
        {
            HPOINTER pointer;
            pSetWindowText(SwData.hwnd, title);
            pointer=pLoadPointer(HWND_DESKTOP, NULL, 1);
            if (!pointer)
                pointer=pQuerySysPointer(HWND_DESKTOP, SPTR_PROGRAM, FALSE);
            pSendMsg(SwData.hwnd, WM_SETICON, (MPARAM)pointer, NULL);
            pInvalidateRect(SwData.hwnd, NULL, TRUE);
            swdata=SwData;
            swdata.hwndIcon=(HWND)pointer;
            strcpy(swdata.szSwtitle, title);
            pChangeSwitchEntry(Hsw, &swdata);
        }
    }
}

static VOID BackSwitchEntry(VOID)
{
    if (Hsw)
    {
        pChangeSwitchEntry(Hsw, &SwData);
    }
}

static VOID SwitchBack(VOID)
{
    Ppib->pib_ultype=SavedType;
}

static VOID StopPM(VOID)
{
    pDestroyMsgQueue(Hmq);
    pTerminate(PmHab);
}

APIRET PmInit(VOID)
{
    APIRET rc;
    Inited=FALSE;
    rc=LoadLibrary(&Handle1, "PMWIN");
    if (!rc)
    {
        rc=LoadLibrary(&Handle2, "PMSHAPI");
        if (!rc)
        {
            rc=LoadFuncs(Handle1, PMWin, ARRAY_SIZE(PMWin));
            if (!rc)
            {
                rc=LoadFuncs(Handle2, PMShapi, ARRAY_SIZE(PMShapi));
                if (!rc)
                {
                    rc=SwitchToPM();
                    if (!rc)
                    {
                        rc=StartPM();
                        if (!rc)
                        {
                            ReinitSwitchEntry();
                            Inited=TRUE;
                            return NO_ERROR;
                        }
                        SwitchBack();
                    }
                }
            }
            DosFreeModule(Handle2);
        }
        DosFreeModule(Handle1);
    }
    return rc;
}

VOID PmTerm(VOID)
{
    if (Inited)
    {
        BackSwitchEntry();
        StopPM();
        SwitchBack();
        DosFreeModule(Handle2);
        DosFreeModule(Handle1);
    }
}

/*APIRET PmInit(VOID)
{
    APIRET rc;
    Handle2=NULL;
    rc=LoadLibrary(&Handle2, "PMSHAPI");
    if (!rc)
    {
        rc=LoadFuncs(Handle2, PMShapi, ARRAY_SIZE(PMShapi));
        if (!rc)
        {
            return NO_ERROR;
        }
        DosFreeModule(Handle2);
    }
    return rc;
}

VOID PmTerm(VOID)
{
    if (Handle2)
    {
        DosFreeModule(Handle2);
    }
}*/
