######################
# Activated features #
######################

FEATURES=

#################################
# Architecture dependant things #
#################################

ARCHITECTURE=-DARCH_IS_IA32
BUS=-DARCH_IS_32BIT
ENDIANNESS=-DARCH_IS_LITTLE_ENDIAN
SHARED_EXTENSION=so
STATIC_EXTENSION=a
OBJECT_EXTENSION=o

############
# Compiler #
############

CXX=g++
CXXFLAGS=-Wall -O2 -fstrength-reduce -finline-functions -freduce-all-givs -ffast-math -fomit-frame-pointer -pthread  -I/home/torri/local/stlport  -I/home/torri/local/include

#############
# Assembler #
#############

AS=nasm
AFLAGS=-I$(<D)/ -f elf 
ASSEMBLY_EXTENSION=asm

##########
# Linker #
##########

SPECIFIC_LDFLAGS=-Wl,-soname,libavisynth.$(SHARED_EXTENSION).$(API_MAJOR) -shared -lc -lm 
API_MAJOR=3
API_MINOR=0
RANLIB=ranlib

################
# Installation #
################

INSTALL=/usr/bin/install -c
prefix=/home/torri/local/avisynth
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

###########
# Sources #
###########

GENERIC_SOURCES=$(SRC_GENERIC)
ASSEMBLY_SOURCES=$(SRC_IA32)
DCT_IA64_SOURCES=$()
GENERIC_OBJECTS=$(SRC_GENERIC:.cpp=.o)
ASSEMBLY_OBJECTS=$(SRC_IA32:.asm=.o)
DCT_IA64_OBJECTS=$(:.asm=.o)

STATIC_LIB=libavisynth.$(STATIC_EXTENSION)
SHARED_LIB=libavisynth.$(SHARED_EXTENSION).$(API_MAJOR).$(API_MINOR)
PRE_SHARED_LIB=libavisynth.$(SHARED_EXTENSION).$(API_MAJOR).$(API_MINOR)

