# $Id: Makefile,v 1.1.1.1 2006/02/04 03:35:01 freza Exp $
# first compile
# make cris-axis-linux-gnu


CROSS  = bfin-uclinux-
CC    = $(CROSS)gcc

CFLAGS += -Wall -DMOENCHD -DMCB_FUNCS -DDACS_INT  -DDEBUG #-DVERBOSE #-DVERYVERBOSE  #-DVIRTUAL  #-DDACS_INT_CSERVER


PROGS= 		moenchDetectorServer 
INSTDIR= 	/tftpboot
INSTMODE= 	0777



BINS = testlib_sharedlibc
SRCS = server.c server_funcs.c communication_funcs.c firmware_funcs.c mcb_funcs.c trimming_funcs.c  sharedmemory.c 
OBJS = $(SRCS:%.c=%.o) 



all:	clean versioning $(PROGS) 

boot: $(OBJS)

versioning: 
	@echo `tput setaf 6;	./updateGitVersion.sh; tput sgr0;`

$(PROGS): $(OBJS)
	echo $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS_$@) $(LDFLAGS_$@)


install:	$(PROGS)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)


romfs:
	$(ROMFSINST)	/bin/$(PROGS) 

clean:
	rm -rf $(PROGS)  *.o *.gdb
	





