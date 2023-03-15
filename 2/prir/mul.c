#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//#define DEBUG
#define TIME_MEASURE

double** init_matrix(int rows, int cols){
    double** result = malloc(sizeof(double*)*rows);
    for(int i = 0;i < rows; i++)
        result[i] = calloc(cols, sizeof(double));

    return result;
}

double** load_matrix_from_file(char* filename, int* rows, int* cols){
    double** result;
    FILE *fpa;

    fpa = fopen(filename, "r");
    if( fpa == NULL )
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }

    fscanf (fpa, "%d", rows);
    fscanf (fpa, "%d", cols);

    result = init_matrix(*rows, *cols);

    double x;
    for(int i = 0; i < *rows; i++)
    {
        for(int j = 0; j < *cols; j++)
        {
            fscanf( fpa, "%lf", &x );
            result[i][j] = x;
        }
    }
    fclose(fpa);
    return result;

}

void free_matrix(double** matrix, int rows){
    for(int i = 0; i < rows; i++){
        free(matrix[i]);
    }
    free(matrix);
}


void print_matrix(double**A, int m, int n)
{
    int i, j;
    printf("[\n");
    for(i =0; i< m; i++)
    {
        printf("[ ");
        for(j=0; j<n; j++)
        {
            printf("%f ", A[i][j]);
        }
        printf("]\n");
    }
    printf("]\n\n");
}

double** result_matrix;
double result_matrix_rows;
double result_matrix_cols;

typedef struct {
    int thread_id;
    int n_threads;

    double** A;
    int ra;
    int ca;

    double** B;
    int rb;
    int cb;

} assigment_t;

void multiply_matrixes(void *arg){
    assigment_t* assigment = (assigment_t*)arg;

    int start_row = assigment->thread_id * assigment-> ra / assigment->n_threads;
    int end_row = (assigment->thread_id + 1) * assigment-> cb /assigment->n_threads;

    double** A = assigment->A;
    double** B = assigment->B;

    double s=0;
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < assigment-> ca; j++) {
            s=0;
            for (int k = 0; k < assigment-> rb; k++) {
                #ifdef DEBUG
                    printf("%.0lf*%.0lf + ",A[i][k], B[k][j]);
                #endif
                result_matrix[i][j] += A[i][k]*B[k][j];
            }
            #ifdef DEBUG
                printf("\n");
            #endif
        }
        #ifdef DEBUG
            printf("\n");
        #endif
    }
    pthread_exit(NULL);
}

double** gen(int r, int c){
    double** result = init_matrix(r,c);
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            result[i][j] = i+j;
    
    return result;
}


int main(int argc, char** argv)
{
    int n_threads = argc > 1 ? atoi(argv[1]) : 3;

    pthread_t threads[n_threads];
    assigment_t assigments[n_threads];

    int ra=1000,ca=1000,rb=1000,cb=1000;
    //  double** A = load_matrix_from_file("AA.txt",&ra,&ca);
    //  double** B = load_matrix_from_file("BB.txt",&rb,&cb);

    double** A = gen(ra,ca);
    double** B = gen(rb,cb);

    result_matrix = init_matrix(ra,cb);
    result_matrix_rows = ra;
    result_matrix_cols = cb;

    printf("%d wątków\n",n_threads);
    //print_matrix(B,rb,cb);
    //printf("Printed\n");

        struct timespec start, finish;
        double elapsed;
        clock_gettime(CLOCK_MONOTONIC, &start);


    for (int i = 0; i < n_threads; i++) {
        #ifdef DEBUG
            printf("Preparing\n");
        #endif
        assigments[i].thread_id = i;
        assigments[i].n_threads = n_threads;
        assigments[i].A = A;
        assigments[i].ra = ra;
        assigments[i].ca = ca;
        assigments[i].B = B;
        assigments[i].rb = rb;
        assigments[i].cb = cb;
        #ifdef DEBUG
            printf("Launch thread %d\n",i);
        #endif
        if (pthread_create(&threads[i], NULL, (void *)multiply_matrixes, (void *)&assigments[i])) {
            printf("ERROR; return code from pthread_create() is\n");
            exit(-1);
        }
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }


    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


    printf("Czas wykonania: %f\n", elapsed);

    #ifdef DEBUG
        printf("Wynik:\n");
        print_matrix(result_matrix,result_matrix_rows,result_matrix_cols);
    #endif

    free_matrix(A,ra);
    free_matrix(B,rb);
    free_matrix(result_matrix,result_matrix_rows);


    return 0;
}