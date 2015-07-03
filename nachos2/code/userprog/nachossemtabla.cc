#include "nachossemtabla.h"

NachosSemTabla::NachosSemTabla ()
{
    openSemaphoreMap = new BitMap (maxSize);
    openSemaphores = new long ( );

}

NachosSemTabla::~NachosSemTabla ()
{

}


void NachosSemTabla::addThread ()
{
    usage++;
}


int NachosSemTabla::CreateSem( long SemHandle )
{


//int NachosHandle = open (UnixSemHandle);
//si devuelve -1 es porque hubo error
    int NachosHandle = openSemaphoreMap->Find ( ); //siguiente posicion libre en el vector
    openSemaphores [NachosHandle] = SemHandle;
    return NachosHandle;
}// Register the file handle

int NachosSemTabla::DestroySem( int NachosHandle )
{
    if (isOpened (NachosHandle))
    {
        openSemaphores[NachosHandle] = 0; //Se tiene que hacer close al unixHandle en algun lado
        openSemaphoreMap [NachosHandle] = 0;
    }
    return openSemaphores[NachosHandle] ;
}     // Unregister the file handle


bool NachosSemTabla::isOpened( int NachosHandle )
{
//recorre el vector para ver si el NachosHandle que entra
//como parámetro está en el vector de archivos abiertos
    bool abierto = false;
    if (openSemaphores[NachosHandle]!=0)
    {
        abierto = true;
    }
    return abierto;
}

long NachosSemTabla::getSemHandle( int NachosSemHandle )
{

    return openSemaphores[NachosSemHandle];

}

void NachosSemTabla::delThread ()
{
    usage--;
    if (usage == 0)
    {
        for ( int i = 0; i <maxSize; i++)
        {
            if (isOpened(openSemaphores[i]))
            {
                Close(openSemaphores[i]);
            }
        }
    }
}

void NachosSemTabla::Print ( )
{
    for ( int i = 0; i < maxSize; i++)
    {
        if (isOpened(openSemaphores[i]))
        {
            cout<<openSemaphores[i]<<endl;
        }
    }
}
