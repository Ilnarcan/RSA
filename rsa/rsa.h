void genkeys(int f_prime,int s_prime,char *name1,char *name2);
int encrypt(char msg[], int e, int n,int j); //����������
int decrypt(int msg[], int d, int n,int j); //������������
int test_prime(); // ���� �� �������� �������-������
int decrypt_hash(char msg[], int d, int n,int j); //������������