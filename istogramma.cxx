class istogramma
	{
//	ATTRIBUTI PRIVATI
	int *occorrenze, numerodati, nbin, occorrenzamax;
	double *arraydati, *valbin, min, max, range, rangebin;
	
	friend ifstream & operator>> (ifstream &, istogramma &);
	
	public:
//	FUNZIONI PER GESTIONE ARRAY
	double ricevidato(int i, double d)
		{
		arraydati[i] = d;
		return arraydati[i];
		}
	double restdato(int i)
		{return arraydati[i];}
	int restnumerodati()
		{return numerodati;}
//	FUNZIONI PER MANIPOLAZIONE DATI
	double calcmin()
		{
		min = arraydati[0];
		for(int i = 0; i < numerodati; i++) if(arraydati[i] < min) min = arraydati[i];
		return min;
		}
	double calcmax()
		{
		max = arraydati[0];
		for(int i = 0; i < numerodati; i++) if(arraydati[i] > max) max = arraydati[i];
		return max;
		}
	double calcrange()
		{return range = max-min;}
	void impmin(double d)
		{min = d;}
	void impmax(double d)
		{max = d-0.000000000001;}
	double restmin()
		{return min;}
	double restmax()
		{return max;}
	void imprangebin(double a)
		{rangebin = a;}
	double restrangebin()
		{return rangebin;}
	int ottieninbin()
		{
		nbin = (range/rangebin)+0.999999999999;
		if(min+nbin*rangebin == max) nbin++;
		return nbin;
		}
	int restnbin()
		{return nbin;}
	void impvalbin()
		{
		valbin = new double[nbin+1];
		valbin[0] = min;
		for(int i = 1; i <= nbin; i++) {valbin[i] = valbin[i-1] + rangebin;}
		}
	double restvalbin(int i)
		{return valbin[i];}
	int contaoccorrenze()
		{
		occorrenze = new int[nbin];
		int a;
		for(int i = 0; i < nbin; i++)
			{
			a = 0;
			for(int j = 0; j < numerodati; j++) 
				{
				if((arraydati[j] >= valbin[i])&&(arraydati[j] < valbin[i+1])) a++;
			
				}
			occorrenze[i] = a;
//			if(occorrenze[i] != 1) cout << "Nell'intervallo tra " << valbin[i] << " e " << valbin[i+1] << " ci sono " << occorrenze[i] << " dati.\n";
//			else cout <<"Nell'intervallo tra " << valbin[i] << " e " << valbin[i+1] << " c'è " << occorrenze[i] << " dato.\n";
			}
		occorrenzamax = occorrenze[0];
		for(int i = 1; i < nbin; i++) if(occorrenze[i] > occorrenzamax) occorrenzamax = occorrenze[i];
//		clog << occorrenzamax << '\n';
		}
	int restoccorrenze(int i)
		{return occorrenze[i];}
	int restoccmax()
		{return occorrenzamax;}
//	COSTRUTTORE
	istogramma(int n)
		{
		numerodati = n;
		arraydati = new double[numerodati];
		}
	};

