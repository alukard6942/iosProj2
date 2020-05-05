// proj2.c
// Milan Koval
// 23.04.2020

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <time.h>


#ifdef DEBUG
	#define DEBUGMSG(val, i) fprintf(stderr, "============%i: %s: %i\n",__LINE__, val,i)
	int value; 
	#define DEBUGSEM(sem) sem_getvalue(sem, &value);  DEBUGMSG(#sem, value)
#else
	#define DEBUGSEM(sem) 
	#define DEBUGMSG(val, i) 
#endif

// globalni promene vsech procesu mozna bychom mohli zabalit do strucktu


typedef struct{

	FILE *fileOut;

	int PI;
	int IG;
	int JG;
	int IT;
	int JT;


	int line;
	int NE;
	int NC;
	int NB;
	sem_t linePrint; 
	sem_t door;
	sem_t confirmation;
	sem_t check;
	sem_t checkedin;
	sem_t certificate;
	sem_t leave;

	int soudceFlag;
	
} shared_variables;


void *imigrant( void *ptr,int ID);
void *soudce (void *ptr);



int main (int argc, char * argv[]){

	// spravny pocet argumentu
	if ( argc < 2) return 1;

	// shared memmory hopfully
	shared_variables *data;		
	if((data = mmap(NULL, sizeof(shared_variables), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED){
		fprintf(stderr, "Mapping error!\n");
		return 2;
	}

	// get number of mexican
	data->PI = atoi(argv[1]);
	data->IG = atoi(argv[2]);
	data->JG = atoi(argv[3]);
	data->IT = atoi(argv[4]);
	data->JT = atoi(argv[5]);

	data->fileOut = fopen("proj2.out", "w");

	data->line = 0;
	data->NE = 0;
	data->NC =0;
	data->NB =0;

	// semafors
	sem_init(&data->linePrint,	1, 1);
	sem_init(&data->door, 		1, 1);
	sem_init(&data->confirmation,1,0); // starts on red light
	sem_init(&data->check, 		1, 1);
	sem_init(&data->checkedin,	1, 1);
	sem_init(&data->certificate,1, 1);
	sem_init(&data->leave,		1, 1);

	data->soudceFlag = 1;


	int p = fork();
	if(p < 0){
		fprintf(stderr, "%s\n","fork failed");
		return 2;
	}

	// chilld proces
	if ( p == 0){
		p = fork();
		if(p < 0){
			fprintf(stderr, "%s\n","fork failed");
			return 2;
		}

		// parent proces
		if( p > 0) {
			// generate emigrants
			for (int i = 1; i <= data->PI; i++){ 
				p = fork();
				if(p < 0){
					fprintf(stderr, "%s\n","fork failed");
					return 2;
				}

				// parent proces
				if( p > 0){ 
					if(data->IG ) usleep(rand() % data->IG );
					continue;
				}

				// child proces
				if ( p == 0){
	 				DEBUGMSG("proces",i);

	 				imigrant(data, i);

	 				DEBUGSEM(&data->leave);
	 				sem_post(&data->leave);
	 				DEBUGSEM(&data->leave);
	 				DEBUGMSG("proces leaves",i);
	 				break;
				} 
			}
		}
		else {
			// generate soudce
			DEBUGMSG("soudce",0);
			 while (data->soudceFlag){
				if(data->JG) usleep(rand()%data->JG);
				soudce(data);
			}
		}
	}
	else{

		for (int i = 1; i <= data->PI; i++){ 
			sem_wait(&data->leave);
			DEBUGSEM(&data->leave);
		}
		DEBUGMSG("after leave sem",0);

		data->soudceFlag = 0;

		//sem_destroy(&data->linePrint);
		sem_destroy(&data->door);
		sem_destroy(&data->confirmation);
		sem_destroy(&data->check);
		sem_destroy(&data->checkedin);
		sem_destroy(&data->certificate);
		sem_destroy(&data->leave);

	}

	// printf("========================END OF PROGRAM===============================\n");
	return 0;
}

void *imigrant( void *ptr, int ID){
	shared_variables *data = (shared_variables *) ptr;

	//sem_wait(&data->linePrint);
	fprintf(data->fileOut,"%i\t: IMG %i \t: starts: \n", ++data->line, ID);
	//sem_post(&data->linePrint);
	
	// vejdi do budovi
	sem_wait(&data->door);
	//sem_wait(&data->linePrint);
	fprintf(data->fileOut,"%i\t: IMG %i \t: enters:             \t: %i\t: %i\t: %i\n", ++data->line, ID, ++data->NE, data->NC, ++data->NB );
	//sem_post(&data->linePrint);
	sem_post(&data->door);
	

	// check in
	sem_wait(&data->check);
	//sem_wait(&data->linePrint);
	fprintf(data->fileOut,"%i\t: IMG %i \t: checks:             \t: %i\t: %i\t: %i\n", ++data->line, ID, data->NE, ++data->NC, data->NB );
	//sem_post(&data->linePrint);
	sem_post(&data->check);

	// imigrnat je dovolen vzit si certificate pouze kdyz to soudce dovali
	sem_post(&data->checkedin);
	sem_wait(&data->confirmation);

	// get to get certificate
	sem_wait(&data->certificate);
	//sem_wait(&data->linePrint);
	fprintf(data->fileOut,"%i\t: IMG %i \t: wants certificate:  \t: %i\t: %i\t: %i\n", ++data->line, ID, data->NE, data->NC, data->NB );
	//sem_post(&data->linePrint);

	// get certificate
	if(data->IT) usleep(rand()% data->IT);

	//sem_wait(&data->linePrint);
	fprintf(data->fileOut,"%i\t: IMG %i \t: got certificate:    \t: %i\t: %i\t: %i\n", ++data->line, ID, data->NE, data->NC, data->NB );
	//sem_post(&data->linePrint);
	sem_post(&data->certificate); 

	// leave
	//if(()%40) usleep(rand()%40);
	sem_wait(&data->door);
	//sem_wait(&data->linePrint);
	fprintf(data->fileOut,"%i\t: IMG %i \t: leaves:              \t: %i\t: %i\t: %i\n", ++data->line, ID, data->NE, data->NC, --data->NB );
	//sem_post(&data->linePrint);
	sem_post(&data->door);


	//sem_post(&data->leave);
	return ptr;
}

void *soudce (void *ptr){
shared_variables *data = (shared_variables *) ptr;
	//////////////////////
	//	soudce 			//
	//////////////////////

	//if(()%40) usleep(rand()%40);

	
	//sem_wait(&data->linePrint);
		fprintf(data->fileOut,"%i\t: JUDGE \t: wats to enter:\n", ++data->line);
	//sem_post(&data->linePrint);
	
	sem_wait(&data->door); // soudce vchazi do budovy
		//sem_wait(&data->linePrint);
		fprintf(data->fileOut,"%i\t: JUDGE \t: enters:              \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );
		//sem_post(&data->linePrint);
		//if(()%40) usleep(rand()%40);


		// wait for all inigransts
		if( data->NE != data->NC )
			fprintf(data->fileOut,"%i\t: JUDGE \t: waits for imm        \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );

		for(int i = 0; i < data->NE; i++){
			DEBUGMSG("NE", data->NE);
			DEBUGMSG("NC", data->NC);
			DEBUGSEM(&data->checkedin);
			sem_wait(&data->checkedin);
		}



		//sem_wait(&data->linePrint);
		fprintf(data->fileOut,"%i\t: JUDGE \t: starts confirmation  \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );
		//sem_post(&data->linePrint);
		
		if(data->JT ) usleep(rand()% data->JT );

		// confoirm NC imigrants
		while(data->NC > 0){ // for(int i = 0; i < data->NC; i++) nc chenges
			sem_post(&data->confirmation);
			DEBUGSEM(&data->confirmation);
			data->NE--;
			data->NC--;
		}

		fprintf(data->fileOut,"%i\t: JUDGE \t: ends confirmation   \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );
		
		


		//sem_wait(&data->linePrint);
		//fprintf(data->fileOut,"%i\t: JUDGE \t: jaf;lk:              \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );
		//sem_post(&data->linePrint);

		//sem_wait(&data->linePrint);
		//fprintf(data->fileOut,"%i\t: JUDGE \t: lefajdkl:              \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );
		//sem_post(&data->linePrint);
	


		//sem_wait(&data->linePrint);
		fprintf(data->fileOut,"%i\t: JUDGE \t: leaves:              \t: %i\t: %i\t: %i\n", ++data->line, data->NE, data->NC, data->NB );
		//sem_post(&data->linePrint);
	sem_post(&data->door);


	return ptr;
}