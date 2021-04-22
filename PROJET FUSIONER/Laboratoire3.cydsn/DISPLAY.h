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
#include "TraitementSignal.h"
#include "Declaration.h"



// structure qui permet de savoir la page affichée, afin de configurer les fonctionnalités des boutons en conséquences
typedef enum
{
    PRINCIPAL_PAGE = 0x00u,
    MAIN_MENU   = 0x01u,
    OPTION_1   = 0x02u,    
    OPTION_2   = 0x03u,     
    OPTION_3_superieur= 0x04u,         
    OPTION_3_inferieur= 0x05u,  
    OPTION_3_ETAT= 0x06u 
}   location; 

typedef struct
{
    location        screen;     // FLAG qui permet de savoir la page affichée 
    uint8_t         FLAG1;      // FLAG qui permet de savoir la fonctionalité du bouton 0, selon la page affichée
    uint8_t         FLAG2;      // FLAG qui permet de savoir la fonctionalité du bouton 1, selon la page affichée   
    uint8_t         FLAG3;      // FLAG qui permet de retourner au MENU, si on se trouve dans une des options   
    uint8_t         FLAG_DEPART;    //FLAG qui permet de retourner à la page principale, si on se trouve dans le MENU
    uint8_t        TYPEGRAPH;       //FLAG qui permet de savoir quel graphique affichée, tel que s'il vaut 1, on affiche la courbe d'absorption de l'onde infrarouge et s'il vaut 0, on affiche celle de l'onde rouge 
}   screen;

 
volatile screen  currentScreen; // STRUCT qui permet de garder en mémoire la page affichée et les boutons appuyés 
uint8 postion_MENU;             // Variable qui permet de connaître à quel option se trouve le curseur du MENU
uint8 selection_MENU;           // Variable qui permet de savoir quel option du MENU a été sélectionnée 
I16 data[336];                  // vecteur permettant de transformer les données UINT32 en I16 
I16 vecteurespacee [250];       // vecteur qui permet d'espacer les données afin d'obtenir un signal global, vu que la fréquence d'échantillonage est grande 
 
                 
 
//Fonctions qui permettent de déterminer le minimum/maximum d'un vecteur de type uint32_t de taille n
//Les fonctions des librairies étant pour des vecteurs de type float, elle ne pouvait pas toujours être utilisées
uint32_t minimum_1(uint32_t arr[], int n)  ;    
uint32_t maximum_1(uint32_t arr[], int n); 
//vecteurs de type uint32_t des données du signal à afficher par l'interface 
uint32_t INFRAROUGE [BUFFER_LENGTH]; 
uint32_t ROUGE [BUFFER_LENGTH]; 
// Sachant que les signaux filtrés contiennt des valeurs négatif, on a procédé à la translation de ceux-ci tel que 
// leur minimum soit égal à 0. Pour ce faire, on déterminer la valeur absolue du minimum négatif des signaux et on l'additione à l'ensemble du signal.
// Les vecteurs suivant présentent la valeur absolue de tous les points négatifs des signaux filtrés. Leur taille a été posé arbitrairement. 
uint32_t INFRAROUGE_minimum_abs [100]; 
uint32_t ROUGE_minimum_abs [100];
// Déclaration de la tâche d'affichage 
void display_task();               

/* [] END OF FILE */
