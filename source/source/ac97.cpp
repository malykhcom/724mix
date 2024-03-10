#include "global.h"
#include "ac97.h"

struct VENDOR_NAME
{
    ULONG ID;
    PCSZ Name;
};

struct CODEC_NAME
{
    ULONG ID;
    PCSZ Name;
};

static struct VENDOR_NAME Vendors[]=
{
    {0x414453, "Analog Devices AD"},
    {0x414B4D, "Asahi Kasei AK"},
    {0x435259, "Cirrus Logic CS"},
    {0x4e5343, "National Semiconductor LM"},
    {0x53494c, "Silicon Laboratory Si"},
    {0x545241, "TriTech TR"},
    {0x574D4C, "Wolfson WM"},
    {0x838476, "SigmaTel STAC"},
};

static struct CODEC_NAME Codecs[]=
{
    {0x41445340, "1881"},
    {0x414B4D00, "4540"},
    {0x43525900, "4297"},
    {0x43525903, "4297"},
    {0x43525913, "4297A"},
    {0x43525923, "4298"},
    {0x43525931, "4299"},
    {0x4e534331, "4549"},
    {0x53494c22, "3036"},
    {0x53494c23, "3038"},
    {0x54524106, "28026"},
    {0x54524108, "28028"},
    {0x574D4C00, "9704"},
    {0x83847604, "9701/3/4/5"},
    {0x83847605, "9704"},
    {0x83847608, "9708"},
    {0x83847609, "9721/23"},
};

static PCSZ Modes[32]=
{
    "No 3D Stereo Enhancement",
    "Analog Devices Phat Stereo",
    "Creative Stereo Enhancement",
    "National Semi 3D Stereo Enhancement",
    "YAMAHA Ymersion",
    "BBE 3D Stereo Enhancement",
    "Crystal Semi 3D Stereo Enhancement",
    "Qsound QXpander",
    "Spatializer 3D Stereo Enhancement",
    "SRS 3D Stereo Enhancement",
    "Platform Tech 3D Stereo Enhancement",
    "AKM 3D Audio",
    "Aureal Stereo Enhancement",
    "Aztech 3D Enhancement",
    "Binaura 3D Audio Enhancement",
    "ESS Technology Stereo Enhancement",
    "Harman International VMAx",
    "Nvidea 3D Stereo Enhancement",
    "Philips Incredible Sound",
    "Texas Instruments 3D Stereo Enhancement",
    "VLSI Technology 3D Stereo Enhancement",
    "TriTech 3D Stereo Enhancement",
    "Realtek 3D Stereo Enhancement",
    "Samsung 3D Stereo Enhancement",
    "Wolfson Microelectronics 3D Enhancement",
    "Delta Integration 3D Enhancement",
    "SigmaTel 3D Enhancement",
    "Reserved (27)",
    "Rockwell 3D Stereo Enhancement",
    "Reserved (29)",
    "Reserved (30)",
    "Reserved (31)"
};

PCSZ AC97QueryVendor(ULONG id)
{
    ULONG t;
    PCSZ vendor="Unknown ";
    id>>=8;
    for(t=0;t<ARRAY_SIZE(Vendors);t++)
        if (Vendors[t].ID==id)
            vendor=Vendors[t].Name;
    return vendor;
}

PCSZ AC97QueryCodecName(ULONG id)
{
    ULONG t;
    PCSZ codec="????";
    for(t=0;t<ARRAY_SIZE(Codecs);t++)
        if (Codecs[t].ID==id)
            codec=Codecs[t].Name;
    return codec;
}

PCSZ AC97QueryStereoMode(USHORT id)
{
    if (id>=ARRAY_SIZE(Modes))
        return "Unknown";
    return Modes[id];
}
