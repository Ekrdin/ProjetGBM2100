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
#include "Declaration.h"
// Low pass pour avoir le DC
void FiltreLowPass(float* buffer ,float* filteredSignal) {
    
    //uint32_t compteur;
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S,NUM_TAPS,(float32_t *) &firCoeffs32_LowPass[0],&firStateF32[0],blockSize);
    arm_fir_f32(&S,buffer,filteredSignal,blockSize);
    
    //CECI EST UTILISE POUR FILTRER L'ENSEMBLE DES ENCHANTILLONS
    /*for (compteur=0; compteur< numBlocks; compteur++){
        arm_fir_f32(&S,buffer + (compteur*blockSize),filteredSignal+ (compteur*blockSize),blockSize);
 
    }*/
}
    
// high pass pour avoir le AC
void FiltreHighPass(float* buffer ,float* filteredSignal) {
    
    //uint32_t compteur;
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S,NUM_TAPS,(float32_t *) & firCoeffs32_HighPass[0],&firStateF32[0],blockSize);
    arm_fir_f32(&S,buffer,filteredSignal,blockSize);
    
    //CECI EST UTILISE POUR FILTRER L'ENSEMBLE DES ENCHANTILLONS
    /*for (compteur=0; compteur< numBlocks; compteur++){
        arm_fir_f32(&S,buffer + (compteur*blockSize),filteredSignal+ (compteur*blockSize),blockSize);
    }*/
    
}

_Bool estUnPic(uint16_t index){

    return (dataSpo2[index][RED][AC]>dataSpo2[index-1][RED][AC])&& (dataSpo2[index][RED][AC]>dataSpo2[index+1][RED][AC]) && (dataSpo2[index][RED][AC]>1000);

}

uint8_t calculBPM(uint16_t index){
    
        uint8_t nb_battements = 0;
        uint16_t position[2];
        
         BPM = 0;
        
        for(int i = index - BLOCK_SIZE; i<index; i++){
                if(estUnPic(i)){
                    
                    position[nb_battements++]=i;
                    
                    if(nb_battements == 2){
                        
                        BPM = 60.0/((position[1]-position[0])*50.0);
 
                    }           
                }
        }
        return  BPM; // on retourne le bpm

}

float32_t calculSPO2(uint16_t index){
    
        uint8_t nb_battements = 0;
        uint16_t position[2];
        float32_t ratio = 0.0;
        
        for(int i = index - BLOCK_SIZE; i<index; i++){
                if(estUnPic(i)){
                    
                    position[nb_battements++]=i;
                    
                    if(nb_battements == 2){
                        
                        ratio = (dataSpo2[RED][AC][i]*dataSpo2[IR][DC][i])/ (dataSpo2[RED][DC][i]*dataSpo2[IR][AC][i]);
                        pourcentageSpO2 = -50*(ratio*ratio) + 5*ratio + 117;

                    }
                }
        }
        return  pourcentageSpO2; // on retourne le sp02
}


//data correspond a un regroupement de donnee a un temps precis (x = fixe/)
void filtrage(uint16_t index){
        
        //float point_intermediaire = 0.0;
    
        //RED
        FiltreLowPass(&dataSpo2[RED][RAW][index],&dataSpo2[RED][DC][index]);
        FiltreHighPass(&dataSpo2[RED][RAW][index],&dataSpo2[RED][AC][index]);
        
        //IR
        FiltreLowPass(&dataSpo2[IR][RAW][index],&dataSpo2[IR][DC][index]);
        FiltreHighPass(&dataSpo2[IR][RAW][index],&dataSpo2[IR][AC][index]);
        
}

void TraitementSignal( void*arg){ // cette tache a moins de priorité que la tache d'ecriture pour que l'index soit toujours en retard par rapport a l'ecriture.

    (void)arg;
    uint16_t indexLecture = 0;
    
    for(;;)
    {
      
       
        //a chaque seconde (donc a chaque 50 nouvelles donnees)
        if(((indexLecture-99)%100)==0){
            //calculer BPM + SpO2
            filtrage(indexLecture);
            BPM = calculBPM(indexLecture);
            pourcentageSpO2 = calculSPO2(indexLecture);
        }
        
        indexLecture++;
        if(indexLecture == BUFFER_LENGTH){
            indexLecture = 0;
        }
        
       vTaskDelay(pdMS_TO_TICKS(1));  
}

}

/* [] END OF FILE */
