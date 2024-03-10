!include D:\DEFFIE\DEFLIB\lib1.mif

srcdirs = $(lib1path);INCLUDE;SOURCE;;$(%WATCOM)\H;$(%WATCOM)\H\OS2

dest = 724mix
desc = '724mix v0.2 (c) Anton Malykh, 2000-2001.'

lopt = debug all
copt = -d1 -wx -zp4 -i=$(srcdirs) -sg -bm -zv -s -ocehilnrtuz -ol+ -3r -fp3
#copt = -d3 -wx -zp4 -i=$(srcdirs) -sg

objs1 = 724mix.obj pci.obj ymf724.obj memio.obj ac97.obj 
objs2 = video.obj refresh.obj control.obj pmvio.obj ini.obj

objs = $(objs1) $(objs2) $(lib1objs)

.erase
.cpp: $(srcdirs)

.cpp.obj: .AUTODEPEND
	wpp386 $(copt) $<
        
$(dest).exe : $(objs) makefile $(dest).lnk $(dest).rc
	unlock $(dest).exe
        wlink @$(dest).lnk
#	wstrip $(dest).exe $(dest).exe $(dest).sym
        lxfix $(dest).exe /a
        rc $(dest).rc $(dest).exe 
	d:\lxlite\lxlite $(dest).exe

$(dest).lnk : makefile
	echo option description $(desc) >$^@
        echo option stack=64k >>$^@
	echo name $(dest).exe >>$^@
#	echo $(lopt) >>$^@
	echo option eliminate >>$^@
	echo system os2v2 >>$^@
	for %i in ($(objs)) do echo file %i >>$^@
