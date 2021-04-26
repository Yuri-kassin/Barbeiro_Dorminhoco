//Yuri Kassin Moura Ferreira
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>

#define N_CLIENTES 150
#define N_CADEIRAS 5


sem_t sem_cadeiras; //declara��o de semaforos

sem_t sem_cad_barbeiro;//declara��o de semaforos

// obs: neste programa 1 hora equivale a 100 unidades de tempo, logo 
// 1 dia de trabalho tem 2400 unidades de tempo, suponde que o barbeiro 
// trabalhe 24 horas 

int contaTimesleep = 0; //contador de horas dormidas
int contaTime = 0; // contador de tempo para indicar o fim do dia
int contaHora = 0; 
int conta_clientes = 0; // contador de clientes
int cliente = 0; // variavel para parar o loop
int umaHora = 100; // variavel auxiliar referente a 1 hora

int aux = 0;

void* f_cliente(void* v){	//thread cliente
	int id = *(int*) v;	

	
if (sem_trywait(&sem_cadeiras) == 0) { // verificar se a barbearia estaja livre
	
    printf("Cliente %d entrou na barbearia.\n", id);
    sem_wait(&sem_cad_barbeiro);
    printf("Cliente %d sentou na cadeira do barbeiro.\n", id);    
     
     // os ifs a seguir servem para determinar quanto tempo o barbeiro gasta para cortar o cabelo do cliente
   	    
    if(aux == 0){ // barbeiro gatou 20 min para atender o cliente
    
   		umaHora = umaHora - 20;
     
	} 
	else if ( aux == 1){ // barbeiro gatou 25 min para atender o cliente
	
   		umaHora = umaHora - 25;
   	
   			}
	else if ( aux == 2){ // barbeiro gatou 30 min para atender o cliente
	
   		umaHora = umaHora - 30;
   	
   	
	}
    
    
    
    sem_post(&sem_cadeiras);
    sem_post(&sem_cad_barbeiro);
    printf("Cliente %d corotu o cabelo e saiu.\n", id);
    
  } else // caso a barbearia estaja lotada
    printf("Cliente %d nao entrou !!!\n", id);
  return NULL;
}


int main(){
	
  srand((unsigned)time(NULL)); // setando a fun��o rand() para gerar numeros aleatorios de tempo de atendimento
	
  pthread_t thr_clientes[N_CLIENTES]; //cria a thread cliente
  int i = 1, id[N_CLIENTES]; // vetor de clientes e suas identidades

  sem_init(&sem_cadeiras, 0, 5); // semaforos para caderias diasponiveis
  sem_init(&sem_cad_barbeiro, 0, 1); // semaforos para cadeira do barbeiro

  	

	while(contaTime != 1440){ // loop representando 1 dia de trabalho
		
		while(cliente != 2){ // loop representa 1 hora do dia e gera 2 clientes
				id[i] = i;
				aux = rand() % 3; // numeros aleatorios
				pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id[i]);	//thread cliente
				pthread_join(thr_clientes[i], NULL);
				i++; // mudar o id do cliente
				cliente++; // incrementando o numero de cliente na hora para limitar a 2 por hora
				conta_clientes++; // contador de clientes no dia
				  	
		}
				
	    contaTimesleep = contaTimesleep + umaHora; // conta para obter a quantidade de horas que o barbeiro dormiu
		umaHora = 60; // setando a variavel 1 hora para 60 min novamente
		contaTime = contaTime + 60; // contador para definar um dia
		cliente = 0;
	}	
  printf("Horas dormindas: %d minutos \n", contaTimesleep);// imprimindo os valores
  printf("Clientes atendidos: %d\n", conta_clientes);
	
	
	
	return 0;
}
