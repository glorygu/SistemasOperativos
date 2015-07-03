#include "system.h"
#include "syscall.h"
#include "swap.h"
#include <sstream>

Swap::Swap(SpaceId sid, int pageNum){
  
  std::stringstream nameStream;
  nameStream << "Swap." << sid;
  swapFileName = nameStream.str();

  fileSystem->Create(swapFileName.c_str(), pageNum * PageSize);
  swapFile = fileSystem->Open(swapFileName.c_str());
  bitmapSwap = new BitaMap (pageNum);
  swapMap.resize(pageNum, false);
  id = sid;
}

Swap::~Swap(){
  delete swapFile;
  fileSystem->Remove(swapFileName.c_str());
}

// Retrieves a page from the swap file into main memory
bool Swap::sacarDelSwap(int page, int frame){
  

  int charsRead;
  char* dest = machine->mainMemory + frame * PageSize;

  charsRead = swapFile->ReadAt(dest, PageSize, page * PageSize);

  bool res = (charsRead == PageSize);
  
  if (res){

	currentThread->space->PageTable[page].valid = true; 
	currentThread->space->PageTable[page].dirty = false; 
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

  if (entry.dirty || !isSwaped(page)){ 
    
    int charsWritten;
    char* source = machine->mainMemory + frame * PageSize;

    charsWritten = swapFile->WriteAt(source, PageSize, page * PageSize);
    ASSERT(charsWritten == PageSize);
  }

  entry.valid = false;
  entry.dirty = false;
  entry.physicalPage = -1;
  swapMap[page] = true;
  coremap->usedFrames[frame] = true;

  space->savePageTableEntry(entry, entry.virtualPage);

  return true;
}