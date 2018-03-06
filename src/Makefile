#
#	Makefile for OpenGCL Source
#
#	Written By Soo-Htyk Nam.
#		2001. 1. 9. Tue.
#

TOPDIR		:= ../
include $(TOPDIR)makehead.env 

TARGETS		:= libopengcl.a
C_SRC		:= gque.c gsocket.c gtok.c gtime.c gthread.c gutils.c gmbox.c
CXX_SRC		:= gsocket_x.cpp gthread_x.cpp gmmap.cpp


## targets ----------

$(TARGETS): $(OBJS)
	$(AR) rs $@ $(OBJS)
	$(STRIP) $(TARGETS)


include $(TOPDIR)maketail.env 
