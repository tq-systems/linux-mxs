#
# Makefile for the linux kernel.
#
obj-y += pinctrl.o pingroup.o clock.o device.o serial.o power.o bus_freq.o pm.o sleep.o

# Board select
obj-$(CONFIG_MACH_MX28EVK) += mx28evk.o mx28evk_pins.o
obj-$(CONFIG_MACH_MBA28) += mx28evk.o mx28evk_pins.o
obj-$(CONFIG_MODULE_IEQMA28) += module-ieqma28.o
obj-$(CONFIG_MODULE_TQMA28L) += module-tqma28l.o
obj-$(CONFIG_MACH_QX28) += qx28.o qx28_pins.o
obj-$(CONFIG_MACH_VC200) += board-vc200.o board-vc200_pins.o
obj-$(CONFIG_GENERIC_GPIO) += gpio.o
obj-$(CONFIG_MXS_RAM_FREQ_SCALING) +=emi.o emi_settings.o

# USB support
ifneq ($(strip $(CONFIG_USB_GADGET_ARC) $(CONFIG_USB_EHCI_ARC_OTG)),)
	obj-y   += usb_dr.o
endif
obj-$(CONFIG_USB_EHCI_ARC_H1)   += usb_h1.o
