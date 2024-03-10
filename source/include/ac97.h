#ifndef __AC97_H__
#define __AC97_H__

/* AC'97 registers */

#define  AC97_RESET                     0x0000

#define  AC97_MASTER_VOL                0x0002
#define  AC97_HEADPHONE_VOL             0x0004
#define  AC97_MASTER_VOL_MONO           0x0006
#define  AC97_MASTER_TONE        0x0008      //
#define  AC97_PCBEEP_VOL                0x000a      // none
#define  AC97_PHONE_VOL                 0x000c      // TAD Input (mono)
#define  AC97_MIC_VOL                   0x000e      // MIC Input (mono)
#define  AC97_LINEIN_VOL                0x0010      // Line Input (stereo)
#define  AC97_CD_VOL                    0x0012      // CD Input (stereo)
#define  AC97_VIDEO_VOL                 0x0014      // none
#define  AC97_AUX_VOL                   0x0016      // Aux Input (stereo)
#define  AC97_PCMOUT_VOL                0x0018      // Wave Output (stereo)

#define  AC97_RECORD_SELECT      0x001a      //
#define  AC97_RECORD_GAIN        0x001c
#define  AC97_RECORD_GAIN_MIC    0x001e
#define  AC97_GENERAL_PURPOSE           0x0020
#define  AC97_3D_CONTROL                0x0022
#define  AC97_POWER_CONTROL      0x0026

#define  AC97_VENDOR_ID1                0x007c
#define  AC97_VENDOR_ID2                0x007e

/* volume control bit defines */

#define AC97_MUTE                0x8000
#define AC97_MICBOOST            0x0040
#define AC97_LEFTVOL             0x3f00
#define AC97_RIGHTVOL            0x003f

/* record mux defines */

#define AC97_RECMUX_MIC          0x0000
#define AC97_RECMUX_CD           0x0101
#define AC97_RECMUX_VIDEO        0x0202      /* not used */
#define AC97_RECMUX_AUX          0x0303
#define AC97_RECMUX_LINE         0x0404
#define AC97_RECMUX_STEREO_MIX   0x0505
#define AC97_RECMUX_MONO_MIX     0x0606
#define AC97_RECMUX_PHONE        0x0707

PCSZ AC97QueryVendor(ULONG);
PCSZ AC97QueryCodecName(ULONG);
PCSZ AC97QueryStereoMode(USHORT);

#endif
