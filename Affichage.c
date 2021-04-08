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

#include "Affichage.h"

//vecteur sinus de 150 élément entre -1 et 1
uint32_t vecteurdonnees[]={99513,98160,99413,98419,99319,98729,99145,99050,98949,99419,98798,99851,98679,99846,98657,100433,98597,100453,98713,100550,98880,100921,98942,100758,99148,100637,99243,100496,99426,100316,99677,100090,99871,99833,100155,99657,100544,99416,100533,99350,101127,99266,100986,99359,101151,99370,101493,99586,101282,99597,101251,99713,101027,99896,100926,100109,100625,100345,100369,100625,100180,100986,99983,101152,99930,101751,99809,101581,99950,101877,100112,102210,100251,102006,100391,102068,100506,101956,100783,101872,101092,101692,101296,101532,101733,101311,102054,101081,101967,100723,102263,100171,101478,99530,101051,98881,100564,98080,99478,97452,98752,96709,97796,96168,97009,95842,96177,95534,95551,95512,95055,95623,94635,95492,94373,96057,94212,95856,94325,96206,94439,96594,94606,96428,94889,96724,95093,96645,95436,96622,95818,96520,96042,96387,96551,96312,96959,96200,97075,96116,97747,96027,97625,96166,98102,96258,98464,96444,98339,96751,98615,96864,98429,97113,98280,97281,98005,97386,97811,97731,97436,97948,97145,97891,96849,98402,96607,98112,96544,98337,96419,98565,96500,98246,96486,98393,96524,98100,96726,97962,96902,97688,97033,97499,97468,97311,97709,97142,97975,97062,98518,96929,98493,97060,98982,97142,99388,97406,99269,97602,99597,97792,99457,98096,99419,98376,99307,98614,99245,99053,99051,99352,98867,99665,98764,100215,98688,100144,98743,100627,98752,100923,99032,100799,99094,101111,99244,100868,99403,100797,99621,100589,99763,100377,100100,100140,100269,99873,100563,99674,101021,99524,100969,99551,101462,99564,101645,99692,101446,99770,101803,99901,101652,100125,101504,100279,101331,100457,101110,100778,100915,101007,100671,101289,100594,101818,100354,101721,100377,102270,100321,102378,100537,102362,100684,102799,100805,102585,101090,102610,101267,102405,101459,102259,101910,102105,102156,101966,102442,101795,102953,101616,102905,101644,103542,101586,103483,101618,103314,101416,103288,101016,102513,100613,101743,99933,100761,99438,99818,98908,98895,98425,97997,98168,97200,98008,96533,97569,96158,97837,95820,97544,95761,97496,95758,97826,95863,97578,96039,97544,96209,97493,96416,97421,96883,97340};
int size= sizeof(vecteurdonnees)/sizeof (vecteurdonnees[0]); //confirmé 
I16 data[336]; 
I16 vecteurespacee [250]; 
/* Image buffer cache */
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

uint32_t maximum_int32(uint32_t arr[], int n){   
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
uint32_t minimum(uint32_t arr[], int n)      
{
    int i;
    // Initialize maximum element
    uint32_t min = arr[0];
    // Traverse array elements from second and
    // compare every element with current max 
    for (i = 1; i < n; i++){
        if (arr[i] < min){
            min = arr[i];}}
    return min;
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
*********************************************************************************/
void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}
/*******************************************************************************
* Function Name: void WaitforSwitchPressAndRelease(void)
*****************************************************************************/
void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while(Status_SW2_Read() != 0);
    
    /* Wait for SW2 to be released */
    while(Status_SW2_Read() == 0);
}
/*******************************************************************************
* Function Name: void drawGraph(int longueur, int espacement)
*****************************************************************************/
void drawGraph(uint32_t donnees[], int espacement){   
    GUI_Clear();
    GUI_SetPenSize(1);
    char string[400] = "";
    //GUI_DrawRect(7, 5, 257, 155);
    uint32_t maximum=0; 
    uint32_t minimum_1=0;
    int longueur; 
    longueur= sizeof(&donnees)/sizeof (&donnees[0]); // à vérifier 
    maximum = (maximum_int32(vecteurdonnees, longueur))/5;
    minimum_1= (minimum(vecteurdonnees, longueur))/5; 
    for(int i=0;i<longueur;i++){
        data[i]=donnees[i]/5;   //fait une ligne, à régler!     
        }
  
    for(int y=0;y<longueur;y++){
        data[y]=((data[y]-minimum_1))*(maximum-minimum_1)/(2*maximum);   //fait une ligne, à régler!     
         
        }
    
    int a=0; 
        for (int b=0; b<longueur;  b+=espacement) {  
            vecteurespacee[a]= data[b]; 
            a++; 
        }
 
    int sizeespace=0;
    sizeespace= (sizeof vecteurespacee/sizeof vecteurespacee[0]); //toujours égal à 250 
    GUI_DrawGraph(vecteurespacee, sizeespace, 1,100);   
}
void DrawMenu ( uint8_t pos_curseur) { //fonction à vérifier 
    GUI_Clear(); 
    GUI_DispStringAt ("MENU", 10, 10);
    GUI_DispStringAt ("Activation/desactivation de l’alarme issue de mouvement de l’acceleration", 10, 20);
    GUI_DispStringAt ("Configuration des bornes inférieur/supérieur pour l’alarme", 10, 40); 
    GUI_DispStringAt ("Modification du courant des LED", 10, 60); 
    GUI_DrawCircle (5, 20*pos_curseur, 4); 
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
  
}

void DrawOption2 ()
{   
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

}

void DrawOption3 (){    

//Présentation OPTION 3 
   //drawGraph(vecteurdonnees); 
  //I16 a[]= {10000, 20000, 30000,4550,7800,9600,24500}; 



}
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
* Task Principe:void affichage() 
********************************************************************************/
 
/*
*******************************************************************************/
void affichage () 
{
     __enable_irq(); /* Enable global interrupts. */
    UART_1_Start();
    /* Initialize emWin Graphics */
    GUI_Init();

    /* Start the eInk display interface and turn on the display power */
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    //PRÉSENTATION PAGE D'ACCEUIL
    
    /*GUI_SetPenSize(1);
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
    updateParameters(100,1666); */
    
    
    
    
   
    
    
   
   drawGraph(vecteurdonnees, 1); 
   
 UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
   // void GUI_DrawGraph (I16 * pay, int NumPoints, int x0, int y0);
   //I16 range is 	-32,768 to 32,76
    
    for(;;)
    {
        
    }
}


/* [] END OF FILE */


