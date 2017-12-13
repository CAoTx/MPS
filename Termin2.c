// Loesung zu Termin2
// Aufgabe 1
// von:
// vom: 
// 

#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/aic.h"
#include "../h/tc.h"

/* Functions */
void    interrupt (void) __attribute__ ((interrupt));
void    Timer_init( void );
int     enable (unsigned int* port, unsigned int* output, unsigned int* set, unsigned int* clear);
void    audiBlink (unsigned int* port, unsigned int* output, unsigned int* set, unsigned int* clear);
int    blink0_5();
int    blink1();

/* Globals */
int blinkStatus = 0;


int main (){
    
    /* INIT */
    unsigned int* PIOB_PER = (int*)(0xFFFF0000);
    unsigned int* PIOB_OER = (int*)(0xFFFF0010);
    unsigned int* PIOB_SOER = (int*)(0xFFFF0030);
    unsigned int* PIOB_COER = (int*)(0xFFFF0034);
    
    StructPMC* pmcbase = PMC_BASE;	// Basisadresse des PMC
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller
    
    /* PMC */
    pmcbase->PMC_PCER = 1 << PIOB_ID;   //0x4000, enable piob  - header aic.h
    pmcbase->PMC_PCER = 1 << TC0_ID;    //enable TC0 clock                              ----------------------
    
    /* AIC */
    aicbase->AIC_IDCR = 1 << PIOB_ID;          //Interrupt f?r PIOB ausschalten
    aicbase->AIC_ICCR = 1 << PIOB_ID;          //Interrupt f?r PIOB l?schen
    aicbase->AIC_SVR[PIOB_ID]= (unsigned int)interrupt;
    //aicbase->AIC_SVR[9] = (unsigned int)timedInterrupt;

    aicbase->AIC_SMR[PIOB_ID] = 0x7;    // Priorit?t	
    aicbase->AIC_IECR = 1 << PIOB_ID;         // Interrrupt f?r PIOB aktivieren
    
    /* PIOB */
    piobaseB->PIO_PER = ALL_LEDS ;
    piobaseB->PIO_OER = ALL_LEDS ;
    piobaseB->PIO_ODR = KEY1 | KEY2 | KEY3;
    piobaseB->PIO_IER = KEY1 | KEY2 | KEY3;
    piobaseB->PIO_SODR = ALL_LEDS;    
    
    

    
    while(1)
    audiBlink(PIOB_PER, PIOB_OER, PIOB_SOER, PIOB_COER);
   
    
  //  Timer3_init();
   
    
    return 0;
}


/* Interrupts */
void interrupt (void)
{
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    StructAIC* aicbase = AIC_BASE;        //Basisadresse AIC - advanced interrupt controller
    
     aicbase->AIC_EOICR = piobaseB->PIO_ISR; //Zur?cksetzen des Interrupts
    if (~piobaseB->PIO_PDSR & KEY1)
        blinkStatus=2;
    if (~piobaseB->PIO_PDSR & KEY2)
        blinkStatus=1;
         if (~piobaseB->PIO_PDSR & KEY3)
             blinkStatus=0;
       
    
  //  aicbase->AIC_EOICR = piobaseB->PIO_ISR;     //Reset des /EndofInterruptCommandRegister/ mittels schreiben   -   Reset des /InterruptStatusRegister/ durch lesen 
}


void Timer_init( void )
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

/* NORMAL FUNCTIONS */
int enable (unsigned int* port, unsigned int* output, unsigned int* set, unsigned int* clear){

	*port = 0xff00;
	*output = 0xff00;
	return 1;
}

void audiBlink (unsigned int* port, unsigned int* output, unsigned int* set, unsigned int* clear){

	if (enable(port, output, set, clear) != 1)
            return;
		
	//Actual turn off
	*set = 0xff00;
			
	if (blinkStatus == 0){
        blink1();
	*clear = 0x800;
	*clear = 0x1000;
        blink1();
	*clear = 0x400;
	*clear = 0x2000;
        blink1();
	*clear = 0x4000;
	*clear = 0x200;
        blink1();
	*clear = 0x100;
	*clear = 0x8000;
        blink1();
	*set = 0xff00;
        }
        
        if(blinkStatus==1){
        blink0_5();
	*clear = 0x100;
        blink0_5();
	*clear = 0x200;
        blink0_5();
	*clear = 0x400;
        blink0_5();
	*clear = 0x800;
        blink0_5();
	*clear = 0x1000;
        blink0_5();
	*clear = 0x2000;
        blink0_5();
	*clear = 0x4000;
        blink0_5();
	*clear = 0x8000;
        blink0_5();
        *set = 0xff00;
}
        
        if(blinkStatus==2){
        blink0_5();
	*clear = 0x8000;
        blink0_5();
	*clear = 0x4000;
        blink0_5();
	*clear = 0x2000;
        blink0_5();
	*clear = 0x1000;
        blink0_5();
	*clear = 0x800;
        blink0_5();
	*clear = 0x400;
        blink0_5();
	*clear = 0x200;
        blink0_5();
	*clear = 0x100;
        blink0_5();
        *set = 0xff00;
        }
return;
}

int blink0_5(){
    
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    
    int rechencounter = (25000000 / 5) / 150 ;
	    int i = 0;
	    while( i < rechencounter){
                i = i+1;
            }
            return 1;
}
int blink1(){
    
    StructPIO* piobaseB = PIOB_BASE;	// Basisadresse PIOB
    
    int rechencounter = (25000000 / 5) / 50 ;
	    int i = 0;
	    while( i < rechencounter){
                i = i+1;
            }
            return 1;
}

