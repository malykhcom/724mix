#include "global.h"
#include "control.h"

#include "video.h"
#include "724mix.h"
#include "pci.h"
#include "ymf724.h"
#include "ac97.h"
#include "refresh.h"

#define CLR_LIGHT   0xf
#define CLR_NORMAL  0xb
#define CLR_TITLE   0x70
#define CLR_BAR     0x17
#define CLR_STATUS  0x30
#define CLR_KEY     0x3b
#define CLR_MENU    0x7
#define CLR_NMENU   0x47
#define CLR_DISABLED 0x8
#define CLR_LIMIT   0xe

static CHAR Buffer[256];

static VOID DrawData1(ULONG menu)
{
    ULONG y=MixMenu[menu].Y;
    UCHAR level=MixMenu[menu].Level;
    BOOL limited;
    ULONG x;
    if (MixMenu[menu].Limited && level>23)
        limited=TRUE;
    else
        limited=FALSE;
    x=17;
    x+=VideoWriteStr("Mute: [", x, y, CLR_NORMAL);
    x+=VideoWriteStr(MixMenu[menu].State?" ":"x", x, y, CLR_LIGHT);
    VideoWriteStr("]", x, y, CLR_NORMAL);
    x=27;
    x+=VideoWriteStr("Level: [", x, y, CLR_NORMAL);
    VideoWriteChar('þ', level, x, y, limited ? CLR_LIMIT : CLR_LIGHT);
    VideoWriteChar('ú', 31-level, x+level, y, CLR_LIGHT);
    x+=31;
    x+=VideoWriteStr("] (", x, y, CLR_NORMAL);
    if (!MixMenu[menu].Limited)
        _snprintf(Buffer, sizeof(Buffer), "%+05.1f", 0.0-1.5*(31-level));
    else
        _snprintf(Buffer, sizeof(Buffer), "%+05.1f", 12.0-1.5*(31-level));
    x+=VideoWriteStr(Buffer, x, y, CLR_LIGHT);
    VideoWriteStr(" dB)", x, y, CLR_NORMAL);
}

#define ENABLED(x) (enabled ? x : CLR_DISABLED)

static VOID DrawData2(ULONG menu)
{
    BOOL enabled=MixMenu[menu].Level?TRUE:FALSE;
    BOOL state=MixMenu[menu].State;
    ULONG y=MixMenu[menu].Y;
    ULONG x;
    x=17;
    x+=VideoWriteStr("[", x, y, ENABLED(CLR_NORMAL));
    x+=VideoWriteStr(state?"x":" ", x, y, ENABLED(CLR_LIGHT));
    x+=VideoWriteStr("] (", x, y, ENABLED(CLR_NORMAL));
    x+=VideoWriteStr(state?"On":"Off", x, y, ENABLED(CLR_LIGHT));
    VideoWriteStr(") ", x, y, ENABLED(CLR_NORMAL));
}

static VOID DrawData3(ULONG menu)
{
    UCHAR level=MixMenu[menu].Level;
    ULONG y=MixMenu[menu].Y;
    ULONG x;
    x=17;
    x+=VideoWriteStr("Level: [", x, y, CLR_NORMAL);
    VideoWriteChar('þ', level, x, y, CLR_LIGHT);
    VideoWriteChar('ú', 15-level, x+level, y, CLR_LIGHT);
    x+=15;
    x+=VideoWriteStr("] (", x, y, CLR_NORMAL);
    _snprintf(Buffer, sizeof(Buffer), "%3d", level*100/15);
    x+=VideoWriteStr(Buffer, x, y, CLR_LIGHT);
    VideoWriteStr("%)", x, y, CLR_NORMAL);
}

static VOID DrawData4(ULONG menu)
{
    UCHAR level=MixMenu[menu].Level;
    ULONG y=MixMenu[menu].Y;
    ULONG x;
    x=17;
    _snprintf(Buffer, sizeof(Buffer), "%d", level*10);
    x+=VideoWriteStr(Buffer, x, y, CLR_LIGHT);
    VideoWriteStr(" msecs  ", x, y, CLR_NORMAL);
}

static VOID DrawMenu(ULONG selected)
{
    ULONG t;
    for(t=0;t<ARRAY_SIZE(MixMenu);t++)
        VideoWriteStr(MixMenu[t].Text, 2, MixMenu[t].Y,
                      t==selected ? CLR_NMENU : CLR_MENU);
}

VOID ControlDraw(VOID)
{
    ULONG x;
    VideoCls();
    {
        VideoWriteChar(' ', VideoX, 0, 0, CLR_TITLE);
        VideoWriteStr(MixTitle, 2, 0, CLR_TITLE);
    }
    {
        ULONG y;
        y=max(VideoY-1, 24);
        
        VideoWriteChar(' ' ,VideoX, 0, y, CLR_STATUS);
        VideoWriteStr("  Esc: Exit ³ Up/Down: Move cursor ³ Left/Right: Change level ³ Space: Switch",
                      0, y, CLR_STATUS);
        VideoWriteStr("Esc", 2, y, CLR_KEY);
        VideoWriteStr("Up", 14, y, CLR_KEY);
        VideoWriteStr("Down", 17, y, CLR_KEY);
        VideoWriteStr("Left", 37, y, CLR_KEY);
        VideoWriteStr("Right", 42, y, CLR_KEY);
        VideoWriteStr("Space", 64, y, CLR_KEY);
    }
    {
        ULONG y=2;
        x=2;
        x+=VideoWriteStr("PCI BIOS ", x, y, CLR_NORMAL);
        _snprintf(Buffer, sizeof(Buffer), "v%d.%02d",
                  PCIVersionMajor, PCIVersionMinor);
        x+=VideoWriteStr(Buffer, x, y, CLR_LIGHT);
        x+=VideoWriteStr(", Audio Device: ", x, y, CLR_NORMAL);
        VideoWriteStr(YMF724QueryDeviceName(), x, y, CLR_LIGHT);
    }
    {
        ULONG y=3;
        ULONG id;
        {
            USHORT id1=YMF724ReadAC(AC97_VENDOR_ID1);
            USHORT id2=YMF724ReadAC(AC97_VENDOR_ID2);
            id=id1;
            id<<=16;
            id+=id2;
        }
        x=2;
        x+=VideoWriteStr("AC'97 name: ", x, y, CLR_NORMAL);
        x+=VideoWriteStr(AC97QueryVendor(id), x, y, CLR_LIGHT);
        x+=VideoWriteStr(AC97QueryCodecName(id), x, y, CLR_LIGHT);
        x+=VideoWriteStr(" (VendorID: ", x, y, CLR_NORMAL);
        _snprintf(Buffer, sizeof(Buffer),
                  "%06Xh", id>>8);
        x+=VideoWriteStr(Buffer, x, y, CLR_LIGHT);
        x+=VideoWriteStr(" [", x, y, CLR_NORMAL);
        VideoWriteChar((id>>24) & 0xff, 1, x, y, CLR_LIGHT);
        VideoWriteChar((id>>16) & 0xff, 1, x+1, y, CLR_LIGHT);
        VideoWriteChar((id>>8) & 0xff, 1, x+2, y, CLR_LIGHT);
        x+=3;
        x+=VideoWriteStr("], Rev: ", x, y, CLR_NORMAL);
        _snprintf(Buffer, sizeof(Buffer), "%d", id & 0xff);
        x+=VideoWriteStr(Buffer, x, y, CLR_LIGHT);
        VideoWriteStr(")", x, y, CLR_NORMAL);
    }
    {
        USHORT cap=YMF724ReadAC(AC97_RESET);
        static CHAR notsupported[]=
            "Not supported by the CODEC.";
        x=28;
        if (!(cap & (1<<3)))
        {
            VideoWriteStr(notsupported, x, MixMenu[MENU_STEREO].Y, CLR_LIGHT);
            MixMenu[MENU_STEREO].Level=0;
        }
        if (!(cap & (1<<5)))
        {
            VideoWriteStr(notsupported, x, MixMenu[MENU_BASS].Y, CLR_LIGHT);
            MixMenu[MENU_BASS].Level=0;
        }
        cap=(cap>>10) & 0x1f;
        VideoWriteStr(AC97QueryStereoMode(cap),
                      x, MixMenu[MENU_3D].Y, CLR_LIGHT);
        if (!cap)
            MixMenu[MENU_3D].Level=0;
    }
    DrawMenu(0);
    {
        ULONG t;
        for(t=0;t<ARRAY_SIZE(MixMenu);t++)
        {
            switch (MixMenu[t].Type)
            {
            case 1:
                DrawData1(t);
                break;
            case 2:
                DrawData2(t);
                break;
            case 3:
                DrawData3(t);
                break;
            case 4:
                DrawData4(t);
            }
        }
    }
    VideoRefresh();
}

static VOID Inc1(ULONG selected)
{
    UCHAR level=MixMenu[selected].Level;
    if (level<31)
    {
        MixMenu[selected].Level=level+1;
        DrawData1(selected);
        VideoRefresh();
        RefreshRegs();
    }
}

static VOID Dec1(ULONG selected)
{
    UCHAR level=MixMenu[selected].Level;
    if (level>0)
    {
        MixMenu[selected].Level=level-1;
        DrawData1(selected);
        VideoRefresh();
        RefreshRegs();
    }
}

static VOID Switch1(ULONG selected)
{
    MixMenu[selected].State=!MixMenu[selected].State;
    DrawData1(selected);
    VideoRefresh();
    RefreshRegs();
}

static VOID Switch2(ULONG selected)
{
    if (MixMenu[selected].Level)
    {
        MixMenu[selected].State=!MixMenu[selected].State;
        DrawData2(selected);
        VideoRefresh();
        RefreshRegs();
    }
    else
        DosBeep(1000, 10);
}

static VOID Inc3(ULONG selected)
{
    UCHAR level=MixMenu[selected].Level;
    if (level<15)
    {
        MixMenu[selected].Level=level+1;
        DrawData3(selected);
        VideoRefresh();
        RefreshRegs();
    }
}

static VOID Dec3(ULONG selected)
{
    UCHAR level=MixMenu[selected].Level;
    if (level>0)
    {
        MixMenu[selected].Level=level-1;
        DrawData3(selected);
        VideoRefresh();
        RefreshRegs();
    }
}

static VOID Inc4(ULONG selected)
{
    UCHAR level=MixMenu[selected].Level;
    if (level<100)
    {
        MixMenu[selected].Level=level+1;
        DrawData4(selected);
        VideoRefresh();
        RefreshRegs();
    }
}

static VOID Dec4(ULONG selected)
{
    UCHAR level=MixMenu[selected].Level;
    if (level>1)
    {
        MixMenu[selected].Level=level-1;
        DrawData4(selected);
        VideoRefresh();
        RefreshRegs();
    }
}

static VOID UpPressed(PULONG selected)
{
    if (*selected)
        *selected=*selected-1;
    else
        *selected=ARRAY_SIZE(MixMenu)-1;
    DrawMenu(*selected);
    VideoRefresh();
}

static VOID DownPressed(PULONG selected)
{
    if (*selected==ARRAY_SIZE(MixMenu)-1)
        *selected=0;
    else
        *selected=*selected+1;
    DrawMenu(*selected);
    VideoRefresh();
}

static VOID RightPressed(ULONG selected)
{
    switch (MixMenu[selected].Type)
    {
    case 1:
        Inc1(selected);
        break;
    case 3:
        Inc3(selected);
        break;
    case 4:
        Inc4(selected);
        break;
    }
}

static VOID LeftPressed(ULONG selected)
{
    switch (MixMenu[selected].Type)
    {
    case 1:
        Dec1(selected);
        break;
    case 3:
        Dec3(selected);
        break;
    case 4:
        Dec4(selected);
        break;
    }
}

static VOID SpacePressed(ULONG selected)
{
    switch (MixMenu[selected].Type)
    {
    case 1:
        Switch1(selected);
        break;
    case 2:
        Switch2(selected);
        break;
    }
}

VOID ControlDo(VOID)
{
    ULONG selected=0;
    for(;;)
    {
        CHAR t=getch();
        if (t==27)
            break;
        if (t==0 || t==224)
        {
            t=getch();
            switch (t)
            {
            case 80:
                DownPressed(&selected);
                break;
            case 72:
                UpPressed(&selected);
                break;
            case 77:
                RightPressed(selected);
                break;
            case 75:
                LeftPressed(selected);
                break;
            }
        }
        else
        {
            switch (t)
            {
            case ' ':
                SpacePressed(selected);
                break;
            }
        }
    }
}
