/*
 * lidar.h
 *
 *  Created on: Mar 7, 2021
 *      Author: Carlos Silva
 */

#include "lidar.h"

void lidar_readFrame(UART_HandleTypeDef* huart, unsigned short* distData, unsigned short* streData, unsigned short* tempData){
	unsigned char bufferLidar[9];  // Buffer de recepção
	unsigned char curChar;
	unsigned char lastChar;

	//memset(bufferLidar,0,sizeof(bufferLidar));

	curChar  = 0;
	lastChar = 0;

	  // Step 1: Read the serial stream until we see the beginning of the TF Mini header, or we timeout reading too many characters.
	  while (1) {
		  HAL_UART_Receive(huart, &curChar, 1, HAL_MAX_DELAY);
	      if ((lastChar == 0x59) && (curChar == 0x59)) {
	        // Break to begin frame
	    	curChar  = 0;
	    	lastChar = 0;
	        break;

	      } else {
	        // We have not seen two 0x59's in a row -- store the current character and continue reading.
	        lastChar = curChar;
	        curChar = 0;
	      }
	    }

	  HAL_UART_Receive(huart, &curChar, 1, HAL_MAX_DELAY);
	  if(curChar == 0x59) HAL_UART_Receive(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
	  else{
		  bufferLidar[0] = curChar;
		  HAL_UART_Receive(huart, bufferLidar+1, sizeof(bufferLidar)-1, HAL_MAX_DELAY);
	  }

	HAL_UART_Receive(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
	*distData = (bufferLidar[1] << 8) + bufferLidar[0];
	*streData = (bufferLidar[3] << 8) + bufferLidar[2];
	*tempData = (bufferLidar[5] << 8) + bufferLidar[4];
	*tempData = (unsigned short)*tempData/8-256;
}

void lidar_sysReset(UART_HandleTypeDef* huart){
	unsigned char bufferLidar[4];

	bufferLidar[0] = 0x5A;
	bufferLidar[1] = 0x04;
	bufferLidar[2] = 0x02;
	bufferLidar[3] = 0x60;
	HAL_UART_Transmit(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
}

void lidar_sendTrigger(UART_HandleTypeDef* huart, unsigned short* distData, unsigned short* streData, unsigned short* tempData){
	unsigned char bufferLidarSend[4];
	unsigned char bufferLidar[9];
	unsigned char curChar;
	unsigned char lastChar;

	bufferLidarSend[0] = 0x5A;
	bufferLidarSend[1] = 0x04;
	bufferLidarSend[2] = 0x04;
	bufferLidarSend[3] = 0x62;

	HAL_UART_Transmit(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
	HAL_Delay(5);

	//memset(bufferLidar,0,sizeof(bufferLidar));

	curChar  = 0;
	lastChar = 0;

	  // Step 1: Read the serial stream until we see the beginning of the TF Mini header, or we timeout reading too many characters.
	  while (1) {
		  HAL_UART_Receive(huart, &curChar, 1, HAL_MAX_DELAY);
		  if ((lastChar == 0x59) && (curChar == 0x59)) {
			// Break to begin frame
			curChar  = 0;
			lastChar = 0;
			break;

		  } else {
			// We have not seen two 0x59's in a row -- store the current character and continue reading.
			lastChar = curChar;
			curChar = 0;
		  }
		}

	  HAL_UART_Receive(huart, &curChar, 1, HAL_MAX_DELAY);
	  if(curChar == 0x59) HAL_UART_Receive(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
	  else{
		  bufferLidar[0] = curChar;
		  HAL_UART_Receive(huart, bufferLidar+1, sizeof(bufferLidar)-1, HAL_MAX_DELAY);
	  }

	HAL_UART_Receive(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
	*distData = (bufferLidar[1] << 8) + bufferLidar[0];
	*streData = (bufferLidar[3] << 8) + bufferLidar[2];
	*tempData = (bufferLidar[5] << 8) + bufferLidar[4];
	*tempData = (unsigned short)*tempData/8-256;
}

void lidar_outFormat(UART_HandleTypeDef* huart, unsigned char value){
	// 0 cm
	// 1 mm
	unsigned char bufferLidar[5];

	if(value){ // mm
		bufferLidar[0] = 0x5A;
		bufferLidar[1] = 0x05;
		bufferLidar[2] = 0x05;
		bufferLidar[3] = 0x06;
		bufferLidar[4] = 0x6A;
	}
	else{ // cm
		bufferLidar[0] = 0x5A;
		bufferLidar[1] = 0x05;
		bufferLidar[2] = 0x05;
		bufferLidar[3] = 0x01;
		bufferLidar[4] = 0x65;
	}
	HAL_UART_Transmit(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
}

void lidar_enOutput(UART_HandleTypeDef* huart, unsigned char value){
	// 1 enable
	// 0 disable
	unsigned char bufferLidar[5];

	if(value){ // enable
		bufferLidar[0] = 0x5A;
		bufferLidar[1] = 0x05;
		bufferLidar[2] = 0x07;
		bufferLidar[3] = 0x01;
		bufferLidar[4] = 0x67;
		}
	else{ // disable
		bufferLidar[0] = 0x5A;
		bufferLidar[1] = 0x05;
		bufferLidar[2] = 0x07;
		bufferLidar[3] = 0x00;
		bufferLidar[4] = 0x67;
	}
}

void lidar_setFactory(UART_HandleTypeDef* huart){
	unsigned char bufferLidar[4];

	bufferLidar[0] = 0x5A;
	bufferLidar[1] = 0x04;
	bufferLidar[2] = 0x10;
	bufferLidar[3] = 0x6E;
	HAL_UART_Transmit(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
}

void lidar_saveSettings(UART_HandleTypeDef* huart){
	unsigned char bufferLidar[4];

	bufferLidar[0] = 0x5A;
	bufferLidar[1] = 0x04;
	bufferLidar[2] = 0x11;
	bufferLidar[3] = 0x6F;
	HAL_UART_Transmit(huart, bufferLidar, sizeof(bufferLidar), HAL_MAX_DELAY);
}
