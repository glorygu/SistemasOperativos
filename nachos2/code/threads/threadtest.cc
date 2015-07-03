// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include <iostream>
using namespace std;
//Semaphore semH("Semáforo H", 0), semO("Semáforo O", 0);
//int hCounter = 0, oCounter = 0;
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

//Entrar 
Semaphore* semO = new Semaphore("SemO", 0);
Semaphore* semH = new Semaphore("SemH", 0);
int contadorO = 0;
int contadorH = 0;
//Supersticioso 

Semaphore* semE = new Semaphore("SemE", 0); //semaforo para entrar
Semaphore* semS = new Semaphore("SemS", 0);//semaforo para salir 
int totalAdentro  =0; 
int totalAfuera = 0;
int totalSaliendo = 0; 

void Entrando (int nada ){


if (totalAdentro == 12){

	if (totalAfuera==0){
	
	totalAfuera++;
	printf ("Adentro hay %d, quieren entrar %d, Espera para entrar\n", totalAdentro, totalAfuera);
	semE->P( );  
	printf ("Sem E terminó de esperar \n Ahora Adentro hay %d, quieren entrar %d, Espera para 		entrar\n", totalAdentro, totalAfuera);
	totalAfuera--;
	printf ("Entrando, ahora adentro hay %d \n", totalAdentro+1); 
        totalAdentro++; 
	}
	else
	{
	printf ("Adentro hay %d, quieren entrar %d, Manda señal a sem Entrar para que entre\n", totalAdentro, totalAfuera);
	 semE->V ( );    
	 
         printf ("SemE mando la señal, Entrando, ahora adentro hay %d \n", totalAdentro);    
         totalAdentro++; 
	}

}
else {
        totalAdentro++;
        printf ("Entrando, ahora adentro hay %d \n", totalAdentro); 
}
if (totalSaliendo ==1){
	printf ("Adentro hay %d, quieren entrar %d, Manda desde Entrando señal a sem Salir para que salga\n", totalAdentro, totalAfuera);
        semS->V ( );
} 
}

//Salir
void Saliendo (int nada) {


if (totalAdentro == 14){

	if (totalSaliendo==0){
	
	totalSaliendo++;
	printf ("Adentro hay %d, quieren salir %d, Espera para salir\n", totalAdentro, totalSaliendo);
	semS->P(); 
	printf ("SemS recibio señal Adentro hay %d, quieren salir %d y quieren entrar %d \n",totalAdentro, totalSaliendo, totalAfuera); 
	totalSaliendo--;
	printf ("Saliendo, adentro quedan %d \n", totalAdentro-1);
        totalAdentro --;
	}
	else
	{printf ("Adentro hay %d, quieren salir %d, Manda señal a sem Salir para que salgan\n", totalAdentro, totalSaliendo);
	 semS->V ( );       
         totalAdentro--; 
	}

}
else {
     
        totalAdentro--;
        printf ("Saliendo, adentro quedan %d \n", totalAdentro);
}
if (totalAfuera ==1){
printf ("Adentro hay %d, quieren entrar %d, Manda dedes Saliendo señal a sem Entrar para que entre\n", totalAdentro, totalAfuera);
        semE->P ( );
} 
}

void haceHiloSup  ( ){
	Thread *t; 
	t = new Thread ("Creador hilos Sup\n");
	if (Random()%3){
		t -> Fork(Entrando, 0);
	} else {
		t -> Fork(Saliendo, 0);
	}
}

void
SimpleThread(int which)
{
    int num;
    Semaphore * s = new Semaphore ("threadDebugName", 0); 
    for (num = 0; num < 10; num++) { // se cambio la condicion 
	//Se agrega sem
	s->V( ); 
	printf("*** thread %d looped %d times\n", which, num);
	s->P ( );         
	currentThread->Yield();
    }
}
/*
void H (int co ){

hCounter++;
if (oCounter>0 &&hCounter>1){
hCounter-=2; 
oCounter--;
printf("Haciendo agua!!\n");
semH.V();
semO.V();
}
else {
semH.P();  //wait
}
}

void O (int co ) {
oCounter ++;
if (hCounter >1){
hCounter-=2; 
oCounter--;
printf("Haciendo agua!!\n");
semH.V();
semO.V();
}
else 
{
semO.P ( ); //wait
}
}
 */

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------
/*
void createWaterThread ( ){
Thread *waterT = new Thread ("water thread");
if (Random () %3){
 waterT ->Fork(H, 0);
}
else 
{
 waterT ->Fork(O, 0);
}
}

void
ThreadTest()
{

    DEBUG('t', "Entering SimpleTest");
*/
 /** 
    Thread *t = new Thread("forked thread");
 
    t->Fork(SimpleThread, 5);
for (int i = 0; i < 5; i++){ //se agrego el for 
    SimpleThread(i);
**/
/*
for (int i = 0; i < 30; i++){
createWaterThread ( );
}
 
}

*/





void H(int co )
{
	contadorH++;
	if (contadorH < 2 || contadorO < 1)
	{
		semH -> P();
	}
	else
	{
		contadorH -= 2;
		contadorO--;
		cout << "Se ha creado agua!" << endl;
		semO -> V();
		semH -> V();
	}
}

void O(int co)
{
	contadorO++;
	if (contadorH < 2)
	{
		semO -> P();
	}
	else
	{
		contadorO--;
		contadorH -= 2;
		cout << "Se ha creado agua!" << endl;
		semH -> V();
		semH -> V();
	}
}


void haceHilo()
{
	Thread * t;
	t = new Thread("Creador de Hilos\n");
	if (Random()%3) {
		t -> Fork(H, 0);
	} else {
		t -> Fork(O, 0);
	}
}


void
ThreadTest()
{
    //Thread * Ph;

    DEBUG('t', "Entering SimpleTest");

/*
    dp = new DinningPh();

    for ( long k = 0; k < 5; k++ ) {
        Ph = new Thread( "dp" );
        Ph->Fork( Philo, (void *) k );
    }

    return;
*/
/*    for ( int k=1; k<=8; k++) {
      char* threadname = new char[100];
      sprintf(threadname, "Hilo %d", k);
      Thread* newThread = new Thread (threadname);
      newThread->Fork (SimpleThread, (void*)threadname);
    }
    
    SimpleThread( (void*)"Hilo 0");
*/    
    DEBUG('t', "Entering Supersticioso Thread");
    for (int i = 0; i < 40; i++) {
    	haceHiloSup();
    }
}
