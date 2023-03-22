#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>



#define N 10000000
#define S (int)sqrt(N)
#define M N/10

#define CHUNKSIZE 100

int main(int argc, char**argv){
    omp_set_num_threads(argc > 1 ? atoi(argv[1]) : 2);
long int a[S + 1]; /*tablica pomocnicza*/
long int pierwsze[M]; /*liczby pierwsze w przedziale 2..N*/
long i, k, liczba, reszta;
long int lpodz; /* l. podzielnikow*/
long int llpier =0; /*l. liczb pierwszych w tablicy pierwsze*/
int chunk = CHUNKSIZE;
FILE *fp;
        struct timespec start, finish;
        double elapsed;
        clock_gettime(CLOCK_MONOTONIC, &start);


/*wyznaczanie podzielnikow z przedzialow 2..S*/
#pragma omp parallel for schedule(dynamic)
for(i=2; i<=S; i++){
    a[i] = 1; /*inicjowanie*/
}
for(i=2; i<=S; i++){
    	if(a[i] == 1){
            pierwsze[llpier++] = i; /*zapamietanie podzielnika*/
            /*wykreslanie liczb zlozonych bedacych wielokrotnosciami i*/
            for(k = i+i; k<=S; k+=i){
                a[k] = 0;
        }
	}
}

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


    printf("Czas wykonania pierwszej czesci: %f\n", elapsed);


lpodz = llpier; /*zapamietanie liczby podzielnikow*/
/*wyznaczanie liczb pierwszych*/
#pragma omp parallel shared(pierwsze, llpier, chunk) private(liczba, k, reszta)
{
    #pragma omp for schedule(dynamic,chunk) nowait
    
        for(liczba = S+1; liczba <=N; liczba++){
            for(k=0; k<lpodz; k++){
                reszta = (liczba % pierwsze[k]);
                if(reszta == 0) break; /*liczba zlozona*/
            }
            #pragma omp critical
            {
                if(reszta != 0)
                    pierwsze[llpier++] = liczba; /*zapamietanie liczby pierwszej*/
            }
        }
    

}



    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


    printf("Czas wykonania caly: %f\n", elapsed);





if((fp = fopen("primes.txt", "w")) == NULL)
{
	printf("Nie moge otworzyc pliku do zapisu\n");
	exit(1);
}
for(i=0; i< llpier; i++)
	fprintf(fp,"%ld ", pierwsze[i]);
fclose(fp);
printf("KONIEC: %ld\n",llpier);
return 0;
}
