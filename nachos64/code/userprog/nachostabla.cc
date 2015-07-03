#include "nachostabla.h"

NachosTabla::NachosTabla (){
	openFilesMap = new BitMap (maxSize);
	openFiles = new int ( );
	//openFileCounter = 0;
}

NachosTabla::~NachosTabla (){

}


void NachosTabla::addThread (){
	usage++;
}


int NachosTabla::Open( int UnixHandle ){


//int NachosHandle = open (UnixHandle);
//si devuelve -1 es porque hubo error

int NachosHandle = openFilesMap->Find ();
if (NachosHandle < 3){
    NachosHandle = 3;
}
printf ("NachosTabla \n");
openFiles [NachosHandle] = UnixHandle;
//openFileCounter++;
return NachosHandle;
}// Register the file handle

int NachosTabla::Close( int NachosHandle ){
	if (isOpened (NachosHandle)){
		openFiles[NachosHandle] = 0; //Se tiene que hacer close al unixHandle en algun lado
		openFilesMap[NachosHandle] = 0;
	}
	return openFiles[NachosHandle] ;
}     // Unregister the file handle


bool NachosTabla::isOpened( int NachosHandle ){
//recorre el vector para ver si el NachosHandle que entra
//como parámetro está en el vector de archivos abiertos
	bool abierto = false;
	if (openFiles[NachosHandle]!=0){
		abierto = true;
	}
	return abierto;
}

int NachosTabla::getUnixHandle( int NachosHandle ){

return openFiles[NachosHandle];

}

void NachosTabla::delThread (){
	usage--;
	if (usage == 0){
		for ( int i = 0; i <maxSize;i++){
			if (isOpened(openFiles[i])){
				Close(openFiles[i]);
			}
		}
	}
}

void NachosTabla::Print ( ){
			for ( int i = 0; i < maxSize;i++){
			if (isOpened(openFiles[i])){
				cout<<openFiles[i]<<endl;
			}
}
}
