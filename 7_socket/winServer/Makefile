##############################################################################
# Copyright (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
#                No part of this program may be photocopied, reproduced,
#                or translated to another programming language without the
#                prior written consent of Innovative Data Solutions Inc.
##############################################################################


########################### File Header ###########################
# 
# File Name: Makefile
# 
# Description: 
# 
# Revision History 
# ________________ 
# Date      	Author    	Description 
# 
# Mar-26-2005	Hanna Bullata, PhD	
# 
###################################################################

CC			= gcc
#CFLAGS			= -Wall 
CFLAGS			= -g
MFLAG			= 
DEPEND			= src/.dependency_$(CC)

SOURCES			= error.c read_write.c \
			  net_serv.c net_serv_funcs.c
ESOURCES		= 
PROGNAME		= winServer
LIBS 			= -lsocket -lnsl


BINDIR			= ./bin-cc-${OSTYPE}
SRCDIR			= ./src
INCDIR			= -Iinclude

OBJECTS			= $(SOURCES:%.c=$(BINDIR)/%.o)

all:			depend $(PROGNAME)

depend:			$(DEPEND)

clean:
	rm -rf $(OBJECTS) $(BINDIR)/$(PROGNAME)


#Final Phase ... linking
$(PROGNAME): 		$(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(OBJECTS) $(LIBS)

# convert from .o to .c
$(BINDIR)/%.o :			$(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $(INCDIR) $<

$(BINDIR)/%.o :			%.c
	$(CC) $(CFLAGS) -c -o $@ $(INCDIR) $<

##Create a dependency file
$(DEPEND):		$(SOURCES:%.c=$(SRCDIR)/%.c)
	$(CC) $(MFLAG) $(INCDIR) $(SOURCES:%.c=$(SRCDIR)/%.c)| sed 's~^.*\.o~$(BINDIR)/&~' > $(DEPEND)

include $(DEPEND)

