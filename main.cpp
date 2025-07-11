#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>

using namespace std;

#define NOME_FILE "fumetteria.dat"
#define DIM_BLOCCO  80

/*struct that rapresent a ´fumetto´, optimized for optimal access by the cpu,
allowing you to not use pragma pack */
typedef struct {
	int  numero;
	char titolo[26];
	char note[50];
} Fumetto;


/*insert stream of byte in a .dat file*/
void inserisciFumetto(){
	Fumetto fumetto;
	
	cout << "Inserisci il numero del fumetto" << endl;
	cin >> fumetto.numero;
	cin.ignore(80, '\n');
	
	cout << "inserisci il titolo" << endl;
	cin.getline(fumetto.titolo, 26); 
	
	cout << "Inserisci una nota" << endl;
	cin.getline(fumetto.note, 50);
	
	ofstream file;
	file.open(NOME_FILE, ios::binary | ios::app);
	
	//file.write( (char*)&fumetto, sizeof(fumetto) );
	
	file.write(reinterpret_cast<char*>(&fumetto), sizeof(Fumetto));
	
	file.close();
}


/*remove a stream of byte from a .dat file, optimized by working in place and not
creating a temp file to transfer data*/
void rimuoviFumetto() {
	char titoloFumetto[26];
	cout << "Inserisci il titolo del fumetto da rimuovere: ";
	cin.getline(titoloFumetto, 26);

	fstream file;
	file.open(NOME_FILE, ios::out | ios::in | ios::binary);

	if(!file){
		cerr << "Il file non e´ aperto correttamente" << endl;
		return;
	}


	file.seekg(0, ios::end);
	streampos fileSize = file.tellg();
	file.seekg(0, ios::beg);

	int numeroBlocchi = fileSize / DIM_BLOCCO;

	Fumetto fumetto;
	int posizione = -1;
	for(int i=0; i<numeroBlocchi; i++){
		file.read(reinterpret_cast<char*>(&fumetto), sizeof(Fumetto));
		if (strncmp(fumetto.titolo, titoloFumetto, 26) == 0) {
			posizione = i;
			break;
        }
	}

	if(posizione == -1){
		cout << "Fumetto non trovato!" << endl;
		file.close();
		return; 
	}

	int blocchiRimanenti = numeroBlocchi - (posizione+1);
	if(blocchiRimanenti > 0){
		char* buffer = new char[blocchiRimanenti * DIM_BLOCCO];
	
		file.seekg((posizione+1)*DIM_BLOCCO, ios::beg);
		file.read(buffer, blocchiRimanenti*DIM_BLOCCO);

		file.seekp(posizione * DIM_BLOCCO, ios::beg);
		file.write(buffer, blocchiRimanenti * DIM_BLOCCO);

		delete[] buffer;
	}

	file.close();

	streamoff fileSizeInt = static_cast<streamoff>(fileSize);
	off_t sizeNuova = fileSizeInt - DIM_BLOCCO;
	if(truncate(NOME_FILE, sizeNuova) != 0) {
		perror("truncate");
	} else {
		cout << "Fumetto rimosso correttamente" << endl;
	}
}


/*listing the serialized object that are stored in the .dat file*/
void stampaArchivio() {

	ifstream file;
	
	file.open(NOME_FILE, ios::binary | ios::in);
	
	if(!file.is_open()) {
		cerr << "Errore file non aperto" << endl; 
		return;
	}
	

	Fumetto fumetto;
	file.seekg(0, ios::beg);
	
	while( file.read(reinterpret_cast<char*>(&fumetto), sizeof(Fumetto)) ) {
		cout << "\nNumero: " << fumetto.numero << "\nTitolo: " << fumetto.titolo << "\nNote: " << fumetto.note << endl;
	}
	
	file.close();
	return;
}


/*check for the presence of a ´fumetto´ in the file*/
void cercaFumetto() {
    char titoloFumetto[26];
    cout << "Inserisci il titolo del fumetto da cercare: ";
    cin.getline(titoloFumetto, 26);

    ifstream file(NOME_FILE, ios::binary);
    
    if (!file.is_open()) {
        cerr << "Errore nell'aprire il file!" << endl;
        return;
    }

    Fumetto fumetto;
    file.seekg(0, ios::beg);

    while (file.read(reinterpret_cast<char*>(&fumetto), sizeof(Fumetto))) {
        // Confronta i primi 26 byte del titolo del fumetto con quello inserito
        if (strncmp(fumetto.titolo, titoloFumetto, 26) == 0) {
            // Titolo trovato, stampa le informazioni
            cout << "\nFumetto trovato!" 
                 << "\nNumero: " << fumetto.numero 
                 << "\nTitolo: " << fumetto.titolo 
                 << "\nNote: "   << fumetto.note << endl;
            file.close();
            return;
        }
    }

    cout << "Fumetto non trovato!" << endl;
    file.close();
	return;
}




int main() {
	int scelta;

	cout << "Inserisci scelta" << endl;
	cout << "1. Inserisci fumetto" << endl;
	cout << "2. Rimuovi fumetto" << endl;
	cout << "3. cerca fumetto" <<  endl;
	cout << "4. stampa archivio" <<  endl;
	cout << "0. esci" <<  endl;
	
	do {
		cin >> scelta;
		cin.ignore(4,'\n');
		
		switch(scelta){
			case 1:
				inserisciFumetto();
				break;
			case 2:
				rimuoviFumetto();
				break;
			case 3:
				cercaFumetto();
				break;
			case 4:
				stampaArchivio();
				break;
		}
	} while(scelta != 0);
	
	return 0;
}