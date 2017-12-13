unsigned int CPU_CLOCK = 25000000; //CPU Takte pro sekunde

unsigned int fuellmenge = 100; //F??llmenge in GRAMM
unsigned int becher = 5; //Waagentoleranz in GRAMM

unsigned int FREQUENZ = 50; //Frequenz welche die Pumpe ben??tigt in HERZ
unsigned int VORTEILER_INT = 8;
unsigned int C1 = 2000; //C1 der verwendeten Seitenwaage
unsigned int  C2 = 0; //C2 der verwendeten Seitenwaage

#define VORTEILER TC_CLKS_MCK8

#define TC_INIT  TC_CLKS_MCK2 | TC_LDBSTOP | TC_CAPT | TC_LDRA_RISING_EDGE | TC_LDRB_RISING_EDGE //Timerkonfiguration f??r das Messen der von der Waage generierten Signale.
