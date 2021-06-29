/*
 * dynamixel.h
 *
 *  Created on: Mar 7, 2021
 *      Author: Carlos Silva
 */

#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_

#include "main.h"

// Endereços
#define DYNA_TORQUEEN_REG 24
#define DYNA_LED_REG 25
#define DYNA_PIDD_REG 26
#define DYNA_PIDI_REG 27
#define DYNA_PIDP_REG 28
#define DYNA_GOAL_REG 30
#define DYNA_MOVESPEED_REG 32
#define DYNA_TORQUELIMIT_REG 34
#define DYNA_PRESENTPOS_REG 36
#define DYNA_PRESENTSPEED_REG 38
#define DYNA_PRESENTLOAD_REG 40
#define DYNA_PRESENTVOLT_REG 42
#define DYNA_TEMP_REG 43
#define DYNA_REGISTERED_REG 44
#define DYNA_MOVING_REG 46
#define DYNA_GOALACC_REG 73

// Instruções
#define DYNA_PING 0x01
#define DYNA_READ 0x02
#define DYNA_WRITE 0x03
#define DYNA_REGWRITE 0x04
#define DYNA_ACTION 0x05
#define DYNA_RESET 0x06
#define DYNA_REBOOT 0x08
#define DYNA_SYNCWRITE 0x83
#define DYNA_BULKREAD 0x92

/* Protótipos de Funções */

// Escrita
void dyna_sendFrame(UART_HandleTypeDef* huart, unsigned char ID, unsigned char instruct, unsigned char params[], unsigned char paramLen);
void dyna_torqueEnable(UART_HandleTypeDef* huart, unsigned char ID, unsigned char value);
void dyna_led(UART_HandleTypeDef* huart, unsigned char ID, unsigned char value);
void dyna_setPID(UART_HandleTypeDef* huart, unsigned char ID, unsigned char valueP, unsigned char valueI, unsigned char valueD);
void dyna_moveTo(UART_HandleTypeDef* huart, unsigned char ID, unsigned short value);
void dyna_moveSpeed(UART_HandleTypeDef* huart, unsigned char ID, unsigned short value);
void dyna_torqueLimit(UART_HandleTypeDef* huart, unsigned char ID, unsigned short value);

// Leitura
unsigned char  dyna_readFrame(UART_HandleTypeDef* huart, unsigned char paramLen);
unsigned short dyna_readDoubleFrame(UART_HandleTypeDef* huart);
unsigned short dyna_presentPos(UART_HandleTypeDef* huart, unsigned char ID);
unsigned short dyna_presentSpeed(UART_HandleTypeDef* huart, unsigned char ID);
unsigned short dyna_presentLoad(UART_HandleTypeDef* huart, unsigned char ID);
unsigned char  dyna_presentVolt(UART_HandleTypeDef* huart, unsigned char ID);
unsigned char  dyna_readTemp(UART_HandleTypeDef* huart, unsigned char ID);
unsigned char  dyna_registered(UART_HandleTypeDef* huart, unsigned char ID);
unsigned char  dyna_moving(UART_HandleTypeDef* huart, unsigned char ID);

#endif /* DYNAMIXEL_H_ */
