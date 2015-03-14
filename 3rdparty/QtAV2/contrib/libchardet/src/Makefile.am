#Makefile.am
AM_LDFLAGS = -fPIC
lib_LTLIBRARIES = libchardet.la
libchardet_la_SOURCES = CharDistribution.cpp \
nsEUCJPProber.h \
nsEscCharsetProber.cpp \
nsHebrewProber.h \
prmem.h \
nsCharSetProber.cpp \
nsSBCSGroupProber.cpp \
entry/impl.cpp \
JpCntx.cpp \
LangThaiModel.cpp \
nsMBCSGroupProber.cpp \
nsEscCharsetProber.h \
nsBig5Prober.h \
nsBig5Prober.cpp \
nsLatin1Prober.h \
nsPkgInt.h \
nsUniversalDetector.h \
nsEUCTWProber.cpp \
nsEUCKRProber.h \
nsEscSM.cpp \
nsEUCKRProber.cpp \
nscore.h \
LangGreekModel.cpp \
nsCodingStateMachine.h \
nsUniversalDetector.cpp \
CharDistribution.h \
nsSBCharSetProber.h \
nsEUCTWProber.h \
Makefile.am \
chardet.h \
nsSBCSGroupProber.h \
nsCharSetProber.h \
LangBulgarianModel.cpp \
nsMBCSSM.cpp \
nsGB2312Prober.h \
nsSBCharSetProber.cpp \
JpCntx.h \
LangHungarianModel.cpp \
nsUTF8Prober.h \
nsMBCSGroupProber.h \
nsGB2312Prober.cpp \
nsEUCJPProber.cpp \
nsHebrewProber.cpp \
LangHebrewModel.cpp \
nsLatin1Prober.cpp \
nsSJISProber.h \
tables/JISFreq.tab \
tables/Big5Freq.tab \
tables/EUCTWFreq.tab \
tables/GB2312Freq.tab \
tables/EUCKRFreq.tab \
nsUTF8Prober.cpp \
nsSJISProber.cpp \
LangCyrillicModel.cpp

AM_CXXFLAGS = -Wall -Wno-non-virtual-dtor -nostdinc++ -fno-rtti -fno-exceptions 
include_HEADERS = chardet.h
libchardet_la_CPPFLAGS = -nostdinc++ -fno-rtti -fno-exceptions -I tables

bin_PROGRAMS = chardetor
chardetor_SOURCES = chardetor.cpp
chardetor_LDADD = libchardet.la 
chardetor_LDFLAGS = -static

