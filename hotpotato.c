#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpi.h>


int nuevo_token(int token){
	
	int x,primer_token;
	x = rand() % token + 1;
	primer_token = token - x;

	return primer_token;

}

int cambiar_token(int token, int max){
	int new_token,y;
	y = rand() % max + 1;
	new_token = token - y;

	return new_token;
}

int main(int argc, char** argv) {
	int c,t;
	int index=0;
	srand (time(NULL));

	static struct option long_options[] = {
		{"Token",1,NULL,'t'},
		{0,0,0,0}
	};

	while(1){
		c = getopt_long(argc, argv, "t:",long_options,&index);

		if(c==-1){
			break;
		}

		switch(c){
			case 't':
				t = atoi(optarg);

			default:		
				break;
		}
	}
	MPI_Init(NULL, NULL);
  	int rank,size,token;
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);

  	token = nuevo_token(t);
  	int sigo_en_juego= 1;
  	int i = 100	;
  	
  	if (rank == 0){
  		printf("Proceso %d tiene la papa con valor %d\n",rank,token);
  		token = cambiar_token(token,t);
  		MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  	}
  	while (i>=10){
	  	if (rank == 0) {
	  		if(sigo_en_juego == 1){
	  			
	  				//printf("Espero desde %d a %d\n",size-1,rank );
		  			MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		  			//printf("Le recibo desde %d a %d \n",size-1,rank);
		  				if(token < 0){
		  					printf("Proceso %d tiene la papa con valor %d (proceso %d sale del juego)\n", rank, token,rank);
		  					token= nuevo_token(t);
		  					MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
		  			//		printf("Le envie desde %d a %d\n",rank, rank+1 );
		  					sigo_en_juego = 0;

		  				}

		  				else{
		    				printf("Proceso %d tiene la papa con valor %d\n",rank,token);
		    				token= cambiar_token(token,t);
		    				MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
		    		//		printf("Le envie desde %d a %d\n",rank, rank+1 );
		    		}
		    	
		    	
		    }
	  		else{
	  			// Si no sigo en juego, solamente paso el token
	  			//printf("soy el procesor %d y espero \n",rank );
	  			MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			//printf("Le recibo desde %d a %d\n",size-1, rank );
	  			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  			//printf("Le envie desde %d a %d\n",rank, rank+1 );
	  		}
	  	}
	  	

	  	if (rank != 0){
	  		if(sigo_en_juego == 1){
	  			
	  				//printf("Espero desde %d a %d\n",rank-1, rank );
	  				MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  				//printf("Le recibo desde %d a %d\n",rank-1, rank );
	  				if(token < 0){
	  					printf("Proceso %d tiene la papa con valor %d (proceso %d sale del juego)\n", rank, token,rank);
	  					token= nuevo_token(t);
	  					MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
	  				//	printf("Le envie desde %d a %d\n",rank, rank+1 );
	  					sigo_en_juego = 0;

	  				}

	  				else{
	    				printf("Proceso %d tiene la papa con valor %d\n",rank,token);
	    				token= cambiar_token(token,t);
	    				MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	    			//	printf("Le envie desde %d a %d\n",rank, rank+1 );
	    		}
	    	
	  		
	  	}

	  		else{
	  			// Si no sigo en juego, solamente paso el token
	  			//printf("soy el procesor %d y espero \n",rank );
	  			MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			//printf("soy el procesor %d y recibo \n",rank );
	  			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  			//printf("Le envie desde %d a %d\n",rank, rank+1 );
	  		}
	  	
  	}
  	i--;
  }
  	MPI_Finalize();
}