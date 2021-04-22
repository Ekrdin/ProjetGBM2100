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
    location        screen;
    uint8_t         FLAG1; 
    uint8_t         FLAG2;
    uint8_t         FLAG3;
    uint8_t         FLAG_DEPART;
    uint8_t        TYPEGRAPH; 
}   screen;

volatile screen  currentScreen; 
uint8 postion_MENU; 
uint8 selection_MENU; 
I16 data[336]; 
I16 vecteurespacee [250]; 

void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle); 
/*bool yassine; 
uint8_t andree; */

extern uint32_t INFRAROUGE []; 
extern uint32_t ROUGE []; 
extern int taille_rouge; 
extern int taille_infrarouge; 

void display_task(); 

/* [] END OF FILE */
