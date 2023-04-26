#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#define CPU_F ((double)25001984)

#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_s(x)  __delay_cycles((long)(CPU_F*(double)x))

void configSysMainClock(void);

#endif
