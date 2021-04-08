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
#include "CapSens.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include "stdio.h"
#include "project.h"

uint32_t maximum_int32(uint32_t arr[], int n);
uint32_t minimum(uint32_t arr[], int n);
void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle);
void ClearScreen(void);
void WaitforSwitchPressAndRelease(void);
void drawGraph(uint32_t donnees[], int espacement);
void updateParameters(int param1, int param2);
void DrawMenu (uint8_t pos_curseur);
void DrawOption2 ();
void DrawOption2 ();
void Task_PRINCIPAL(int HR, int Sat, int nbr_donnees, int espace);
void Task_MENU();



/* [] END OF FILE */
