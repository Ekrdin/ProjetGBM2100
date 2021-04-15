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
#include "TraitementdeSignal.h"

void FiltreLowPass(float* buffer ,float* filteredSignal) {
    
    uint32_t compteur;
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S,NUM_TAPS,(float32_t *) &firCoeffs32_LowPass[0],&firStateF32[0],blockSize);
    for (compteur=0; compteur< numBlocks; compteur++){
        arm_fir_f32(&S,buffer + (compteur*blockSize),filteredSignal+ (compteur*blockSize),blockSize);
         
        
    }
}
    

void FiltreHighPass(float* buffer ,float* filteredSignal) {
    
    uint32_t compteur;
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S,NUM_TAPS,(float32_t *) & firCoeffs32_HighPass[0],&firStateF32[0],blockSize);
    for (compteur=0; compteur< numBlocks; compteur++){
        arm_fir_f32(&S,buffer + (compteur*blockSize),filteredSignal+ (compteur*blockSize),blockSize);
    }
    
}

//preFilteredData est un tableau, filteredData aussi
void filtrage(float* preFilteredData, float* filteredData){
    
        float point_intermediaire = 0.0;
        FiltreLowPass(preFilteredData,&point_intermediaire);
        FiltreHighPass(&point_intermediaire,filteredData);
        
}

void SpO2_calc(){
    
    for (int k=2; k<BUFFER_LENGTH -1;k++){
       
         }

}


















/* [] END OF FILE */
