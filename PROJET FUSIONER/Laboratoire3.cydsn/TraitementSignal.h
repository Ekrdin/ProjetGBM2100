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
#include <arm_math.h>
#include <core_cm4.h> 
#include <stdio.h>
#include "Declaration.h"
#include "FreeRTOS.h"
#include "task.h"

#define TEST_LENGTH_SAMPLES  1000 // longueur des échantillons qu'on a pris
#define BLOCK_SIZE           1000 //  même size que la longueur des échantillons ( nbre d'échantillons à traiter)
#define NUM_TAPS             11 // nombre de coefficents pris pour les filtres
#define FREQUENCE_ECH        200 // frequence d'echantillonage de 200 samples per second

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; // size du buffer 
float32_t firCoeffs32_LowPass[NUM_TAPS]; 
float32_t firCoeffs32_HighPass[NUM_TAPS];
uint32_t blockSize;


// Fonction de filtre passe-bas avec parametress de float buffer pour le input data et filteredsginal pour le ouput data
void FiltreLowPass(float* buffer ,float* filteredSignal);
// Fonction de filtre passe-haut avec parametres de float buffer pour le input data et filteredsginal pour le ouput data
void FiltreHighPass(float* buffer ,float* filteredSignal);
// Fonction qui trouve les zeros pour le BPM avec comme paramètres le zeroinitial et zerofinal et le nbre de zero
void trouverZero(int* ZeroI, int* ZeroF, int* nZero);
//Calcule le BPM
uint8_t calculBPM(void);
// Calcule le SpO2
float calculSPO2(void);
// Fonction qui fait passer le signal des valeurs brutes et filtre toutes les valeurs 
void filtrage(uint16_t index);
// Tache qui englobe le tout ( filtrage, calculBPM,calculSpO2)

void determiner_negatif (void) ; 
void LectureDonne( void); 
int minimum_infrarouge; 
int minimum_rouge;  
int taille_infrarouge; 
int taille_rouge; 

void TraitementSignal( void*arg);



/* [] END OF FILE */



