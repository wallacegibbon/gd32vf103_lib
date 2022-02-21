#ifndef __N200_ECLIC_H
#define __N200_ECLIC_H

#include <riscv_const.h>

#define ECLICINTCTLBITS		4

//ECLIC memory map
// Offset
// 0x0000	1B		RW	ecliccfg
#define ECLIC_CFG_OFFSET	0x0

// 0x0004	4B		R	eclicinfo
#define ECLIC_INFO_OFFSET	0x4

// 0x000B       1B		RW	mintthresh 
#define ECLIC_MTH_OFFSET	0xB


// 0x1000+4*i	1B/input	RW	eclicintip[i]
#define ECLIC_INT_IP_OFFSET	_AC(0x1000, UL)

// 0x1001+4*i   1B/input	RW	eclicintie[i]
#define ECLIC_INT_IE_OFFSET	_AC(0x1001, UL)

// 0x1002+4*i   1B/input	RW	eclicintattr[i]
#define ECLIC_INT_ATTR_OFFSET	_AC(0x1002, UL)

#define ECLIC_INT_ATTR_SHV		1
#define ECLIC_INT_ATTR_TRIG_LEVEL	0
#define ECLIC_INT_ATTR_TRIG_EDGE	2
#define ECLIC_INT_ATTR_TRIG_POS		0
#define ECLIC_INT_ATTR_TRIG_NEG		4

// 0x1003+4*i	1B/input	RW	eclicintctl[i]
#define ECLIC_INT_CTRL_OFFSET	_AC(0x1003, UL)
//
//  ...
//
#define ECLIC_ADDR_BASE		0xd2000000

#define ECLIC_CFG_NLBITS_MASK	_AC(0x1E, UL)
#define ECLIC_CFG_NLBITS_LSB	1

#define MSIP_HANDLER	eclic_msip_handler
#define MTIME_HANDLER	eclic_mtip_handler
#define BWEI_HANDLER	eclic_bwei_handler
#define PMOVI_HANDLER	eclic_pmovi_handler

#endif
