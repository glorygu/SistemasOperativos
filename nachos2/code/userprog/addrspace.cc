// addrspace.cc
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
//#include "noff.h"

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader *noffH)
{
    noffH->noffMagic = WordToHost(noffH->noffMagic);
    noffH->code.size = WordToHost(noffH->code.size);
    noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
    noffH->initData.size = WordToHost(noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost(noffH->uninitData.size);
    noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable)
{
    NoffHeader noffH; //Data structures defining the Nachos Object Code Format

    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0); //noffH es de donde va a leer, lee tantos bytes como el sizeof de noffh, empezando en la posicion 0
    if ((noffH.noffMagic != NOFFMAGIC) && (WordToHost(noffH.noffMagic) == NOFFMAGIC)) //NOFFMAGIC 0xbadfad
        //deberia ser true

        SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);
	archivoEjecutable = executable;
// how big is address space?
	encabezado = noffH;
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size
           + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    ASSERT(numPages <= NumPhysPages);		// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG('a', "Initializing address space, num pages %d, size %d\n",
          numPages, size);
// first, set up the translation
    pageTable = new TranslationEntry[numPages];
    int j = 0;


    // j = 0; j es la primera libre del bitmap
    for (i = 0; i < numPages; i++)
    {
        pageTable[i].virtualPage = i;
        j = MiMapa->Find ( );
        pageTable[i].physicalPage = j;
        pageTable[i].valid = true;
        pageTable[i].use = false;
        pageTable[i].dirty = false ;
        pageTable[i].readOnly = false;  // if the code segment was entirely on
        // a separate page, we could set its
        // pages to be read-only
    }

// zero out the entire address space, to zero the unitialized data segment
// and the stack segment
//    bzero(machine->mainMemory, size);
// Linea agregada por farroyo
//      memset(machine->mainMemory, 0, size);

// then, copy in the code and data segments into memory
    int tamArchivo = noffH.code.size + noffH.initData.size;
    int numPagsArchivo = tamArchivo / PageSize + 1;

    for (int k = 0; k < numPagsArchivo; k++)
    {

        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n",
              noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[pageTable[k].physicalPage*PageSize]),
                           PageSize, noffH.code.inFileAddr+PageSize*k);

    }




    /*    	for (int i = 0; i < numPages; i++){
            	DEBUG('a', "Initializing data segment, at 0x%x, size %d\n",
    			noffH.initData.virtualAddr, noffH.initData.size);
            	executable->ReadAt(&(machine->mainMemory				[noffH.initData.virtualAddr]),
    			128, noffH.initData.inFileAddr+128*i);
        	}
    */
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------
AddrSpace::AddrSpace (AddrSpace *espacioPapa){
//hacer pageTable del mismo tamaño de la del papa y copiar todo hasta la pila
	unsigned int tam = espacioPapa->numPages;
    pageTable = new TranslationEntry [tam];
	this->pageTable = espacioPapa->pageTable;
    int j = 0;
    int pagPila = UserStackSize/PageSize;
    for (unsigned int i = 0; i < pagPila; i++){
    //reserva espacio para la pila

        j = MiMapa->Find ( );
        pageTable[tam-pagPila+i].physicalPage = j;
        pageTable[tam-pagPila+i].valid = true;
        pageTable[tam-pagPila+i].use = false;
        pageTable[tam-pagPila+i].dirty = false ;
        pageTable[tam-pagPila+i].readOnly = false;
    }
}
AddrSpace::~AddrSpace()
{
    delete pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
