include $(TOPDIR)/config.mk

LIB = $(obj)lib$(BOARD).o

COBJS-y	+= $(BOARD).o ddr_init_table.o
COBJS-$(CONFIG_BOARD_VENOM) += venom.o

COBJS	:= $(sort $(COBJS-y))
SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

$(LIB): $(obj).depend $(OBJS) $(SOBJS)
	$(call cmd_link_o_target, $(OBJS), $(SOBJS))

clean:
	rm -f $(SOBJS) $(OBJS)

distclean:	clean
	rm -f $(LIB) core *.bak .depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk
#include /home/lab/uboot_from_denx_tot/uboot/rules.mk

sinclude $(obj).depend

#########################################################################
