// Loesung zu Termin2
// Aufgabe 1
// von:
// vom: 
// 

#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/aic.h"

void interrupt (void) __attribute__ ((interrupt));

void interrupt (void)
{}

int main (){
    
    StructPMC* pmcbase = PMC_BASE;	// Basisadresse des PMC
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller
    
    pmcbase->PMC_PCER = 1 << PIOB_ID;  //0x4000, enable piob  - header aic.h
    
    piobaseB->PIO_PER = LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8 | KEY1 | KEY2 ;
    piobaseB->PIO_OER = LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8 ;
    piobaseB->PIO_ODR = KEY1 | KEY2 ;
    piobaseB->PIO_IER = KEY1 | KEY2 ;
    
    
    
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