#include "nachoshilotabla.h"

NachosHiloTabla::NachosHiloTabla ()
{
    openHiloMap = new BitMap (maxSize);
    openHilo = new long ( );

}

NachosHiloTabla::~NachosHiloTabla ()
{

}

void NachosHiloTabla::addThread ( ){
    usage++;
}
int NachosHiloTabla::CreateHilo( long HiloHandle )
{


//int NachosHandle = open (UnixHiloHandle);
//si devuelve -1 es porque hubo error
    int NachosHandle = openHiloMap->Find ( ); //siguiente posicion libre en el vector
    openHilo [NachosHandle] = HiloHandle;
    return NachosHandle;
}// Register the file handle

int NachosHiloTabla::DestroyHilo( int NachosHandle )
{
    if (isOpened (NachosHandle))
    {
        openHilo[NachosHandle] = 0; //Se tiene que hacer close al unixHandle en algun lado
        openHiloMap [NachosHandle] = 0;
    }
    return openHilo[NachosHandle] ;
}     // Unregister the file handle


bool NachosHiloTabla::isOpened( int NachosHandle )
{
//recorre el vector para ver si el NachosHandle que entra
//como parámetro está en el vector de archivos abiertos
    bool abierto = false;
    if (openHilo[NachosHandle]!=0)
    {
        abierto = true;
    }
    return abierto;
}

long NachosHiloTabla::getHiloHandle( int NachosHiloHandle )
{

    return openHilo[NachosHiloHandle];

}


