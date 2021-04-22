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

uint32_t maximum_1(uint32_t arr[], int n){   
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
uint32_t minimum_1(uint32_t arr[], int n)      
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

void drawGraph(uint32_t donnees[1300], uint8_t espacement, int size ){   //size est déclarer comme int taille= sizeof(vecteurdonnees)/sizeof (vecteurdonnees[0]);
    
    GUI_SetPenSize(1);
    
    uint32_t maximum=0; 
    uint32_t minimum=0;
    maximum = (maximum_1(donnees, size))/5;
    minimum= (minimum_1(donnees, size))/5; 
    sprintf(buffer,"%lu\n\r",(unsigned long) minimum);
    UART_1_PutString(buffer);
    for(int y=0;y<size;y++){
        data[y]=(((donnees[y]/5)-minimum)*(maximum-minimum))/(2*maximum);    //fait une ligne, à régler!
        }
    int a=0; 
        for (int b=0; b<size;  b+=espacement) {  
            vecteurespacee[a]= data[b]; 
            a++;        
        }
    GUI_DrawGraph(vecteurespacee, 260, 5,1); //250 est la taille du vecteurespacee, donc changer celui-ci
    
    GUI_DrawRect(1,1,263,125);
    GUI_DrawRect(153,129,263,175);
    GUI_DispStringAt ("COURBE AFFICHEE:", 160, 133);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt ("SPECTRE", 155, 143);
       
        if (currentScreen.TYPEGRAPH){GUI_DispStringAt ("INFRAROUGE", 155, 157);}
        if (currentScreen.TYPEGRAPH==0) {GUI_DispStringAt ( "ROUGE", 155, 157);}
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);   
    
}

void updateParameters(uint8_t paramBPM, float32_t paramSpO2){
    
    GUI_Clear(); 
    char frequence[50];
    char Saturation[50]; 
    
   uint32_t paramSpO2_int = (uint32_t) paramSpO2; // on perd la virgule, mais nécessaire pour pouvoir print car bug en print float
    
    sprintf(frequence,"Frequence cardiaque: %d ",paramBPM);
    sprintf(Saturation,"Taux d'oxygene: %lu ",(unsigned long)paramSpO2_int);
    
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt(frequence, 3, 130); 
    GUI_DispStringAt("BPM ", 125, 130);     
    GUI_DispStringAt(Saturation, 3, 150);
    GUI_DispStringAt("HRC", 100, 150);     
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

void DrawMenu ( uint8_t pos_curseur) { //fonction à vérifier 
    
    GUI_Clear(); 
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("MENU", 132, 2);
        GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("ETAT DE L'ALARME D'ACCELERATION", 15, 40);
    GUI_DispStringAt ("MODIFICATION DU COURANT DES LEDS", 15, 70);
    GUI_DispStringAt ("BORNE SUPERIEUR- RYTHME CARDIAQUE", 15, 100); 
    GUI_DispStringAt ("BORNE INFERIEUR- RYTHME CARDIAQUE", 15, 130); 
    GUI_DispStringAt ("ETAT DE L'ALARME DE LA FC", 15, 160);
    GUI_DrawCircle (4, pos_curseur+2, 3);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
} 

void DrawOption1 ()
{       
          GUI_Clear(); 
    GUI_SetFont(GUI_FONT_16_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("ALARME D'ACCELERATION", 132, 20);
        GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 00 POUR ACTIVER L'ALARME", 10, 70);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 01 POUR ACTIVER L'ALARME", 10, 100); 
     GUI_DrawRect(7,120,230,160);
     GUI_SetFont(GUI_FONT_16_1);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
} 

void DrawOption2 ()
{         GUI_Clear(); 
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("COURANT DELs", 132, 20);
        GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("LE COURANT CONFIGURER DES LEDS EST:", 10, 70);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DrawLine(5,120,75,120);
   GUI_SetTextAlign(GUI_TA_CENTER);
   sprintf(buffer,"%d\n\r",courant);
     GUI_DispStringAt (buffer, 10, 100);
    GUI_DispStringAt ("mA", 45, 100);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}
void DrawOption3_superieur (){    
   GUI_Clear(); 
    GUI_SetFont(GUI_FONT_8_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("DEFINITION DE L'INTERVALLE DE L'ALARME", 132, 20);
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt ("DEFINIISSEZ LA BORNE SUPERIEUR DE LA FC", 10, 70);
    GUI_DispStringAt ("BORNE SUPERIEUR:", 10, 110);
    GUI_SetFont(GUI_FONT_32B_1);
    sprintf(buffer,"%d\n\r",borne_superieur);
     GUI_DispStringAt (buffer, 120, 95);
     GUI_DispStringAt ("HRC ", 175, 95);
     UpdateDisplay(CY_EINK_FULL_4STAGE, true); 

}

void DrawOption3_inferieur (){    
  GUI_Clear(); 
    GUI_SetFont(GUI_FONT_16_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("DEFINITION DE L'INTERVALLE DE L'ALARME", 132, 20);
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("DEFINIISSEZ LA BORNE INFERIEUR DE LA FC", 10, 70);
    GUI_DispStringAt ("BORNE INFERIEUR", 10, 110);
    GUI_SetFont(GUI_FONT_32B_1);
    sprintf(buffer,"%d\n\r",borne_inferieur);
     GUI_DispStringAt (buffer, 120, 95);
     GUI_DispStringAt ("HRC ", 175, 95);
     UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}
void DrawOption3_ETAT ()
{       
          GUI_Clear(); 
    GUI_SetFont(GUI_FONT_16_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("ALARME DE LA FREQUENCE CARDIAQUE", 132, 20);
        GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 00 POUR ACTIVER L'ALARME", 10, 70);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 01 POUR ACTIVER L'ALARME", 10, 100); 
     GUI_DrawRect(7,120,230,160);
     GUI_SetFont(GUI_FONT_16_1);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
} 

void display_task( void ) {
   
    GUI_Init();
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    GUI_Clear(); 
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetPenSize(1);
    GUI_SetFont(GUI_FONT_8_1);
        
    for (;;) {              
                if (currentScreen.FLAG_DEPART) {

                             updateParameters(BPM,pourcentageSpO2);      //faire update parameters apres graph sans gui_clear(); 
                            
                            if (currentScreen.TYPEGRAPH)                
                                        {   
                                            drawGraph(INFRAROUGE, 3,taille_infrarouge ) ;           //trouver fonction qui déterminer espacement (pertinent?) 
                                             sprintf(buffer_1,"%d\n\r",taille_infrarouge);
                                                UART_1_PutString(buffer_1);
                                               
                                        } 
                            if (currentScreen.TYPEGRAPH==0) 
                                        {
                                           
                                            drawGraph(ROUGE, 3,taille_rouge);
                                            sprintf(buffer_1,"%d\n\r",taille_rouge);
                                               UART_1_PutString(buffer_1);
                                           
                                        } 
                            currentScreen.screen= PRINCIPAL_PAGE; 
                            currentScreen.FLAG_DEPART=0;        
                 }
                if (currentScreen.FLAG1==1 || currentScreen.FLAG3==1) {
                     
                     currentScreen.screen= MAIN_MENU; 
                     postion_MENU=40;
                    DrawMenu(postion_MENU);
                    if (currentScreen.FLAG1==1) {currentScreen.FLAG1=0;}
                    if (currentScreen.FLAG3==1) {currentScreen.FLAG3=0;}
                }
                if (currentScreen.FLAG1==2)
                {
                    if (postion_MENU==160)
                    {postion_MENU=40; }
                    else 
                    {postion_MENU= postion_MENU+30;} 
                    DrawMenu(postion_MENU); 
                    currentScreen.FLAG1=0; 
                } 
                if (currentScreen.FLAG1== 3 || selection_MENU==1 || currentScreen.FLAG2== 3 )
                {
                    
                    DrawOption1 (); 
                   
                                
                                        if  (currentScreen.FLAG1==3) {
                                             alarme_acc= true; 
                                             GUI_SetFont(GUI_FONT_16B_1);
                                             GUI_DispStringAt ("L'alarme est activee ",25, 130); 
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG1=0; }
                                        if  (currentScreen.FLAG2==3) { 
                                             alarme_acc= false;
                                            Cy_GPIO_Write (Pin_ACC_0_PORT,Pin_ACC_0_NUM,1);
                                            GUI_SetFont(GUI_FONT_16B_1);
                                            GUI_DispStringAt ("L'alarme est desactivee", 25, 130);
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG2=0; }
                                        if (selection_MENU==1) {

                                            currentScreen.screen= OPTION_1; 
                                            selection_MENU=0;}

                }
                if (currentScreen.FLAG1==4 || currentScreen.FLAG2==4 || selection_MENU==2){     //PERMET D'UPDATER LA PAGE DE COURANT     
        // sprintf(buffer_courant,"%d\n\r",valeur);
                    DrawOption2();
                                        if  (currentScreen.FLAG1==4) { currentScreen.FLAG1=0; }
                                        if  (currentScreen.FLAG2==4) { currentScreen.FLAG2=0; }
                                        if (selection_MENU==2) {
                                            currentScreen.screen= OPTION_2; 
                                            selection_MENU=0;}
                }
                if (currentScreen.FLAG1==5 || currentScreen.FLAG2==5 || selection_MENU==3) {          //BORNE SUPÉRIEUR
                    DrawOption3_superieur();
                   
                                        if  (currentScreen.FLAG1==5) { currentScreen.FLAG1=0; }
                                        if  (currentScreen.FLAG2==5) { currentScreen.FLAG2=0; }
                                        if (selection_MENU==3) {

                                            currentScreen.screen= OPTION_3_superieur;
                                            selection_MENU=0;}
                }
                if (currentScreen.FLAG1==6 || currentScreen.FLAG2==6 || selection_MENU==4) {          //BORNE INFÉRIEUR

                    DrawOption3_inferieur();
                    
                                        if  (currentScreen.FLAG1==6) { currentScreen.FLAG1=0; }
                                        if  (currentScreen.FLAG2==6) { currentScreen.FLAG2=0; }
                                        if (selection_MENU==4) {
                                            currentScreen.screen= OPTION_3_inferieur; 
                                            selection_MENU=0;}
                }
                if (currentScreen.FLAG1==7 || currentScreen.FLAG2==7 || selection_MENU==5) {          //BORNE INFÉRIEUR

                    DrawOption3_ETAT();
                    
                                        if  (currentScreen.FLAG1==7) {
                                             alarme_FC= true; 
                                             GUI_SetFont(GUI_FONT_16B_1);
                                             GUI_DispStringAt ("L'alarme est activee ",25, 130); 
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG1=0; }
                                        if  (currentScreen.FLAG2==7) { 
                                             alarme_FC= false;
                                            Cy_GPIO_Write (Pin_FC_0_PORT,Pin_FC_0_NUM,1);
                                            GUI_SetFont(GUI_FONT_16B_1);
                                            GUI_DispStringAt ("L'alarme est desactivee", 25, 130);
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG2=0; }
                                        if (selection_MENU==5) {
                                            currentScreen.screen= OPTION_3_ETAT; 
                                            selection_MENU=0;}
                }
                if (alarme_acc) 
                {
                        if (AlarmeBMI160) {
                         Cy_GPIO_Write(Pin_ACC_0_PORT,Pin_ACC_0_NUM, 0);
                        }
                        else {Cy_GPIO_Write (Pin_ACC_0_PORT,Pin_ACC_0_NUM,1); }  
                }
               if (alarme_FC) 
                {
                        if (BPM<borne_inferieur || BPM>borne_superieur) {                         
                         Cy_GPIO_Write(Pin_FC_0_PORT,Pin_FC_0_NUM, 0); 
                        }
                        else {Cy_GPIO_Write (Pin_FC_0_PORT,Pin_FC_0_NUM,1); }     
                }
                 vTaskDelay(pdMS_TO_TICKS(350));
        }
        
    } 
    
