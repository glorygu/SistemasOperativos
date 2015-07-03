

#include "bitmap.h"
#define maxSize 256
#include <iostream>

using namespace std;
class NachosHiloTabla
{
public:
    NachosHiloTabla( );        // Initialize
    ~NachosHiloTabla();      // De-allocate

    int CreateHilo( long HiloHandle ); // Se registra el semaforo
    int DestroyHilo(int NachosHiloHandle );      // se elimina la referencia del semaforo
    bool isOpened( int NachosHiloHandle );
    long getHiloHandle( int NachosHiloHandle );
    void addThread ( );  //Semaphore * semHilo;

private:
    long* openHilo;		// A vector with user opened files
    BitMap * openHiloMap;	// A bitmap to control our vector
    		// How many threads are using this table
    // int openSemCounter;
    int usage;

};
