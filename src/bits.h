/* 
 * File:   bits.h
 * Author: emtszeb
 *
 * Created on September 11, 2024, 7:30 PM
 */

#ifndef BITS_H
#define	BITS_H

#ifdef	__cplusplus
extern "C" {
#endif


#define SET_BIT(x, pos) (x |= (1U << pos))
#define CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))
#define TOGGLE_BIT(x, pos) x ^= (1U<< pos)
#define CHECK_BIT(x, pos) (x & (1UL << pos) )
#define GET_BIT(x, pos) ((x & ( 1 << pos)) >> pos)


#ifdef	__cplusplus
}
#endif

#endif	/* BITS_H */

