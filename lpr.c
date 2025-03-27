#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define N 10000000
#define S (int)sqrt(N)
#define M N/10
#define ITERATIONS 10

int compare (const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

int main(int argc, char**argv){

    // WSTEPNE INICJALIZOWANIE 

    double total_time = 0;

    for (int it = 0 ; it < ITERATIONS ; it++)
    {
            // WCZYTYWANIE ARG
        int num_threads = -1;
        if(argc != 2)
        {
            printf("Usage: %s <num_threads>\n", argv[0]);
            return 1;
        }
        num_threads = atoi(argv[1]);
        if (num_threads <= 0)
        {
            fprintf(stderr,"Invalid number of threads, has to be a positive integer\n");
            return 1;
        }


        long int a[S + 1]; /*tablica pomocnicza*/
        long int pierwsze[M]; /*liczby pierwsze w przedziale 2..N*/
        long int lpodz; /* l. podzielnikow*/
        long int llpier =0; /*l. liczb pierwszych w tablicy pierwsze*/
        FILE *fp;

        //DO POMIARU CZASU RZECZYWISTEGO
        struct timeval start,end;
        gettimeofday(&start,NULL);

        //INICJALIZACJA TABLICY SITA
        omp_set_num_threads(num_threads);
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            int nthreads = omp_get_num_threads();
            if (id == 0) num_threads = nthreads;

            for(long i=2+id; i<=S; i+=num_threads)
            {
                a[i] = 1; 
            }

        }  

        // WYZNACZANIE DZIELNIKOW Z PRZEDZIALU 2..S
        for(long i=2; i<=S; i++)
        {
            if(a[i] == 1)
            {
                pierwsze[llpier++] = i; /*zapamietanie podzielnika*/

                /*wykreslanie liczb zlozonych bedacych wielokrotnosciami i*/
                #pragma omp parallel
                {
                    int id = omp_get_thread_num();
                    // dla 3 watkow i zalozmy a[5] = 1 wykreslamy 10,15,20 w pierwszej iteracji watkow potem 25,30,35 itd
                    for(long j = i*(id+2); j<=S; j+=num_threads*i) 
                    {
                        a[j] = 0;
                    }
                }
            }
        }

        #pragma omp barrier

        lpodz = llpier; /*zapamietanie liczby podzielnikow*/


        /*wyznaczanie liczb pierwszych*/
        #pragma omp parallel
        {
            int id = omp_get_thread_num();

            int liczba;
            //ta sama metoda jak wyzej w wyznaczaniu dzielnikow
            //kazdy watek idzie swoim tokiem liczb S+1+id, S+1+id+num_threads, S+1+id+2*num_threads itd
            //to pokrywa calosc
            for(liczba = S+1+id; liczba <= N; liczba+=num_threads)
            {
                int reszta;
                for(int k = 0 ; k < lpodz ;k++)
                {
                    reszta = liczba%pierwsze[k];
                    if(reszta == 0) break; 
                }
                #pragma omp critical
                {
                    if (reszta != 0)
                    {
                        pierwsze[llpier++] = liczba;
                    }
                }
            }
        }

        

        //najszybszy sort?
        qsort(pierwsze, llpier, sizeof(long int), compare);

        // POMIAR CZASUS
        gettimeofday(&end,NULL);
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        total_time += elapsed_time;
        printf("Elapsed time: %f\n", elapsed_time);

        if((fp = fopen("pr.txt", "w")) == NULL)
        {
            printf("Nie moge otworzyc pliku do zapisu\n");
            exit(1);
        }
        for(long i=0; i< llpier; i++)
            fprintf(fp,"%ld ", pierwsze[i]);
        fclose(fp);

    }
    printf("Average time: %f\n", total_time/ITERATIONS);
  
    
  
   
    return 0;
}
