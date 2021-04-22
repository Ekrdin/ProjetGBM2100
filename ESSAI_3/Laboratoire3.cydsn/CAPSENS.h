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

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"


volatile uint8_t courant; 
bool alarme_acc; 
bool alarme_FC; 
uint8_t borne_inferieur;
uint8_t borne_superieur;
char buffer[100]; 
char buffer_0[100];
char buffer_1[100]; 
uint8 imageBufferCache[CY_EINK_FRAME_SIZE]; 
volatile int  etat; 
volatile int bouton; 
void capSense_task(); 
 




/* [] END OF FILE */
