void genkeys(int f_prime,int s_prime,char *name1,char *name2);
char encrypt(char msg, int e, int n,int j); //шифрование
char decrypt(int msg, int d, int n,int j); //дешифрование
int test_prime(); // тест на простоту Миллера-Рабина
