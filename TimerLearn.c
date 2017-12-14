 
#include "pmc.h"
#include "pio.h"
#include "aic.h"
#include "tc.h"
#include <stdio.h>

//void    interrupt_piob (void) __attribute__ ((interrupt));
void    interrupt_tc0 (void) __attribute__ ((interrupt));

void initTimer(void);

int main (){
    
        /* INIT */
    unsigned int* PIOB_PER = (int*)(0xFFFF0000);
    unsigned int* PIOB_OER = (int*)(0xFFFF0010);
    unsigned int* PIOB_SOER = (int*)(0xFFFF0030);
    unsigned int* PIOB_COER = (int*)(0xFFFF0034);
    unsigned int* PIOB_PDSR = (int*)(0xFFFF0038);
    
    
    StructPMC* pmcbase = PMC_BASE;	// Basisadresse des PMC
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller
    
     /* PMC */
   // pmcbase->PMC_PCER = (1 << PIOB_ID);   //0x4000, enable piob  - header aic.h
    pmcbase->PMC_PCER = (1 << TC0_ID);    //enable TC0 clock   
    
    /* AIC */
   // aicbase->AIC_IDCR = (1 << PIOB_ID);          //Interrupt f?r PIOB ausschalten
    aicbase->AIC_IDCR = (1 << TC0_ID);  

   // aicbase->AIC_ICCR = (1 << PIOB_ID);          //Interrupt f?r PIOB l?schen
    aicbase->AIC_ICCR = (1 << TC0_ID); 
    
   // aicbase->AIC_SVR[PIOB_ID] = (unsigned int)interrupt_piob;
    aicbase->AIC_SVR[TC0_ID]  = (unsigned int)interrupt_tc0;

    //aicbase->AIC_SMR[PIOB_ID] = 0x7;    	// Priorit?t	
   // aicbase->AIC_IECR = (1 << PIOB_ID);         	// Interrrupt f?r PIOB aktivieren
    aicbase->AIC_IECR = (1 << TC0_ID);
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    initTimer();
    
    int i = 0;
	while(1)
	;
    
    return 0;
    
}


void interrupt_tc0 (void){
    
    
    StructPIO* piobaseB = PIOB_BASE;
    StructAIC* aicbase = AIC_BASE;

        
    piobaseB->PIO_PER = LED1 | LED2;
    piobaseB->PIO_OER = LED1 | LED2;
    if ((piobaseB->PIO_PDSR & LED1)&&(piobaseB->PIO_PDSR & LED2))
    {
        piobaseB->PIO_CODR = LED1 | LED2;
    }
    else {
    piobaseB->PIO_SODR = LED1 | LED2;
        }
	aicbase->AIC_EOICR = piobaseB->PIO_ISR;		//for reset to get ready for next interrupt
}

//void interrupt_piob (void){}


void initTimer (void){
    
    StructTC* timerbase0 = TCB0_BASE;
    StructPIO* piobaseB = PIOB_BASE;
    StructAIC* aicbase = AIC_BASE;
    
    
    timerbase0->TC_CCR = TC_CLKDIS;
    timerbase0->TC_IER = 1 << 4 ;
    
          // Initialize the mode of the timer 3
    timerbase0->TC_CMR =
    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
    TC_WAVE               |    //WAVE    : Waveform mode
    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
    TC_CLKS_MCK1024;           //TCCLKS  : MCKI / 1024
    
    
    
    //Initialize mode of TC
    timerbase0->TC_RA = 6103;
    timerbase0->TC_RC = 12207;
    

    // Starten des Timer und Ausgabe eines Low-Pegel an die Pumpe
    timerbase0->TC_CCR = TC_CLKEN ;
    timerbase0->TC_CCR = TC_SWTRG ; //muss noch getriggerrt werden um z?hlvoprgang zu starten   //?ber c,b register kann CPCSTOP eingeschaltet werden    
    
}





    

