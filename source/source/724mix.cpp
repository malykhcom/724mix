#include "global.h"
#include "724mix.h"

#include "pci.h"
#include "ymf724.h"
#include "refresh.h"
#include "control.h"
#include "video.h"
#include "pmvio.h"
#include "ini.h"

struct MENU MixMenu[16]=
{
    {1, "Master        ",  5, TRUE, 31, FALSE, "master"},
    {1, "Headphone     ",  6, TRUE, 31, FALSE, "headphone"},
    {1, "Master Mono   ",  7, TRUE, 31, FALSE, "mastermono"},
    {1, "Wave          ",  8, TRUE,  0, TRUE,  "wave"},
    {1, "Phone         ",  9, TRUE,  0, TRUE,  "phone"},
    {1, "Mic           ", 10, TRUE,  0, TRUE,  "mic"},
    {1, "Line In       ", 11, TRUE,  0, TRUE,  "linein"},
    {1, "CD            ", 12, TRUE,  0, TRUE,  "cd"},
    {1, "Video         ", 13, TRUE,  0, TRUE,  "video"},
    {1, "Aux           ", 14, TRUE,  0, TRUE,  "aux"},
    {2, "Mono to Stereo", 16, FALSE, 1, FALSE, "monotostereo"},
    {2, "Bass Boost    ", 17, FALSE, 1, FALSE, "bassboost"},
    {2, "3D Mode       ", 18, FALSE, 1, FALSE, "3dmode"},
    {3, "3D Control 1  ", 19, FALSE, 0, FALSE, "3dcontrol1"},
    {3, "3D Control 2  ", 20, FALSE, 0, FALSE, "3dcontrol2"},
    {4, "Resend delay  ", 22, FALSE,10, FALSE, "resenddelay"}
};

BOOL MixSMaster=TRUE;
UCHAR MixLMaster=31;
BOOL MixSPCM=TRUE;
UCHAR MixLPCM=0;

CHAR MixTitle[]="724mix v0.2 (c) Anton Malykh, 2000-2001.";

static VOID PrintErrorMsg(PCSZ error, APIRET rc)
{
    printf("%s\n\n", MixTitle);
    printf("%s error: Return Code = %u\n", error, rc);
}

static VOID Terminated(INT signal)
{
    IniClose();
    abort();
}

int main(int argc, char* argv[])
{
    APIRET rc;
    rc=PCIOpen();
    if (rc)
    {
        PrintErrorMsg("PCI BIOS init", rc);
        return 1;
    }
    rc=YMF724Find();
    if (rc)
    {
        PrintErrorMsg("Sound Card init", rc);
        return 1;
    }
    PCIClose();
    rc=YMF724MapMemory();
    if (rc)
    {
        PrintErrorMsg("Memory I/O init", rc);
        if (rc==ERROR_PATH_NOT_FOUND)
        {
            puts("USL Generic I/O Driver (uslgenio.sys) is not installed.\n");
        }
        return 1;
    }
    rc=VideoInit();
    if (rc)
    {
        PrintErrorMsg("VIO init", rc);
        return 1;
    }
    signal(SIGTERM, Terminated);
    PmInit();
    IniOpen(argv[0]);
    RefreshRegs();
    ControlDraw();
    VideoHideCursor();
    RefreshInit();
    ControlDo();
    VideoCls();
    VideoRefresh();
    VideoSetCursorPos(0, 0);
    VideoShowCursor();
    IniClose();
    PmTerm();
    return 0;
}