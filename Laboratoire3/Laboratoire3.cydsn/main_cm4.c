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

//vecteur sinus de 150 élément entre -1 et 1
uint32_t vecteurdonnees[]={99513,98160,99413,98419,99319,98729,99145,99050,98949,99419,98798,99851,98679,99846,98657,100433,98597,100453,98713,100550,98880,100921,98942,100758,99148,100637,99243,100496,99426,100316,99677,100090,99871,99833,100155,99657,100544,99416,100533,99350,101127,99266,100986,99359,101151,99370,101493,99586,101282,99597,101251,99713,101027,99896,100926,100109,100625,100345,100369,100625,100180,100986,99983,101152,99930,101751,99809,101581,99950,101877,100112,102210,100251,102006,100391,102068,100506,101956,100783,101872,101092,101692,101296,101532,101733,101311,102054,101081,101967,100723,102263,100171,101478,99530,101051,98881,100564,98080,99478,97452,98752,96709,97796,96168,97009,95842,96177,95534,95551,95512,95055,95623,94635,95492,94373,96057,94212,95856,94325,96206,94439,96594,94606,96428,94889,96724,95093,96645,95436,96622,95818,96520,96042,96387,96551,96312,96959,96200,97075,96116,97747,96027,97625,96166,98102,96258,98464,96444,98339,96751,98615,96864,98429,97113,98280,97281,98005,97386,97811,97731,97436,97948,97145,97891,96849,98402,96607,98112,96544,98337,96419,98565,96500,98246,96486,98393,96524,98100,96726,97962,96902,97688,97033,97499,97468,97311,97709,97142,97975,97062,98518,96929,98493,97060,98982,97142,99388,97406,99269,97602,99597,97792,99457,98096,99419,98376,99307,98614,99245,99053,99051,99352,98867,99665,98764,100215,98688,100144,98743,100627,98752,100923,99032,100799,99094,101111,99244,100868,99403,100797,99621,100589,99763,100377,100100,100140,100269,99873,100563,99674,101021,99524,100969,99551,101462,99564,101645,99692,101446,99770,101803,99901,101652,100125,101504,100279,101331,100457,101110,100778,100915,101007,100671,101289,100594,101818,100354,101721,100377,102270,100321,102378,100537,102362,100684,102799,100805,102585,101090,102610,101267,102405,101459,102259,101910,102105,102156,101966,102442,101795,102953,101616,102905,101644,103542,101586,103483,101618,103314,101416,103288,101016,102513,100613,101743,99933,100761,99438,99818,98908,98895,98425,97997,98168,97200,98008,96533,97569,96158,97837,95820,97544,95761,97496,95758,97826,95863,97578,96039,97544,96209,97493,96416,97421,96883,97340};
int size= sizeof(vecteurdonnees)/sizeof (vecteurdonnees[0]); 
I16 data[150]; 
I16 vecteurespacee [250]; 
/* Image buffer cache */
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

/*******************************************************************************
* Function Name: void UpdateDisplay(void)
********************************************************************************
*
* Summary: This function updates the display with the data in the display 
*			buffer.  The function first transfers the content of the EmWin
*			display buffer to the primary EInk display buffer.  Then it calls
*			the Cy_EINK_ShowFrame function to update the display, and then
*			it copies the EmWin display buffer to the Eink display cache buffer
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  It takes about a second to refresh the display.  This is a blocking function
*  and only returns after the display refresh
*
*******************************************************************************/
uint32_t largest(uint32_t arr[], int n)      //dans quel document H je déclare cette fonction? 
{
    int i;
    // Initialize maximum element
    uint32_t max = arr[0];
    // Traverse array elements from second and
    // compare every element with current max 
    for (i = 1; i < n; i++){
        if (arr[i] > max){
            max = arr[i];}}
    return max;
}

void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
{
    cy_eink_frame_t* pEmwinBuffer;

    /* Get the pointer to Emwin's display buffer */
    pEmwinBuffer = (cy_eink_frame_t*)LCD_GetDisplayBuffer();

    /* Update the EInk display */
    Cy_EINK_ShowFrame(imageBufferCache, pEmwinBuffer, updateMethod, powerCycle);

    /* Copy the EmWin display buffer to the imageBuffer cache*/
    memcpy(imageBufferCache, pEmwinBuffer, CY_EINK_FRAME_SIZE);
}


/*******************************************************************************
* Function Name: void ClearScreen(void)
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}
/*******************************************************************************
* Function Name: void WaitforSwitchPressAndRelease(void)
********************************************************************************
*
* Summary: This implements a simple "Wait for button press and release"
*			function.  It first waits for the button to be pressed and then
*			waits for the button to be released.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function and exits only on a button press and release
*
*******************************************************************************/
void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while(Status_SW2_Read() != 0);
    
    /* Wait for SW2 to be released */
    while(Status_SW2_Read() == 0);
}
void drawGraph(int longueur, int espacement){
    
    GUI_Clear();
    GUI_SetPenSize(1);
    //GUI_DrawRect(7, 5, 257, 155);
    uint32_t maximum=0; 
    maximum = (largest(vecteurdonnees, longueur)); 
    for(int i=0;i<longueur;i++){
        data[i]=(vecteurdonnees[i]*150/(maximum));  //fait une ligne, à régler! 
    }
    for (int a=0;a<longueur; a+=espacement) {
        for (int b=0; b<longueur;  b++) {
            vecteurespacee[b]= data[a]; 
        }
    }
    int sizeespace=0;
    sizeespace= (sizeof vecteurespacee/sizeof vecteurespacee[0]); 
 
    GUI_DrawGraph(vecteurespacee, sizeespace, 1,100);
    
}
/*******************************************************************************
* Function Name: drawGraph
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

/* Function Name: drawGraph
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
/*void updateParameters(param1, param2)
{   GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
   //Write a title and subtile	
   GUI_SetFont(GUI_FONT_8_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("Duty Cycle:", 45, 163);
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt("Moyenne:", 150, 163);

} */
void updateParameters(int param1, int param2){
    GUI_Clear(); 
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    
    char frequence[50];
    char Saturation[50];
    
    sprintf(frequence,"Frequence cardiaque: %d ",param1);
    sprintf(Saturation,"Taux d'oxygene: %d ",param2);
    
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt(frequence, 1, 130); //n'affiche pas, pourquoi?
    GUI_DispStringAt(Saturation, 1, 150);
}
/*******************************************************************************
* Function Name: int main(void)
********************************************************************************/
 
/*
*******************************************************************************/
int main(void)
{
     __enable_irq(); /* Enable global interrupts. */
    
    /* Initialize emWin Graphics */
    GUI_Init();

    /* Start the eInk display interface and turn on the display power */
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    //PRÉSENTATION PAGE D'ACCEUIL
    
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DrawRect(1,1,263,120); //graphique à afficher  
    GUI_DrawRect(120,124,263,175); //Rectangle pour choix de courbe à affichée 
    GUI_DrawLine(192,126,192,173);
    GUI_DispStringAt("COURBE", 130, 145);
    GUI_DispStringAt("ROUGE", 130, 155);
    GUI_DispStringAt("COURBE", 200, 145);
    GUI_DispStringAt("INFRAROUGE", 200,155);
    updateParameters(100,1666); 
    
    
    //Présentation page de MENU
    /*
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt("MENU D'OPTIONS", 76,1);
    GUI_DrawRect(1,16,263,66);
    GUI_DispStringAt("Désac./Activ. Alarme accéléromètre", 5,35);
    GUI_DrawRect(1,71,263,121);
    GUI_DispStringAt("Choix courant DELs", 5,90);
    GUI_DrawRect(1,126,263,175);
    GUI_DispStringAt("Choix des bornes de l'intervalle HR", 5,145);
    */
    
    //Présentation OPTION 2 
    /*
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DispStringAt("Veuillez entrez un courant pour les DELs",1,1);
    GUI_DrawRect(5,20,259,150);
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_DispStringAt("50 mA",78,85);           //Faire un sprintf avec la variable du courant 
    */
    
    
   //Présentation OPTION 3 
   //drawGraph(vecteurdonnees); 
  //I16 a[]= {10000, 20000, 30000,4550,7800,9600,24500}; 
    drawGraph(size, 2); 
   
 

 UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
   // void GUI_DrawGraph (I16 * pay, int NumPoints, int x0, int y0);
   //I16 range is 	-32,768 to 32,767
   UART_1_Start();
    
   
    
    for(;;)
    {
        
    }
}


/* [] END OF FILE */
