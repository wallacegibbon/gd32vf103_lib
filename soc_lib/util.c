#include <stdint.h>
#include <gd32vf103.h>

void delay_ms(uint32_t count) {
	uint64_t start_mtime, delta_mtime;

	// do not start measuring until we see an mtime tick
	uint64_t tmp = get_timer_value();
	do {
		start_mtime = get_timer_value();
	} while (start_mtime == tmp);

	// the frequency of mtime is `sys_core_clk / 4`
	do {
		delta_mtime = get_timer_value() - start_mtime;
	} while (delta_mtime < (sys_core_clk / 4000 * count));
}
