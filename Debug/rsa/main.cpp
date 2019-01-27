#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
 

int main(int argc, char *argv[])
{
	int f_prime=1,s_prime=1,en=0,chet=0,n,e=1,d=0,dn=0;
	char msg[100];
	int enc_msg[100];
	string comm;
	char ens;

	printf("                             RSA system\n");

	while(1){

	cout << "\nInput command\n\n";
	cin >> comm;

	if (comm == "gen") {			// gen public.txt private.txt
		
		printf("generation of two prime numbers\n");

		while(f_prime == 1)
			f_prime = test_prime();

		while(s_prime == 1)
			s_prime = test_prime();

		printf("numbers generated\n");
		printf("first_prime=%d, ",f_prime);
		printf("second_prime=%d, ",s_prime);
		printf("n=%d \n",n=f_prime*s_prime);

		genkeys(f_prime,s_prime,"public.txt","private.txt");
	}

	else if(comm == "help") {
		printf("\n  gen pubic.txt private.txt - writes the public exponent (e) to file public.txt, closed (d) - a file private.txt\n");
		printf("\n  enc a.txt out.txt public.txt - encrypts data from the file a.txt with a key in public.txt; code would be stored out.txt\n");
		printf("\n  dec out.txt b.txt private.txt - decrypts the data from out.txt with the key in private.txt; puts them in a file b.txt\n");
		exit(1);
	}

	if (comm == "enc"){ // enc a.txt out.txt public.txt

			char s;
			char encc;

			fstream  a("a.txt");
			fstream  publ("public.txt");
			fstream  out("out.txt");

			int j=0;
			publ >> en;

				while (a.get(s) > 0) {
					encc = encrypt(s,en,n,j);
					out << encc;
					chet++;
				}
		}

	if (comm == "dec") { // dec out.txt b.txt private.txt

			char s;
			char dencc;
			
			fstream  b("b.txt");
			fstream  priv("private.txt");
			fstream  out("out.txt");

			int j=0;
			priv >> dn;

			while (out.get(s) > 0) {
				dencc = decrypt(s,dn,n,j);
				b << dencc;
			}
	}

	if (comm == "exit") { // dec out.txt b.txt private.txt

		break;
	}

	}
}