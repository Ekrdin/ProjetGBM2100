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
#include "CAPSENS.h"
#include "DISPLAY.h"
#include "Declaration.h"


void capSense_task()
{
    
    for(;;)
    {
        CapSense_ProcessAllWidgets();
        
       if(CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
        
        etat++; 
        sprintf(buffer_0,"%d\n\r",etat);
        UART_1_PutString(buffer_0);

            if ( currentScreen.screen ==  PRINCIPAL_PAGE)  {currentScreen.FLAG1=1;}
            if ( currentScreen.screen == MAIN_MENU)       //VA VERS LE BAS 
                        {currentScreen.FLAG1=2; }
            if ( currentScreen.screen == OPTION_1)  
            {currentScreen.FLAG1=3;}
            if ( currentScreen.screen == OPTION_2) 
            {courant++; 
             currentScreen.FLAG1=4;}        
            if ( currentScreen.screen == OPTION_3_superieur) 
               { borne_superieur++; 
                currentScreen.FLAG1=5;}        
            if ( currentScreen.screen == OPTION_3_inferieur) 
            {borne_inferieur++; 
                currentScreen.FLAG1=6;}           
            if ( currentScreen.screen == OPTION_3_ETAT) //PERMET D'ACTIVER L'ALARME
            {currentScreen.FLAG1=7;}          
          
    }

       if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID))
        {
            
         etat--; 
        sprintf(buffer_1,"%d\n\r",etat);
        UART_1_PutString(buffer_1);
        if (currentScreen.screen == MAIN_MENU) {            

                
                if (postion_MENU==40) 
                            { selection_MENU=1;}

                if (postion_MENU==70) 
                            { selection_MENU=2;}
                if (postion_MENU==100)
                             { selection_MENU=3;} 
                if (postion_MENU==130)
                             {  selection_MENU=4;} 
                if (postion_MENU==160)
                             {  selection_MENU=5;} 
            }
         if ( currentScreen.screen == OPTION_1) 
                    {currentScreen.FLAG2=3;} 
            
         if ( currentScreen.screen == OPTION_2)
                    {courant--; 
                     currentScreen.FLAG2= 4; }
         if ( currentScreen.screen == OPTION_3_superieur) {
                borne_superieur--; 
                currentScreen.FLAG2=5;}          
         if ( currentScreen.screen == OPTION_3_inferieur) {
                borne_inferieur--; 
                currentScreen.FLAG2=6;}
         if ( currentScreen.screen == OPTION_3_ETAT) //PERMET D'ACTIVER L'ALARME  
                    {currentScreen.FLAG2=7;}         
            
        }
        CapSense_UpdateAllBaselines();
        CapSense_ScanAllWidgets();
        vTaskDelay(pdMS_TO_TICKS(350));  
    }
        
        
}
    



/* [] END OF FILE */
