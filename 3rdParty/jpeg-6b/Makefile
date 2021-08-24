# Makefile for Independent JPEG Group's software

# This makefile is for Microsoft Visual C++ on Windows NT (and 95?).
# It builds the IJG library as a statically linkable library (.LIB),
# and builds the sample applications as console-mode apps.
# Thanks to Xingong Chang, Raymond Everly and others.

# Read installation instructions before saying "nmake" !!
# To build an optimized library without debug info, say "nmake nodebug=1".

# Pull in standard variable definitions
!include <win32.mak>

# You may want to adjust these compiler options:
CFLAGS= $(cflags) $(cdebug) $(cvars) -I.
# Generally, we recommend defining any configuration symbols in jconfig.h,
# NOT via -D switches here.

# Link-time options:
LDFLAGS= $(ldebug) $(conlflags)

# To link any special libraries, add the necessary commands here.
LDLIBS= $(conlibs)

# Put here the object file name for the correct system-dependent memory
# manager file.  For NT we suggest jmemnobs.obj, which expects the OS to
# provide adequate virtual memory.
SYSDEPMEM= jmemnobs.obj

# miscellaneous OS-dependent stuff
# file deletion command
RM= del

# End of configurable options.


# source files: JPEG library proper
LIBSOURCES= jcapimin.cpp jcapistd.cpp jccoefct.cpp jccolor.cpp jcdctmgr.cpp jchuff.cpp \
        jcinit.cpp jcmainct.cpp jcmarker.cpp jcmaster.cpp jcomapi.cpp jcparam.cpp \
        jcphuff.cpp jcprepct.cpp jcsample.cpp jctrans.cpp jdapimin.cpp jdapistd.cpp \
        jdatadst.cpp jdatasrc.cpp jdcoefct.cpp jdcolor.cpp jddctmgr.cpp jdhuff.cpp \
        jdinput.cpp jdmainct.cpp jdmarker.cpp jdmaster.cpp jdmerge.cpp jdphuff.cpp \
        jdpostct.cpp jdsample.cpp jdtrans.cpp jerror.cpp jfdctflt.cpp jfdctfst.cpp \
        jfdctint.cpp jidctflt.cpp jidctfst.cpp jidctint.cpp jidctred.cpp jquant1.cpp \
        jquant2.cpp jutils.cpp jmemmgr.cpp
# memmgr back ends: compile only one of these into a working library
SYSDEPSOURCES= jmemansi.cpp jmemname.cpp jmemnobs.cpp jmemdos.cpp jmemmac.cpp
# source files: cjpeg/djpeg/jpegtran applications, also rdjpgcom/wrjpgcom
APPSOURCES= cjpeg.cpp djpeg.cpp jpegtran.cpp rdjpgcom.cpp wrjpgcom.cpp cdjpeg.cpp \
        rdcolmap.cpp rdswitch.cpp transupp.cpp rdppm.cpp wrppm.cpp rdgif.cpp wrgif.cpp \
        rdtarga.cpp wrtarga.cpp rdbmp.cpp wrbmp.cpp rdrle.cpp wrrle.cpp
SOURCES= $(LIBSOURCES) $(SYSDEPSOURCES) $(APPSOURCES)
# files included by source files
INCLUDES= jchuff.h jdhuff.h jdct.h jerror.h jinclude.h jmemsys.h jmorecfg.h \
        jpegint.h jpeglib.h jversion.h cdjpeg.h cderror.h transupp.h
# documentation, test, and support files
DOCS= README install.doc usage.doc cjpeg.1 djpeg.1 jpegtran.1 rdjpgcom.1 \
        wrjpgcom.1 wizard.doc example.cpp libjpeg.doc structure.doc \
        coderules.doc filelist.doc change.log
MKFILES= configure makefile.cfg makefile.ansi makefile.unix makefile.bcc \
        makefile.mc6 makefile.dj makefile.wat makefile.vc makelib.ds \
        makeapps.ds makeproj.mac makcjpeg.st makdjpeg.st makljpeg.st \
        maktjpeg.st makefile.manx makefile.sas makefile.mms makefile.vms \
        makvms.opt
CONFIGFILES= jconfig.cfg jconfig.bcc jconfig.mc6 jconfig.dj jconfig.wat \
        jconfig.vc jconfig.mac jconfig.st jconfig.manx jconfig.sas \
        jconfig.vms
CONFIGUREFILES= config.guess config.sub install-sh ltconfig ltmain.sh
OTHERFILES= jconfig.doc ckconfig.cpp ansi2knr.cpp ansi2knr.1 jmemdosa.asm
TESTFILES= testorig.jpg testimg.ppm testimg.bmp testimg.jpg testprog.jpg \
        testimgp.jpg
DISTFILES= $(DOCS) $(MKFILES) $(CONFIGFILES) $(SOURCES) $(INCLUDES) \
        $(CONFIGUREFILES) $(OTHERFILES) $(TESTFILES)
# library object files common to compression and decompression
COMOBJECTS= jcomapi.obj jutils.obj jerror.obj jmemmgr.obj $(SYSDEPMEM)
# compression library object files
CLIBOBJECTS= jcapimin.obj jcapistd.obj jctrans.obj jcparam.obj jdatadst.obj \
        jcinit.obj jcmaster.obj jcmarker.obj jcmainct.obj jcprepct.obj \
        jccoefct.obj jccolor.obj jcsample.obj jchuff.obj jcphuff.obj \
        jcdctmgr.obj jfdctfst.obj jfdctflt.obj jfdctint.obj
# decompression library object files
DLIBOBJECTS= jdapimin.obj jdapistd.obj jdtrans.obj jdatasrc.obj \
        jdmaster.obj jdinput.obj jdmarker.obj jdhuff.obj jdphuff.obj \
        jdmainct.obj jdcoefct.obj jdpostct.obj jddctmgr.obj jidctfst.obj \
        jidctflt.obj jidctint.obj jidctred.obj jdsample.obj jdcolor.obj \
        jquant1.obj jquant2.obj jdmerge.obj
# These objectfiles are included in libjpeg.lib
LIBOBJECTS= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)
# object files for sample applications (excluding library files)
COBJECTS= cjpeg.obj rdppm.obj rdgif.obj rdtarga.obj rdrle.obj rdbmp.obj \
        rdswitch.obj cdjpeg.obj
DOBJECTS= djpeg.obj wrppm.obj wrgif.obj wrtarga.obj wrrle.obj wrbmp.obj \
        rdcolmap.obj cdjpeg.obj
TROBJECTS= jpegtran.obj rdswitch.obj cdjpeg.obj transupp.obj

# Template command for compiling .cpp to .obj
.cpp.obj:
	$(cc) $(CFLAGS) $*.cpp


all: libjpeg.lib cjpeg.exe djpeg.exe jpegtran.exe rdjpgcom.exe wrjpgcom.exe

libjpeg.lib: $(LIBOBJECTS)
	$(RM) libjpeg.lib
	lib -out:libjpeg.lib  $(LIBOBJECTS)

cjpeg.exe: $(COBJECTS) libjpeg.lib
	$(link) $(LDFLAGS) -out:cjpeg.exe $(COBJECTS) libjpeg.lib $(LDLIBS)

djpeg.exe: $(DOBJECTS) libjpeg.lib
	$(link) $(LDFLAGS) -out:djpeg.exe $(DOBJECTS) libjpeg.lib $(LDLIBS)

jpegtran.exe: $(TROBJECTS) libjpeg.lib
	$(link) $(LDFLAGS) -out:jpegtran.exe $(TROBJECTS) libjpeg.lib $(LDLIBS)

rdjpgcom.exe: rdjpgcom.obj
	$(link) $(LDFLAGS) -out:rdjpgcom.exe rdjpgcom.obj $(LDLIBS)

wrjpgcom.exe: wrjpgcom.obj
	$(link) $(LDFLAGS) -out:wrjpgcom.exe wrjpgcom.obj $(LDLIBS)


clean:
	$(RM) *.obj *.exe libjpeg.lib
	$(RM) testout*

test: cjpeg.exe djpeg.exe jpegtran.exe
	$(RM) testout*
	.\djpeg -dct int -ppm -outfile testout.ppm  testorig.jpg
	.\djpeg -dct int -bmp -colors 256 -outfile testout.bmp  testorig.jpg
	.\cjpeg -dct int -outfile testout.jpg  testimg.ppm
	.\djpeg -dct int -ppm -outfile testoutp.ppm testprog.jpg
	.\cjpeg -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
	.\jpegtran -outfile testoutt.jpg testprog.jpg
	fc /b testimg.ppm testout.ppm
	fc /b testimg.bmp testout.bmp
	fc /b testimg.jpg testout.jpg
	fc /b testimg.ppm testoutp.ppm
	fc /b testimgp.jpg testoutp.jpg
	fc /b testorig.jpg testoutt.jpg


jcapimin.obj: jcapimin.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcapistd.obj: jcapistd.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jccoefct.obj: jccoefct.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jccolor.obj: jccolor.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcdctmgr.obj: jcdctmgr.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jchuff.obj: jchuff.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jchuff.h
jcinit.obj: jcinit.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmainct.obj: jcmainct.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmarker.obj: jcmarker.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmaster.obj: jcmaster.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcomapi.obj: jcomapi.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcparam.obj: jcparam.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcphuff.obj: jcphuff.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jchuff.h
jcprepct.obj: jcprepct.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcsample.obj: jcsample.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jctrans.obj: jctrans.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdapimin.obj: jdapimin.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdapistd.obj: jdapistd.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdatadst.obj: jdatadst.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h
jdatasrc.obj: jdatasrc.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h
jdcoefct.obj: jdcoefct.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdcolor.obj: jdcolor.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jddctmgr.obj: jddctmgr.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jdhuff.obj: jdhuff.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdhuff.h
jdinput.obj: jdinput.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmainct.obj: jdmainct.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmarker.obj: jdmarker.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmaster.obj: jdmaster.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmerge.obj: jdmerge.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdphuff.obj: jdphuff.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdhuff.h
jdpostct.obj: jdpostct.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdsample.obj: jdsample.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdtrans.obj: jdtrans.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jerror.obj: jerror.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jversion.h jerror.h
jfdctflt.obj: jfdctflt.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jfdctfst.obj: jfdctfst.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jfdctint.obj: jfdctint.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctflt.obj: jidctflt.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctfst.obj: jidctfst.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctint.obj: jidctint.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctred.obj: jidctred.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jquant1.obj: jquant1.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jquant2.obj: jquant2.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jutils.obj: jutils.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jmemmgr.obj: jmemmgr.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemansi.obj: jmemansi.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemname.obj: jmemname.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemnobs.obj: jmemnobs.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemdos.obj: jmemdos.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemmac.obj: jmemmac.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
cjpeg.obj: cjpeg.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h
djpeg.obj: djpeg.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h
jpegtran.obj: jpegtran.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h transupp.h jversion.h
rdjpgcom.obj: rdjpgcom.cpp jinclude.h jconfig.h
wrjpgcom.obj: wrjpgcom.cpp jinclude.h jconfig.h
cdjpeg.obj: cdjpeg.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdcolmap.obj: rdcolmap.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdswitch.obj: rdswitch.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
transupp.obj: transupp.cpp jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h transupp.h
rdppm.obj: rdppm.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrppm.obj: wrppm.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdgif.obj: rdgif.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrgif.obj: wrgif.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdtarga.obj: rdtarga.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrtarga.obj: wrtarga.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdbmp.obj: rdbmp.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrbmp.obj: wrbmp.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdrle.obj: rdrle.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrrle.obj: wrrle.cpp cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
