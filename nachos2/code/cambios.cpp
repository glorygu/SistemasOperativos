cambios exception
bool buscarArchivoAbierto( )
{
    bool encontrado = false;
    for (int i =0; i < 32; i++)
    {
        if (strcmp(ArchivosAbiertos[i],nombreArchivo)==0)
        {
            encontrado = true;
        }
    }
    return encontrado;
}
//----------------------------------------------------------------------
//------------------------------------------------
void
StartProcess(char *filename)
{
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
	printf("Unable to open file %s\n", filename);
	return;
    }
    space = new AddrSpace(executable);    
    currentThread->space = space;

    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register

    machine->Run();			// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
					// the address space exits
					// by doing the syscall "exit"
}

void Nachos_Exec ( )
{//va a devolver un spaceId que es el indice de la tabla de hijos
//del proceso que se acaba de crear
    int spcID = 0;
    int dirNombre = machine->ReadRegister (4);
    printf ("direccion nombre %d", dirNombre);
    bool correcto = true;
    char nombre [100];
    char valorChar = ' ';
    int i = 0;
    int valor;
    int UnixId;
    int codError;
    while (i < 100 && correcto && valorChar!= '\0')
    {
        //se pasa de char a int
        correcto = machine->ReadMem (dirNombre, 1, &valor);
        valorChar = valor;
        if (correcto)
        {
            nombre [i] = valorChar;
            i++;
            dirNombre++;
        }
        else
        {
            cout << "error lectura al abrir"<<endl;
        }
    }
	StartProcess(nombre);
    machine->WriteRegister (2, spcID);
    returnFromSystemCall();
}

