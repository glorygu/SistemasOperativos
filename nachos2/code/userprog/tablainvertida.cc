#include "tablainvertida.h"

TablaInvertida::TablaInvertida(int n)
{
	tam = n;
    entradas = new EntradaInvertida[tam];
}

TablaInvertida::~TablaInvertida()
{

}

void TablaInvertida::Mark(int n)
{
    entradas[n].valida = true;
}

void TablaInvertida::Clear(int n)
{
    entradas[n].valida = false;
}

int TablaInvertida::Find()
{
    int indice = -1;
    int i = 0;
    bool encontrado = false;
    while (i < tam && !encontrado)
    {
        if (entradas[i].valida = true)
        {
            indice = i;
            encontrado = true;
        }
        i++;
    }
    if (encontrado)
    {
	Mark(indice);
    }
    return indice;
}
