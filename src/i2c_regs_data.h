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


#define I2C_CLIENT_LOCATION_SIZE 20
    

extern volatile  uint8_t CLIENT_DATA[] ;



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REG_DATA_H */

