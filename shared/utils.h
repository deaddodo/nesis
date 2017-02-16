#ifndef __SHARED_H__
#define __SHARED_H__
#define SET_BIT(v, x) v |= 1 << x
#define CLEAR_BIT(v, x) v &= ~(1 << x)
#define TOGGLE_BIT(v, x) v ^= 1 << x
#define GET_BIT(v, x) (v >> x) & 1

#define LE_BYTE_TO_SHORT(a, b) (unsigned short)((unsigned short)b<<8|a)
#endif