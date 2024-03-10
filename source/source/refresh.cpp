#include "global.h"
#include "refresh.h"

#include "724mix.h"
#include "ymf724.h"
#include "ac97.h"

volatile static USHORT RefreshMaster;
volatile static USHORT RefreshHeadphone;
volatile static USHORT RefreshMasterMono;
volatile static USHORT RefreshPhone;
volatile static USHORT RefreshMic;
volatile static USHORT RefreshLineIn;
volatile static USHORT RefreshCD;
volatile static USHORT RefreshVideo;
volatile static USHORT RefreshAux;
volatile static USHORT RefreshPCM;
volatile static USHORT RefreshGeneral;
volatile static USHORT Refresh3D;
volatile static ULONG  RefreshDelay;

static USHORT Make1(ULONG menu)
{
    USHORT tmp;
    UCHAR level;
    if (MixMenu[menu].State)
        tmp=0x0000;
    else
        tmp=0x8000;
    level=31-MixMenu[menu].Level;
    tmp|=level;
    tmp|=level<<8;
    return tmp;
}

static USHORT Make2(ULONG menu1, ULONG menu2, ULONG menu3)
{
    USHORT tmp;
    tmp=0x0000;
    if (MixMenu[menu1].State)
        tmp|=1<<14;
    if (MixMenu[menu2].State)
        tmp|=1<<12;
    if (MixMenu[menu3].State)
        tmp|=1<<13;
    return tmp;
}

static USHORT Make3(ULONG menu1, ULONG menu2)
{
    USHORT tmp;
    tmp=MixMenu[menu2].Level;
    tmp|=MixMenu[menu1].Level<<8;
    return tmp;
}

static ULONG Make4(ULONG menu)
{
    ULONG tmp;
    tmp=MixMenu[menu].Level;
    tmp*=10;
    return tmp;
}

VOID RefreshRegs(VOID)
{
    RefreshMaster=Make1(MENU_MASTER);
    RefreshHeadphone=Make1(MENU_HEADPHONE);
    RefreshMasterMono=Make1(MENU_MASTERMONO);
    RefreshPCM=Make1(MENU_PCM);
    RefreshPhone=Make1(MENU_PHONE);
    RefreshMic=Make1(MENU_MIC);
    RefreshLineIn=Make1(MENU_LINE_IN);
    RefreshCD=Make1(MENU_CD);
    RefreshVideo=Make1(MENU_VIDEO);
    RefreshAux=Make1(MENU_AUX);

    RefreshGeneral=Make2(MENU_STEREO, MENU_BASS, MENU_3D);

    Refresh3D=Make3(MENU_3D_CONTROL_1, MENU_3D_CONTROL_2);

    RefreshDelay=Make4(MENU_DELAY);
}

static VOID Thread(PVOID)
{
    for(;;)
    {
        YMF724WriteAC(AC97_MASTER_VOL, RefreshMaster);
        YMF724WriteAC(AC97_HEADPHONE_VOL, RefreshHeadphone);
        YMF724WriteAC(AC97_MASTER_VOL_MONO, RefreshMasterMono);
        YMF724WriteAC(AC97_PHONE_VOL, RefreshPhone);
        YMF724WriteAC(AC97_MIC_VOL, RefreshMic);
        YMF724WriteAC(AC97_LINEIN_VOL, RefreshLineIn);
        YMF724WriteAC(AC97_CD_VOL, RefreshCD);
        YMF724WriteAC(AC97_VIDEO_VOL, RefreshVideo);
        YMF724WriteAC(AC97_AUX_VOL, RefreshAux);
        YMF724WriteAC(AC97_PCMOUT_VOL, RefreshPCM);
        YMF724WriteAC(AC97_GENERAL_PURPOSE, RefreshGeneral);
        YMF724WriteAC(AC97_3D_CONTROL, Refresh3D);
        DosSleep(RefreshDelay);
    }
}

VOID RefreshInit(VOID)
{
    _beginthread(Thread, NULL, 65536, NULL);
}
