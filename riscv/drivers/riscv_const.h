#ifndef __RISCV_CONST_H__
#define __RISCV_CONST_H__

#ifdef __ASSEMBLER__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define _AC(X,Y)	(X##Y)
#define _AT(T,X)	((T) (X))
#endif

#define _BITUL(x)	(_AC(1, UL) << (x))
#define _BITULL(x)	(_AC(1, ULL) << (x))

#endif
