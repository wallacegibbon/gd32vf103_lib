#ifndef GD32VF103_EXTI_H
#define GD32VF103_EXTI_H

#include "gd32vf103.h"

#define EXTI				EXTI_BASE

// interrupt enable register
#define EXTI_INTEN			REG32(EXTI + 0x00U)

// event enable register
#define EXTI_EVEN			REG32(EXTI + 0x04U)

// rising edge trigger enable register
#define EXTI_RTEN			REG32(EXTI + 0x08U)

// falling trigger enable register
#define EXTI_FTEN			REG32(EXTI + 0x0CU)

// software interrupt event register
#define EXTI_SWIEV			REG32(EXTI + 0x10U)

// pending register
#define EXTI_PD				REG32(EXTI + 0x14U)


// interrupt from line 0-18
#define EXTI_INTEN_INTEN0		BIT(0)
#define EXTI_INTEN_INTEN1		BIT(1)
#define EXTI_INTEN_INTEN2		BIT(2)
#define EXTI_INTEN_INTEN3		BIT(3)
#define EXTI_INTEN_INTEN4		BIT(4)
#define EXTI_INTEN_INTEN5		BIT(5)
#define EXTI_INTEN_INTEN6		BIT(6)
#define EXTI_INTEN_INTEN7		BIT(7)
#define EXTI_INTEN_INTEN8		BIT(8)
#define EXTI_INTEN_INTEN9		BIT(9)
#define EXTI_INTEN_INTEN10		BIT(10)
#define EXTI_INTEN_INTEN11		BIT(11)
#define EXTI_INTEN_INTEN12		BIT(12)
#define EXTI_INTEN_INTEN13		BIT(13)
#define EXTI_INTEN_INTEN14		BIT(14)
#define EXTI_INTEN_INTEN15		BIT(15)
#define EXTI_INTEN_INTEN16		BIT(16)
#define EXTI_INTEN_INTEN17		BIT(17)
#define EXTI_INTEN_INTEN18		BIT(18)

// event from line 0-18
#define EXTI_EVEN_EVEN0			BIT(0)
#define EXTI_EVEN_EVEN1			BIT(1)
#define EXTI_EVEN_EVEN2			BIT(2)
#define EXTI_EVEN_EVEN3			BIT(3)
#define EXTI_EVEN_EVEN4			BIT(4)
#define EXTI_EVEN_EVEN5			BIT(5)
#define EXTI_EVEN_EVEN6			BIT(6)
#define EXTI_EVEN_EVEN7			BIT(7)
#define EXTI_EVEN_EVEN8			BIT(8)
#define EXTI_EVEN_EVEN9			BIT(9)
#define EXTI_EVEN_EVEN10		BIT(10)
#define EXTI_EVEN_EVEN11		BIT(11)
#define EXTI_EVEN_EVEN12		BIT(12)
#define EXTI_EVEN_EVEN13		BIT(13)
#define EXTI_EVEN_EVEN14		BIT(14)
#define EXTI_EVEN_EVEN15		BIT(15)
#define EXTI_EVEN_EVEN16		BIT(16)
#define EXTI_EVEN_EVEN17		BIT(17)
#define EXTI_EVEN_EVEN18		BIT(18)

// rising edge from line 0-18
#define EXTI_RTEN_RTEN0			BIT(0)
#define EXTI_RTEN_RTEN1			BIT(1)
#define EXTI_RTEN_RTEN2			BIT(2)
#define EXTI_RTEN_RTEN3			BIT(3)
#define EXTI_RTEN_RTEN4			BIT(4)
#define EXTI_RTEN_RTEN5			BIT(5)
#define EXTI_RTEN_RTEN6			BIT(6)
#define EXTI_RTEN_RTEN7			BIT(7)
#define EXTI_RTEN_RTEN8			BIT(8)
#define EXTI_RTEN_RTEN9			BIT(9)
#define EXTI_RTEN_RTEN10		BIT(10)
#define EXTI_RTEN_RTEN11		BIT(11)
#define EXTI_RTEN_RTEN12		BIT(12)
#define EXTI_RTEN_RTEN13		BIT(13)
#define EXTI_RTEN_RTEN14		BIT(14)
#define EXTI_RTEN_RTEN15		BIT(15)
#define EXTI_RTEN_RTEN16		BIT(16)
#define EXTI_RTEN_RTEN17		BIT(17)
#define EXTI_RTEN_RTEN18		BIT(18)

// falling edge from line 0
#define EXTI_FTEN_FTEN0			BIT(0)
#define EXTI_FTEN_FTEN1			BIT(1)
#define EXTI_FTEN_FTEN2			BIT(2)
#define EXTI_FTEN_FTEN3			BIT(3)
#define EXTI_FTEN_FTEN4			BIT(4)
#define EXTI_FTEN_FTEN5			BIT(5)
#define EXTI_FTEN_FTEN6			BIT(6)
#define EXTI_FTEN_FTEN7			BIT(7)
#define EXTI_FTEN_FTEN8			BIT(8)
#define EXTI_FTEN_FTEN9			BIT(9)
#define EXTI_FTEN_FTEN10		BIT(10)
#define EXTI_FTEN_FTEN11		BIT(11)
#define EXTI_FTEN_FTEN12		BIT(12)
#define EXTI_FTEN_FTEN13		BIT(13)
#define EXTI_FTEN_FTEN14		BIT(14)
#define EXTI_FTEN_FTEN15		BIT(15)
#define EXTI_FTEN_FTEN16		BIT(16)
#define EXTI_FTEN_FTEN17		BIT(17)
#define EXTI_FTEN_FTEN18		BIT(18)

// software interrupt/event request from line 0-18
#define EXTI_SWIEV_SWIEV0		BIT(0)
#define EXTI_SWIEV_SWIEV1		BIT(1)
#define EXTI_SWIEV_SWIEV2		BIT(2)
#define EXTI_SWIEV_SWIEV3		BIT(3)
#define EXTI_SWIEV_SWIEV4		BIT(4)
#define EXTI_SWIEV_SWIEV5		BIT(5)
#define EXTI_SWIEV_SWIEV6		BIT(6)
#define EXTI_SWIEV_SWIEV7		BIT(7)
#define EXTI_SWIEV_SWIEV8		BIT(8)
#define EXTI_SWIEV_SWIEV9		BIT(9)
#define EXTI_SWIEV_SWIEV10		BIT(10)
#define EXTI_SWIEV_SWIEV11		BIT(11)
#define EXTI_SWIEV_SWIEV12		BIT(12)
#define EXTI_SWIEV_SWIEV13		BIT(13)
#define EXTI_SWIEV_SWIEV14		BIT(14)
#define EXTI_SWIEV_SWIEV15		BIT(15)
#define EXTI_SWIEV_SWIEV16		BIT(16)
#define EXTI_SWIEV_SWIEV17		BIT(17)
#define EXTI_SWIEV_SWIEV18		BIT(18)

// interrupt/event pending status from line 0-18
#define EXTI_PD_PD0			BIT(0)
#define EXTI_PD_PD1			BIT(1)
#define EXTI_PD_PD2			BIT(2)
#define EXTI_PD_PD3			BIT(3)
#define EXTI_PD_PD4			BIT(4)
#define EXTI_PD_PD5			BIT(5)
#define EXTI_PD_PD6			BIT(6)
#define EXTI_PD_PD7			BIT(7)
#define EXTI_PD_PD8			BIT(8)
#define EXTI_PD_PD9			BIT(9)
#define EXTI_PD_PD10			BIT(10)
#define EXTI_PD_PD11			BIT(11)
#define EXTI_PD_PD12			BIT(12)
#define EXTI_PD_PD13			BIT(13)
#define EXTI_PD_PD14			BIT(14)
#define EXTI_PD_PD15			BIT(15)
#define EXTI_PD_PD16			BIT(16)
#define EXTI_PD_PD17			BIT(17)
#define EXTI_PD_PD18			BIT(18)

/* constants definitions */
/* EXTI line number */
enum exti_line {
	EXTI_0		= BIT(0),
	EXTI_1		= BIT(1),
	EXTI_2		= BIT(2),
	EXTI_3		= BIT(3),
	EXTI_4		= BIT(4),
	EXTI_5		= BIT(5),
	EXTI_6		= BIT(6),
	EXTI_7		= BIT(7),
	EXTI_8		= BIT(8),
	EXTI_9		= BIT(9),
	EXTI_10		= BIT(10),
	EXTI_11		= BIT(11),
	EXTI_12		= BIT(12),
	EXTI_13		= BIT(13),
	EXTI_14		= BIT(14),
	EXTI_15		= BIT(15),
	EXTI_16		= BIT(16),
	EXTI_17		= BIT(17),
	EXTI_18		= BIT(18),
};

/* external interrupt and event  */
enum exti_mode {
	EXTI_INTERRUPT		= 0,
	EXTI_EVENT
};

/* interrupt trigger mode */
enum exti_trig_type {
	EXTI_TRIG_RISING	= 0,
	EXTI_TRIG_FALLING,
	EXTI_TRIG_BOTH,	
	EXTI_TRIG_NONE
};

// initialization, EXTI lines configuration functions
void exti_deinit(void);

// enable the configuration of EXTI initialize
void exti_init(enum exti_line linex, enum exti_mode mode,
		enum exti_trig_type trig_type);

void exti_interrupt_enable(enum exti_line linex);

void exti_event_enable(enum exti_line linex);

void exti_interrupt_disable(enum exti_line linex);

void exti_event_disable(enum exti_line linex);

// interrupt & flag functions
enum flag_status exti_flag_get(enum exti_line linex);

void exti_flag_clear(enum exti_line linex);

enum flag_status exti_interrupt_flag_get(enum exti_line linex);

void exti_interrupt_flag_clear(enum exti_line linex);

void exti_software_interrupt_enable(enum exti_line linex);

void exti_software_interrupt_disable(enum exti_line linex);

#endif
