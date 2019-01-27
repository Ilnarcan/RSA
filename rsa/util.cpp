#include <stdlib.h>

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

