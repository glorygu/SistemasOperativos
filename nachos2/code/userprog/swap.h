#ifndef SWAP_H
#define SWAP_H

#include "openfile.h"
#include <vector>
#include <string>
//#include "bitmap.h"
#define pageNum 64


class Swap{

 public:

  Swap();
  ~Swap();

  bool isSwaped(int page);
  bool sacarDelSwap(int page, int frame);
  bool meterAlSwap(int frame);
  //BitMap * bitmapSwap;
 private:


  OpenFile* archivoSwap;
  std::string nombreArchivoSwap;

};

#endif
