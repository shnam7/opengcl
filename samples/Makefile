#
#		Makefile for OpenGCL Examples.
#
#		All the examples are asumed to be composed of single file.
#
#		Written by Soo-Hyuk Nam.
#			2001. 1. 9. Tue.
#

TOPDIR		:= ../
include $(TOPDIR)makehead.env

C_SRC		:= exthread.c
CXX_SRC 	:= exque.cpp exsocket.cpp extok.cpp exthread_x.cpp extime.cpp exmmap.cpp \
				exmbox.cpp exlist.cpp
GCLLIB		:= $(TOPDIR)src/libopengcl.a
LIBS		:= $(GCLLIB) -lpthread
TARGETS		:= $(CXX_SRC:.cpp=) $(C_SRC:.c=)

CFLAGS		+= -D_REENTRANT


%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)
	

### targets ----------
all: $(GCLLIB) $(TARGETS)

$(GCLLIB):
	$(MAKE) -C $(TOPDIR) opengcl

include $(TOPDIR)maketail.env
