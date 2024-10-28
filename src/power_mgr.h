/*
 * File:   bat_mgr.h
 * Author: emtszeb
 *
 * Created on July 11, 2024, 6:24 PM
 */

#ifndef BAT_MGR_H
#define BAT_MGR_H
#include "tasks.h"
#ifdef __cplusplus
extern "C" {
#endif
#define BQ_I2C_ADDR (0x6b)

uint64_t GetTimeMs(void);
void PowMgrSystemReset(volatile TaskDescr*);
void PowMgrEnableDisableCharging(volatile TaskDescr* taskd);
int PowMgrGoToShipMode(void);
void PowMgrStartMesIBAT(volatile TaskDescr* taskd);
void BQ_INT_PinChanged(void);
void PowMgrReadIBAT(volatile TaskDescr* taskd) ;
int PowMgrMesIBATOneShot(void);
int ReadPartId(void);
#ifdef __cplusplus
}
#endif

#endif /* BAT_MGR_H */
