#include <arm_math.h>
#include <core_cm4.h> 
#include <stdio.h>

#define TEST_LENGTH_SAMPLES  300
#define BLOCK_SIZE           100 // traitement toutes les deux secondes , frequence d'echantillonage est de 50 sps
#define NUM_TAPS             125
#define BUFFER_LENGTH        1300

static float32_t Output[TEST_LENGTH_SAMPLES]; // buffer a la sortie du signal
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; // size du buffer 
float32_t firCoeffs32_LowPass[NUM_TAPS];
float32_t firCoeffs32_HighPass[NUM_TAPS];
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;
float vecteur_filtre_rouge[BUFFER_LENGTH]; 
float vecteur_filtre_infrarouge[BUFFER_LENGTH]; 



void FiltreLowPass(float* buffer ,float* filteredSignal);
void FiltreHighPass(float* buffer ,float* filteredSignal);
void filtrage(float* preFilteredData, float* filteredData);







/* [] END OF FILE */
