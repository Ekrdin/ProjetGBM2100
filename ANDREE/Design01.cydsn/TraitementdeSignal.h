#include "project.h"
#include <arm_math.h>
#include <core_cm4.h> 
#include <stdio.h>
#include "Declaration.h"
#include "FreeRTOS.h"
#include "task.h"

#define TEST_LENGTH_SAMPLES  1000
#define BLOCK_SIZE           1000 //500 parce que le plus bas BPM 50 -> 2 battements a 2.5 secondes. Avec un SR de 200SPS, 500 SAMPLES pour 2 battements
#define NUM_TAPS             11
#define FREQUENCE_ECH        200

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; // size du buffer 
float32_t firCoeffs32_LowPass[NUM_TAPS];
float32_t firCoeffs32_HighPass[NUM_TAPS];
uint32_t blockSize;


void FiltreLowPass(float* buffer ,float* filteredSignal);

void FiltreHighPass(float* buffer ,float* filteredSignal);

void trouverZero(int* ZeroI, int* ZeroF, int* nZero);

uint8_t calculBPM(void);

float calculSPO2(void);

void filtrage(uint16_t index);

void TraitementSignal( void*arg);


/* [] END OF FILE */
