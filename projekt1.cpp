#include <iostream>

using namespace std;

const int N = 12;//liczba miesiecy - stala

const int M = 10000;

class Kredyt {
    private:
        double dlug;
        int pozostale_raty;
    public:
        Kredyt(double kwota, int czas_splaty) {//kwota-kwota kredytu
            dlug = kwota;
            pozostale_raty = czas_splaty;
        }

        Kredyt(){}//konstruktor domyslny

        double splac_rate() {
            if (pozostale_raty == 0) {
                return 0;
            }
            double rata_podst = dlug / pozostale_raty;//podstawowa rata(kapitałowa) ile winny jest
            double odsetki = dlug * 0.12 / 12;//dodatek z odsetek
            double rata_laczna = rata_podst + odsetki;
            dlug = dlug - rata_podst;
            pozostale_raty--;
            return rata_laczna;
        }

        void drukuj_kredyt() {
            cout << "dlug: " << dlug << ", pozostale raty: " << pozostale_raty << endl;
        }

        double get_dlug() {
            return dlug;
        }
};

class Pracownik {
    protected:
        string imie;
    public:
        virtual void print() = 0;
        virtual int pensja() = 0;
        Pracownik(string nowe_imie) : imie(nowe_imie) {};       
};

class Inz :public Pracownik {
    private:
        string wydzial;
    public:
        static int CI;//stala wydajnosci
        void print() { cout << "inz " << imie <<", wydzial: "<<wydzial<<endl;}
        int pensja() { 
            return 450;// returnuje wartosc z gory zalozonej pensji  
        };
        Inz(string nowe_imie, string wydzial) : Pracownik(nowe_imie) { this->wydzial = wydzial; }
};
int Inz::CI = 300;

class Mkt :public Pracownik {
    private:
        int follows;
    public:
        static int Cmkt;//stala wydajnosci
        void print() { cout << "Mkt " << imie << ", follows: " << follows << endl; }
        int pensja() { 
            return 400; // returnuje wartosc z gory zalozonej pensji  
        };
        Mkt(string nowe_imie, int follows) : Pracownik(nowe_imie) { this->follows = follows; }
};
int Mkt::Cmkt = 125;

class Mag :public Pracownik {
    private:
        bool obsl_widl;
    public:
        static int CMag;//stala wydajnosci
        void print() { cout << "Mag " << imie << ", obsl_widl: " << obsl_widl<< endl; }//override
        int pensja() { 
            return 250; // returnuje wartosc z gory zalozonej pensji  
        };
        Mag(string nowe_imie, bool obsl_widl) : Pracownik(nowe_imie) { this->obsl_widl = obsl_widl; }
};
int Mag:: CMag = 150;

class Rob:public Pracownik {
    private:
        double but;
    public:
        static int CRob;//stala wydajnosci
        void print() { cout << "Rob " << imie << ", but: " << but << endl; }//override
        int pensja() { 
            return 300; // returnuje wartosc z gory zalozonej pensji  
        };
        Rob(string nowe_imie, double but) : Pracownik(nowe_imie) { this->but = but; }
};
int Rob::CRob = 75;

class Firma {
    private:
               
        double stan_konta;
        int n_kredytow; int pojemnosc_kredyty;
        Kredyt* tab_kredytow; 
        int n_prac;
        int pojemnosc_prac;//size, capacity
        Pracownik** tab_prac;//wskaznik na pracownika i na tablice
        double historia_przych [N];
        int gdzie_wstawiac_przychod;
        
    public:

        static const int Kwota_poczatkowa;
        Firma() {
            pojemnosc_kredyty = 4;
            pojemnosc_prac = 4;
            n_kredytow = 0;
            n_prac = 0;
            tab_kredytow = new Kredyt[pojemnosc_kredyty];
            tab_prac = new Pracownik *[pojemnosc_prac];
            stan_konta = Kwota_poczatkowa;
            gdzie_wstawiac_przychod = 0;
            for (int i = 0; i < N; i++) {//zerujemy poczatkowa wartosc firmy w kazdym miesiacu
                historia_przych[i] = 0;
            }
        }
        const void drukuj_pracownikow() {
            for (int i = 0; i < n_prac; i++) {
                tab_prac[i]->print();
            }
        }

        void drukuj_kredyty() {
            for (int i = 0; i < n_kredytow; i++) {
                tab_kredytow[i].drukuj_kredyt();
            }
        }

        void wez_kredyt(double kwota, int czas_splaty) {
            Kredyt nowy_kredyt(kwota, czas_splaty);
            if (n_kredytow < pojemnosc_kredyty){
                tab_kredytow[n_kredytow] = nowy_kredyt;
                n_kredytow++;
            }
            else{
                Kredyt* tab_pom = new Kredyt [2* pojemnosc_kredyty];
                for (int i = 0; i < n_kredytow; i++) {
                    tab_pom[i] = tab_kredytow[i];//kopiowanie kredytow
                }
                tab_pom [n_kredytow] = nowy_kredyt;
                n_kredytow++;
                delete[] tab_kredytow;
                tab_kredytow = tab_pom;//przekierowanie wskaznika na nową tablicę
                pojemnosc_kredyty = pojemnosc_kredyty * 2;
            }
            stan_konta += kwota;//kwota kredytu;
        }
            
        void zatrudnij(Pracownik* nowy_prac) {
                if (n_prac < pojemnosc_prac) {
                    tab_prac[n_prac] = nowy_prac;
                }
                else {
                    pojemnosc_prac = pojemnosc_prac * 2;
                    Pracownik** nowa_tab_prac = new Pracownik * [pojemnosc_prac];
                    for (int i = 0; i < n_prac; i++) {
                        nowa_tab_prac[i] = tab_prac[i];
                    }
                    nowa_tab_prac [n_prac] = nowy_prac;
                    delete tab_prac;
                    tab_prac = nowa_tab_prac;
                }
            n_prac++;
        }

        void zaplac_wynagrodzenie() {
            int suma_pensji = 0;
            for (int i = 0; i < n_prac; i++) {
                suma_pensji+=tab_prac[i]->pensja();
            }
            stan_konta -= suma_pensji;
        }

        void otrzymaj_przychod() {
            double przychod=oblicz_przychoody();
            historia_przych[gdzie_wstawiac_przychod] = przychod;
            gdzie_wstawiac_przychod = (gdzie_wstawiac_przychod + 1) % N;
            stan_konta += przychod;
        }

        const double oblicz_przychoody() {//sprzedarz produktow
        
            int teoretyczna_liczba_prodoktow= licznik_pracownikow <Rob>() * Rob::CRob;//Rob * CRob
            int fakt_liczba_wyprodukowanych = minimum(pojemnosc_magazynu(),teoretyczna_liczba_prodoktow);
            int liczba_sprzedanych = minimum(popyt(), fakt_liczba_wyprodukowanych);
            double przychod_firmy = liczba_sprzedanych * cena_produktu();
            return przychod_firmy;
        }

        int minimum(int a, int b) {
            if (a < b) {
                return a;
            }
            else
                return b;
        }

        void splac_raty() {//splac raty musi odejmować od siebie sumę rat
            double do_splaty;
            double wszystkie_raty=0;
            for (int i = 0; i < n_kredytow; i++) {
                do_splaty=tab_kredytow[i].splac_rate();
                std:: cout << "splacam kwote: " << do_splaty<<endl;
                wszystkie_raty += do_splaty;
            }
            stan_konta = stan_konta - wszystkie_raty;
        }

        const double get_stan_konta() {
            return stan_konta;
        }

        double wartosc_firmy() {
            double suma_przychodow=0;
            for (int i = 0; i < N; i++) {
                suma_przychodow += historia_przych[i];
            }
            double wart_firmy = suma_przychodow / N;
            return wart_firmy;
        }

        void drukuj_stan_firmy() {
            cout << "stan firmy: "<<endl;
            cout << "lista pracownikow: " << endl;
            drukuj_pracownikow();
            cout << "wziete kredyty "<<endl;
            drukuj_kredyty();
            cout << endl;
            std::cout<<"wartosc firmy "<< wartosc_firmy();
            cout << endl;
            std::cout <<"stan konta "<< get_stan_konta();
        }

        template <typename Rodzaj_pracowanika>

        int licznik_pracownikow() {
            int licznik_prac=0;
            for (int i = 0; i < n_prac; i++) {
                Rodzaj_pracowanika* prac = dynamic_cast <Rodzaj_pracowanika*> (tab_prac[i]);//rzutowanie rodzajow pracownikow na pracownika
                if(prac!=0 ){
                    licznik_prac ++;
                }
            }
            return licznik_prac;
        }

        double cena_produktu() {
            double cena_prod = licznik_pracownikow <Inz>() * Inz::CI;
            return cena_prod;
        }

        int pojemnosc_magazynu() {
            int pojemnosc_mag = licznik_pracownikow <Mag>() * Mag::CMag;//mag * CMag
            return pojemnosc_mag;
        }

        int popyt() {
            int popyt_ = licznik_pracownikow <Mkt>() * Mkt::Cmkt;//Mkt * Cmkt
            return popyt_;
        }

        double zadluzenie() {
            double suma_zadluzenia = 0;
            for (int i = 0; i < n_kredytow; i++) {
                suma_zadluzenia = tab_kredytow[i].get_dlug();
            }
            return suma_zadluzenia;
        }

        ~Firma() { 
            delete[] tab_prac;
            delete[] tab_kredytow; 
        }
};

const int Firma:: Kwota_poczatkowa = 50;//inicjacja zmiennej statycznej

class Gra {
    private:
        bool stan;
        Firma* firma;
        void tick() {
            firma->otrzymaj_przychod();
            firma->zaplac_wynagrodzenie();
            firma->splac_raty();
        }

    public:

        Gra() { firma = new Firma; };

        bool akcja_gracza() {
            string komenda;
            cout << "podaj komende: " << endl;
            cin >> komenda;
            if (komenda == "kred") {
                obsluguj_kredyty();
            }
            else if (komenda == "zinz") {
                cout << "podaj imie(string) i wydzial inzyniera(string) ";
                string imie; string wydzial;
                cin >> imie >> wydzial;
                Inz* inzynier = new Inz(imie, wydzial);
                firma->zatrudnij(inzynier);
            }
            else if (komenda == "zmag") {
                cout << "podaj imie(string) i czy obsluguje wozek(true=1/false=0) ";
                string imie; bool obsl_widl;
                cin >> imie >> obsl_widl;
                Mag* magazynier = new Mag(imie, obsl_widl);
                firma->zatrudnij(magazynier);
            }
            else if (komenda == "zmkt") {
                cout << "podaj imie(string) i ilu obserwujacych w mediach spolecznosciowych(int) ";
                string imie; int followers;
                cin >> imie >> followers;
                Mkt* markreter = new Mkt(imie, followers);
                firma->zatrudnij(markreter);
            }
            else if (komenda == "zrob") {
                cout << "podaj imie (string)i rozmiar buta (double)";
                string imie; double but;
                cin >> imie >> but;
                Rob* robotnik = new Rob(imie, but);
                firma->zatrudnij(robotnik);
            }
            else if (komenda == "kt") {
                cout << "koniec tury/miesiaca" << endl;
                return true;
            }
            else if (komenda == "lp") {
                firma->drukuj_pracownikow();
            }
            return false;
        }

        void menu() {//drukowanie na ekranie 
            cout << endl;
            cout << "Mozliwe komunikaty gracza: " << endl; 
            cout << "- lp - wylistuj pracownikow (imiona i wynagrodzenia)," << endl;
            cout << "- zinz - zatrudnij inz," << endl;
            cout << "- zmag - zatrudnij mag," << endl;
            cout << "- zmkt - zatrudnij mark," << endl;
            cout << "- zrob - zatrudnij rob," << endl;
            cout << "- kred - wez kredyt (w następnym kroku podaj kwotę i czas splaty)," << endl;
            cout << "- kt - zakoncz turę/miesiac i wyswietl stan firmy na poczatku nastepnego miesiaca," << endl;
            cout << endl;
        }

        void obsluguj_kredyty() {
            double kwota_kredytu;
            int liczba_rat;
            cout << "podaj kwote kredytu: ";
            cin >> kwota_kredytu;
            cout << endl;
            cout << "podaj liczbe rat(max12): ";
            do{
                cout << "nie mozna wziac kredytu na wiecej niz 12 miesiecy" << endl;
                cin >> liczba_rat;
            }while (liczba_rat > 12);
            cout << endl;
            firma->wez_kredyt(kwota_kredytu, liczba_rat);
            firma->drukuj_kredyty();//mowi nam jakie kredyty wzielismy, pokazuje wszystkie kredyty, dlug i ilosc rat
        }

        void tura_gracza() {
            bool koniec_tury = false;
            do{
                koniec_tury=akcja_gracza();
            } while (!koniec_tury);
            cout << endl << "====================================" << endl;
            cout << "stan przed zmiana tury: " << endl;
            firma->drukuj_stan_firmy();
            cout << endl;
            tick();
            cout << "stan po zmianie tury: " << endl;
            firma->drukuj_stan_firmy();
            cout <<endl<<"-----------------------------"<<endl;
        }

        void stan_poczatkowy() {
            Inz* inzynier = new Inz("Kamil", "MEiL");
            firma->zatrudnij(inzynier);
            Mkt* marketer = new Mkt("Marek", 48);
            firma->zatrudnij(marketer);
            Mag* magazynier = new Mag("Magda", 1);
            firma->zatrudnij(magazynier);
            Rob* robotnik = new Rob("Robert", 46.5);
            firma->zatrudnij(robotnik);

            Firma:: Kwota_poczatkowa;
            cout << endl <<"kwota poczatkowa: "<< firma->Kwota_poczatkowa<<endl;
            firma->drukuj_pracownikow();
            }

        void graj() {
            menu();
            stan_poczatkowy();
            while ((firma->get_stan_konta()>=0)&&((firma->wartosc_firmy()-firma->zadluzenie())<M)&&(firma->zadluzenie()<=(M * firma->wartosc_firmy()))) {
                tura_gracza();
            }
            double wart_spolki = firma->wartosc_firmy() - firma->zadluzenie();
            if ((firma->get_stan_konta() < 0))
            {
                cout <<endl<< "twoj stan konta wynosi: "<<firma->get_stan_konta()<<", jest mniejszy od 0 -"<<endl<<"PRZEGRALES"<<endl;
            }
            else if (wart_spolki > M) {
                cout << endl << "wartosc twojej spolki wynosi: " << firma->wartosc_firmy() - firma->zadluzenie() << ", jest wieksza od: " << M << endl <<" WYGRANA"<< endl;
            }
            else if(firma->zadluzenie()>(M*firma->wartosc_firmy())){
                cout << "zadluzenie wynosi: " << firma->zadluzenie() << " i przekroczylo wartosc firmy: " << firma->wartosc_firmy() << " razy wartosc stalej M=" << M << " rownej= " << M * firma->wartosc_firmy();
                    cout<<endl<<" PRZEGRANA"<<endl;
            }
            else {
                cout << "blad";
                return;
            }
        }
        ~Gra() {
            delete firma;
        }
};

int main()
{
    Gra nowa_gra;
    nowa_gra.graj();
    nowa_gra.menu();

    return 0;
}
