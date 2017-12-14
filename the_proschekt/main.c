#include "defines.h"
#include "seriell.c"
#include "../h/pio.h"
#include "../h/tc.h"
#include "../h/pmc.h"
#include "../h/aic.h"



//tescht

void sleep(double time);
void putStr(char str[]);
int MessungderMasse(void);
void PIO_Init(void);
void taste_irq_handler (void) __attribute__ ((interrupt));
void intToStr(char str[], int n);
void reverseString(char str[]);
void Timer3_init();


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
		putStr("Nutzer hat abgebrochen");
		Masse = 0;
	}
	else if(input == 'r'){
		putStr("pumpen gestartet");
		piobaseA->PIO_PDR = (1 << PIOTIOA3);
	}
	else if(input == 'p'){
		putStr("pumpen pausiert");
		piobaseA->PIO_PER = (1 << PIOTIOA3);
	}
	if(Masse < (alteMasse-5)){
		putStr("Becher entfernt");
	}
	else if(alteMasse == Masse)
		;
	else{
		alteMasse = Masse;
		intToStr(str, Masse);
		putStr(str);
	
	}
	}
	putStr("Ende erreicht");
	
	piobaseA->PIO_PER = (1 << PIOTIOA3);
	
	}
	
	return 0;
	
    
}

int becherInit(){

    StructPIO* piobaseB   	= PIOB_BASE;

    putStr("Bitte Becher aufstellen. Mit Taste 1 bestaetigen");
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

    putStr("Becher gewicht:");
    
    m = MessungderMasse();
   
    char char_Masse[12];
    intToStr(char_Masse, m);
    
    putStr(char_Masse);

     while(putch(0xa) == 0)
	;
     while(putch(0xd) == 0)
	;
    
    return m;
    
}

void intToStr(char str[], int n){
    int ziffer = 0;
    int i = 0;
    int isNegativ = 0;
    
    if(n < 0){
    	isNegativ = 1;
	n = n * -1;
    }
    
    while (n != 0){
    	ziffer = n % 10;
	n = n / 10;
	str[i] = ziffer + 48;     //'0' geht och 
	i++;    
    }
    
    if(isNegativ){
    	str[i] = '-';
	i++;
    }
    
    str[i] = 0;
    
    reverseString(str);
}




void itos(char str[], int x){
    
    int numeral;
    int boolnegativ;
    int safeup = x;
    
    if(n > 0)
        x = -1*x;
        boolnegativ = 0;
    else boolnegativ = 1;
    
    //counting charactersize
    int size = 0;
    
    while (x != 0){
        numeral = x % 10;
        x = x / 10;
        size++;
    }
    
    char arrstr[size];
    int i = 0;
    
    while (x != 0){
        numeral = x % 10;
        x = x / 10;
        size++;
        
    }
    
    
    
}

































void reverseString(char str[]){
	char tmp = 0;
	int vorne = 0;
	int count = 0;
	while(str[count] != 0)
		count = count + 1;
	count --;
	while(vorne < count){
		tmp = str[count];
		str[count] = str[vorne];
		str[vorne] = tmp;
		count--;
		vorne++;
	}

}

void sleep(double time){
    
    volatile unsigned int count = 0;        //volatile = nicht wegoptimieren 
    unsigned int i = 0;
    for(i = 0; i < (time * CPU_CLOCK); i++)
        count = count + 1;
}

void putStr(char str[]){

     while(putch(0xa) == 0)
	;
     while(putch(0xd) == 0)
	;

    int i = 0;
    //sleep(0.001);
    while(str[i] != 0){
    	while(putch(str[i]) == 0)
	;
	i++;
    }

}

void Timer3_init( void )
{

  
  StructTC* tcbase4 = TCB4_BASE;
  StructTC* tcbase5 = TCB5_BASE;

  StructPMC* pmcbase = PMC_BASE;
  pmcbase->PMC_PCER = 0x06f80;

  int TimerValueC = CPU_CLOCK;
  TimerValueC = TimerValueC / VORTEILER_INT;
  TimerValueC = TimerValueC / FREQUENZ;
  int TimerValueA = TimerValueC;
  TimerValueA = TimerValueA / 2;
	
  StructTC* timerbase3  = TCB3_BASE;	// Basisadressse TC Block 1
  StructPIO* piobaseA   = PIOA_BASE;	// Basisadresse PIO B

	timerbase3->TC_CCR = TC_CLKDIS;	// Disable Clock
 
  // Initialize the mode of the timer 3
  timerbase3->TC_CMR =
    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
    TC_WAVE               |    //WAVE    : Waveform mode
    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
    VORTEILER;           //TCCLKS  : MCKI / 1024

  // Initialize the counter:
  timerbase3->TC_RA = TimerValueA;	//Systemtakt / Vorteiler / Frequenz = RC. Bei Gleichm??igem high low, ist RA = RC/2. Hier z.b.: 25000000 / 8 / 50 = 62500
  timerbase3->TC_RC = TimerValueC;	//__

  // Start the timer :
  timerbase3->TC_CCR = TC_CLKEN ;	//__
  timerbase3->TC_CCR = TC_SWTRG ;	//__
  piobaseA->PIO_PER  = (1<<PIOTIOA3) ;	//__
  piobaseA->PIO_OER  = (1<<PIOTIOA3) ;	//__
  piobaseA->PIO_CODR = (1<<PIOTIOA3) ;	//__
  
  //Initalisieren der Messtimer
  
	pmcbase->PMC_PCER	= (1<<9) | (1<<13) | (1<<14);
    
    piobaseA->PIO_PDR	=	0x090;		
	tcbase4->TC_CCR		=	TC_CLKDIS;
	tcbase4->TC_CMR		=	TC_INIT;
	tcbase4->TC_CCR		=	TC_CLKEN;
	
	tcbase5->TC_CCR		=	TC_CLKDIS;
	tcbase5->TC_CMR		=	TC_INIT;
	tcbase5->TC_CCR		=	TC_CLKEN;
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
   	aicbase->AIC_SVR[14]= (unsigned int)taste_irq_handler;
	
	aicbase->AIC_IECR = 1 <<14;           // Interrrupt f?r PIOB aktivieren
    
    	piobaseB->PIO_PER = KEY1 | KEY2;
    	piobaseB->PIO_ODR = KEY1 | KEY2;
    	piobaseB->PIO_IER = KEY1 | KEY2;
	
	pmcbase->PMC_PCER	= (1<<9) | (1<<13) | (1<<14);

}

void taste_irq_handler (void)
{
   StructPIO* piobaseB   = PIOB_BASE;		// Basisadresse PIO B
   StructPIO* piobaseA   = PIOA_BASE;
   StructAIC* aicbase  = AIC_BASE;		//__
	
// ab hier entsprechend der Aufgabestellung erg?nzen
// *************************************************
    if (~piobaseB->PIO_PDSR & KEY2)
    	if(warteAufBecher == 0)
       		piobaseA->PIO_PDR = (1 << PIOTIOA3);
    if (~piobaseB->PIO_PDSR & KEY1){
    	//warteAufBecher = 0;
    	if(Masse > becher){
		warteAufBecher = 0;
	}
	else{
		warteAufBecher = 1;
	}
    
    }
       	

	
	
	aicbase->AIC_EOICR = piobaseB->PIO_ISR;	//__
}
