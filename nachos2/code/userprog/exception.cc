// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
#include "system.h"
#define ConsoleError 	2
#include "copyright.h"
// returnFromSystemCall
#include <unistd.h>
#include "nachossemtabla.h"
#include "syscall.h"
//#include <synch.h>
#include "nachostabla.h"
//#include "nachoshilotabla.h"
#include <machine.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
//#include <unistd.h>
#include <thread.h>

#include <string.h>
#include "addrspace.h"
using namespace std;

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------
NachosTabla *tabla = new NachosTabla ( );
NachosSemTabla *semTabla = new NachosSemTabla ( );

void returnFromSystemCall()
{

    int pc, npc;

    pc = machine->ReadRegister( PCReg );
    npc = machine->ReadRegister( NextPCReg );
    machine->WriteRegister( PrevPCReg, pc );        // PrevPC <- PC
    machine->WriteRegister( PCReg, npc );           // PC <- NextPC
    machine->WriteRegister( NextPCReg, npc + 4 );   // NextPC <- NextPC + 4

}       // returnFromSystemCall


void Nachos_Halt()                      // System call 0
{

    DEBUG('a', "Shutdown, initiated by user program.\n");
    interrupt->Halt();

}       // Nachos_Halt

void Nachos_Open()
{
    printf ("entrando a Nachos Open\n");  // System call 5
    int dirNombre = machine->ReadRegister (4);
    printf ("direccion nombre %d", dirNombre);
    bool correcto = true;
    char nombre [100];
    char valorChar = ' ';
    int i = 0;
    int valor;
    int UnixId;
    int codError;
    while (i < 100 && correcto && valorChar!= '\0')
    {
        //se pasa de char a int
        correcto = machine->ReadMem (dirNombre, 1, &valor);
        valorChar = valor;
        if (correcto)
        {
            nombre [i] = valorChar;
            i++;
            dirNombre++;
        }
        else
        {
            cout << "error lectura al abrir"<<endl;
        }
    }
    printf("paso nombre");
    cout <<endl<<nombre<<endl;

    if (correcto)  //deberia abrir con el open de nachos tabla
    {
        UnixId = open(nombre, O_RDWR);
        cout << "unixid" << UnixId << endl;
        if (UnixId != -1)
        {
            codError = tabla->Open (UnixId);
            if (codError == -1)
            {
                cout << "Error al asociar archivo" <<endl;
            }
        }
    }

//¿Se tiene que poner algo en r2?
    machine->WriteRegister (2, codError);
    cout << endl<< codError << endl;
    returnFromSystemCall ( ) ;

}

// Read the name from the user memory, see 4 below
// Use NachosOpenFilesTable class to create a relationship
// between user file and unix file
// Verify for errors

void Nachos_Create ( )
{
    int dirNombre = machine->ReadRegister (4);
    printf ("direccion nombre %d", dirNombre);
    bool correcto = true;
    char nombre [100];
    char valorChar = ' ';
    int i = 0;
    int valor;
    int UnixId;
    int codError;
    while (i < 100 && correcto && valorChar!= '\0')
    {
        //se pasa de char a int
        correcto = machine->ReadMem (dirNombre, 1, &valor);
        valorChar = valor;
        if (correcto)
        {
            nombre [i] = valorChar;
            i++;
            dirNombre++;
        }
        else
        {
            cout << "error lectura al abrir"<<endl;
        }
    }
    printf("paso nombre");
    cout <<endl<<nombre<<endl;

    if (correcto)  //deberia abrir con el open de nachos tabla
    {
        UnixId = creat(nombre, S_IRWXU);
        cout << "unixid" << UnixId << endl;
        if (UnixId != -1)
        {
            codError = tabla->Open (UnixId);
            if (codError == -1)
            {
                cout << "Error al asociar archivo" <<endl;
            }
        }
    }


    machine->WriteRegister (2, codError);
    cout << endl<< codError << endl;
    returnFromSystemCall ( ) ;

}
void Nachos_Close ()
{
//aqui se tiene que cerrar un archivo que fue abierto
//deberia poder asociarse con el nachos tabla
    int toCloseId = machine->ReadRegister (4); //se supone que es el nachosHandle
    int returnVal;
    int unixId = tabla->getUnixHandle (toCloseId);
    if (tabla->isOpened (toCloseId))
    {
        tabla->Close (toCloseId);
        returnVal = close (unixId); //si es 0 fue exitoso y -1 sino
        //manda mensaje si es -1
    }

    //machine->WriteRegister (2, returnVal);
    returnFromSystemCall ( );
}


//¿Se tiene que poner algo en r2?

void Nachos_Write()
{
    int buffer = machine -> ReadRegister(4);
    int size = machine -> ReadRegister(5);
    OpenFileId id = machine -> ReadRegister(6);
    char contenido[size];
    //Semaphore * consola = new Semaphore("Acceso Consola", 0);
    bool correcto = true;
    int i = 0;
    int  valor;
    printf("Hola");

    while (i < size && correcto)
    {
        correcto = machine->ReadMem(buffer, 1, &valor);
        if (correcto)
        {
            contenido[i] = (char)valor;
            i++;
            buffer++;
        }
        else
        {
            cout << "Error de lectura" << endl;
        }
    }

    if (correcto)
    {
        //consola -> P();
        switch (id)
        {
        case ConsoleInput:
            machine -> WriteRegister(2, -1);
            break;
        case ConsoleOutput:
            contenido[size] = 0;
            printf("%s", contenido);
            break;
        case ConsoleError:
            printf("%d\n", machine -> ReadRegister(4));
            break;
        default:
            if (tabla -> isOpened(id))
            {
                int unixId = tabla-> getUnixHandle(id);
                int leido = write(unixId, contenido, size);
                if (leido == -1)
                {
                    cout << "Error al escribir en el archivo" << endl;
                }
                else
                {
                    machine -> WriteRegister(2, leido);
                }
            }
            else
            {
                cout << "Este archivo no esta abierto" << endl;
                machine -> WriteRegister(2, -1);
            }
            break;

            //consola -> V();
        }
        stats -> numDiskWrites++;
        returnFromSystemCall();
    }
}

void Nachos_Read()
{
    printf("entro");
    char *buffer = (char*)machine -> ReadRegister(4);
    printf("registro4");
    int size = machine -> ReadRegister(5);
    printf("registro5");
    OpenFileId id = machine -> ReadRegister(6);
    printf("registro6");
    int unixID = tabla -> getUnixHandle(id);
    int cantLeida = read(unixID, buffer, size);
    printf("leer");
    if (cantLeida == -1)
    {
        printf("Error de lectura");
    }
    stats -> numDiskReads++;
    machine -> WriteRegister(2, cantLeida);
    returnFromSystemCall();

}


void Nachos_SemCreate ( )
{
    int initVal = machine->ReadRegister (4);
    long semId =  (long) new Semaphore ("semDebug", initVal);
    int semNachosId= semTabla->CreateSem(semId); //cual es el unixId
    machine->WriteRegister (2, semNachosId);
    returnFromSystemCall();
}

void Nachos_SemDestroy ( )
{
    int semNachosId = machine->ReadRegister (4);
    //el semId es el nachosHandle de la tabla de semaforos;
    if (semTabla->isOpened (semNachosId))
    {
        Semaphore *sTemp = (Semaphore *) semTabla->getSemHandle(semNachosId);
        semTabla->DestroySem(semNachosId);
        delete sTemp;
    }
    returnFromSystemCall();
}

void Nachos_SemSignal()
{
    int SemId = machine -> ReadRegister(4);
    int codigo = -1;
    if (semTabla -> isOpened(SemId))
    {
        Semaphore* Sem = (Semaphore*)(semTabla -> getSemHandle(SemId));
        Sem -> V();
        codigo = 0;
    }
    machine -> WriteRegister(2, codigo);
    returnFromSystemCall();
}

void Nachos_SemWait()
{
    int SemId = machine -> ReadRegister(4);
    int codigo = -1;
    if (semTabla -> isOpened(SemId))
    {
        Semaphore* Sem = (Semaphore*)(semTabla -> getSemHandle(SemId));
        Sem -> P();
        codigo = 0;
    }
    machine -> WriteRegister(2, codigo);
    returnFromSystemCall();
}
void NachosForkThread(int p)
{
    AddrSpace *space;
    space = currentThread -> space;
    space -> InitRegisters();
    space -> RestoreState();
    //int newP = (int) p;
    machine -> WriteRegister(RetAddrReg, 4);
    machine -> WriteRegister(PCReg, (long) p);
    machine -> WriteRegister(NextPCReg,p + 4);
    machine -> Run();
    ASSERT(false);
}

void Nachos_Fork()
{
    int proceso = machine -> ReadRegister(4);
    Thread * newT = new Thread("Proceso de Fork");
    newT -> space = new AddrSpace(currentThread -> space);
    newT -> tabla -> addThread();
    newT -> Fork(*NachosForkThread, proceso);
    returnFromSystemCall();
}



void Nachos_Join()
{
    int hijoId = machine -> ReadRegister(4);
    currentThread -> semHilo = new Semaphore("Semaforo para comunicarse con hijos", 0);
    if (currentThread -> tabla -> isOpened(hijoId))
    {
        Thread * hijo = (Thread *)currentThread -> tabla -> getHiloHandle(hijoId);
        hijo-> semHilo = currentThread -> semHilo;
        currentThread ->semHilo -> P();
        currentThread -> tabla ->DestroyHilo(hijoId);
    }
    returnFromSystemCall();
}
void Nachos_Exit ( )
{
    int status = machine->ReadRegister (4);
    int tamano = currentThread->space->numPages;
    for (int i = 0; i < tamano; i++)
    {
        MiMapa->Clear(currentThread->space->pageTable[i].physicalPage);
    }
    machine->WriteRegister (2, status);
    currentThread->semHilo->V( );
    returnFromSystemCall( );
}
/**
void
StartProcess(char *filename)
{
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
	printf("Unable to open file %s\n", filename);
	return;
    }
    space = new AddrSpace(executable);
    currentThread->space = space;

    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register

    machine->Run();			// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
					// the address space exits
					// by doing the syscall "exit"
}
**/
void Nachos_Exec ( )
{
    //va a devolver un spaceId que es el indice de la tabla de hijos
//del proceso que se acaba de crear
    int spcID = 0;
    int dirNombre = machine->ReadRegister (4);
    printf ("direccion nombre %d", dirNombre);
    bool correcto = true;
    char nombre [100];
    char valorChar = ' ';
    int i = 0;
    int valor;
    int UnixId;
    int codError;
    while (i < 100 && correcto && valorChar!= '\0')
    {
        //se pasa de char a int
        correcto = machine->ReadMem (dirNombre, 1, &valor);
        valorChar = valor;
        if (correcto)
        {
            nombre [i] = valorChar;
            i++;
            dirNombre++;
        }
        else
        {
            cout << "error lectura al abrir"<<endl;
        }
    }
//	StartProcess(nombre);}

    machine->WriteRegister (2, spcID);
    returnFromSystemCall();
}

void Nachos_Yield ( )
{
    currentThread->Yield ( );
    returnFromSystemCall();
}

bool buscarArchivoAbierto(char * nombreArchivo )
{
    bool encontrado = false;
    bool abierto = false;
    for (int i =0; i < 32; i++)
    {
        if (ArchivosAbiertos[i] != 0 && strcmp(ArchivosAbiertos[i],nombreArchivo)==0)
        {
            encontrado = true;
        }
    }

    if (!encontrado)
    {

        int j = 0;
        while (!abierto && j < 32)
        {
            if (ArchivosAbiertos [j] == 0)
            {
                ArchivosAbiertos [j] = nombreArchivo;
                abierto = true;
            }
            j++;
        }
    }
    return encontrado;
}

//int ultimoReemplazo es una variable global de system

void secondChanceMemoria ( )
{
    bool victima_encontrada = false;
    int indice = ultimoReemplazo;
    int posSwap = -1;
    int cont = 0;
    while (!victima_encontrada && cont <32)
    {
        if (pageTable[TablaInv->entradas[indice]->posVirtual].use == false )
        {
            //toma esa esa pagina como victima
            ultimoReemplazo = indice;
            if (pageTable[TablaInv->entradas[indice]->posVirtual].dirty == true)
            {
                //tiene que meter al swap

                posSwap = swap->meterAlSwap(ultimoReemplazo);
                pageTable[TablaInv->entradas[indice]->posVirtual].physicalPage = -1*posSwap;
                TablaInv->Clear (indice);
            }
            pageTable[TablaInv->entradas[indice]->posVirtual].valid = false;
            victima_encontrada = true;
        }
        else
        {
            if (indice >=31)
            {
                indice = 0;
            }
            else indice ++;
        }
        -		cont ++;
    }
    if (cont >= 32 || !victima_encontrada)
    {
        (ultimoReemplazo++)%32;
    }

}

void secondChanceTLB ( )
{
    bool victima_encontrada = false;
    int cont = 0;
    int indiceTemp = indiceTLB;
    while (!victima_encontrada && cont < 4)
    {
        if (machine -> tlb[indiceTemp].use==false)
        {
            indiceTLB = indiceTemp;
            victima_encontrada = true;
        }
        if (indiceTemp >= 3)
        {
            indiceTemp = 0;
        }
        else indiceTemp++;
        cont++;
    }
    if (!victima_encontrada)
    {
        (indiceTLB++)%4;
    }

    int pagVirt = machine -> tlb[indiceTLB].virtualPage;

    //guardar bits de estado
    currentThread->space->pageTable[pagVirt].dirty = machine -> tlb[indiceTLB].dirty;
    currentThread->space->pageTable[pagVirt].use = machine -> tlb[indiceTLB].use;
}



void
ExceptionHandler(ExceptionType which)
{
    /**
    aqui va el switch para manejar cada syscall
    **/ printf("en exception handler\n");
    int type = machine->ReadRegister(2);
    char * algo = "algo.txt\0";
    int direccionLogica =0;
    int numPag = 0;
    int posLibre = -1;
    switch (which)
    {

    case SyscallException:
        switch (type)
        {
        case SC_Halt: //type = 0;
            printf ("Entra a halt");
            DEBUG('a', "Shutdown, initiated by user program.\n");
            interrupt->Halt();
            break;
        case  SC_Exit:	//type = 1
            printf ("exit\n");
            Nachos_Exit();
            break;
        case SC_Exec: //type = 	2
            printf ("exec\n");
            Nachos_Exec ( );
            break;
        case SC_Join : //type =3
            printf ("Join\n");
            Nachos_Join();
            break;
        case SC_Create://type =4
            printf ("Create\n");
            Nachos_Create( );
            break;
        case SC_Open://type =5
            printf ("Open\n");
            //machine-> (4, (void*) algo);
            Nachos_Open ( );
            break;
        case SC_Read://type =6
            printf ("Read\n");
            Nachos_Read();
            break;
        case SC_Write://type =7
            printf ("Write\n");
            Nachos_Write ( );
            break;
        case SC_Close://type =8
            printf ("Close\n");
            Nachos_Close( );
            break;
        case SC_Fork://type =9
            printf ("Fork\n");
            Nachos_Fork();
            break;
        case SC_Yield://type =10
            printf ("yield\n");
            Nachos_Yield();
            break;
        case SC_SemCreate://type =11
            printf ("SemCreate\n");
            Nachos_SemCreate();
            break;
        case SC_SemDestroy://type =12
            printf ("SemDestroy\n");
            Nachos_SemDestroy();
            break;
        case SC_SemSignal://type = 13
            printf ("SemSignal\n");
            Nachos_SemSignal();
            break;
        case SC_SemWait://type = 14
            printf ("SemWait\n");
            Nachos_SemWait();
            break;
        default:
            printf("Unexpected syscall exception %d\n", type );
            ASSERT(false);
            break;
        }
        break;
    case PageFaultException:
        int frameDisponible;
        printf ("Entro en Page Fault Exception %d\n", PageFaultException);
        DEBUG ('n', "PAGE FAULT EXCEPTION");
        direccionLogica = machine->ReadRegister (39);
        numPag = direccionLogica/PageSize;
        printf ("En page fault el numero de pagina es %d\n y la direccion es %d, initDataSize %d, codeSize %d \n", numPag, direccionLogica,currentThread->space->encabezado.initData.size,currentThread->space->encabezado.code.size);
        if (numPag == 208||numPag == 0)
        {
            sleep (5);
        }
        posLibre = tlbBitMap->Find();


        if (currentThread->space->pageTable[numPag].valid) //verifica el estado
        {
            //copiar datos del pageTable al TLB
            printf ("Pagina valida en pageTable");
            sleep (5);
            if (posLibre >= 0)
            {
                printf ("Hay espacio en TLB");
                sleep (5);
                machine -> tlb [indiceTLB].valid = true;
                machine -> tlb [indiceTLB].use = currentThread->space->pageTable [numPag].use;
                machine -> tlb [indiceTLB].dirty = currentThread->space->pageTable [numPag].dirty;
                machine -> tlb [indiceTLB].readOnly = currentThread->space->pageTable [numPag].readOnly;
                machine -> tlb[indiceTLB].virtualPage = currentThread->space->pageTable[numPag].virtualPage;
                //break;
                machine -> tlb[indiceTLB].physicalPage = currentThread->space->pageTable[numPag].physicalPage;

            }
            else
            {
                //si no hay posiciones libres en el TLB
                printf ("No hay posiciones libres en el TLB");
                sleep (5);
                secondChanceTLB(); //algoritmo de reemplazo

                //cargar pagina en tlb
                machine -> tlb[indiceTLB].valid = true;
                machine -> tlb[indiceTLB].use = currentThread->space->pageTable [numPag].use;//???
                machine -> tlb[indiceTLB].dirty = currentThread->space->pageTable [numPag].dirty;
                machine -> tlb[indiceTLB].readOnly = currentThread->space->pageTable [numPag].readOnly;
                machine -> tlb[indiceTLB].virtualPage = currentThread->space->pageTable[numPag].virtualPage;
                machine -> tlb[indiceTLB].physicalPage = currentThread->space->pageTable[numPag].physicalPage;
            }
        }
        else
        {
            //si la pagina es inválida (no esta en pageTable)
            printf ("Pagina invalida \n");
            if (currentThread->space->pageTable [numPag].dirty)
            {
                printf ("Pagina sucia \n");
                //si la pagina fue modificada (dirty)
                //sacar de swap
                frameDisponible = TablaInv -> Find(); //busca una posicion libre en memoria
                if (frameDisponible == -1)
                {
                    //algoritmo de reemplazo
                    secondChanceMemoria();
                    frameDisponible = ultimoReemplazo;
                }

                bool sacado = Swap -> sacarDelSwap(currentThread->space->pageTable[numPag].physicalPage, frameDisponible);

                if (posLibre == -1)
                {
                    //si la TLB esta llena
                    secondChanceTLB();
                }

                //actualizar tlb
                machine -> tlb[indiceTLB].virtualPage = currentThread->space->pageTable[numPag].virtualPage
                                                        machine -> tlb[indiceTLB].physicalPage = currentThread->space->pageTable[numPag].physicalPage
                                                                machine -> tlb[indiceTLB].valid = currentThread->space->pageTable[numPag].valid
                                                                        machine -> tlb[indiceTLB].dirty = currentThread->space->pageTable[numPag].dirty
                                                                                machine -> tlb[indiceTLB].readOnly = currentThread->space->pageTable[numPag].readOnly
                                                                                        machine -> tlb[indiceTLB].use = currentThread->space->pageTable[numPag].use

            }
            else
            {
                printf ("Pagina limpia \n");
                //si la pagina no ha sido modificada (limpia)
                printf ("Pagina limpia %d \n", currentThread->space->encabezado.initData.size);
                if (direccionLogica < currentThread->space->encabezado.initData.size + currentThread->space->encabezado.code.size)
                {
                    //es pagina de codigo o datos inicializados

                    printf ("La pagina es de codigo o de datos inicializados");
                    sleep (5);
                    OpenFile* archivo;
                    printf (currentThread->nombreArchivo);
                    bool abierto = buscarArchivoAbierto(currentThread->nombreArchivo);	//revisa si el archivo ejecutable originakl sigue abierto
                    printf ("pregunta si esta abierto \n");
                    if (!abierto)
                    {
                        //si el archivo ya esta cerrado, se vuelve a abrir
                        printf("Archivo cerrado");
                        archivo = fileSystem->Open(currentThread->nombreArchivo);

                    }
                    else
                    {
                        printf ("archivo abierto");
                        archivo = currentThread->space->archivoEjecutable;	//g
                    }
                    printf ("Se supone que ya reviso archivo \n");
                    frameDisponible = TablaInv->Find(); //busca si hay frame libre en memoria
                    if (frameDisponible == -1)
                    {
                        printf ("no hay frame disponible");
                        //algoritmo de reemplazo
                        secondChanceMemoria();
                        frameDisponible = ultimoReemplazo;

                    }
                    archivo -> ReadAt(&(machine -> mainMemory[frameDisponible*PageSize]), PageSize, currentThread -> space -> encabezado.code.inFileAddr+numPag*PageSize);
                    currentThread->space->pageTable[numPag].physicalPage = frameDisponible;
                    currentThread->space->pageTable[numPag].valid = true;
                    if (direccionLogica < currentThread->space->encabezado.initData.inFileAddr)
                    {
                        //si es pagina de codigo no se deberia poder modificar
                        printf ("ES pagina de codigo \n");
                        currentThread->space->pageTable[numPag].readOnly = true;
                    }
                    else
                    {
                        currentThread->space->pageTable[numPag].readOnly = false;
                    }

                    if (posLibre == -1)
                    {
                        secondChanceTLB();
                    }

                    machine->tlb[indiceTLB].virtualPage = currentThread->space->pageTable[numPag].virtualPage;
                    machine->tlb[indiceTLB].physicalPage = currentThread->space->pageTable[numPag].physicalPage;
                    machine->tlb[indiceTLB].readOnly = currentThread->space->pageTable[numPag].readOnly;
                    machine->tlb[indiceTLB].valid = true;
                    machine->tlb[indiceTLB].use = currentThread->space->pageTable[numPag].use;
                    machine->tlb[indiceTLB].dirty = currentThread->space->pageTable[numPag].dirty;

                }
                else
                {
                    printf ("es de datos no inicializados o pila");
                    sleep (5);
                    //datos no inicializados o pila
                    frameDisponible = TablaInv->Find();//busca si hay posicion libre en memoria
                    if (frameDisponible == -1)
                    {
                        //pregunta si hay frame libre en memoria
                        //si no hay frame libre, corre algoritmo de reemplazo
                        secondChanceMemoria();
                        frameDisponible = ultimoReemplazo;

                    }
                    currentThread->space->pageTable[numPag].physicalPage = frameDisponible;
                    currentThread->space->pageTable[numPag].valid = true;
                    currentThread->space->pageTable[numPag].readOnly = false;

                    if (posLibre == -1)
                    {
                        secondChanceTLB();
                    }

                    machine->tlb[indiceTLB].virtualPage = currentThread->space->pageTable[numPag].virtualPage;
                    machine->tlb[indiceTLB].physicalPage = currentThread->space->pageTable[numPag].physicalPage;
                    machine->tlb[indiceTLB].readOnly = currentThread->space->pageTable[numPag].readOnly;
                    machine->tlb[indiceTLB].valid = true;
                    machine->tlb[indiceTLB].use = currentThread->space->pageTable[numPag].use;
                    machine->tlb[indiceTLB].dirty = currentThread->space->pageTable[numPag].dirty;

                }

            }
        }
        break;
    default:
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(false);
        break;
    }
}


