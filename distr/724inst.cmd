/* 724mix Folder Maker */

if RxFuncQuery('SysCreateObject') then do
    call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
    call SysLoadFuncs
end

folderid='<724MIX02FOLDER>'

rc=SysCreateObject('WPFolder', '724mix v0.2', '<WP_DESKTOP>', 'OBJECTID='folderid)
if rc then
    say '724mix Folder was created.'

call CreateObject 'YMF724 Mixer/2', '724MIX.EXE'
call CreateShadow '724mix.txt', '724mix.txt'
call CreateShadow 'history.txt', 'history.txt'

return

CreateObject:
    parse arg objectname, exename
    rc=SysCreateObject('WPProgram', objectname, folderid, 'EXENAME='directory()'\'exename);
    if rc then
        say objectname' Object was created.'
return

CreateShadow:
    parse arg objectname, filename
    rc=SysCreateObject('WPShadow', objectname, folderid, 'SHADOWID='directory()'\'filename);
    if rc then
        say objectname' Shadow was created.'
return
    