#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpi.h>
#include <time.h>

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
  	int rank,size,token,sigo_en_juego=1,cantidad_jugadores,i;
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);

  	token = rand()%t + 1;
  	cantidad_jugadores = size;  	

  	if (rank == 0){
  		printf("Proceso %d tiene la papa con valor %d\n",rank,token);
  		token = token - (rand()%t + 1);
  		MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  		MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  	}
  	
  	// 
  	while (cantidad_jugadores > 1){
  		if (rank == 0) {
  			if(sigo_en_juego == 1){
	  			
	  				
		  			MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		  			MPI_Recv(&cantidad_jugadores, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		  			
		  				if(token < 0){
		  					printf("Proceso %d tiene la papa con valor %d (proceso %d sale del juego)\n", rank, token,rank);
		  					token= rand()% t + 1;
		  					cantidad_jugadores--;
		  					
		  					MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
		  					MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
		  					
		  		
		  					sigo_en_juego = 0;

		  				}

		  				else{
		  					if(cantidad_jugadores == 1){
		  						printf("Proceso %d es el ganador\n", rank);
		  					}
		  					else{
		    					printf("Proceso %d tiene la papa con valor %d\n",rank,token);
		    				}
		    				token= token - (rand()%t + 1);
		    				MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
		    				MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
		    		
		    		}
		    	
		    	
		    }
		    // Si no sigo en juego, solamente paso el token
	  		else{
	  			
	  			MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			MPI_Recv(&cantidad_jugadores, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  			MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);	
	  		}
	  	}
	  	

	  	if (rank != 0){
	  		if(sigo_en_juego == 1){
	  			
	  				//printf("Espero desde %d a %d\n",rank-1, rank );
	  				MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  				MPI_Recv(&cantidad_jugadores, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  				//printf("Le recibo desde %d a %d\n",rank-1, rank );
	  				if(token < 0){
	  					printf("Proceso %d tiene la papa con valor %d (proceso %d sale del juego)\n", rank, token,rank);
	  					token= rand()%t +1;
	  					cantidad_jugadores=cantidad_jugadores-1;
	  					
	  					
	  					MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
	  					MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
	  				
	  				//	printf("Le envie desde %d a %d\n",rank, rank+1 );
	  					sigo_en_juego = 0;

	  				}

	  				else{
	  					if(cantidad_jugadores == 1){
		  						printf("Proceso %d es el ganador\n", rank);
		  					}
		  				else{
	    				printf("Proceso %d tiene la papa con valor %d\n",rank,token);
	    				}
	    				token=  token - (rand()%t + 1);
	    				MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	    				MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	    			
	    			//	printf("Le envie desde %d a %d\n",rank, rank+1 );
	    		}
	    	
	  		
	  	}

	  		else{
	  			// Si no sigo en juego, solamente paso el token
	  			//printf("soy el procesor %d y espero \n",rank );
	  			MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			MPI_Recv(&cantidad_jugadores, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			//printf("soy el procesor %d y recibo \n",rank );
	  			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  			MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  			//printf("Le envie desde %d a %d\n",rank, rank+1 );
	  			
	  		}
	  	
  	}
  	i--;
  }
  	MPI_Finalize();
}