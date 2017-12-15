//#include "defines.h"
#include "seriell.c"
#include "../h/pio.h"
#include "../h/tc.h"
#include "../h/pmc.h"
#include "../h/aic.h"

#include <stdlib.h>


unsigned int CPU_CLOCK = 25000000; //CPU Takte pro sekunde

unsigned int fuellmenge = 100; //F??llmenge in GRAMM
unsigned int becher = 5; //Waagentoleranz in GRAMM

unsigned int FREQUENZ = 50; //Frequenz welche die Pumpe ben??tigt in HERZ
unsigned int VORTEILER_INT = 8;
unsigned int C1 = 2000; //C1 der verwendeten Seitenwaage
unsigned int C2 = 0; //C2 der verwendeten Seitenwaage

#define PRESCALER TC_CLKS_MCK8

#define TCMEASURE_INIT  TC_CLKS_MCK2 | TC_LDBSTOP | TC_CAPT | TC_LDRA_RISING_EDGE | TC_LDRB_RISING_EDGE //Timerkonfiguration f??r das Messen der von der Waage generierten Signale.


//tescht
void timerPumpInit( void );
void timerMeasureInit( void );

void rest(double time);
void putStr(char str[]);
int MessungderMasse(void);
void PIO_Init(void);
//void taste_irq_handler (void) __attribute__ ((interrupt));
void itos(char str[], int x);
void Timer3_init(void);

int becherInit(void);


volatile int Masse;
char str[12];
int warteAufBecher = 1;

int main(){
    
    StructPIO* piobaseA = PIOA_BASE;
    
	init_ser();

	Timer3_init();

	PIO_Init();


	while(1){
	
	warteAufBecher = 1;
	

    
   	int becherMasse = becherInit();
	Masse = MessungderMasse();
	volatile int alteMasse = Masse;
		
	while((Masse - becherMasse) <= fuellmenge){
	Masse = MessungderMasse();
        char input = getch();
	if(input == 's'){
		putStr("_PRESSED ABORD");
		Masse = 0;
	}
	else if(input == 'r'){
        putStr("_STARTED PUMP");
		piobaseA->PIO_PDR = (1 << PIOTIOA3);
	}
	else if(input == 'p'){
		putStr("_STOPPED PUMP");
		piobaseA->PIO_PER = (1 << PIOTIOA3);
	}
	if(Masse < (alteMasse-5)){
		putStr("_REMOVED CUP");
	}
	else if(alteMasse == Masse)
		;
	else{
		alteMasse = Masse;
		itos(str, Masse);
		putStr(str);
	
	}
	}
	putStr("_END_");
	
	piobaseA->PIO_PER = (1 << PIOTIOA3);
	
	}
	
	return 0;
	
    
}

int becherInit(){

    putStr("Put on the cup - 5g at least");
    while(putch(0xa) == 0)
	;
    while(putch(0xd) == 0)
	;

    
    int m = 0;
    
    while(warteAufBecher)
    	if(getch()=='b'){
		Masse = MessungderMasse();
		if(Masse < becher)
			;
		else
			warteAufBecher = 0;
	}

    putStr("cup weight:");
    
    m = MessungderMasse();
   
    char char_Masse[12];
    itos(char_Masse, m);
    
    putStr(char_Masse);

     while(putch(0xa) == 0)
	;
     while(putch(0xd) == 0)
	;
    
    return m;

}

void putStr(char str[]){
    
    while(putch(0xa) == 0)
        ;
    while(putch(0xd) == 0)
        ;
    
    int i = 0;
    //rest(0.001);
    while(str[i] != 0){
        while(putch(str[i]) == 0)
            ;
        i++;
    }
}


void itos(char str[], int x) {
    
    int numeral;
    int boolnegativ;
    int safeup = x;
    
    //check if negativ
    if (x > 0) {
        x = -1 * x;
        boolnegativ = 0;
    } else boolnegativ = 1;
    
    //counting charactersize
    int size = 0;
    
    while (x != 0) {
        numeral = x % 10;
        x = x / 10;
        size++;
    }
    x = safeup;
    //extract form int into array
    char arrstr[size];
    int i = 0;
    
    while (i != size) {
        numeral = x % 10;
        arrstr[i] = numeral + 48;
        x = x / 10;
        i++;
    }
    
    //put array to string
    size--;
    i = 0;
    
    if (boolnegativ) {
        i = 1;
        str[0] = '-';
    }
    char test;
    while (size > -1) {
        test = arrstr[size];
        // str[i] = arrstr[size];
        str[i] = test;
        i++;
        size--;
    }
    //Works 15.12 12:33 on RA NEtbeans
}


void rest(double time){
    
    volatile unsigned int count = 0;        //volatile = nicht wegoptimieren 
    unsigned int i = 0;
    for(i = 0; i < (time * CPU_CLOCK); i++)
        count = count + 1;
}






void timerPumpInit( void ){
    
    StructPIO* piobaseA = PIOA_BASE;
    StructPMC* pmcbase = PMC_BASE;
    StructTC*  timerbase3  = TCB3_BASE;
    
   
    timerbase3->TC_CCR = TC_CLKDIS;    // Disable Clock to Configure
    
   
    //setup registers
    int tc3regC = CPU_CLOCK;
    int tc3regA;
    tc3regC = (tc3regC / VORTEILER_INT) / FREQUENZ;
    tc3regA = tc3regC / 2;
    
    timerbase3->TC_RA = tc3regA;
    timerbase3->TC_RC = tc3regC;
    
    // Initialize mode
    timerbase3->TC_CMR =
    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
    TC_WAVE               |    //WAVE    : Waveform mode
    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
    PRESCALER;                 //TCCLKS  : MCKI / 1024
    
    
    //Start Timer
    timerbase3->TC_CCR = TC_CLKEN | TC_SWTRG;
    piobaseA->PIO_PER  = (1<<PIOTIOA3) ;
    piobaseA->PIO_OER  = (1<<PIOTIOA3) ;
    piobaseA->PIO_CODR = (1<<PIOTIOA3) ;

    at91_reg safeup = pmcbase->PMC_PCSR;
    pmcbase->PMC_PCER = safeup | (1<<TC3_ID);       //Enable clock
}


void timerMeasureInit( void ){
    
    StructPIO* piobaseA = PIOA_BASE;
    StructTC*  timerbase4  = TCB4_BASE;
    StructTC*  timerbase5  = TCB5_BASE;
    
    piobaseA->PIO_PDR         =    0x090;
    timerbase4->TC_CCR        =    TC_CLKDIS;
    timerbase4->TC_CMR        =    TCMEASURE_INIT;
    timerbase4->TC_CCR        =    TC_CLKEN;
    
    timerbase5->TC_CCR        =    TC_CLKDIS;
    timerbase5->TC_CMR        =    TCMEASURE_INIT;
    timerbase5->TC_CCR        =    TC_CLKEN;
}





























int MessungderMasse(void){


	StructTC* tcbase4 = TCB4_BASE;
	StructTC* tcbase5 = TCB5_BASE;

	
	StructAIC* aicbase = AIC_BASE;
	 aicbase->AIC_IDCR = 1 <<PIOB_ID;

	volatile int	captureRA1;
	volatile int	captureRB1;
	volatile int	captureRA2;
	volatile int	captureRB2;
	volatile int	capturediff1;
	volatile float Periodendauer1;
	volatile int	capturediff2;
	volatile float Periodendauer2;
	unsigned int masse = 0;
	
	volatile int drop;
	drop= tcbase4->TC_SR;          //liest au sdem register den wert aus //cleared beim lesen
	drop = tcbase5->TC_SR;         

	tcbase4->TC_CCR	=	TC_SWTRG;
	while (!( tcbase4->TC_SR & 0x40))
		;
	captureRA1	= tcbase4->TC_RA;				// 	
	captureRB1	= tcbase4->TC_RB;

	
	tcbase5->TC_CCR	=	TC_SWTRG;
	while (!( tcbase5->TC_SR & 0x40))
		;
	captureRA2	= tcbase5->TC_RA;				// 	
	captureRB2	= tcbase5->TC_RB;
        
        
        //wegen zeit und so 
        capturediff1	=	abs(captureRB1) - abs(captureRA1);
	Periodendauer1 = abs(capturediff1) / 12.5;	// Zeit in us
	capturediff2	= abs(captureRB2) - abs(captureRA2);
	Periodendauer2 = abs(capturediff2) / 12.5;	// Zeit in us
	
	
	 
	 aicbase->AIC_IECR = 1 << PIOB_ID;	     //pio darf interrupt werfen 
	
	float zw1 = 0;
	zw1 = Periodendauer1 / Periodendauer2;
	zw1 = zw1 - 1;
	zw1 = zw1 * C1;
	masse = zw1 - C2; //masse = (C1 * ((captureRA1 / captureRB1) - 1) - C2);
	
	
	
	return masse;
}


void PIO_Init(void)
{

	StructPIO* piobaseA = PIOA_BASE;
	StructTC* tcbase4 = TCB4_BASE;
	StructTC* tcbase5 = TCB5_BASE;
	StructAIC* aicbase = AIC_BASE;
	StructPMC* pmcbase = PMC_BASE;	
	StructPIO* piobaseB   	= PIOB_BASE;
	
		
	pmcbase->PMC_PCER = 0x06f80;

	
	aicbase->AIC_IDCR = 1 << 14;          //Interrupt f?r PIOB ausschalten
    	aicbase->AIC_ICCR = 1 << 14;          //Interrupt f?r PIOB l?schen
  	//aicbase->AIC_SVR[14]= (unsigned int)taste_irq_handler;
	
	aicbase->AIC_IECR = 1 <<14;           // Interrrupt f?r PIOB aktivieren
    
    	piobaseB->PIO_PER = KEY1 | KEY2;
    	piobaseB->PIO_ODR = KEY1 | KEY2;
    	piobaseB->PIO_IER = KEY1 | KEY2;
	
	pmcbase->PMC_PCER	= (1<<9) | (1<<13) | (1<<14);

}


//void taste_irq_handler (void)
//{
//   StructPIO* piobaseB   = PIOB_BASE;        // Basisadresse PIO B
//   StructPIO* piobaseA   = PIOA_BASE;
//   StructAIC* aicbase  = AIC_BASE;        //__
//
//// ab hier entsprechend der Aufgabestellung erg?nzen
//// *************************************************
//    if (~piobaseB->PIO_PDSR & KEY2)
//        if(warteAufBecher == 0)
//               piobaseA->PIO_PDR = (1 << PIOTIOA3);
//    if (~piobaseB->PIO_PDSR & KEY1){
//        //warteAufBecher = 0;
//        if(Masse > becher){
//        warteAufBecher = 0;
//    }
//    else{
//        warteAufBecher = 1;
//    }
//
//    }
//
//
//
//
//    aicbase->AIC_EOICR = piobaseB->PIO_ISR;    //__
//}


/*konnte noch nicht getestet werden*/

//void Timer3_init( void )
//{
//
//
//  StructTC* tcbase4 = TCB4_BASE;
//  StructTC* tcbase5 = TCB5_BASE;
//
//  StructPMC* pmcbase = PMC_BASE;
//  pmcbase->PMC_PCER = 0x06f80;
//
//  int TimerValueC = CPU_CLOCK;
//  TimerValueC = TimerValueC / VORTEILER_INT;
//  TimerValueC = TimerValueC / FREQUENZ;
//  int TimerValueA = TimerValueC;
//  TimerValueA = TimerValueA / 2;
//
//  StructTC* timerbase3  = TCB3_BASE;    // Basisadressse TC Block 1
//  StructPIO* piobaseA   = PIOA_BASE;    // Basisadresse PIO B
//
//    timerbase3->TC_CCR = TC_CLKDIS;    // Disable Clock
//
//  // Initialize the mode of the timer 3
//  timerbase3->TC_CMR =
//    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
//    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
//    TC_WAVE               |    //WAVE    : Waveform mode
//    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
//    PRESCALER;           //TCCLKS  : MCKI / 1024
//
//  // Initialize the counter:
//  timerbase3->TC_RA = TimerValueA;    //Systemtakt / Vorteiler / Frequenz = RC. Bei Gleichm??igem high low, ist RA = RC/2. Hier z.b.: 25000000 / 8 / 50 = 62500
//  timerbase3->TC_RC = TimerValueC;    //__
//
//  // Start the timer :
//  timerbase3->TC_CCR = TC_CLKEN ;    //__
//  timerbase3->TC_CCR = TC_SWTRG ;    //__
//  piobaseA->PIO_PER  = (1<<PIOTIOA3) ;    //__
//  piobaseA->PIO_OER  = (1<<PIOTIOA3) ;    //__
//  piobaseA->PIO_CODR = (1<<PIOTIOA3) ;    //__
//
//  //Initalisieren der Messtimer
//
//    pmcbase->PMC_PCER    = (1<<9) | (1<<13) | (1<<14);
//
//    piobaseA->PIO_PDR    =    0x090;
//    tcbase4->TC_CCR        =    TC_CLKDIS;
//    tcbase4->TC_CMR        =    TCMEASURE_INIT;
//    tcbase4->TC_CCR        =    TC_CLKEN;
//
//    tcbase5->TC_CCR        =    TC_CLKDIS;
//    tcbase5->TC_CMR        =    TCMEASURE_INIT;
//    tcbase5->TC_CCR        =    TC_CLKEN;
//}




