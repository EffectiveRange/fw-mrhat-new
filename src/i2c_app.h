/* 
 * File:   i2c_app.h
 * Author: emtszeb
 *
 * Created on August 2, 2024, 5:33 PM
 */

#ifndef I2C_APP_H
#define	I2C_APP_H
#include "i2c1_multimode.h"

#ifdef	__cplusplus
extern "C" {
#endif


    
    
    
#define I2C_TRY_CNT 3U
#define I2C_TMOUT_MS 100U //todo check if lower is better

extern volatile  uint8_t CLIENT_DATA[] ;


int I2CWrite(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len);
int I2CWriteWithPI(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len);

int I2CWriteRead(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len);
int I2CWriteReadWithPI(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len);
//I2CWriteReadWithPi
void I2CSwitchMode(enum I2C1_Mode new_mode);
int I2CReadByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *dest) ;
int I2CWriteByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t val);
int I2CWrite(uint8_t dev_addr, uint8_t *tx_buf, size_t tx_len);
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_APP_H */

