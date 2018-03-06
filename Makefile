#
#	Makefile for OpenGCL
#
#	Written by Soo-Hyuk Nam.
#		2003. 3. 3. Mon.
#

PREFIX		:= /usr/
INST_INC	:= $(PREFIX)include/
INST_LIB	:= $(PREFIX)lib/

H_LIST		:= $(shell ls -1 include | grep .h$$)
L_LIST		:= $(shell ls -1 src | grep .a$$)

MAKE	:= make

### Targets
all: opengcl ex

opengcl:
	$(MAKE) -C src

ex:
	$(MAKE) -C samples

clean:
	$(MAKE) -C src clean
	$(MAKE) -C samples clean
	$(MAKE) -C win32 clean

install:
	-mkdir -p $(INST_INC)
	-mkdir -p $(INST_LIB)
	cp $(addprefix include/, $(H_LIST)) $(INST_INC)
	cp $(addprefix src/, $(L_LIST)) $(INST_LIB)

uninstall:
	-rm $(addprefix $(INST_INC), $(H_LIST))
	-rm $(addprefix $(INST_LIB), $(L_LIST))
	-rmdir $(INST_INC)
	-rmdir $(INST_LIB)
	-rmdir $(PREFIX)

lsav: clean
	tar cvfz ../opengcl-`date +%Y-%m-%d-%H-%M-%S`.tgz *	

cl:
	rm -f ChangeLog
	cvs2cl --no-wrap -S -T --hide-filenames
