#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void genkeys(int f_prime,int s_prime,char *name1,char *name2)
{
	int func_euler,d=0,e;
	func_euler = (f_prime-1)*(s_prime-1);
	printf("f(n)=%d \n",func_euler);

	while (d==0) {		
		e = rand();
		d = get_d(func_euler,e);
	}
	
	printf("public exponent e=%d\n",e);
	printf("private exponent d=%d\n",d);

	FILE *publ;
	publ = fopen(name1,"w");
	FILE *priv;
	priv = fopen(name2,"w");

	fprintf(publ,"%d",e);
	fprintf(priv,"%d",d);
	fclose(publ);fclose(priv);
}
char encrypt(char msg, int e, int n,int j) //шифрование
{
	int x;
	char kod;
	x = msg;
	
	kod = mod_pow(x,e,n);
	return kod;
}

char decrypt(int msg, int d, int n,int j) //дешифрование
{
	int x;
	char kod;
	x = msg;

	kod = mod_pow(x,d,n);
	return kod;
}
int test_prime() // тест на простоту Миллера-Рабина
{
	int a,n,m,k=0,i,T,chet=0;

	int N = rand();
	N = rand();

	N = N % 128;

	if((N % 2) == 0 || (N % 3) == 0 || (N % 5) == 0 || (N % 7) == 0 || (N % 9) == 0) {
		return 1;
	}
	
	n = N-1;
	m = n;

	while ((m%2) == 0) {
		m = m/2;
		k++;
	}

	for (a=2;a<11;a++) {

		T = mod_pow(a,m,N);

		if (T==1 || T==n) {
			chet++;
			T = 0;
		}

		for (i=1;i<k;i++) {

			T = (T*T) % N;

			if (T == 0)
				break;

			if (T == 1) {
				chet--;
				break;
			}

			if (T == n) {
				chet++;             
				break;
			}
		}
	}	

	if (chet == 9)
		return N;
	if (chet != 1)
		return 1;
}

