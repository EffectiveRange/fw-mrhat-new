/* 
 * File:   i2c_reg_data.h
 * Author: emtszeb
 *
 * Created on August 11, 2024, 5:57 PM
 */

#ifndef I2C_REG_DATA_H
#define	I2C_REG_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "system.h"
#include "i2c_regs.h"
#include "bits.h"
#if 0
#define SET_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= SHUT_REQ; } while(0);
#define CLEAR_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] &= ~(SHUT_REQ) ; } while(0);
#define IS_SHUTDOWN_REQ()  ((CLIENT_DATA[REG_STAT_0_ADDR] & SHUT_REQ_MASK) == SHUT_REQ)

#define SET_PI_HB_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= PI_HB; } while(0);
#define SET_PI_HB_NOT_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] &= ~(PI_HB) ; } while(0); 
#define IS_PI_HB_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR] & PI_HB_MASK) == PI_HB)
#define IS_PI_HB_NOT_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR] & PI_HB_MASK) == 0x0)

#define SET_BAT_AVAIL()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] |= (BAT_AVAIL); } while(0);
#define CLEAR_BAT_AVAIL()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] &= ~(BAT_AVAIL) ; } while(0);
#define IS_BAT_AVAIL()  ((CLIENT_DATA[REG_BAT_STAT_0_ADDR] & BAT_AVAIL_MASK ) == (BAT_AVAIL))


#define SET_BAT_CHECK_ERR()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] |= BAT_CHECK_ERR; } while(0);
#define CLEAR_BAT_ERR()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] &=~(BAT_CHECK_ERR) ; } while(0);
#define IS_BAT_CHECK_ERR()  ((CLIENT_DATA[REG_BAT_STAT_0_ADDR] & BAT_CHECK_ERR_MASK) == (BAT_CHECK_ERR))
#else

#define REG_SET_SHUTDOWN_REQ()  do {  SET_BIT (CLIENT_DATA[REG_STAT_0_ADDR] ,SHUT_REQ_POS); } while(0);
#define REG_CLEAR_SHUTDOWN_REQ()  do {  CLEAR_BIT (CLIENT_DATA[REG_STAT_0_ADDR] ,SHUT_REQ_POS); } while(0);
#define REG_IS_SHUTDOWN_REQ()   (GET_BIT(CLIENT_DATA[REG_STAT_0_ADDR] ,SHUT_REQ_POS) == 1)

#define REG_SET_PI_HB_OK()  do { SET_BIT(CLIENT_DATA[REG_STAT_0_ADDR] , PI_HB_POS); } while(0);
#define REG_CLEAR_PI_HB_OK()  do { CLEAR_BIT(CLIENT_DATA[REG_STAT_0_ADDR],PI_HB_POS ); } while(0); 
#define REG_IS_PI_HB_OK()  (GET_BIT(CLIENT_DATA[REG_STAT_0_ADDR] , PI_HB_POS) == 1)


#define REG_SET_BAT_AVAIL()  do { SET_BIT(CLIENT_DATA[REG_BAT_STAT_0_ADDR] , BAT_AVAIL_POS); } while(0);
#define REG_CLEAR_BAT_AVAIL()  do { CLEAR_BIT(CLIENT_DATA[REG_BAT_STAT_0_ADDR] , BAT_AVAIL_POS) ; } while(0);
#define REG_IS_BAT_AVAIL()  ((GET_BIT(CLIENT_DATA[REG_BAT_STAT_0_ADDR] , BAT_AVAIL_POS ) == 1))


#define REG_SET_BAT_CHECK_ERR()  do { SET_BIT(CLIENT_DATA[REG_BAT_STAT_0_ADDR] , BAT_CHECK_ERR_POS); } while(0);
#define REG_CLEAR_BAT_ERR()  do { CLEAR_BIT(CLIENT_DATA[REG_BAT_STAT_0_ADDR] ,BAT_CHECK_ERR_POS) ; } while(0);
#define REG_IS_BAT_CHECK_ERR()  ((GET_BIT(CLIENT_DATA[REG_BAT_STAT_0_ADDR] , BAT_CHECK_ERR_POS) == 1))



#endif
#define I2C_CLIENT_LOCATION_SIZE 20
    

extern volatile  uint8_t CLIENT_DATA[] ;



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REG_DATA_H */

