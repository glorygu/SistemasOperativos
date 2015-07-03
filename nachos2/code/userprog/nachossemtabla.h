#include "bitmap.h"
#define maxSize 256
#include <iostream>
using namespace std;
class NachosSemTabla
{
public:
    NachosSemTabla( );        // Initialize
    ~NachosSemTabla();      // De-allocate

    int CreateSem( long SemHandle ); // Se registra el semaforo
    int DestroySem(int NachosSemHandle );      // se elimina la referencia del semaforo
    bool isOpened( int NachosSemHandle );
    long getSemHandle( int NachosSemHandle );
    void addThread();		// se añada un thread si este usa el semaforo
    void delThread();
    void Print();               // Print contents

private:
    long* openSemaphores;		// A vector with user opened files
    BitMap * openSemaphoreMap;	// A bitmap to control our vector
    int usage;			// How many threads are using this table
    // int openSemCounter;

};
