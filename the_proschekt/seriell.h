
#ifndef seriell_h
#define seriell_h

#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/usart.h"

int init_ser(void);
char putch(char);
char getch(void);

#endif /* seriell_h */
