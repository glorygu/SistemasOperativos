#ifndef TABLAINVERTIDA_H
#define TABLAINVERTIDA_H

#include "entradainvertida.h"

class TablaInvertida {
  public:
    TablaInvertida(int n);
    ~TablaInvertida();
    void Mark(int n);
    void Clear(int n);
    int Find();
    EntradaInvertida* entradas;
    int tam;	
  private:
    
  
};

#endif // TABLAINVERTIDA_H
