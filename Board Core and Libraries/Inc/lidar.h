/*
 * lidar.h
 *
 *  Created on: Mar 7, 2021
 *      Author: Carlos Silva
 */


#ifndef INC_LIDAR_H_
#define INC_LIDAR_H_

#include "main.h"

void lidar_readFrame(UART_HandleTypeDef* huart, unsigned short* distData, unsigned short* streData, unsigned short* tempData);
void lidar_sysReset(UART_HandleTypeDef* huart);
void lidar_sendTrigger(UART_HandleTypeDef* huart, unsigned short* distData, unsigned short* streData, unsigned short* tempData);
void lidar_outFormat(UART_HandleTypeDef* huart, unsigned char value);
void lidar_enOutput(UART_HandleTypeDef* huart, unsigned char value);
void lidar_setFactory(UART_HandleTypeDef* huart);
void lidar_saveSettings(UART_HandleTypeDef* huart);

#endif /* INC_LIDAR_H_ */
