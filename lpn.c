#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define N 100000
#define S (int)sqrt(N)
#define M N/10
#define ITERATIONS 10

int main(int argc, char**argv){

	double total_time = 0;

	for (int it = 0 ; it < ITERATIONS ; it++)
    {
		long int a[S + 1]; /*tablica pomocnicza*/
		long int pierwsze[M]; /*liczby pierwsze w przedziale 2..N*/
		long i, k, liczba, reszta;
		long int lpodz; /* l. podzielnikow*/
		long int llpier =0; /*l. liczb pierwszych w tablicy pierwsze*/
		double czas; /*zmienna do  mierzenia czasu*/
		FILE *fp;


		struct timeval start,end;
		gettimeofday(&start,NULL);

		/*wyznaczanie podzielnikow z przedzialow 2..S*/
		for(i=2; i<=S; i++)
			a[i] = 1; /*inicjowanie*/
		for(i=2; i<=S; i++)
			if(a[i] == 1){
				pierwsze[llpier++] = i; /*zapamietanie podzielnika*/
				/*wykreslanie liczb zlozonych bedacych wielokrotnosciami i*/
			for(k = i+i; k<=S; k+=i) 
				a[k] = 0;
		}

		lpodz = llpier; /*zapamietanie liczby podzielnikow*/
		/*wyznaczanie liczb pierwszych*/
		
		for(liczba = S+1; liczba <=N; liczba++){
			for(k=0; k<lpodz; k++){
			reszta = (liczba % pierwsze[k]);
			if(reszta == 0) break; /*liczba zlozona*/
			}
			if(reszta != 0)
			pierwsze[llpier++] = liczba; /*zapamietanie liczby pierwszej*/
		}

		gettimeofday(&end,NULL);
		double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
		printf("Elapsed time: %f\n", elapsed_time);
		total_time += elapsed_time;

		if((fp = fopen("pn.txt", "w")) == NULL)
		{
			printf("Nie moge otworzyc pliku do zapisu\n");
			exit(1);
		}
		for(i=0; i< llpier; i++)
			fprintf(fp,"%ld ", pierwsze[i]);
		fclose(fp);
	}
	printf("Average time: %f\n", total_time/ITERATIONS);
	return 0;
}
