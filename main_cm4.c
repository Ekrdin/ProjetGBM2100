/******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.2
*
* Description: This file main application code for the CE223727 EmWin Graphics
*				library EInk Display 

********************************************************************************
*************MODIFIED by Christophe Cloutier-Tremblay.**************************
********************************************************************************
* 
*
* Hardware Dependency: CY8CKIT-028-EPD E-Ink Display Shield
*					   CY8CKIT-062-BLE PSoC6 BLE Pioneer Kit
*
******************************************************************************* 
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved. 
******************************************************************************* 
* This software, including source code, documentation and related materials 
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its 
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent 
* protection (United States and foreign), United States copyright laws and 
* international treaty provisions. Therefore, you may use this Software only 
* as provided in the license agreement accompanying the software package from 
* which you obtained this Software (“EULA”). 
* 
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive, 
* non-transferable license to copy, modify, and compile the Software source 
* code solely for use in connection with Cypress’s integrated circuit products. 
* Any reproduction, modification, translation, compilation, or representation 
* of this Software except as specified above is prohibited without the express 
* written permission of Cypress. 
* 
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
********************************************************************************/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "params.h"
#include "queue.h"
#include "Affichage.h"
#include "CapSens.h"



int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    UART_1_Start();
    /* Initialize emWin Graphics */
    GUI_Init();
    /* Start the eInk display interface and turn on the display power */
   /* Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    xTaskCreate(Task_PRINCIPAL,"page_principal",80,NULL,3,NULL);
    xTaskCreate(Task_MENU,"page_MENU",80,NULL,3,NULL);
    xTaskCreate(Task_OPTION2,"Menu_2",80,NULL,3,NULL);
    xTaskCreate(Task_OPTION3,"Menu_3",80,NULL,3,NULL);*/
    GUI_Clear(); 
    GUI_DispStringAt ("MENU", 10, 10);
    GUI_DispStringAt ("Activation/desactivation Alarme", 10, 20);
    GUI_DispStringAt ("bornes inferieur/superieur pour Alarme", 10, 40); 
    GUI_DispStringAt ("Modification du courant des LED", 10, 60); 
  
    vTaskStartScheduler();
    
   
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);       //pourquoi ne marche pas? 
   
    
 for(;;)
    {
   
    }
}

/* [] END OF FILE */
