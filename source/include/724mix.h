#ifndef __724MIX_H__
#define __724MIX_H__

extern CHAR MixTitle[];

struct MENU
{
    ULONG Type;
    PCSZ Text;
    ULONG Y;
    BOOL State;
    UCHAR Level;
    BOOL Limited;
    PCSZ Key;
};

extern struct MENU MixMenu[16];

#define MENU_MASTER           0
#define MENU_HEADPHONE        1
#define MENU_MASTERMONO       2
#define MENU_PCM              3
#define MENU_PHONE            4
#define MENU_MIC              5
#define MENU_LINE_IN          6
#define MENU_CD               7
#define MENU_VIDEO            8
#define MENU_AUX              9
#define MENU_STEREO           10
#define MENU_BASS             11
#define MENU_3D               12
#define MENU_3D_CONTROL_1     13
#define MENU_3D_CONTROL_2     14
#define MENU_DELAY            15

#endif
