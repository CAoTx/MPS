// Loesung zu Termin2
// Aufgabe 1
// von:
// vom: 
// 

#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/aic.h"
#include "../h/tc.h"

void interrupt (void) __attribute__ ((interrupt));
void timedInterrupt(void) __attribute__((interrupt));

void interrupt (void)
{
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller
    
     aicbase->AIC_EOICR = piobaseB->PIO_ISR; //Zurücksetzen des Interrupts
    
    if (~piobaseB->PIO_PDSR & KEY1)
        piobaseB->PIO_CODR = LED2;
    if (~piobaseB->PIO_PDSR & KEY2)
        piobaseB->PIO_SODR = LED2;
    
  //  aicbase->AIC_EOICR = piobaseB->PIO_ISR;     //Reset des /EndofInterruptCommandRegister/ mittels schreiben   -   Reset des /InterruptStatusRegister/ durch lesen 
}

void timedInterrupt(void){

	StructPIO* piobaseB   = PIOB_BASE;
	StructAIC* aicbase = AIC_BASE;
	
    if(~piobaseB->PIO_PDSR & LED3)
        piobaseB->PIO_CODR = LED3;
    else if(piobaseB->PIO_PDSR & (~LED3))
        piobaseB->PIO_SODR = LED3;
	
	aicbase->AIC_EOICR = piobaseB->PIO_ISR; 

}



void Timer3_init( void )
{
  StructTC* timerbase3  = TCB3_BASE;		// Basisadressse TC Block 1
  StructPIO* piobaseA   = PIOA_BASE;		// Basisadresse PIO B

	timerbase3->TC_CCR = TC_CLKDIS;			// Disable Clock
 
  // Initialize the mode of the timer 3
  timerbase3->TC_CMR = TC_ACPC_CLEAR_OUTPUT  |     TC_ACPA_SET_OUTPUT    |     TC_WAVE               |        TC_CPCTRG             |     TC_CLKS_MCK1024;           //TCCLKS  : MCKI / 1024

  // Initialize the counter:
  timerbase3->TC_RA = 300;	//__
  timerbase3->TC_RC = 600;	//__

  // Start the timer :
  timerbase3->TC_CCR = TC_CLKEN ;				//__
  timerbase3->TC_CCR = TC_SWTRG ;				//__
  piobaseA->PIO_PDR  = (1<<PIOTIOA3) ;	//__
  piobaseA->PIO_OER  = (1<<PIOTIOA3) ;	//__
  piobaseA->PIO_CODR = (1<<PIOTIOA3) ;	//__
}



int main (){
    
    StructPMC* pmcbase = PMC_BASE;	// Basisadresse des PMC
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller
    
    pmcbase->PMC_PCER = 1 << PIOB_ID;  //0x4000, enable piob  - header aic.h
    
    aicbase->AIC_IDCR = 1 << 14;          //Interrupt f?r PIOB ausschalten
    aicbase->AIC_ICCR = 1 << 14;          //Interrupt f?r PIOB l?schen
    aicbase->AIC_SVR[14]= (unsigned int)interrupt;
    aicbase->AIC_SVR[9] = (unsigned int)timedInterrupt;

    aicbase->AIC_SMR[PIOB_ID] = 0x7; // Priorit?t	
    aicbase->AIC_IECR = 1 <<14;           // Interrrupt f?r PIOB aktivieren
    
    piobaseB->PIO_PER = LED1 | LED2 | LED3 | KEY1 | KEY2 ;
    piobaseB->PIO_OER = LED1 | LED2 | LED3;
    piobaseB->PIO_ODR = KEY1 | KEY2 ;
    piobaseB->PIO_IER = KEY1 | KEY2 ;
    
    piobaseB->PIO_SODR = LED1 | LED2 | LED3;
    
    Timer3_init();

    
    while(1)
    {
    	    if(~piobaseB->PIO_PDSR & KEY1) {             
	    piobaseB->PIO_CODR = LED1;     
	    }        
	    if(~piobaseB->PIO_PDSR & KEY2 ){
	    piobaseB->PIO_SODR = LED1;            
	    }
    }
    
    return 0;
}









/*
void interrupt (void) __attribute__ ((interrupt));

void interrupt (void)
{
    StructPIO* piobaseB = PIOB_BASE;
    StructAIC* aicbase = AIC_BASE;

    if(!(piobaseB->PIO_PDSR = KEY1) == 0)
    {
        piobaseB->PIO_CODR = LED1;
    }
    
    if(!(piobaseB->PIO_PDSR = KEY2) == 0)
    {
    	piobaseB->PIO_SODR = LED1;
    }
    
    aicbase->AIC_EOICR = piobaseB ->PIO_ISR;
}

int main(void)
{
  StructPMC* pmcbase = PMC_BASE;	// Basisadresse des PMC
  StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
  StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller 
  
  pmcbase->PMC_PCER = (1 << 14);   // | (1 << PIOA_ID); // Shift auf 14tes bit zum einschalten der PIOB
  aicbase->AIC_IDCR = 1 << 14;          //Interrupt f?r PIOB ausschalten
  aicbase->AIC_ICCR = 1 << 14;          //Interrupt f?r PIOB l?schen
  aicbase->AIC_SVR[14]= (unsigned int)interrupt;
  aicbase->AIC_IECR = 1 <<14;           // Interrrupt f?r PIOB aktivieren

  piobaseB->PIO_PER = LED1 | LED2 | LED4 |LED5 |LED6 |LED7 |LED8 |KEY1 | KEY2;
  piobaseB->PIO_OER = LED1 | LED2;
  piobaseB->PIO_ODR = KEY1 | KEY2; 
  piobaseB->PIO_IER = KEY1 | KEY2;      //Interrupt erlauben f?r KEY1 KEY2    
    
	 piobaseB->PIO_CODR = LED1;
    
  while(1)
  {
  //if()
   // piobaseB->PIO_CODR = 0x1;
 // if()  
   // piobaseB->PIO_SODR = 0x1;
    
    //piobaseB->PIO_CODR = 0xFE00;
    
    //piobaseB->PIO_SODR = 0xFE00;
    
  }
  
  aicbase->AIC_IDCR = 1 << 14;      //Interrupt f?r PIOB ausschalten
  aicbase->AIC_ICCR = 1 << 14;      //Interrupt f?r PIOB l?schen
    
  return 0;
}
*/
