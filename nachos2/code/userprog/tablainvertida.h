#ifndef TABLAINVERTIDA_H
#define TABLAINVERTIDA_H

#include entradainvertida.h

class TablaInvertida {
  public:
    TablaInvertida(int n);
    ~TablaInvertida();
    void Mark(int n);
    void Clear(int n);
    int Find();

  private:
    EntradaInvertida[] entradas;
};

#endif // TABLAINVERTIDA_H
