
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * 
 */
void intToStr(char str[], int n);
void reverseString(char str[]);

int main(int argc, char** argv) {
    
    

    return (EXIT_SUCCESS);
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