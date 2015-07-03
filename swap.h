#ifndef SWAP_H
#define SWAP_H

#include "openfile.h"
#include <vector>
#include <string>
#include "bitmap.h"
typedef int SpaceId;


class Swap{

 public:

  Swap(SpaceId id, int pageNum);
  ~Swap();
  
  bool isSwaped(int page){
    ASSERT(page >= 0 && page < swapMap.size());
    return swapMap[page];
  }

  bool sacarDelSwap(int page, int frame);
  bool meterAlSwap(int frame);
  BitMap * bitmapSwap;
 private:
  
  SpaceId id;
  OpenFile* swapFile;
  std::string swapFileName;
  
};

#endif