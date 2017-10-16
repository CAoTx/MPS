// Lösung zu Termin1
// Aufgabe 1
// Namen: ___________; ____________
// Matr.: ___________; ____________
// vom:   ___________

/*#define PIOB_PER ((unsigned int *) 0xFFFF0000)

unsigned int adr_PIOB_CODR = 0xFFFF0034;

int main (){

unsigned int adr_PIOB_OER = 0xFFF0010;
*PIOB_PER = 0x100;
*((unsigned int *) adr_PIOB_OER) = 0x100;

while(1){
*((unsigned int *) 0xFFFF0030) = 0x100;
*((unsigned int *) adr_PIOB_CODR) = 0x100;
}

return 0;


}
*/
int blinEnable(unsigned int* port,unsigned int* output,unsigned int* set,unsigned int* clear);
void audiBlink(unsigned int* port,unsigned int* output,unsigned int* set,unsigned int* clear);


int main (){

 unsigned int* PIOB_PER = (int*)(0xFFFF0000);
 unsigned int* PIOB_OER = (int*)(0xFFFF0010);
 unsigned int* PIOB_SOER = (int*)(0xFFFF0030);
 unsigned int* PIOB_COER = (int*)(0xFFFF0034);

while (1)
 audiBlink(PIOB_PER, PIOB_OER, PIOB_SOER, PIOB_COER);

	return 0;

}


int blinkEnable (unsigned int* port, unsigned int* output, unsigned int* set, unsigned int* clear){
	
	*port = 0xff00;
	*output = 0xff00;

	return 1;
}

void audiBlink (unsigned int* port, unsigned int* output, unsigned int* set, unsigned int* clear){

	if (blinkEnable(port, output, set, clear) != 1)
		return;
	
	//Actual turn on
	*set = 0xff00;
				
	//Actual turn off;
	*clear = 0x100;
	*clear = 0x200;
	*clear = 0x400;
	*clear = 0x800;
	*clear = 0x1000;
	*clear = 0x2000;
	*clear = 0x4000;
	*clear = 0x8000;

	*set = 0xff00;
return;
}









