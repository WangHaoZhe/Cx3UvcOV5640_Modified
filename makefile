## Copyright Cypress Semiconductor Corporation, 2010-2023,
## All Rights Reserved
## UNPUBLISHED, LICENSED SOFTWARE.
##
## CONFIDENTIAL AND PROPRIETARY INFORMATION
## WHICH IS THE PROPERTY OF CYPRESS.
##
## Use of this file is governed
## by the license agreement included in the file
##
##      <install>/license/license.txt
##
## where <install> is the Cypress software
## installation root directory path.
##

FX3FWROOT=../../..

all:compile

# Add the OmniVision 5640 Image Sensor library.
ifeq ($(CYFXBUILD),arm)
    LDLIBS = $(FX3FWROOT)/fw_lib/$(CYSDKVERSION)/$(CYCONFOPT)/libcy_ov5640.a
else
    LDLIBS = -lcy_ov5640
endif

include $(FX3FWROOT)/fw_build/fx3_fw/cx3_build_config.mak

MODULE = cycx3_uvc

SOURCE= $(MODULE).c 	\
	cycx3_uvcdscr.c	\
	cycx3_videostreaming.c \
	cyu3imagesensor.c \
	cyfxtx.c

ifeq ($(CYFXBUILD),arm)
SOURCE_ASM=cyfx_startup.S
else
SOURCE_ASM=cyfx_gcc_startup.S
endif

C_OBJECT=$(SOURCE:%.c=./%.o)
A_OBJECT=$(SOURCE_ASM:%.S=./%.o)

EXES = $(MODULE).$(EXEEXT)

$(MODULE).$(EXEEXT): $(A_OBJECT) $(C_OBJECT)
	$(LINK)

cyfxtx.c:
	cp $(FX3FWROOT)/fw_build/fx3_fw/cyfxtx.c .

cyfx_startup.S:
	cp $(FX3FWROOT)/fw_build/fx3_fw/cyfx_startup.S .

cyfx_gcc_startup.S:
	cp $(FX3FWROOT)/fw_build/fx3_fw/cyfx_gcc_startup.S .

$(C_OBJECT) : %.o : %.c
	$(COMPILE)

$(A_OBJECT) : %.o : %.S
	$(ASSEMBLE)

clean:
	rm -f ./$(MODULE).$(EXEEXT)
	rm -f ./$(MODULE).map
	rm -f ./*.o
	rm -f cyfxtx.c cyfx_startup.S cyfx_gcc_startup.S


compile: $(C_OBJECT) $(A_OBJECT) $(EXES)

#[]#
