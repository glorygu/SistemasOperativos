#include tablainvertida.h

TablaInvertida::TablaInvertida(int n)
{
    entradas = new EntradaInvertida[n];
    for (int i = 0; i < n; i++)
    {
        entradas[i] = new EntradaInvertida();
    }
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
    while (i < entradas.size() && !encontrado)
    {
        if (entradas[i] -> valida = true)
        {
            indice = i;
            encontrado = true;
        }
        i++;
    }
    return indice;
}
