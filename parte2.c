#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <features.h>
//////////////////////////
// ???????????????????? //
//////////////////////////

typedef struct {
	int nroCuenta;
	int saldo;
	char nbre[54];
	char direccion[54];
} clienteBanco;

void actualizarSaldos(char *cli, char *transacciones){
    int clientesInBanco = 1;
	clienteBanco *infoclientes = (clienteBanco*) malloc (sizeof(clienteBanco)*clientesInBanco*5*5*5*5*5);
	if (!infoclientes)
	{
	    printf("No se pudo asignar memoria");
	    exit(-1);
	}
	FILE* cliente= fopen(cli,"rb");
	FILE* trans  = fopen(transacciones, "r");
	int i=0, nro_cuenta, saldo_cli, contador_cuentas=0;
	//char nombre[53], direccion[53];
	if(!cliente || !trans){
		printf("Uno de los dos archivos no se encuentra\n");
		exit(1);
	}
	printf("Leyendo clientes...");
	while(!feof(cliente)){
	
		if(i%4==0){
			fread(&nro_cuenta, sizeof(int), 1, cliente);
		    //infoclientes[contador_cuentas] = clienteBanco;
		    if(contador_cuentas == clientesInBanco)
		    {
		        clientesInBanco*=5;
		        clienteBanco *newinfoclientes = (clienteBanco*) realloc(infoclientes,sizeof(clienteBanco)*clientesInBanco);
		        while(!newinfoclientes)
		            newinfoclientes = (clienteBanco*) realloc(infoclientes,sizeof(clienteBanco)*clientesInBanco);
		        infoclientes = newinfoclientes;
		        //infoclientes = (clienteBanco*) realloc(infoclientes,sizeof(clienteBanco)*10000);
		    }
			infoclientes[contador_cuentas].nroCuenta=nro_cuenta;
		    contador_cuentas++;
		}
		else if (i%4==1){
			fread(&saldo_cli, sizeof(int), 1, cliente);
			infoclientes[contador_cuentas].saldo=saldo_cli;
		}
		else if (i%4==2){
		    //for( int j = 0; nombre[j]!=0; j++ )
			//fscanf(cliente, "%s", nombre);
			//fgets(nombre, 53, cliente);
			//strcpy(nombre, infoclientes[contador_cuentas].nbre);
            fread( &infoclientes[contador_cuentas].nbre     , sizeof(char), 52, cliente );
		}
		else{
			//fscanf(cliente, "%s", direccion);
			//fgets(direccion, 52, cliente);
			//strcpy(direccion, infoclientes[contador_cuentas].direccion);
            fread( &infoclientes[contador_cuentas].direccion, sizeof(char), 52, cliente );
		}
		//contador_cuentas++;
		i++;
		
	}
	printf(" %d clientes.\n", infoclientes[contador_cuentas-1].nroCuenta);

	//infoclientes = realloc(infoclientes, contador_cuentas* sizeof(clienteBanco));
	if(cliente)
	    fclose(cliente);
	printf("Leyendo y realizando cambios...\n");
	while (!feof(trans)){
		int monto, numero_cuen1, numero_cuen2;
		char signo[1];
		int j = fscanf(trans, "%c %d %d %d\n", signo, &numero_cuen1, &numero_cuen2, &monto);
		
		if(j==3)
		{
			monto=numero_cuen2;
            for(  i = 0; infoclientes[i].nroCuenta != numero_cuen1 && i < contador_cuentas; ) i++;
			//numero_cuen2=numero_cuen1;
			if (strcmp(signo,"+")){
				//infoclientes[numero_cuen1].saldo+=monto;
                infoclientes[i].saldo +=monto;
					
			}
			
			else{	
				//infoclientes[numero_cuen1].saldo-=monto;
                infoclientes[i].saldo +=monto;
			}
		}
		else if (j==4){
			if(strcmp(signo, ">")){
				//infoclientes[numero_cuen2].saldo+=monto;
				//infoclientes[numero_cuen1].saldo-=monto;
				int k = 0;
                for(  k = 0, i = 0; i < contador_cuentas; i++ )
                {
                    //for(  i = 0; cliente[i].nroCuenta != numeros[1] && i < clientesProcesados; i++ );
                    if( infoclientes[i].nroCuenta == numero_cuen1 )
                    {
                        infoclientes[i].saldo -= monto;
                        k++;
                    } else if( infoclientes[i].nroCuenta == numero_cuen2 )
                    {
                        infoclientes[i].saldo += monto;
                        k++;
                    }
                    if( k%2 == 0 )
                        break;
                }
			}
		}
	}
	if (trans)
	    fclose(trans);
	FILE* cl=fopen("salida.dat", "w");
	printf("Escribiendo cambios...\n");
	for(int n=1; n<contador_cuentas; n++){
	    //printf( "%d\n", infoclientes[n].nroCuenta  );
	    //printf( "%d\n", infoclientes[n].saldo  );
	    //printf( "%s\n", infoclientes[n].nbre  );
	    //printf( "%s\n", infoclientes[n].direccion  );
        //printf("--------------------\n");
	    
		//fprintf(cl, "%d\n",infoclientes[n].nroCuenta);
		//fprintf(cl, "%d\n",infoclientes[n].saldo);
		//fprintf(cl, "%s\n", infoclientes[n].nbre);
		//fprintf(cl, "%s\n", infoclientes[n].direccion);
		
        fwrite( &infoclientes[n].nroCuenta, sizeof(int),   1, cl );
        fwrite( &infoclientes[n].saldo    , sizeof(int),   1, cl );
        fwrite( &infoclientes[n].nbre     , sizeof(char), 52, cl );
        fwrite( &infoclientes[n].direccion, sizeof(char), 52, cl );
	}
	if (cl)
	    fclose(cl);
	if (infoclientes)
	    free(infoclientes);	
}


int main(){
	//char clientes[20], transacciones[20];
	//printf("Ingrese nombre del archivo binario:\n");
	//scanf("%s", clientes);
	//printf("Ingrese nombre del archivo ASCII:\n");
	//scanf("%s", transacciones);
	actualizarSaldos("clientes-10K.dat", "transacciones-10K.txt");
	return 0;

}