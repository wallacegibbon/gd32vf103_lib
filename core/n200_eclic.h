#ifndef __N200_ECLIC_H
#define __N200_ECLIC_H

// This header file may also be included by assembly language source code.
// Do not use typecasts `(like (* (volatile uint8_t *) Name))` that only exists
// in C language.

#include <riscv_const.h>

#define ECLIC_ADDR_BASE			0xD2000000

#define ECLICINTCTLBITS			4

// ecliccfg (8 bit)
#define ECLIC_CFG_OFFSET		0

// eclicinfo (32 bit)
#define ECLIC_INFO_OFFSET		4

// mintthresh (8 bit)
#define ECLIC_MTH_OFFSET		0xB

// eclicintip[i] (8 bit):	0x1000 + 4*i
#define ECLIC_INT_IP_OFFSET		_AC(0x1000, UL)

// eclicintie[i] (8 bit):	0x1001 + 4*i
#define ECLIC_INT_IE_OFFSET		_AC(0x1001, UL)

// eclicintattr[i] (8 bit):	0x1002 + 4*i
#define ECLIC_INT_ATTR_OFFSET		_AC(0x1002, UL)

// eclicintctl[i] (8 bit):	0x1003 + 4*i
#define ECLIC_INT_CTRL_OFFSET		_AC(0x1003, UL)

#define ECLIC_INT_ATTR_SHV		1
#define ECLIC_INT_ATTR_TRIG_LEVEL	0
#define ECLIC_INT_ATTR_TRIG_EDGE	2
#define ECLIC_INT_ATTR_TRIG_POS		0
#define ECLIC_INT_ATTR_TRIG_NEG		4

#define ECLIC_CFG_NLBITS_MASK		_AC(0x1E, UL)
#define ECLIC_CFG_NLBITS_LSB		1

#define MSIP_HANDLER			eclic_msip_handler
#define MTIME_HANDLER			eclic_mtip_handler
#define BWEI_HANDLER			eclic_bwei_handler
#define PMOVI_HANDLER			eclic_pmovi_handler

#endif

