#ifndef SWAP_H
#define SWAP_H

#include "openfile.h"
#include <vector>
#include <string>
#include <cstdlib>
#include "bitmap.h"
#define pageNum 64


class Swap{

 public:

  Swap();
  ~Swap();

  bool isSwaped(int page);
  bool sacarDelSwap(int page, int frame);
  int meterAlSwap(int frame);

  BitMap * bitmapSwap; 
 private:


  OpenFile* archivoSwap;
  std::string nombreArchivoSwap;

};

#endif
