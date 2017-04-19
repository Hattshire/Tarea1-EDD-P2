#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int nroCuenta;
	int saldo;
	char nbre[52];
	char direccion[52];
} clienteBanco;

void
actualizarSaldos( char *cli, char *trans )
{   
    // Leer archivos
    //     Se abre cli como binario en modo lectura
    //     Se abre trans en modo lectura
    FILE* clientes, *transacciones;
    clientes = fopen( cli, "rb" );
    transacciones = fopen( trans, "r" );
    
    int clientesInBanco = 1, clientesProcesados = 0;
    
    // Array de clientes
    clienteBanco * cliente = realloc( NULL, sizeof( clienteBanco ) * clientesInBanco );
    
    while( !feof( clientes ) )
    {
        if( clientesInBanco == clientesProcesados )
        {
            clientesInBanco *= 5;
            clienteBanco * newCliente = realloc( cliente, sizeof( clienteBanco ) * clientesInBanco );
            if( !newCliente )
                exit(-1);
            cliente = newCliente;
            //printf( "%ld Bytes Alojados\n", sizeof( clienteBanco ) * clientesInBanco  );
        }
        fread( &cliente[clientesProcesados].nroCuenta, sizeof(int),   1, clientes );
        fread( &cliente[clientesProcesados].saldo    , sizeof(int),   1, clientes );
        fread( &cliente[clientesProcesados].nbre     , sizeof(char), 52, clientes );
        fread( &cliente[clientesProcesados].direccion, sizeof(char), 52, clientes );
        //printf( "Leida cuenta %d de %s\n", cliente[clientesProcesados].nroCuenta, cliente[clientesProcesados].nbre );
        clientesProcesados++;
    }
    printf( "-------------\nClientes: %d\nDesde la cuenta %d hasta %d\n", clientesProcesados, cliente[0].nroCuenta, cliente[clientesProcesados-2].nroCuenta );
    fclose( clientes );
    
    char simbolo, signo;
    int numeros[3], i;
    
    printf( "Procesando transacciones...\n" );
    while( !feof( transacciones ) )
    {
        signo = 1;
        fscanf( transacciones, "%c ", &simbolo );
        switch( simbolo )
        {
            case '-':
                signo = -1;
            case '+':
                fscanf( transacciones, "%d %d\n", &numeros[0], &numeros[1] );
                numeros[1] *= signo;
                for(  i = 0; cliente[i].nroCuenta != numeros[0] && i < clientesProcesados; i++ );
                cliente[i].saldo +=numeros[1];
                //printf( "Operacion '%c $%d' > %d exitosa\n", simbolo, numeros[0], numeros[1] );
                break;
            case '>':
                fscanf( transacciones, "%d %d %d\n", &numeros[0], &numeros[1], &numeros[2] );
                int k = 0;
                for(  k = 0, i = 0; i < clientesProcesados; i++ )
                {
                    //for(  i = 0; cliente[i].nroCuenta != numeros[1] && i < clientesProcesados; i++ );
                    if( cliente[i].nroCuenta == numeros[0] )
                    {
                        cliente[i].saldo -=numeros[2];
                        k++;
                    } else if( cliente[i].nroCuenta == numeros[1] )
                    {
                        cliente[i].saldo +=numeros[2];
                        k++;
                    }
                    if( k%2 == 0 )
                        break;
                }
                //printf( "Operacion %d > $%d > %d exitosa\n", numeros[0], numeros[2], numeros[1] );
                break;
        }
    }
    fclose( transacciones );
    printf( "Escribiendo de vuelta al archivo...\n" );
    clientes = fopen( "NiMuerto al default(10094).bin", "wb" );
    for( i = clientesProcesados; i!=0 ; i--)
    {
        fwrite( &cliente[clientesProcesados-i].nroCuenta, sizeof(int),   1, clientes );
        fwrite( &cliente[clientesProcesados-i].saldo    , sizeof(int),   1, clientes );
        fwrite( &cliente[clientesProcesados-i].nbre     , sizeof(char), 52, clientes );
        fwrite( &cliente[clientesProcesados-i].direccion, sizeof(char), 52, clientes );
    }
    fclose( clientes );
    free( cliente );
}

int
main()
{
    char clientes[30], transacciones[30];
    printf( "Nombre del archivo clientes: " );
    scanf( "%s", clientes );
    printf( "Nombre del archivo transacciones: " );
    scanf( "%s", transacciones );
    actualizarSaldos( clientes, transacciones );
    //actualizarSaldos( "cuentas.bin", "transacciones-10K.txt" );
    return 0;
}