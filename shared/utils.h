#ifndef __SHARED_H__
#define __SHARED_H__
#define SET_BIT(v, x) v |= 1 << x
#define CLEAR_BIT(v, x) v &= ~(1 << x)
#define TOGGLE_BIT(v, x) v ^= 1 << x
#define GET_BIT(v, x) (v >> x) & 1

#define LE_BYTE_TO_SHORT(a, b) (unsigned short)((unsigned short)b<<8|a)

#define FLAG_CARRY 0
#define FLAG_ZERO 1
#define FLAG_INTERRUPT 2
#define FLAG_DECIMAL 3
#define FLAG_BREAK 4
#define FLAG_UNUSED 5
#define FLAG_OVERFLOW 6
#define FLAG_NEGATIVE 7
#endif