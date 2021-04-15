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
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"

#include "Accelerometre.h"
#include "max30102.h"


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_1_Start();
     xTaskCreate( GetInterruptStatus, "GetInterruptStatus",400,0,1,0);
    //xTaskCreate( motionTask, "motionTask",400,0,1,0);
   //xTaskCreate(TaskReadingSample, "TaskReadingSample",400,0,1,0);
    vTaskStartScheduler();
    for(;;)
    {
        /* Place your application code here. */
        
    }
}

/* [] END OF FILE */
