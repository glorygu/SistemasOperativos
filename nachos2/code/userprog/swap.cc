#include "system.h"
#include "syscall.h"
#include "swap.h"
#include <sstream>
#include "machine.h"


Swap::Swap( ){

  std::stringstream nameStream;
  nameStream << "Swap.";
  nombreArchivoSwap = nameStream.str();
  bitmapSwap = new BitMap(64);
  fileSystem->Create((char*)nombreArchivoSwap .c_str(), pageNum * PageSize);
  archivoSwap = fileSystem->Open((char*)nombreArchivoSwap .c_str());


}

Swap::~Swap(){

}

// Saca una pagina de swap a memoria
//page es la pagina a sacar del swap y frame es el frame en memoria donde se va a meter la informacion de page
bool Swap::sacarDelSwap(int pos, int frame){

  int page = abs (pos);//saca valor absoluto de pos
  int cantidadInfoLeida;
  char* dest = machine->mainMemory + frame * PageSize; 
  cantidadInfoLeida = archivoSwap->ReadAt(dest, PageSize, page * PageSize); //carga a memoria
//devuelve la cantidad de caracteres pasados a la memoria
  bool leyoBien = (cantidadInfoLeida == PageSize);
  if (leyoBien){

	currentThread->space->pageTable[page].valid = true;
	currentThread->space->pageTable[page].dirty =true;
  }

  return leyoBien;
}


//Mete una pagina de memoria al swap
//Recibe el frame de memoria a meter al swap
int Swap::meterAlSwap( int frame){

int pos = bitmapSwap->Find( );
   if (pos > 0){
    char* source = machine->mainMemory + frame * PageSize; //lo que se va a meter

    charsWritten = archivoSwap->WriteAt(source, PageSize, pos * PageSize); //se trae de memoria y no 
    }

  return pos;
}
