#include <iostream>
#include <fstream>
#include <string>
#include <passe_par_tout.h>
#include "istogramma.cxx"
using namespace std;

ifstream & operator>> (ifstream &file, istogramma &histo)
	{
	for(int i = 0; i < histo.numerodati; i++) file >> histo.arraydati[i];
	return file;
	}

int main()
	{
	string nomefile;
	ifstream file;
	double num, xmin, ymin, xmax, ymax, vertpol[8], x[2];
	int ndati = 0, numdati, l = 1000, h = 900, val, nfin = 0, col[]={0, -1};
	char contr, quote[2], formato[] = {'f', 'f'}, cifre[] = {0, 0}, *assex = new char[1000000], assexx[1000000] = "Asse delle ascisse: ";

//	ACQUISISCI NOME FILE
	cout << "\n\t\t\t\tBENVENUTO!\n\n"
	"Scopo del programma è realizzare un istogramma di dati acquisiti da un file.\n"
	"\n\t\t\t\tIstruzioni:\n-tutti i dati devono essere in formato numerico, separati solo da spazi,\n segni di 'a capo' o tabulazioni;\n-il file in cui sono elencati i dati non deve contenere segni di punteggiatura.\n L'unico segno di punteggiatura riconosciuto dal programma è il punto '.',\n usato per separare le cifre intere dalle cifre decimali;\n-ogni dato coincidente con il valore di confine tra due bin è inserito nel bin\n situato più a destra tra i due;\n-in alcuni punti del programma sarà richiesto l'inserimento di un numero. In tal\n caso è richiesto all'utente l'inserimento di UNO E UN SOLO numero, che non\n contenga spazi tra le sue cifre."
	"\n\nAssicurarsi che il file contenente i dati sia presente nella directory in cui\nsi trova l'eseguibile e indicare il nome del file.\n";
	while(true)
		{
		getline(cin, nomefile);
		file.open(nomefile);
		if (file.fail())
			{
			cout << "Il file non è presente nella directory.\nAssicurarsi che il file sia presente e riprovare.\n"; //se non apre il file dà errore
			continue;
			}
//	CONTA IL NUMERO DI DATI PRESENTI NEL FILE
		while(true)
			{
			file >> num;
			if(file.eof())
				{
				file.clear();
				file.seekg(0, ios::beg);
				break;
				}
			if(!file)
				{
				cout << "Errore nell'acquisizione: il file contiene almeno un dato non valido.\nAssicurarsi che tutti i dati siano in formato numerico e reinserire il nome\ndel file.\n";
				file.close();
				ndati = 0;
				break;
				}
			ndati++;
			}
		if(!file)
			{
			file.clear();
			continue;
			}
		if(ndati == 0)
			{
			cout << "Il file è vuoto. Inserire il nome di un file con valori validi.\n";
			file.clear();
			file.close();
			continue;
			}
		break;
		}

	cout << "\nNel file sono presenti " << ndati << " dati.\n";

//	RICHIEDI QUANTITA' DI DATI DA ACQUISIRE
	cout << "Quanti dati devo acquisire? ";
	while(true)
		{
		cin >> numdati;
		if(!cin || numdati <= 0 || numdati > ndati || cin.get() != '\n')
			{
			cout << "Il valore immesso non è valido.\nInserire UN numero intero positivo non superiore a " << ndati << ".\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
			}
		break;
		}

//	CREA ARRAY CON I DATI ACQUISITI
	istogramma mioisto(numdati);
	file >> mioisto;
	file.clear();
	file.close();

//	TROVA MAX E MIN, RANGE
	while(true)
		{
		mioisto.calcmin();
		mioisto.calcmax();
//		if ((mioisto.restmin() == 0) && (mioisto.restmax() == 0))
//			{
//			cout << "Errore nell'acquisizione: i dati presenti nel file sono tutti nulli.\n";
//			return 0;
//			}
		if (mioisto.calcrange() == 0)
			{
			cout << "I dati presenti nel file sono tutti uguali: non è presente alcuna distribuzione.\n";
			return 0;
			}

		cout
		<< "\nIl valore minimo è " << mioisto.restmin()
		<< ".\nIl valore massimo è " << mioisto.restmax()
		<< ".\nIl range dei valori è " << mioisto.calcrange() << ".\n";

	//	RICHIEDI PER QUANTO SI ESTENDE OGNI BIN
		cout << "Quale deve essere il valore minimo del primo bin? ";
		while(true)
			{
			cin >> num;
			if(!cin || cin.get() != '\n')
				{
				cout << "Il valore immesso non è valido. Inserire UN numero.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
				}
			if(num > mioisto.restmin())
				{
				cout << "E' stato inserito un numero maggiore del dato più piccolo e saranno persi dei\ndati!! Si vuole continuare comunque? ('S' = \"sì\", qualunque altra combinazione\ndi caratteri = \"no, reinserisco il dato\") ";
				cin >> contr;
				if((contr != 's' && contr != 'S') || cin.get() != '\n')
					{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Reinserire il minimo. ";
					continue;
					}
				}
			mioisto.impmin(num);
			break;
			}
			cout << "Quale deve essere il valore massimo dell'ultimo bin? ";
		while(true)
			{
			cin >> num;
			if(!cin || cin.get() != '\n')
				{
				cout << "Il valore immesso non è valido. Inserire UN numero.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
				}
			if(num <= mioisto.restmax())
				{
				cout << "E'stato inserito un numero minore o uguale al dato più grande e saranno persi\ndei dati!! Si vuole continuare comunque? ('S' = \"sì\", qualunque altra combina-\nzione di caratteri = \"no, reinserisco il dato\") ";
				cin >> contr;
				if((contr != 's' && contr != 'S') || cin.get() != '\n')
					{
					cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Reinserire il massimo. ";
					continue;
					}
				}
			mioisto.impmax(num);
			break;
			}
		if (mioisto.restmin() > mioisto.restmax())
			{
			cout << "Qualcosa non va: il minimo inserito è maggiore del massimo inserito!!\nReinserire i valori.";
			continue;
			}
		break;
		}
	cout << "Il nuovo range è " << mioisto.calcrange() << ".\n";
	cout << "Quanto vale ogni bin? ";
	while(true)
		{
		cin >> num;
		if(!cin || num <= 0 || cin.get() != '\n')
			{
			cout << "Il valore immesso non è valido. Inserire UN numero positivo.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
			}
		break;
		}
	mioisto.imprangebin(num);

//	RICAVA IL NUMERO TOTALE DI BIN E CONTA I VALORI CHE CADONO IN OGNI BIN
	mioisto.ottieninbin();
	mioisto.impvalbin();
	mioisto.contaoccorrenze();



//	PARTE GRAFICA
	xmin = mioisto.restvalbin(0), ymin = 0.0, xmax = mioisto.restvalbin(mioisto.restnbin()), ymax = mioisto.restoccmax()+2;
	quote[0] = mioisto.restnbin()-1, quote[1] = mioisto.restoccmax()+1;
	cout << "\nCi siamo quasi!\nInserire il numero di cifre decimali da usare per le quotature dell'asse x\n(tra 0 e 9 compresi).\n";
	while(true)
		{
		cin >> val;
		if(!cin || val < 0 || val > 9 || cin.get() != '\n')
			{
			cout << "Il valore immesso non è valido. Inserire UN numero intero compreso tra 0 e 9\n(inclusi gli estremi).\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
			}
		break;
		}
	cifre[0] = val;

//	clog << xmin << ' ' << ymin << ' ' <<  xmax << ' ' << ymax << '\n';

	m_startg(const_cast<char *>("Istogramma"), &l, &h);
	h = l = 95;
	h|=RICHIEDO_LEGENDA;
	m_window(&l, &h);
	
//	DISEGNA COORDINATE
	val = 18;
	m_frame(&xmin, &ymin, &xmax, &ymax);
	m_quote(&nfin, quote, formato, cifre, &val);
	
//	DISEGNA RETTANGOLI E LINEE DI CONTORNO
	val = 1;
	m_color(&val);
	val = 4;
	for(int i=0; i < mioisto.restnbin(); i++)
		{
		vertpol[0]=vertpol[2]=mioisto.restvalbin(i), vertpol[1]=vertpol[7]=0, vertpol[3]=vertpol[5]=mioisto.restoccorrenze(i), vertpol[4]=vertpol[6]=mioisto.restvalbin(i+1);
		m_polygon(vertpol, &val);
//		clog << vertpol[0] <<";"<< vertpol[2] <<";"<< vertpol[3] <<";"<< vertpol[5] <<";"<< vertpol[4] <<";"<< vertpol[6]<< endl;
		}
	val = 0;
	m_color(&val);
	for(int i=0; i < mioisto.restnbin(); i++)
		{
		x[0] = mioisto.restvalbin(i), x[1] = 0;
		m_move(x);
		x[1] = mioisto.restoccorrenze(i);
		m_line(x);
		x[0] = mioisto.restvalbin(i+1);
		m_line(x);
		x[1] = 0;
		m_line(x);
		}

	l = EVENTO_ENTER;
	m_wait_for_events(&l);

//	CREA LEGENDA
	h = 0;
	assex = new char[1000000];
	cout << "\nInserire una legenda per i dati sull'asse delle ascisse, seguita da uno spazio\ne da 'S\\' (qualora si inserisca solo 'S\\', la legenda non sarà mostrata).\n";
	while(true)
		{
		cin >> assex;
		if(assex[0] == 'S' && assex[1] == '\\' && assex[2] == 0) break;
		strcat(assexx, assex);
		strcat(assexx, " ");
		h++;
		}
	const char *legenda[] = {assexx, "Asse delle ordinate: Occorrenze", "\0"};

	if(h != 0)
		{
		cout << "Dove si vuole inserire la legenda? (1=in alto a sinistra, 2=in alto a destra) ";
		while(true)
			{
			cin >> h;
			if(h != 1 && h != 2)
				{
				cout << "\nIl comando inserito non è valido, battere 1 per inserire la legenda in alto\na sinistra, 2 in alto a destra.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
				}
			break;
			}
		switch(h)
			{
			case 1: m_legend(&nfin, const_cast<char **>(legenda), NULL, NULL, col);
				break;
			case 2: x[0] = xmax, x[1] = ymax;
				val = 6;
				m_legend(&nfin, const_cast<char **>(legenda), x, &val, col);
				break;
			}
		}

	cout << "\nEcco l'istogramma terminato! Seguire le istruzioni per chiudere il programma.\n";
//	  TERMINA PROGRAMMA
	m_wait_for_events(&l);
	m_endg();
	}
