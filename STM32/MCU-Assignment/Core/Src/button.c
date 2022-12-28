/*
 * button.c
 *
 *  Created on: Oct 7, 2022
 *      Author: dell
 */

#include "button.h"

// Set thoi gian
int longkeyPress = 2000;
int change_statelongkey = 250;

// Khoi tao cac bien de xu li chong rung, xu li tin hieu, xu li nut nhan o hai che do (nhan thuong va nhan giu)
int KeyReg0[NUM_OF_BUTTONS] = {NORMAL_STATE};
int KeyReg1[NUM_OF_BUTTONS] = {NORMAL_STATE};
int KeyReg2[NUM_OF_BUTTONS] = {NORMAL_STATE};
int KeyReg3[NUM_OF_BUTTONS] = {NORMAL_STATE};

// Khoi tao cac flag cho tung button: button_flag (nhan thuong) va buttonlong_flag (nhan giu)
int button_flag[NUM_OF_BUTTONS] = {0};
int buttonlong_flag[NUM_OF_BUTTONS] = {0};

// Khoi tao cac bien dem chu ki de xac dinh hai su kien co the xay ra cua nut nhan
int TimeOutForKeyPress[NUM_OF_BUTTONS] = {0};

// Xac dinh flag da duoc bao len gia tri 1
int isButtonPressed(int index) {
	if (button_flag[index] == 1) {
		button_flag[index] = 0;
		return 1;
	}
	return 0;
}

// Set flag = 1
void subKeyProcess(int index){
	button_flag[index] = 1;
}

// getKeyInput()
void getKeyInput() {
	for (int i = 0; i < NUM_OF_BUTTONS; i++) {
		KeyReg0[i] = KeyReg1[i];	// Xu li chong rung
		KeyReg1[i] = KeyReg2[i];

		// Doc cac tin hieu tu cac nut nhan
		if (i == 0) {
			KeyReg2[i] = HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin); // MODE
		}
		else if (i == 1) {
			KeyReg2[i] = HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin); // INC
		}
		else if (i == 2) {
			KeyReg2[i] = HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin); // SET
		}
//		else if (i == 3) {
//			KeyReg2[i] = HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin); // PEDESTRIAN BUTTON
//		}
		// Xu li tin hieu
		if ((KeyReg1[i] == KeyReg0[i]) && (KeyReg1[i] == KeyReg2[i])){
			if (KeyReg3[i] != KeyReg2[i]){
				KeyReg3[i] = KeyReg2[i];
				if (KeyReg2[i] == PRESSED_STATE){
					subKeyProcess(i);	// Kich timer_flag[i] = 1;
					if (buttonlong_flag[i] == 0) {
						TimeOutForKeyPress[i] = longkeyPress / TIME_CYCLE; // Set thoi gian dieu kien cho su kien longkeyPress dien ra
					}
					else {
						TimeOutForKeyPress[i] = change_statelongkey / TIME_CYCLE;  // Set thoi gian dieu kien cho nhung lan thay doi trang thai tiep theo
					}
				}
			} else {
				TimeOutForKeyPress[i]--;
				// Neu sau khoang thoi gian TimeOutForKeyPress lan dau tien ma van con nhan giu button, thi buttonlong_flag[i] se bang 1, nguoc lai se bang 0
				if (TimeOutForKeyPress[i] == 0){
					KeyReg3[i] = NORMAL_STATE;
					buttonlong_flag[i] = 1;
				}
			}
		}
		else {
			buttonlong_flag[i] = 0;
		}
	}
}

