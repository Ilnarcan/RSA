#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

int get_d(int a, int e) //находим закрытую экспоненту d
{
	int b,i,q,r,d,d1,d2,d3;
	d1=0;d2=1;
	int euler = a;
	b = e;

	for (i=0;i<euler;i++) {

		q = a / b;
		r = a % b;
		d3 = d1-(d2*q);
		a = b;
		b = r;
		d1 = d2;
		d2 = d3;

		if (r == 0)
			return 0;

		if (r == 1)	{
			if (d3<0)
				while (d3<0)
					d3 = d3+euler;
			d = d3;
			return d;
		}
	}
	return 0;
}

int mod_pow(int x, int power,int mod) //функция быстрого возведения в степень
{
	int y=1;

	while (power!=0) {

		if (power%2!=0) 
			y = (y*x) % mod;

		power /= 2;
		x = (x*x) % mod;
	}
	return y;
}

int encrypt(char msg[], int e, int n,int j) //шифрование
{
	int x,kod;
	x = msg[j];
	
	kod = mod_pow(x,e,n);
	return kod;
}

int decrypt(int msg[], int d, int n,int j) //дешифрование
{
	int x,kod;
	x = msg[j];

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

int main(int argc, char *argv[])
{
	int f_prime=1,s_prime=1,en=0,chet=0,ch1,n,e=1,d=0,dn=0;
	char msg[100];
	int enc_msg[100];

	printf("                             RSA system\n");

	if (strcmp(argv[1], "gen") == 0) {			// gen public.txt private.txt
		
		printf("generation of two prime numbers\n");

		while(f_prime == 1)
			f_prime = test_prime();

		while(s_prime == 1)
			s_prime = test_prime();

		printf("numbers generated\n");
		printf("first_prime=%d, ",f_prime);
		printf("second_prime=%d, ",s_prime);
		printf("n=%d \n",n=f_prime*s_prime);

		genkeys(f_prime,s_prime,argv[2],argv[3]);
	}

	else if(strcmp(argv[1], "help") == 0) {
		printf("\n  gen pubic.txt private.txt - writes the public exponent (e) to file public.txt, closed (d) - a file private.txt\n");
		printf("\n  enc a.txt out.txt public.txt - encrypts data from the file a.txt with a key in public.txt; code would be stored out.txt\n");
		printf("\n  dec out.txt b.txt private.txt - decrypts the data from out.txt with the key in private.txt; puts them in a file b.txt\n");
		exit(1);
	}

	if (strcmp(argv[4], "enc") == 0) { // enc a.txt out.txt public.txt

			FILE *a;
			a = fopen(argv[5],"rb");
			FILE *out;
			out = fopen(argv[6],"wb");
			FILE *publ;
			publ = fopen(argv[7],"r");
			/*FILE *a;
			a = fopen("D:/a.txt","rb");
			FILE *out;
			out = fopen("D:/out.txt","wb");
			FILE *publ;
			publ = fopen("D:/public.txt","r");*/
			int j=0;

			if (a == NULL || publ == NULL) {
				printf("File not found\n");
				exit(1);
			}

			while (!feof(a)) {

				if (fread(&msg[j], 1, 1, a)>0) {
					fscanf(publ,"%d",&en);
					enc_msg[j] = encrypt(msg,en,n,j);
					//fprintf(out,"%d",enc_msg[j++]);
					fwrite(&enc_msg[j],4,1,out);
					chet++;
				}
			}
			fclose(a);fclose(out);fclose(publ);
		}

	if (strcmp(argv[8],"dec") == 0) { // dec out.txt b.txt private.txt

			FILE *out;
			out = fopen(argv[9],"rb");
			FILE *b;
			b = fopen(argv[10],"wb");
			FILE *priv;
			priv = fopen(argv[11],"r");
			/*FILE *out;
			out = fopen("D:/out.txt","rb");
			FILE *b;
			b = fopen("D:/b.txt","wb");
			FILE *priv;
			priv = fopen("D:/private.txt","r");*/

			int j=0;

			if (out == NULL || priv == NULL) {
				printf("File not found\n");
				exit(1);
			}
			
			while (!feof(out)) {
			if (fread(&enc_msg[j],4, 1, out)>0) {
				//fscanf(out,"%d",&enc_msg[j]);
				fscanf(priv,"%d",&dn);
				msg[j] = decrypt(enc_msg,dn,n,j);
				fwrite(&msg[j++],1,1,b);
			}
			}
			fclose(out);fclose(b);fclose(priv);
	}
	//}	

	if(argc>12) {
		printf("\n\nParametres are not true input");
		printf("\nTo see the possible commands, input help\n");
		printf("\nTo see the full description of the program, input ?help\n");
		exit(1);
	}
	exit(1);
}