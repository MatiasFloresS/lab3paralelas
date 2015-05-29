#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv) {
	int c,t;
	int index=0;

	// Se crea la estructura la cual sera utilizada para el get_opt y asi obtener los datos por terminal
	static struct option long_options[] = {
		{"Token",1,NULL,'t'},
		{0,0,0,0}
	};
	// Para este caso solo se saca la informacion t, que seria el tiempo que dura el juego 
	// cuando se inicia cada vez que sale una persona
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
	// Inicializo y creo el comunicador para MPI
	MPI_Init(NULL, NULL);
	// Se define el rank, el size, el token, si sigo o no en juego y la cantidad de jugadores que quedan jugando
  	int rank,size,token,sigo_en_juego=1,cantidad_jugadores,i;
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);
  	// Asigno la semilla el cual para cada proceso sera distinto debido a que el tiempo se le suma el rank
  	srand (time(NULL) + rank);
  	// genero el token aleatorio
  	token = rand()%t + 1;
  	// cantidad de jugadores se utilizara para el terminar el juego
  	cantidad_jugadores = size;  	

  	// Proceso 0 entra
  	if (rank == 0){
  		// Envia el primer token al proceso de la derecha
  		printf("Proceso %d tiene la papa con valor %d\n",rank,token);
  		token = token - (rand()%t + 1);
  		// Se envia el token y la cantidad de jugadores que se ira modificando a medida que salgan los jugadores
  		MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  		MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  	}

  	// Si la cantidad de jugadores es mayor que 1, significa que quedan jugadores jugando
  	while (cantidad_jugadores > 1){
  		// Entra el proceso 0
  		if (rank == 0) {
  			// Si sigo en juego entro
  			if(sigo_en_juego == 1){
  				// Recibo el token y la cantidad de jugadores
  				MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		  		MPI_Recv(&cantidad_jugadores, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		  		
		  		// Si el token es menor que 0 entro al else
		  		if(token < 0){
		  			// El proceso sale del juego y se calcula un nuevo token
		  			printf("Proceso %d tiene la papa con valor %d (proceso %d sale del juego)\n", rank, token,rank);
		  			token= rand()% t + 1;
		  			// Decrementamos la cantidad de jugadores
		  			cantidad_jugadores--;
					// Envio el token y la cantidad de jugadores hacia la derecha		  					
		  			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
		  			MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
		  			// El proceso rank deja de jugar
		  			sigo_en_juego = 0;
		  		}
		  		// Entro al else si el token es negativo
		  		else{
		  			// Si la cantidad de jugadores es 1, rank es el ganador
		  			if(cantidad_jugadores == 1){
		  				printf("Proceso %d es el ganador\n", rank);
		  			}
		  			// Sino imprimo el valor de la papa del proceso rank
		  			else{
		  				printf("Proceso %d tiene la papa con valor %d\n",rank,token);
		  			}
		  			// Modifico el toke y envio el token y la cantidad de jugadores
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

	  	// Entran los procesos 
	  	if (rank != 0){
	  		if(sigo_en_juego == 1){
	  			// Recibo el token y cantidad de jugadores desde la derecha
	  			MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			MPI_Recv(&cantidad_jugadores, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			// Si el token en negativo el proceso deja el juego
	  			if(token < 0){
	  				printf("Proceso %d tiene la papa con valor %d (proceso %d sale del juego)\n", rank, token,rank);
	  				// Se calcula nuevo token
	  				token= rand()%t +1;
	  				// Se modifica la cantidad de jugadores
	  				cantidad_jugadores=cantidad_jugadores-1;
	  				// Envio el nuevo token y la cantidad de jugadores
	  				MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
	  				MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0,MPI_COMM_WORLD);
	  				// El proceso deja el juego
	  				sigo_en_juego = 0;
	  			}
	  			// EN el caso de que el token sea positivo entra aca
	  			else{
	  				// Si la cantidad de jugadores es 1, rank es el ganador
	  				if(cantidad_jugadores == 1){
	  					printf("Proceso %d es el ganador\n", rank);
		  			}
		  			else{
		  				printf("Proceso %d tiene la papa con valor %d\n",rank,token);
		  			}
		  			// Modifico el token, restando un numero al azar entre 1 y t
		  			token=  token - (rand()%t + 1);
		  			// Envio el nuevo token hacia la dereha y la cantidad de jugadores restantes
	    			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	    			MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	    		}
	    	}
	    	else{
	  			// Si no sigo en juego, solamente paso el token
	  			MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			MPI_Recv(&cantidad_jugadores, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  			MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  			MPI_Send(&cantidad_jugadores, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	  		}
	  	}
	}
	// Finalizo MPI
	MPI_Finalize();
}