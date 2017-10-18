// Loesung zu Termin2
// Aufgabe 1
// von:
// vom: 
// 

#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/aic.h"

int main(void)
{
  StructPMC* pmcbase = PMC_BASE;	// Basisadresse des PMC
  StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
  pmcbase->PMC_PCER = (1 << PIOB_ID);// | (1 << PIOA_ID); // Shift auf 14tes bit zum einschalten der PIOB

  piobaseB->PIO_PER = LED1 | LED2 | KEY1 | KEY2;
  piobaseB->PIO_OER = LED1 | LED2;
  piobaseB->PIO_ODR = KEY1 | KEY2; 
    
  while(1)
  {
  if()
    piobaseB->PIO_CODR = 0x1;
  if()  
    piobaseB->PIO_SODR = 0x1;
    
    //piobaseB->PIO_CODR = 0xFE00;
    
    //piobaseB->PIO_SODR = 0xFE00;
    
  }
    
  return 0;
}
