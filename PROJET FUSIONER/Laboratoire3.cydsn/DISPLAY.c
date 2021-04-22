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

// Notons que nous n'avons pas déclaré certaines fonctions de ce fichier dans le HEADERFILE DISPLAY.h, car leurs 
// utilités n'est que dans le présent fichier. 
// Le principe de ma tâche d'affichage s'appuie sur les FLAGS. En effet, ayant en mémoire la page actuelle affichée 
// et le bouton appuyée, il est possible de déterminer quel page il faudra afficher. Pour alléger la tâche, j'ai crée des fonctions
// qui permettent d'afficher les différentes page de mon interface. 

// Fonction qui permet de rafraîchir les modifications apportées à l'écran 
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

//Fonctions qui permet de déterminer le maximum d'un vecteur de type uint32_t de taille n
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
//Fonctions qui permet de déterminer le minimum d'un vecteur de type uint32_t de taille n
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
// Fonction qui permet d'afficher la courbe d'absoprtion voulue à partir du signal filtré par l'expert 
// du traitement de signal. Cette fonction prends en paramètre ce vecteur signal, un espacement ainsi que la 
// taille du vecteur signal. Sachant que la fréquence d'échantillonage est grande, les données prélevés sont 
// trop proches et représente seulement un signal local. De ce fait, on introduit un espacement entre les données 
// à afficher afin de visualier un signal élargie. 
void drawGraph(uint32_t donnees[1300], uint8_t espacement, int size ){   //size est déclarer comme int taille= sizeof(vecteurdonnees)/sizeof (vecteurdonnees[0]);
    
    GUI_SetPenSize(1);
    
    
    
    
    // Durant les essai logistique, nous avons réalisé que le début du signal contient des valeurs éronés qui fait en sorte
    // que les paramètre du signal précédement translaté, tel que son maximum et son minimum, sont éronés. De ce fait, 
    // on procède d'abord par éliminer les 300 premières données de ce signal afin de s'assurer de n'avoir que de bonne valeurs. 
    // Le signal étant périodique, la troncation de celui-ci ne comporte pas une perte de données. 
    int z=0; 
    int taille = size-300;
    for (int x=300; x<size; x++){
        donnees[z]= donnees[x]; 
        z++; 
        //printf("%d\r\n", donnees[z]);
    }
   
    // Détermination du maximum et du minimum du signal. Notons que la fonction GUI_DrawGraph ne prends en paramètres
    // qu'un vecteur de type uint16_t. Donc, les données de celui-ci doivent être compris dans l'intervalle de [32 768, 32 767].
    //Toutefois, cela n'est pas le cas pour le vecteur du signal à affiché, sachant que les données qu'ils contient sont dans l'ordre
    // de 10^5. De ce fait, il est possible de divisé ce signal par 5 afin de réduire ces valeurs dans l'intervalle permis. 
    // Ce changement ne comporte aucune perte de données, simplement une modification physique du signal, dont l'amplitude diminue. 
   uint32_t maximum=0; 
   uint32_t minimum=0;
    
   maximum = (maximum_1(donnees, taille))/5;
   minimum= (minimum_1(donnees, size))/5; 
  
   //Modification des valeurs du vecteur signal afin de les adapter aux pixels de l'écran E-INK display Shield Board CY8CKIT-028-EPD
    
    for(int y=0;y<taille;y++){
       data[y]=(((donnees[y]/5)-minimum)*(maximum-minimum))/(2*maximum);
    }
    
    // Espacement des données à afficher du vecteur signal afin d'obtenir un signal global 
    int a=0; 
        for (int b=0; b<taille;  b+=espacement) {  
            vecteurespacee[a]= data[b]; 
            a++; 
            
        }
        
    // Affichage de la page principal
    GUI_DrawGraph(vecteurespacee, 250, 5,20); //250 est la taille du vecteurespacee, donc changer celui-ci
    
    GUI_DrawRect(1,1,263,125);
    GUI_DrawRect(153,129,263,175);
    GUI_DispStringAt ("COURBE AFFICHEE:", 160, 133);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt ("SPECTRE", 155, 143);
       
        if (currentScreen.TYPEGRAPH){GUI_DispStringAt ("INFRAROUGE", 155, 157);}
        if (currentScreen.TYPEGRAPH==0) {GUI_DispStringAt ( "ROUGE", 155, 157);}
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);   
    
}

// Fonction qui permet d'afficher les paramètres de la fréquence cardiaque et le taux d'oxygène sanguin
// sur la page principale 
void updateParameters(int param1, int param2){
    
    GUI_Clear(); 
    char frequence[50];
    char Saturation[50];
    sprintf(frequence,"Frequence cardiaque: %d ",param1);
    sprintf(Saturation,"Taux d'oxygene: %d ",param2);
    
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt(frequence, 3, 140); 
    GUI_DispStringAt("BPM ", 125, 140);     
    GUI_DispStringAt(Saturation, 3, 160);
    GUI_DispStringAt(" %", 98, 160);     
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// Fonction qui permet de traçer la page MENU avec la position du curseur à l'endroit désiré 
void DrawMenu ( uint8_t pos_curseur) { 
    
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
// Fonction qui permet d'afficher l'OPTION 1 de MENU 
void DrawOption1 ()
{       
          GUI_Clear(); 
    GUI_SetFont(GUI_FONT_16_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("ALARME D'ACCELERATION", 132, 20);
    GUI_DrawLine(48,34,215,34);
        GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 00 POUR ACTIVER L'ALARME", 10, 70);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 01 POUR DESACTIVER L'ALARME", 3, 100); 
     GUI_DrawRect(7,120,230,160);
     GUI_SetFont(GUI_FONT_16_1);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
} 

// Fonction qui permet d'afficher l'OPTION 2 du MENU
void DrawOption2 ()
{         GUI_Clear(); 
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("COURANT DELs", 132, 20);
    GUI_DrawLine(20,50,240,50);
        GUI_SetFont(GUI_FONT_8_1);
       GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt ("LE COURANT CONFIGURE DES LEDS", 132, 70);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt ("DU CAPTEUR SPO2 MAX30102 EST:", 132, 80);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DrawLine(114,140,153,140);
   GUI_SetTextAlign(GUI_TA_CENTER);
   sprintf(buffer,"%d\n\r",courant);
     GUI_DispStringAt (buffer, 125, 120);
    GUI_DispStringAt ("mA", 132, 120);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}
// Fonction qui permet d'afficher l'OPTION du MENU qui permet de définir la borne supérieure 
// de l'intervall de la fréquence cardiaque desirée 
void DrawOption3_superieur (){    
   GUI_Clear(); 
    GUI_SetFont(GUI_FONT_8_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("DEFINITION DE L'INTERVALLE DE L'ALARME", 132, 20);
    GUI_DrawLine(25,30,240,30);
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("DEFINIISSEZ LA BORNE SUPERIEUR DE LA FC", 10, 70);
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt ("BORNE SUPERIEUR: ", 10, 110);
    GUI_SetFont(GUI_FONT_32B_1);
    sprintf(buffer,"%d\n\r",borne_superieur);
     GUI_DispStringAt (buffer, 139, 99);
     GUI_DispStringAt ("BPM ", 187, 98);
     UpdateDisplay(CY_EINK_FULL_4STAGE, true); 

}
// Fonction qui permet d'afficher l'OPTION du MENU qui permet de définir la borne supérieure 
// de l'intervall de la fréquence cardiaque desirée 
void DrawOption3_inferieur (){    
  GUI_Clear(); 
    GUI_SetFont(GUI_FONT_8_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("DEFINITION DE L'INTERVALLE DE L'ALARME", 132, 20);
    GUI_DrawLine(25,30,240,30);
    GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("DEFINIISSEZ LA BORNE INFERIEUR DE LA FC", 10, 70);
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt ("BORNE INFERIEUR: ", 10, 110);
    GUI_SetFont(GUI_FONT_32B_1);
    sprintf(buffer,"%d\n\r",borne_inferieur);
     GUI_DispStringAt (buffer, 139, 99);
     GUI_DispStringAt ("BPM ", 187, 98);
     UpdateDisplay(CY_EINK_FULL_4STAGE, true); 
}
// Fonction qui permet d'afficher l'OPTION du MENU qui permet de définir l'état de l'alarme de fréquence cardiaque
void DrawOption3_ETAT ()
{       
          GUI_Clear(); 
    GUI_SetFont(GUI_FONT_16_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("ALARME DE LA FREQUENCE CARDIAQUE", 132, 20);
    GUI_DrawLine(3,34,260,34);
        GUI_SetFont(GUI_FONT_8_1);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 00 POUR ACTIVER L'ALARME", 10, 70);
    GUI_DispStringAt ("APPUYE SUR LE BOUTON 01 POUR DESACTIVER L'ALARME", 3, 100); 
     GUI_DrawRect(7,120,230,160);
     GUI_SetFont(GUI_FONT_16_1);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
} 

// La tâche d'affichage 
void display_task( void ) {
   
    // intialisation des paramètres de l'affichahe sur l'écran E-INK display Shield Board CY8CKIT-028-EPD
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
                             updateParameters(BPM,pourcentageSpO2);     
                             
                           if (currentScreen.TYPEGRAPH)                
                                        {   
                                            drawGraph(INFRAROUGE, 3,BUFFER_LENGTH ) ;                                                        
                                        } 
                            if (currentScreen.TYPEGRAPH==0) 
                                        {
                                           drawGraph(ROUGE, 3,BUFFER_LENGTH ) ;     
                                        } 
                            currentScreen.screen= PRINCIPAL_PAGE; 
                            currentScreen.FLAG_DEPART=0;        
                 }  // à l'appuie du bouton SW2, le type de courbe d'absorption est modifiée 
                if (currentScreen.FLAG1==1 || currentScreen.FLAG3==1) {
                     
                     currentScreen.screen= MAIN_MENU; 
                     postion_MENU=40;
                    DrawMenu(postion_MENU);
                    if (currentScreen.FLAG1==1) {currentScreen.FLAG1=0;}
                    if (currentScreen.FLAG3==1) {currentScreen.FLAG3=0;}
                } // à l'appuie du bouton SW2 ou le bouton 0 dans la page principale, la page du MENU est affichée avec le curseur à la position de la première option
                if (currentScreen.FLAG1==2)
                {
                    if (postion_MENU==160)
                    {postion_MENU=40; }
                    else 
                    {postion_MENU= postion_MENU+30;} 
                    DrawMenu(postion_MENU); 
                    currentScreen.FLAG1=0; 
                } // à l'appuie du bouton 0 dans la page de MENU, le curseur change de position vers le bas 
                if (currentScreen.FLAG1== 3 || selection_MENU==1 || currentScreen.FLAG2== 3 )
                {

                    DrawOption1 (); 
                   
                    //Activation  de l'alarme de l'accéléromètre à l'appuie du bouton 0 sur la page de l'OPTION 1
                    //Permet la lecture des variables transmis par l'expert communication
                                        if  (currentScreen.FLAG1==3) {
                                             alarme_acc= true; 
                                             GUI_SetFont(GUI_FONT_16B_1);
                                             GUI_DispStringAt ("L'alarme est activee ",25, 130); 
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG1=0; }
                  //Désactivation  de l'alarme de l'accéléromètre à l'appuie du bouton 1 sur la page de l'OPTION 1
                  //Bloque la lecture des variables transmis par l'expert communication
                                        if  (currentScreen.FLAG2==3) { 
                                             alarme_acc= false;
                                            Cy_GPIO_Write (Pin_ACC_0_PORT,Pin_ACC_0_NUM,1);
                                            GUI_SetFont(GUI_FONT_16B_1);
                                            GUI_DispStringAt ("L'alarme est desactivee", 25, 130);
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG2=0; }
                  //Affichage de l'OPTION 1  à l'appuie du bouton 1 sur le MENU pour une position du curseur à son niveau 
                                        if (selection_MENU==1) {
                                            currentScreen.screen= OPTION_1; 
                                            selection_MENU=0;}

                } 
                if (currentScreen.FLAG1==4 || currentScreen.FLAG2==4 || selection_MENU==2){       
                    DrawOption2();
                   
                                        if  (currentScreen.FLAG1==4) { currentScreen.FLAG1=0; } // Incrémentation du courant & affichage de la page de l'OPTION 2 
                                        if  (currentScreen.FLAG2==4) { currentScreen.FLAG2=0; } // Dimunition du courant & affichage de la page de l'OPTION 2 
                                        if (selection_MENU==2) {
                                            currentScreen.screen= OPTION_2; 
                                            selection_MENU=0;} //Affichage de l'OPTION 2  à l'appuie du bouton 1 sur le MENU pour une position du curseur à son niveau 
                } 
                if (currentScreen.FLAG1==5 || currentScreen.FLAG2==5 || selection_MENU==3) {          
                    DrawOption3_superieur();
                   
                                        if  (currentScreen.FLAG1==5) { currentScreen.FLAG1=0; } // Incrémentation de la borne supérieur de l'intervalle de FC & affichage de la page de l'OPTION 3 
                                        if  (currentScreen.FLAG2==5) { currentScreen.FLAG2=0; } // Dimunition de la borne supérieur de l'intervalle de FC & affichage de la page de l'OPTION 3 
                                        if (selection_MENU==3) {

                                            currentScreen.screen= OPTION_3_superieur;
                                            selection_MENU=0;} //Affichage de l'OPTION 3  à l'appuie du bouton 1 sur le MENU pour une position du curseur à son niveau 
                }
                if (currentScreen.FLAG1==6 || currentScreen.FLAG2==6 || selection_MENU==4) {          

                    DrawOption3_inferieur();
                    
                                        if  (currentScreen.FLAG1==6) { currentScreen.FLAG1=0; } // Incrémentation de la borne inférieur de l'intervalle de FC & affichage de la page de l'OPTION 4 
                                        if  (currentScreen.FLAG2==6) { currentScreen.FLAG2=0; } // Dimunition de la borne inférieur de l'intervalle de FC & affichage de la page de l'OPTION 4
                                        if (selection_MENU==4) {
                                            currentScreen.screen= OPTION_3_inferieur; 
                                            selection_MENU=0;} //Affichage de l'OPTION 4  à l'appuie du bouton 1 sur le MENU pour une position du curseur à son niveau 
                }
                if (currentScreen.FLAG1==7 || currentScreen.FLAG2==7 || selection_MENU==5) {          

                    DrawOption3_ETAT();
                    //Activation  de l'alarme de la FC à l'appuie du bouton 0 sur la page de l'OPTION 5
                    //Permet la comparaison de l'intervalle de FC désirée et la FC prélevée 
                                        if  (currentScreen.FLAG1==7) {
                                             alarme_FC= true; 
                                             GUI_SetFont(GUI_FONT_16B_1);
                                             GUI_DispStringAt ("L'alarme est activee ",25, 130); 
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG1=0; }
                    //Désactivation  de l'alarme de la FC à l'appuie du bouton 1 sur la page de l'OPTION 5
                    //Bloque la comparaison de l'intervalle de FC désirée et la FC prélevée 
                                        if  (currentScreen.FLAG2==7) { 
                                             alarme_FC= false;
                                            Cy_GPIO_Write (Pin_FC_0_PORT,Pin_FC_0_NUM,1);
                                            GUI_SetFont(GUI_FONT_16B_1);
                                            GUI_DispStringAt ("L'alarme est desactivee", 25, 130);
                                               UpdateDisplay(CY_EINK_FULL_4STAGE, true);
                                            currentScreen.FLAG2=0; }
                      //Affichage de l'OPTION 5  à l'appuie du bouton 1 sur le MENU pour une position du curseur à son niveau 
                                        if (selection_MENU==5) {
                                            currentScreen.screen= OPTION_3_ETAT; 
                                            selection_MENU=0;}
                }
                
                // Si l'alarme de l'accéléromètre est activée, on lit les données transmise par l'expert de communication pour déterminer 
                // si celle-ci s'enclenche ou pas 
                if (alarme_acc) 
                {
                    // signal venant de la communication qui permet d'indiquer s'il y a un mouvement ou non
                        if (AlarmeBMI160) {
                         Cy_GPIO_Write(Pin_ACC_0_PORT,Pin_ACC_0_NUM, 0);
                        }
                        else {Cy_GPIO_Write (Pin_ACC_0_PORT,Pin_ACC_0_NUM,1); }  
                }
                // Si l'alarme de la FC est activée, on compare l'intervalle de FC établie à la FC prélevée afin de savoir
                // si l'alarme s'enclenche ou pas 
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
    
