#include "system.h"
#include "syscall.h"
#include "swap.h"
#include <sstream>
#include "machine.h"

//definir pageNum como 64 adonde no se, puede ser en system o machine
//crear el swap en system y definirlo por todo lado
Swap::Swap( ){

  std::stringstream nameStream;
  nameStream << "Swap.";
  nombreArchivoSwap = nameStream.str();

  fileSystem->Create((char*)nombreArchivoSwap .c_str(), pageNum * PageSize);
  archivoSwap = fileSystem->Open((char*)nombreArchivoSwap .c_str());
  //bitmapSwap = new BitMap (pageNum);

}

Swap::~Swap(){
  //delete archivoSwap;
  //fileSystem->Remove(archivoSwapName.c_str());
}
 bool Swap::isSwaped(int page){
   
    return bitmapSwap->Test(page);
  }
// Saca una pagina de swap a memoria
//page es la pagina a sacar del swap y frame es el frame en memoria donde se va a meter la informacion de page
bool Swap::sacarDelSwap(int page, int frame){


  int cantidadInfoLeida;
  char* dest = machine->mainMemory + frame * PageSize; //no es necesario si tenemos la tabla invertida xq si se tiene le damos ipt[frame]

 cantidadInfoLeida = archivoSwap->ReadAt(dest, PageSize, page * PageSize); //carga a memoria
//devuelve la cantidad de caracteres pasados a la memoria
  bool leyoBien = (cantidadInfoLeida == PageSize);//manejo de errores
  //verifica que lo que leyo sea del mismo tamaño que PageSize
  if (leyoBien){

	currentThread->space->pageTable[page].valid = true;
	currentThread->space->pageTable[page].dirty =true;
  }

  return leyoBien;
}


//Mete una pagina de memoria al swap
//Recibe el frame de memoria a meter al swap
bool Swap::meterAlSwap(int frame){


  int page = 0;//space->getPageNumber(frame);
//saca de la tabla invertida

  TranslationEntry entrada = currentThread->space->pageTable[page];
//el dirty lo hacemos afuera
  if ( !isSwaped(page)){ //preguntar si no esta en el swap

    int charsWritten;
    char* source = machine->mainMemory + frame * PageSize; //lo que se va a mentar

    charsWritten = archivoSwap->WriteAt(source, PageSize, page * PageSize); //se trae de memoria y no lo quita
    ASSERT(charsWritten == PageSize);
  }
//se hace afuera, la parte del entry esta en exception
/*  entry.valid = false;
  entry.dirty = false;
  entry.physicalPage = -1;
  swapMap[page] = true;
  coremap->usedFrames[frame] = true;

  space->savePageTableEntry(entry, entry.virtualPage);
*/
  return true;
}
