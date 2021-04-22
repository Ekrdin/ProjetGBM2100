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


volatile uint8_t courant; //courant des LED du Capteur SpO2 MAX30102 
bool alarme_acc;          // Boléen qui permet de connaître l'état de l'alarme lié au Capteur de mouvement BMI160
bool alarme_FC;           // Boléen qui permet de connaître l'état de l'alarme de la FC
uint8_t borne_inferieur;  // Borne inférieur de l'intervalle de la FC établie par l'utilisateur 
uint8_t borne_superieur;  //Borne supérieur de l'intervalle de la FC établie par l'utilisateur 
char buffer[100];           // vecteur de caractère permettant d'afficher des valeurs sur l'écran E-INK display Shield Board CY8CKIT-028-EPD 
char buffer_0[100];
char buffer_1[100]; 
uint8 imageBufferCache[CY_EINK_FRAME_SIZE]; 
volatile int  etat; 
volatile int bouton; 


// Tâche des Boutons tactiles 
void capSense_task(); 
 




/* [] END OF FILE */
