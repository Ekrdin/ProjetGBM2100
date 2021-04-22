/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#ifndef _DECLARATION_H_
#define _DECLARATION_H_
    
#include "project.h"

int indexBuffer;

//define pour les chaines decouleur
#define RED 0
#define IR 1

//define pour les chaines de frequences
#define RAW 0
#define AC 1
#define DC 2
#define BUFFER_LENGTH        1000
    
    
float dataSpo2[2][3][BUFFER_LENGTH]; //  x =RED/IR, y = RAW/AC/DC, z = temps
volatile float32_t pourcentageSpO2;
volatile uint8_t BPM ; 
volatile bool AlarmeBMI160;
_Bool entree;
#endif