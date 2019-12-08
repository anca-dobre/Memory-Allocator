#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t dim, prim, ultim;
unsigned char *vector;

uint32_t functie (unsigned char *aux)
{	 return aux[0]+aux[1]*256+aux[2]*256*256+aux[3]*256*256*256;
}

void razvan (unsigned char *a, uint32_t val)
{	a[0]=val;
	a[1]=val>>8;
	a[2]=val>>16;
	a[3]=val>>24;
}


void INITIALIZE (uint32_t numar) 
	  { 
	  vector = (unsigned char*)calloc(numar, sizeof(unsigned char));
	  }

void FINALIZE() { free(vector);
		}

void DUMP (uint32_t dim) 
	   {uint32_t i;
	    for (i = 0; i < dim; i++) 
		{ if (i != 0 && i%16 == 0) printf ("\n");
		if ( i%16 == 0) printf ("%08X\t", i);
		if ( i%16 != 0 && i%8 == 0) printf (" ", vector[i]);	
		 printf ("%02X ", vector[i]);
		 }
	    printf ("\n");	
	   } 
void ALLOC (uint32_t size)
 { int ok = 1; uint32_t ind; 
   	if (functie (vector+prim+8) == 0) {//printf("//DACA ARENA E GOALA");
	if (size+12 <= dim)//daca exista loc
		{razvan (vector+8, size);
		 prim = 0;
		 ultim = 0;
		 printf ("12\n");
		 ok = 0;
		}
	else//daca nu exista loc
		{ok = 0;
		 printf ("0\n");}
	}
	else if (prim >= size+12) {//printf("// DACA EXISTA LOC LA INCEPUT");	
	razvan (vector+8, size);
	 razvan (vector, prim);
	 razvan (vector + prim + 4, 0);
	 prim = 0; 
	printf("12\n"); ok = 0;
	}      
	else if (prim == ultim && prim==0) {//printf("//DACA EXISTA UN SINGUR BLOC");
	ind = functie (vector+prim+8)+12;
	razvan (vector+ind+4, prim);
	razvan (vector+prim, ind);
	razvan (vector+ind +8, size);
	 ultim = ind; //printf("%d   %d", ind, ultim);
	 printf ("%d\n", ind+12); ok = 0; 
	}	
   
   	else {//printf("//CAUT LOC IN CONTINUARE");
	uint32_t init, lungime, ceva;
	 init = prim;
	 lungime = functie (vector+init)-12-functie (vector+init+8)-init;
	 while (lungime < size+12 && init != ultim) 
		{
		 init = functie (vector+init); ceva=init;
		 lungime = functie (vector+init)-12-functie (vector+init+8)-init;
		 if (functie (vector+init+8)) lungime = 0;
		}
	if(init == ultim) {//printf("//daca nu gasesc adaug dupa ultimul");
		if (size+12 <= dim-(ultim+12+functie (vector+ultim+8)))
			{init += 12 +functie(vector+ultim+8);			 
			 razvan (vector+ultim, init); 
			 razvan (vector+init+4, ultim);
			 razvan (vector+init+8, size);
			 ultim = init;
			 printf("%d\n", init+12);
			 ok=0;
			}	

		else { printf("0\n"); ok=0;}
		}	
	else{//printf("//daca am gasit totusi loc intre 2 blocuri");
		uint32_t aux;
		 
		 init=functie(vector+functie(vector+ceva+4)+8)+12; 
		 aux = functie (vector+ceva+4);
		 razvan (vector+init, ceva);
		 razvan (vector+init+4, aux);
		 razvan (vector+aux, init);
		 razvan (vector+ceva+4, init );
		 razvan (vector+init+8, size);
		 printf ("%d\n", init+12);
		 ok = 0;
		}
	if(ok == 1) printf ("0\n");

	}
}

void FREE(uint32_t index) { uint32_t i;
		if (index-12 == prim) {//printf("//daca blocul este primul");
			for (i = index; i < index+functie (vector+index-4); i++)
				vector[i] = 0;
			prim = functie (vector+index-12); 
			for (i = index-12; i < index; i++) vector[i]=0;
			razvan (vector+prim+4, 0);
			}
		else if (index-12 == ultim) {//printf("//daca blocul este ultimul");
			uint32_t ant; 
			for(i = index; i < index+functie (vector+index-4); i++)
				vector[i] = 0;
			ant = functie (vector+index-8); 
			for (i = index-12; i < index; i++) 
				vector[i] = 0;
			ultim = ant;			
			razvan (vector+ant, 0);
			}
		else {//printf("//daca blocul este intre alte 2");
			uint32_t ant, urm;
			 for (i = index; i < index+functie(vector+index-4); i++)
				vector[i] = 0; 
			 ant = functie(vector+index-12+4);
			 urm = functie(vector+index-12);
			 razvan (vector+ant, urm);
			 razvan (vector+urm+4, ant);
		     	for (i = index-12; i < index; i++) 
				vector[i] = 0;
			}
}
 
void FILL (uint32_t index, uint32_t size, uint32_t val){	
	uint32_t i = 0;
	uint32_t it, lungime, nr=prim; 
	index -= 12;		
	it = index;
	lungime = functie (vector+it+8);
	while (size && lungime && nr < dim)
		{nr += 12+lungime;
		 for (i = it+12; i < it+12+lungime; i++) 
			{if (size) {*(vector+i) = val;
				  size--;}
			 else break;
			}
		it = functie (vector+it);
		lungime = functie (vector+it+8);
		}
}
					

void parse_command(char* cmd)
{
    const char* delims = " \n";

    char* cmd_name = strtok(cmd, delims);
    if (!cmd_name) {
        goto invalid_command;
    }

    if (strcmp(cmd_name, "INITIALIZE") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        // TODO - INITIALIZE
	INITIALIZE(size);
	dim=size;
    } else if (strcmp(cmd_name, "FINALIZE") == 0) {
        // TODO - FINALIZE
	FINALIZE();
    } else if (strcmp(cmd_name, "DUMP") == 0) {
        // TODO - DUMP
	DUMP(dim);
    } else if (strcmp(cmd_name, "ALLOC") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        // TODO - ALLOC
	ALLOC(size);
    } else if (strcmp(cmd_name, "FREE") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        // TODO - FREE
	FREE(index);
    } else if (strcmp(cmd_name, "FILL") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* value_str = strtok(NULL, delims);
        if (!value_str) {
            goto invalid_command;
        }
        uint32_t value = atoi(value_str);
        // TODO - FILL
	FILL(index, size, value);
    } else if (strcmp(cmd_name, "ALLOCALIGNED") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* align_str = strtok(NULL, delims);
        if (!align_str) {
            goto invalid_command;
        }
        uint32_t align = atoi(align_str);
        // TODO - ALLOCALIGNED

    } else if (strcmp(cmd_name, "REALLOC") == 0) {
        printf("Found cmd REALLOC\n");
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        // TODO - REALLOC

    } else {
        goto invalid_command;
    }

    return;

invalid_command:
    printf("Invalid command: %s\n", cmd);
    exit(1);
}

int main(void)
{	
    ssize_t read;
    char* line = NULL;
    size_t len;
	
    /* parse input line by line */
    while ((read = getline(&line, &len, stdin)) != -1) {
        /* print every command to stdout */
        printf("%s", line);

        parse_command(line);
    }

    free(line);

    return 0;
}
