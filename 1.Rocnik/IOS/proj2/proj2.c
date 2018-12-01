#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//Fork + uSleep
#include <unistd.h>
//Semaphore
#include <semaphore.h>

#include <sys/wait.h>
#include <sys/mman.h>

//random
#include <time.h>

//Makrá pre uvoľnovanie pamate
#define UNMAPVARIABLES	\
	do { \
		munmap(N, sizeof(int)); \
		munmap(I, sizeof(int)); \
		munmap(A, sizeof(int)); \
	} while (0)
						

#define DESTROYSEMS	\
	do { \
		sem_destroy(sem_printer);		\
		sem_destroy(sem_boarding);		\
		sem_destroy(sem_unboarding);	\
		sem_destroy(sem_lastboarded);	\
		sem_destroy(sem_lastunboarded);	\
		sem_destroy(sem_allfinished);	\
		sem_destroy(sem_mainfinished);	\
	} while (0)

#define UNMAPSEMS \
	do { \
		munmap(sem_printer, sizeof(sem_t));			\
		munmap(sem_boarding, sizeof(sem_t));		\
		munmap(sem_unboarding, sizeof(sem_t));		\
		munmap(sem_lastboarded, sizeof(sem_t));		\
		munmap(sem_lastunboarded, sizeof(sem_t));	\
		munmap(sem_allfinished, sizeof(sem_t));		\
		munmap(sem_mainfinished, sizeof(sem_t));	\
	} while (0)

#define CLOSEFILE \
	do { \
		if (fclose(f) == EOF)	\
		{	\
			fprintf(stderr, "%s\n", "Error: Closing file");	\
		}	\
	} while (0)

//Check if char * is digit number >= 0
int IsDigit(char *v)
{
	int i = 0;
	while (v[i] != '\0') i++;

	for (int j = 0; j < i; j++)
	{
		if (v[j] > 57 || v[j] < 48)
			return 0;
        }

	return 1;
}

int main(int argc, char **argv)
{	
	int P;		//Pocet procesov reprezenntujuci pasaziera
	int C;		//Kapacita voziku
	int PT;		//Max doba(ms), pre ktory je generovany novy proces pre pasaziera
	int RT;		//Max doba(ms), prejazdu trate

	//Otvorenie suboru na zapis vypisu procesov
	FILE *f = fopen("proj2.out", "wb");
	if (f == NULL)
	{
		fprintf(stderr, "%s\n", "Error: Opening file");
		exit(2);
	}

	//Cistenie buffera, kvoli bitiu sa procesov pri vypise
	setbuf(f, NULL); 
	setbuf(stderr, NULL);

	//Overenie vstupnych podmienok (argumentov)
	if (argc == 5)
	{
		if (IsDigit(argv[1]) && IsDigit(argv[2]) && IsDigit(argv[3]) && IsDigit(argv[4]))
		{
			P = atoi(argv[1]);
			C = atoi(argv[2]);
			PT = atoi(argv[3]);
			RT = atoi(argv[4]);

			if (P > 0 && C > 0 && P > C && (P % C == 0))
			{
				if (PT < 0 || PT >= 5001 || RT < 0 || RT >= 5001)
				{
					CLOSEFILE;

					fprintf(stderr, "%s\n", "Error: Parameters PT or RT have invalid value");
					exit(1);
				}
			} else
			{
				CLOSEFILE;
				
				fprintf(stderr, "%s\n", "Error: Parameters P or C have invalid value");
				exit(1);
			}
		} else
		{
			CLOSEFILE;
			
			fprintf(stderr, "%s\n", "Error: One of the arguments have wrong format");
			exit(1);
		}

	} else
	{
		CLOSEFILE;
		
		fprintf(stderr, "%s\n", "Error: Wrong number of arguments");
		exit(1);
	}

	//-----------------------------PROGRAM--------------------------------------
	//Inicializacia premennych a semaforov
	int mainPid;
	int pasPid;
	int carPid;

	sem_t *sem_printer;
	sem_t *sem_boarding;
	sem_t *sem_unboarding;

	sem_t *sem_lastboarded;
	sem_t *sem_lastunboarded;
	sem_t *sem_allfinished;

	sem_t *sem_mainfinished;

	sem_printer = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_boarding = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_unboarding = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_lastboarded = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_lastunboarded = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_allfinished = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_mainfinished = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	sem_init(sem_printer, 1, 1);
	sem_init(sem_boarding, 1, 0);
	sem_init(sem_unboarding, 1, 0);
	sem_init(sem_lastboarded, 1, 0);
	sem_init(sem_lastunboarded, 1, 0);
	sem_init(sem_allfinished, 1, 0);
	sem_init(sem_mainfinished, 1, 0);

	//Inicializacia zdielanej pamate
	int *N;
	int *I;
	int *A;
	
	N = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	I = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	A = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	*N = 1;
	*I = 0;
	*A = 1;

	//Fork main procesu pre vytvorenie subprocessu
	if ((mainPid = fork()) < 0)
	{
		//Unmap pamate
		UNMAPVARIABLES;

		//Znicenie semaforov
		DESTROYSEMS;

		//Unmapnutie semaforov z pamate
		UNMAPSEMS;

		//Zavretie vystupneho suboru
		CLOSEFILE;

		fprintf(stderr, "%s\n", "Error: fork main process");
		exit(2);
	}

	if (mainPid == 0)	//Subprocess pre generovanie pasazierov
	{
		for (int k = 0; k < P; k++)
		{	
			//Sleep pre pasaziera
			if (PT != 0)
				usleep(rand() % PT);

			//Vytvorenie pasaziera a osetrenie error-u
			if ((pasPid = fork()) < 0)
			{
				//Unmap pamate
				UNMAPVARIABLES;

				//Znicenie semaforov
				DESTROYSEMS;

				//Unmapnutie semaforov z pamate
				UNMAPSEMS;

				//Zavretie vystupneho suboru
				CLOSEFILE;

				fprintf(stderr, "%s\n", "Error: fork subprocess for passengers");
				exit(2);
			}
			
			//Ak je child == pasazier
			if (pasPid == 0)
			{
				//ID kazdeho pasaziera
				*I = *I + 1;
				int id = *I;

				//Start pasaziera a cakanie na vozik
				sem_wait(sem_printer);
				fprintf(f, "%d %s %d %s\n", *A, ": P ", id, ": started");
				*A += 1;
				sem_post(sem_printer);

				sem_wait(sem_boarding);		//post CAR

				//Zacatie nastupovania na vozik
				sem_wait(sem_printer);
				fprintf(f, "%d %s %d %s\n", *A, ": P ", id, ": board");
				*A += 1;
				//sem_post(sem_printer);

				//Vypis uspesneho nastupenia na vozik
				if (*N == C)
				{
					//sem_wait(sem_printer);
					fprintf(f, "%d %s %d %s\n", *A, ": P ", id, ": board last");
					*A += 1;
					*N = 1;
					sem_post(sem_printer);

					//Semafor pre vozik, ze je vozik plny a moze ist na cestu
					sem_post(sem_lastboarded);
				} else
				{
					//sem_wait(sem_printer);
					fprintf(f, "%d %s %d %s %d\n", *A, ": P ", id, ": board order ", *N);
					*A += 1;
					*N += 1;
					sem_post(sem_printer);
				}

				//Cakanie pasazierov na vystupovanie z vozika 
				//Caka kym vozik urobi jazdu
				sem_wait(sem_unboarding);

				//Zaciatok vystupovanie z vozika
				sem_wait(sem_printer);
				fprintf(f, "%d %s %d %s\n", *A, ": P ", id, ": unboard");
				*A += 1;
				//sem_post(sem_printer);

				if (*N == C)
				{
					//sem_wait(sem_printer);
					fprintf(f, "%d %s %d %s\n", *A, ": P ", id, ": unboard last");
					*A += 1;
					*N = 1;
					sem_post(sem_printer);

					//Semafor pre vozik, vsetci pasazieri vystupili von
					sem_post(sem_lastunboarded);
				} else
				{
					//sem_wait(sem_printer);
					fprintf(f, "%d %s %d %s %d\n", *A, ": P ", id, ": unboard order ", *N);
					*A += 1;
					*N += 1;
					sem_post(sem_printer);
				}

				//Kazdy pasazier caka kym sa vsetci "povozia" a mozu sa ukoncit
				sem_wait(sem_allfinished);

				sem_wait(sem_printer);
				fprintf(f, "%d %s %d %s\n", *A, ": P ", id, ": finished");
				*A += 1;
				sem_post(sem_printer);

				//Semafor pre hlavny proces, aby sa ukoncil ako posledny
				sem_post(sem_mainfinished);

				exit(0);
			}
		}

		exit(0);
	}

	//Fork main procesu pre vytvorenie vozika
	if ((carPid = fork()) < 0)
	{
		//Unmap pamate
		UNMAPVARIABLES;

		//Znicenie semaforov
		DESTROYSEMS;

		//Unmapnutie semaforov z pamate
		UNMAPSEMS;

		//Zavretie vystupneho suboru
		CLOSEFILE;

		fprintf(stderr, "%s\n", "Error: fork main process");
		exit(2);
	}

	//Subprocess pre vozik
	if (carPid == 0)
	{	
		//Inicializacia vozika
		sem_wait(sem_printer);
		fprintf(f, "%d %s\n", *A, ": C  1 : started");
		*A += 1;
		sem_post(sem_printer);

		//Cyklus pre "vozenie" pasazierov po trati
		for (int k = 0; k < P/C; k++)
		{	
			//Zaciatok nastupovanie pasazierov
			sem_wait(sem_printer);
			fprintf(f, "%d %s\n", *A, ": C  1 : load");
			*A += 1;
			sem_post(sem_printer);

			//Povolenie pasazierom nastupovat
			for (int j = 0; j < C; j++)
				sem_post(sem_boarding);

			//Vozik caka, kym sa vsetci nastupia
			sem_wait(sem_lastboarded);

			//Vozik sa pusta na jazdu
			sem_wait(sem_printer);
			fprintf(f, "%d %s\n", *A, ": C  1 : run");
			*A += 1;
			sem_post(sem_printer);
			
			//Sleep -> jazda vozikom
			if (RT != 0)
				usleep(rand() % RT);

			//Vozik dojazdil, zacina vystupovanie posazierov
			sem_wait(sem_printer);
			fprintf(f, "%d %s\n", *A, ": C  1 : unload");
			*A += 1;
			sem_post(sem_printer);

			//Vozik povoluje pasazierom vystupovat
			for (int j = 0; j < C; j++)
				sem_post(sem_unboarding);

			//Vozik caka kym vystupia vsetci pasazieri
			sem_wait(sem_lastunboarded);
		}

		//Vsetci pasazieri sa povozili, ukonceniev vsetkych procesov na koniec (okrem hlavne procesu)
		for (int j = 0; j < P; j++)
		{
			sem_post(sem_allfinished);
		}

		sem_wait(sem_printer);
		fprintf(f, "%d %s\n", *A, ": C  1 : finished");
		*A += 1;
		sem_post(sem_printer);
		
		//Semafor pre hlavny proces, aby sa ukoncil ako posledny
		sem_post(sem_mainfinished);

		exit(0);
	}

	//Hlavny proces caka na ukoncenie vsetkych pasazierov + vozika
	for (int k = 0; k < P + 1; k++)
		sem_wait(sem_mainfinished);

	//Ukoncenie hlavneho procesu + cistenie pamati
	//Unmap pamate
	UNMAPVARIABLES;

	//Znicenie semaforov
	DESTROYSEMS;

	//Unmapnutie semaforov z pamate
	UNMAPSEMS;

	//Zavretie vystupneho suboru
	CLOSEFILE;

	//Koniec procesu
	exit(0);
}
