#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
using namespace std;

#ifdef __alpha
typedef unsigned int UINT4;
#else
typedef unsigned long int UINT4;
#endif
 
#define MD5_INIT_STATE_0 0x67452301
#define MD5_INIT_STATE_1 0xefcdab89
#define MD5_INIT_STATE_2 0x98badcfe
#define MD5_INIT_STATE_3 0x10325476
 
void MD5Init(void);
void MD5Update(unsigned char *bug, unsigned int len);
void MD5Final(char* cReturnStr);
void Transform(UINT4 *buf, UINT4 *in);
 
unsigned char  m_lpszBuffer[64];
unsigned long m_nCount[2];
unsigned long m_lMD5[4];
 
static unsigned char PADDING[64] = {
 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
 
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
 
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
 
#define FF(a, b, c, d, x, s, ac) \
 {(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
#define GG(a, b, c, d, x, s, ac) \
 {(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
#define HH(a, b, c, d, x, s, ac) \
 {(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
#define II(a, b, c, d, x, s, ac) \
 {(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
 
void ByteToDWord(DWORD* Output, ULONG* Input, UINT nLength){
UINT i=0;
UINT j=0;
for (; j < nLength; i++, j += 4)
{
 Output[i] = (ULONG)Input[j]   |
    (ULONG)Input[j+1] << 8 |
    (ULONG)Input[j+2] << 16 |
    (ULONG)Input[j+3] << 24;
}
}
 
void DWordToByte(unsigned char* Output, DWORD* Input, UINT nLength ){
UINT i = 0;
UINT j = 0;
for (; j < nLength; i++, j += 4)
{
 Output[j] =   (UCHAR)(Input[i] & 0xff);
 Output[j+1] = (UCHAR)((Input[i] >> 8) & 0xff);
 Output[j+2] = (UCHAR)((Input[i] >> 16) & 0xff);
 Output[j+3] = (UCHAR)((Input[i] >> 24) & 0xff);
}
}
 
void MD5Init (void)
{
 memset(m_lpszBuffer, 0, 64 );
 m_nCount[0] = m_nCount[1] = 0;
 
m_lMD5[0] = MD5_INIT_STATE_0;
m_lMD5[1] = MD5_INIT_STATE_1;
m_lMD5[2] = MD5_INIT_STATE_2;
m_lMD5[3] = MD5_INIT_STATE_3;
}
 
void MD5Update (unsigned char *inBuf, unsigned int inLen)
{
 register int i, ii;
 int mdi;
 UINT4 in[16];
 
 mdi = (int)((m_nCount[0] >> 3) & 0x3F);
 
 if ((m_nCount[0] + ((UINT4)inLen << 3)) < m_nCount[0])
   m_nCount[1]++;
 m_nCount[0] += ((UINT4)inLen << 3);
 m_nCount[1] += ((UINT4)inLen >> 29);
 
 while (inLen--) {
   m_lpszBuffer[mdi++] = *inBuf++;
 
   if (mdi == 0x40) {
     for (i = 0, ii = 0; i < 16; i++, ii += 4)
       in[i] = (((UINT4)m_lpszBuffer[ii+3]) << 24) |
               (((UINT4)m_lpszBuffer[ii+2]) << 16) |
               (((UINT4)m_lpszBuffer[ii+1]) << 8) |
               ((UINT4)m_lpszBuffer[ii]);
     Transform (m_lMD5, in);
     mdi = 0;
   }
 }
}
 
void MD5Final (char* cReturnStr)
{
unsigned char bits[8];
int nIndex;
unsigned int nPadLen;
const int nMD5Size = 16;
 unsigned char lpszMD5[16];
char temp[16];
int i;
 
cReturnStr[0]='\0';
 
DWordToByte( bits, m_nCount, 8 );
 
nIndex = (int)((m_nCount[0] >> 3) & 0x3f);
nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
MD5Update (PADDING, nPadLen);
 
MD5Update (bits, 8);
 
DWordToByte( lpszMD5, m_lMD5, nMD5Size );
 
for (i=0; i < nMD5Size; i++)
{
 
 if (lpszMD5[i] == 0) {
  temp[0] = '0';
  temp[1]='0';
 }
 else if (lpszMD5[i] <= 15)  {
     sprintf(temp,"0%x",lpszMD5[i]);
 }
 else {
     sprintf(temp,"%x",lpszMD5[i]);
 }
   strcat(cReturnStr,temp);
}
memset(temp, 0, sizeof(temp));
lpszMD5[0]='\0';
}
 
void Transform(register UINT4 *buf,register UINT4 *in)
{
 register UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];
 
#define S11 7
#define S12 12
#define S13 17
#define S14 22
 FF ( a, b, c, d, in[ 0], S11, 0xD76AA478L);
 FF ( d, a, b, c, in[ 1], S12, 0xE8C7B756L);
 FF ( c, d, a, b, in[ 2], S13, 0x242070DBL);
 FF ( b, c, d, a, in[ 3], S14, 0xC1BDCEEEL);
 FF ( a, b, c, d, in[ 4], S11, 0xF57C0FAFL);
 FF ( d, a, b, c, in[ 5], S12, 0x4787C62AL);
 FF ( c, d, a, b, in[ 6], S13, 0xA8304613L);
 FF ( b, c, d, a, in[ 7], S14, 0xFD469501L);
 FF ( a, b, c, d, in[ 8], S11, 0x698098D8L);
 FF ( d, a, b, c, in[ 9], S12, 0x8B44F7AFL);
 FF ( c, d, a, b, in[10], S13, 0xFFFF5BB1L);
 FF ( b, c, d, a, in[11], S14, 0x895CD7BEL);
 FF ( a, b, c, d, in[12], S11, 0x6B901122L);
 FF ( d, a, b, c, in[13], S12, 0xFD987193L);
 FF ( c, d, a, b, in[14], S13, 0xA679438EL);
 FF ( b, c, d, a, in[15], S14, 0x49B40821L);
 
#define S21 5
#define S22 9
#define S23 14
#define S24 20
 GG ( a, b, c, d, in[ 1], S21, 0xF61E2562L);
 GG ( d, a, b, c, in[ 6], S22, 0xC040B340L);
 GG ( c, d, a, b, in[11], S23, 0x265E5A51L);
 GG ( b, c, d, a, in[ 0], S24, 0xE9B6C7AAL);
 GG ( a, b, c, d, in[ 5], S21, 0xD62F105DL);
 GG ( d, a, b, c, in[10], S22, 0x02441453L);
 GG ( c, d, a, b, in[15], S23, 0xD8A1E681L);
 GG ( b, c, d, a, in[ 4], S24, 0xE7D3FBC8L);
 GG ( a, b, c, d, in[ 9], S21, 0x21E1CDE6L);
 GG ( d, a, b, c, in[14], S22, 0xC33707D6L);
 GG ( c, d, a, b, in[ 3], S23, 0xF4D50D87L);
 GG ( b, c, d, a, in[ 8], S24, 0x455A14EDL);
 GG ( a, b, c, d, in[13], S21, 0xA9E3E905L);
 GG ( d, a, b, c, in[ 2], S22, 0xFCEFA3F8L);
 GG ( c, d, a, b, in[ 7], S23, 0x676F02D9L);
 GG ( b, c, d, a, in[12], S24, 0x8D2A4C8AL);
 
#define S31 4
#define S32 11
#define S33 16
#define S34 23
 HH ( a, b, c, d, in[ 5], S31, 0xFFFA3942L);
 HH ( d, a, b, c, in[ 8], S32, 0x8771F681L);
 HH ( c, d, a, b, in[11], S33, 0x6D9D6122L);
 HH ( b, c, d, a, in[14], S34, 0xFDE5380CL);
 HH ( a, b, c, d, in[ 1], S31, 0xA4BEEA44L);
 HH ( d, a, b, c, in[ 4], S32, 0x4BDECFA9L);
 HH ( c, d, a, b, in[ 7], S33, 0xF6BB4B60L);
 HH ( b, c, d, a, in[10], S34, 0xBEBFBC70L);
 HH ( a, b, c, d, in[13], S31, 0x289B7EC6L);
 HH ( d, a, b, c, in[ 0], S32, 0xEAA127FAL);
 HH ( c, d, a, b, in[ 3], S33, 0xD4EF3085L);
 HH ( b, c, d, a, in[ 6], S34, 0x04881D05L);
 HH ( a, b, c, d, in[ 9], S31, 0xD9D4D039L);
 HH ( d, a, b, c, in[12], S32, 0xE6DB99E5L);
 HH ( c, d, a, b, in[15], S33, 0x1FA27CF8L);
 HH ( b, c, d, a, in[ 2], S34, 0xC4AC5665L);
 
#define S41 6
#define S42 10
#define S43 15
#define S44 21
 II ( a, b, c, d, in[ 0], S41, 0xF4292244L);
 II ( d, a, b, c, in[ 7], S42, 0x432AFF97L);
 II ( c, d, a, b, in[14], S43, 0xAB9423A7L);
 II ( b, c, d, a, in[ 5], S44, 0xFC93A039L);
 II ( a, b, c, d, in[12], S41, 0x655B59C3L);
 II ( d, a, b, c, in[ 3], S42, 0x8F0CCC92L);
 II ( c, d, a, b, in[10], S43, 0xFFEFF47DL);
 II ( b, c, d, a, in[ 1], S44, 0x85845DD1L);
 II ( a, b, c, d, in[ 8], S41, 0x6FA87E4FL);
 II ( d, a, b, c, in[15], S42, 0xFE2CE6E0L);
 II ( c, d, a, b, in[ 6], S43, 0xA3014314L);
 II ( b, c, d, a, in[13], S44, 0x4E0811A1L);
 II ( a, b, c, d, in[ 4], S41, 0xF7537E82L);
 II ( d, a, b, c, in[11], S42, 0xBD3AF235L);
 II ( c, d, a, b, in[ 2], S43, 0x2AD7D2BBL);
 II ( b, c, d, a, in[ 9], S44, 0xEB86D391L);
 
 buf[0] += a;
 buf[1] += b;
 buf[2] += c;
 buf[3] += d;
}
 
void GetMD5(char* pBuf, UINT nLength,char* cReturnStr){
MD5Init();
MD5Update((unsigned char *)pBuf, nLength);
MD5Final(cReturnStr);
}

int main(int argc, char *argv[])
{
	int f_prime=1,s_prime=1,en=0,chet=0,n,e=1,d=0,dn=0;
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
		printf("\n  gen public.txt private.txt - writes the public exponent (e) to file public.txt, closed (d) - a file private.txt\n");
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

			int j=0;

			if (a == NULL || publ == NULL) {
				printf("File not found\n");
				exit(1);
			}

			while (!feof(a)) {

				if (fread(&msg[j], 1, 1, a)>0) {
					fscanf(publ,"%d",&en);
					enc_msg[j] = encrypt(msg,en,n,j);
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

			int j=0;

			if (out == NULL || priv == NULL) {
				printf("File not found\n");
				exit(1);
			}
			
			while (!feof(out)) {
			if (fread(&enc_msg[j],4, 1, out)>0) {
				fscanf(priv,"%d",&dn);
				msg[j] = decrypt(enc_msg,dn,n,j);
				fwrite(&msg[j++],1,1,b);
			}
			}
			fclose(out);fclose(b);fclose(priv);
	}
	if (strcmp(argv[12], "md") == 0) { // enc a.txt out.txt public.txt

	//			тест MD5

	char* in="kfgkdfgrdsmfgr";
	char out[33] = {0};
	cout<<in<<endl;
	GetMD5(in,strlen(in),out);
	cout<<"The md5 of yout text is - "<<out<<endl;

	int j = 0;
	int i = 0;

	memset(msg, 0, sizeof(msg));

	for(i; i < 100; i++){
		if(out[i] > 0){
		msg[i] = decrypt_hash(out,dn,n,j);
		j++;
		}
	}
	msg[i++] = '\0';

	cout<<"\n\nThe digital signature - "<< msg <<endl;

	}

	system("pause");
}