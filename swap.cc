#include "system.h"
#include "syscall.h"
#include "swap.h"
#include <sstream>
//definir pageNum como 64 adonde no se, puede ser en system o machine
//crear el swap en system y definirlo por todo lado 
Swap::Swap(SpaceId sid, int pageNum){
  
  std::stringstream nameStream;
  nameStream << "Swap." << sid;
  swapFileName = nameStream.str();

  fileSystem->Create(swapFileName.c_str(), pageNum * PageSize);
  swapFile = fileSystem->Open(swapFileName.c_str());
  bitmapSwap = new BitMap (pageNum);
  //swapMap.resize(pageNum, false);
  id = sid;
}

Swap::~Swap(){
  delete swapFile;
  fileSystem->Remove(swapFileName.c_str());
}

// Saca una pagina de swap a memoria  
bool Swap::sacarDelSwap(int page, int frame){
  

  int charsRead;
  char* dest = machine->mainMemory + frame * PageSize; //no es necesario si tenemos la tabla invertida xq si se tiene le damos ipt[frame]

  charsRead = swapFile->ReadAt(dest, PageSize, page * PageSize); //carga a memoria
//devuelve la cantidad de caracteres pasados a la memoria
  bool res = (charsRead == PageSize);//manejo de errores
  
  if (res){

	currentThread->space->PageTable[page].valid = true; 
	currentThread->space->PageTable[page].dirty =true; 
  }

  return res;
}


// Save a page from main memory into swap file
bool Swap::meterAlSwap(int frame){

  ASSERT(frame >= 0 && frame < NumPhysPages);

  AddrSpace* space = ((Thread*)id)->space;

  int page = space->getPageNumber(frame);
//saca de la tabla invertida 
  if (page == -1){
    ASSERT(false);
    return false;
  }

  TranslationEntry entrada = currentThread->space->PageTable[page];
//el dirty lo hacemos afuera 
  if (entry.dirty || !isSwaped(page)){ //preguntar si no esta en el swap 
    
    int charsWritten;
    char* source = machine->mainMemory + frame * PageSize; //lo que se va a mentar

    charsWritten = swapFile->WriteAt(source, PageSize, page * PageSize); //se trae de memoria y no lo quita
    ASSERT(charsWritten == PageSize);
  }
//se hace afuera, la parte del entry esta en exception
  entry.valid = false;
  entry.dirty = false;
  entry.physicalPage = -1;
  swapMap[page] = true;
  coremap->usedFrames[frame] = true;

  space->savePageTableEntry(entry, entry.virtualPage);

  return true;
}
