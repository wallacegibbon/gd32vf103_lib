#ifndef GD32VF103_ECLIC_H
#define GD32VF103_ECLIC_H

#include "gd32vf103.h"

/* constants definitions */
#define ECLIC_PRIGROUP_LEVEL0_PRIO4 0 /* 0 bits for level 4 bits for priority */
#define ECLIC_PRIGROUP_LEVEL1_PRIO3 1 /* 1 bits for level 3 bits for priority */
#define ECLIC_PRIGROUP_LEVEL2_PRIO2 2 /* 2 bits for level 2 bits for priority */
#define ECLIC_PRIGROUP_LEVEL3_PRIO1 3 /* 3 bits for level 1 bits for priority */
#define ECLIC_PRIGROUP_LEVEL4_PRIO0 4 /* 4 bits for level 0 bits for priority */

#define __SEV           eclic_send_event

/* function declarations */
/* enable the global interrupt */
void eclic_global_interrupt_enable(void);

/* disable the global interrupt */
void eclic_global_interrupt_disable(void);

/* set the priority group */
void eclic_priority_group_set(uint32_t prigroup);

/* enable the interrupt request */
void eclic_irq_enable(uint32_t source, uint8_t level, uint8_t priority);

/* disable the interrupt request */
void eclic_irq_disable(uint32_t source);

/* reset system */
void eclic_system_reset(void);

/* send event(SEV) */
void eclic_send_event(void);

#endif
