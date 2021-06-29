/*
 * dynamixel.c
 *
 *  Created on: Mar 7, 2021
 *      Author: Carlos Silva
 */


#include "dynamixel.h"

/*
 * Funções de Escrita
 */

// Configuração do frame e envio
void dyna_sendFrame(UART_HandleTypeDef* huart, unsigned char ID, unsigned char instruct, unsigned char params[], unsigned char paramLen){
	unsigned char  bufferDynamixel[15];  // Definir melhor valor depois
	unsigned short checksum=0;
	unsigned char checksum_low;
	int i;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(Dir485_GPIO_Port, Dir485_Pin, GPIO_PIN_SET);

	// Header
	bufferDynamixel[0] = 0xFF; // Default
	bufferDynamixel[1] = 0xFF; // Default

	// Packet ID
	bufferDynamixel[2] = ID; // ID=1

	// Length (number of parameters + 2)
	bufferDynamixel[3] = paramLen + 2;

	// Instrução (Ping, Read, Write, etc)
	bufferDynamixel[4] = instruct;

	// Parâmetros
	for(i=0; i<paramLen; i++) bufferDynamixel[i+5] = params[i];

	// Checksum
	for(i=2; i<=(4+paramLen); i++) checksum += bufferDynamixel[i];
	checksum = ~checksum;
	checksum_low  = checksum & 0xFF;	// Retirar apenas o low byte
	bufferDynamixel[5+paramLen] = checksum_low;

	// Enviar frame
	HAL_UART_Transmit(huart, bufferDynamixel, (6+paramLen), HAL_MAX_DELAY);

	// Receber Status packet
	//HAL_GPIO_WritePin(Dir485_GPIO_Port, Dir485_Pin, GPIO_PIN_RESET);
	//HAL_UART_Receive(huart, bufferDynamixel, (6+paramLen), HAL_MAX_DELAY);
}

// Configuração do frame e envio
unsigned char dyna_readSingleFrame(UART_HandleTypeDef* huart){
	unsigned char  bufferDynamixel[15];  // Definir melhor valor depois

	// Receber Status packet
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(Dir485_GPIO_Port, Dir485_Pin, GPIO_PIN_RESET);
	HAL_UART_Receive(huart, bufferDynamixel, (5+2), HAL_MAX_DELAY);

	return bufferDynamixel[5];
}

// Configuração do frame e envio
unsigned short dyna_readDoubleFrame(UART_HandleTypeDef* huart){
	unsigned char  bufferDynamixel[15];  // Definir melhor valor depois
	unsigned short value;

	// Receber Status packet
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(Dir485_GPIO_Port, Dir485_Pin, GPIO_PIN_RESET);
	HAL_UART_Receive(huart, bufferDynamixel, (5+3), HAL_MAX_DELAY);

	value = (((short)bufferDynamixel[6]) << 8) | (0x00FF & bufferDynamixel[5]);

	return value;
}

void dyna_torqueEnable(UART_HandleTypeDef* huart, unsigned char ID, unsigned char value){
	unsigned char params[2];

	params[0] = DYNA_TORQUEEN_REG;
	params[1] = value;

	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

void dyna_led(UART_HandleTypeDef* huart, unsigned char ID, unsigned char value){
	unsigned char params[2];

	params[0] = DYNA_LED_REG;
	params[1] = value;

	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

void dyna_setPID(UART_HandleTypeDef* huart, unsigned char ID, unsigned char valueP, unsigned char valueI, unsigned char valueD){
	unsigned char params[2];

	params[0] = DYNA_PIDP_REG;
	params[1] = valueP;
	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));

	params[0] = DYNA_PIDI_REG;
	params[1] = valueI;
	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));

	params[0] = DYNA_PIDD_REG;
	params[1] = valueD;
	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

void dyna_moveTo(UART_HandleTypeDef* huart, unsigned char ID, unsigned short value){
	unsigned char params[3];

	params[0] = DYNA_GOAL_REG;
	params[1] = value & 0xFF;		// Low Byte
	params[2] = (value>>8) & 0xFF; 	// High Byte

	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

void dyna_moveSpeed(UART_HandleTypeDef* huart, unsigned char ID, unsigned short value){
	unsigned char params[3];

	params[0] = DYNA_MOVESPEED_REG;
	params[1] = value & 0xFF;		// Low Byte
	params[2] = (value>>8) & 0xFF; 	// High Byte

	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

void dyna_torqueLimit(UART_HandleTypeDef* huart, unsigned char ID, unsigned short value){
	unsigned char params[3];

	params[0] = DYNA_TORQUELIMIT_REG;
	params[1] = value & 0xFF;		// Low Byte
	params[2] = (value>>8) & 0xFF; 	// High Byte

	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

void dyna_goalAcc(UART_HandleTypeDef* huart, unsigned char ID, unsigned char value){
	unsigned char params[2];

	params[0] = DYNA_GOALACC_REG;
	params[1] = value;

	dyna_sendFrame(huart, ID, DYNA_WRITE, params, sizeof(params));
}

/*
 * Funções de Leitura
 */

unsigned short dyna_presentPos(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned short value;

	params[0] = DYNA_PRESENTPOS_REG;
	params[1] = 2; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readDoubleFrame(huart);
	return value;
}

unsigned short dyna_presentSpeed(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned short value;

	params[0] = DYNA_PRESENTSPEED_REG;
	params[1] = 2; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readDoubleFrame(huart);
	return value;
}

unsigned short dyna_presentLoad(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned short value;

	params[0] = DYNA_PRESENTLOAD_REG;
	params[1] = 2; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readDoubleFrame(huart);
	return value;
}

unsigned char dyna_presentVolt(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned char value;

	params[0] = DYNA_PRESENTVOLT_REG;
	params[1] = 1; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readSingleFrame(huart);
	return value;
}

unsigned char dyna_readTemp(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned char value;

	params[0] = DYNA_TEMP_REG;
	params[1] = 1; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readSingleFrame(huart);
	return value;
}

unsigned char dyna_registered(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned char value;

	params[0] = DYNA_REGISTERED_REG;
	params[1] = 1; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readSingleFrame(huart);
	return value;
}

unsigned char dyna_moving(UART_HandleTypeDef* huart, unsigned char ID)
{
	unsigned char params[2];
	unsigned char value;

	params[0] = DYNA_MOVING_REG;
	params[1] = 1; // length of data

	dyna_sendFrame(huart, ID, DYNA_READ, params, sizeof(params));
	value = dyna_readSingleFrame(huart);
	return value;
}
