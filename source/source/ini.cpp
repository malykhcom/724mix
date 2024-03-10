#include "global.h"
#include "ini.h"

#include "724mix.h"
#include "pmvio.h"

static CHAR ConfigName[]="724mix.cfg";
static CHAR KeyMute[]=".mute";
static CHAR KeyState[]=".state";
static CHAR KeyLevel[]=".level";

static PCHAR ConfigFullName;

static VOID LoadMute(ULONG handle, ULONG menu)
{
    BOOL tmp;
    CHARS key(strlen(MixMenu[menu].Key)+sizeof(KeyMute));
    strcpy(key.Get(), MixMenu[menu].Key);
    strcat(key.Get(), KeyMute);
    if (configRead(handle, key.Get(), CONFIG_BOOL, &tmp))
    {
        //printf("LoadMute: %s=%s\n", key.Get(), tmp?"TRUE":"FALSE");
        MixMenu[menu].State=!tmp;
    }
}

static VOID SaveMute(ULONG handle, ULONG menu)
{
    BOOL tmp;
    CHARS key(strlen(MixMenu[menu].Key)+sizeof(KeyMute));
    strcpy(key.Get(), MixMenu[menu].Key);
    strcat(key.Get(), KeyMute);
    tmp=!MixMenu[menu].State;
    configCreate(handle, key.Get(), CONFIG_BOOL, &tmp);
}

static VOID LoadState(ULONG handle, ULONG menu)
{
    BOOL tmp;
    CHARS key(strlen(MixMenu[menu].Key)+sizeof(KeyState));
    strcpy(key.Get(), MixMenu[menu].Key);
    strcat(key.Get(), KeyState);
    if (configRead(handle, key.Get(), CONFIG_BOOL, &tmp))
    {
        //printf("LoadState: %s=%s\n", key.Get(), tmp?"TRUE":"FALSE");
        MixMenu[menu].State=tmp;
    }
}

static VOID SaveState(ULONG handle, ULONG menu)
{
    BOOL tmp;
    CHARS key(strlen(MixMenu[menu].Key)+sizeof(KeyState));
    strcpy(key.Get(), MixMenu[menu].Key);
    strcat(key.Get(), KeyState);
    tmp=MixMenu[menu].State;
    configCreate(handle, key.Get(), CONFIG_BOOL, &tmp);
}

static VOID LoadLevel(ULONG handle, ULONG menu)
{
    ULONG tmp;
    CHARS key(strlen(MixMenu[menu].Key)+sizeof(KeyLevel));
    strcpy(key.Get(), MixMenu[menu].Key);
    strcat(key.Get(), KeyLevel);
    if (configRead(handle, key.Get(), CONFIG_ULONG, &tmp))
    {
        //printf("LoadLevel: %s=%u\n", key.Get(), tmp);
        MixMenu[menu].Level=tmp;
    }
}

static VOID SaveLevel(ULONG handle, ULONG menu)
{
    ULONG tmp;
    CHARS key(strlen(MixMenu[menu].Key)+sizeof(KeyLevel));
    strcpy(key.Get(), MixMenu[menu].Key);
    strcat(key.Get(), KeyLevel);
    tmp=MixMenu[menu].Level;
    configCreate(handle, key.Get(), CONFIG_ULONG, &tmp);
}

static VOID Load1(ULONG handle, ULONG menu)
{
    LoadMute(handle, menu);
    LoadLevel(handle, menu);
    MixMenu[menu].Level=min(MixMenu[menu].Level, 31);
}

static VOID Save1(ULONG handle, ULONG menu)
{
    SaveMute(handle, menu);
    SaveLevel(handle, menu);
}

static VOID Load2(ULONG handle, ULONG menu)
{
    LoadState(handle, menu);
}

static VOID Save2(ULONG handle, ULONG menu)
{
    SaveState(handle, menu);
}

static VOID Load3(ULONG handle, ULONG menu)
{
    LoadLevel(handle, menu);
    MixMenu[menu].Level=min(MixMenu[menu].Level, 15);
}

static VOID Save3(ULONG handle, ULONG menu)
{
    SaveLevel(handle, menu);
}

static VOID Load4(ULONG handle, ULONG menu)
{
    LoadLevel(handle, menu);
    MixMenu[menu].Level=min(MixMenu[menu].Level, 100);
    MixMenu[menu].Level=max(MixMenu[menu].Level, 1);
}

static VOID Save4(ULONG handle, ULONG menu)
{
    SaveLevel(handle, menu);
}

APIRET IniOpen(PCSZ path)
{
    ULONG handle;
    ULONG t;
    PCHAR tmp;
    
    ConfigFullName=new CHAR[strlen(path)+sizeof(ConfigName)];
    if (!ConfigFullName)
        return ERROR_INVALID_PATH;
    strcpy(ConfigFullName, path);
    tmp=strrchr(ConfigFullName, '\\');
    if (tmp)
        tmp++;
    else
        tmp=ConfigFullName;
    strcpy(tmp, ConfigName);

    handle=configReadOpen(ConfigFullName);
    //printf("%s: %u\n", ConfigFullName, handle);
    if (!handle)
        return 1;

    for(t=0;t<ARRAY_SIZE(MixMenu);t++)
    {
        switch (MixMenu[t].Type)
        {
        case 1:
            Load1(handle, t);
            break;
        case 2:
            Load2(handle, t);
            break;
        case 3:
            Load3(handle, t);
            break;
        case 4:
            Load4(handle, t);
            break;
        }
    }
    configReadClose(handle);
    return NO_ERROR;
}

VOID IniClose(VOID)
{
    ULONG t;
    ULONG handle;
    handle=configCreateOpen(ConfigFullName);
    delete[] ConfigFullName;
    if (!handle)
        return;
    for(t=0;t<ARRAY_SIZE(MixMenu);t++)
    {
        switch (MixMenu[t].Type)
        {
        case 1:
            Save1(handle, t);
            break;
        case 2:
            Save2(handle, t);
            break;
        case 3:
            Save3(handle, t);
            break;
        case 4:
            Save4(handle, t);
            break;
        }
    }
    configCreateClose(handle);
}
