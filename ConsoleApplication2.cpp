//Harjoitusty� Anton Karala 24Tietoa 
// Hotelli varausj�rjestelm� (3p)


// Tarvittavat kirjastot                
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

// M��ritell��n huoneiden m��r�t vakiona
const int MIN_HUONEET = 30;
const int MAX_HUONEET = 300;

// Sama huoneen hinnalle v�lill� 80e-100e
const int MIN_HINTA = 80;
const int MAX_HINTA = 100;

// Ja edelleen satunnaisten varausnumeroiden tulostukselle
const int MIN_VARAUSNUMERO = 10000;
const int MAX_VARAUSNUMERO = 99999;

// Rakenteet ja tyypit
struct Varaus {
    string nimi;
    int huonenumero;
    int varausnumero;
};

// Aliohjelmien esittely
int arvoLuku(int min, int max);
void naytaValikko();
bool onkoVarattu(const vector<bool>& huoneet, int huonenumero);
void teeVaraus(vector<bool>& huoneet, map<int, Varaus>& varaukset, int& varattujaHuoneita);
void haeVaraus(const map<int, Varaus>& varaukset);
bool tarkistaSyote(string syote);

// P��ohjelma
int main() {
    //��kk�sten tulostus 
    setlocale(LC_ALL,"fi_FI");
    srand(time(0)); // Satunnaislukugeneraattori

    // huoneiden m��rien ja hintojen arvonta 
    int huonemaara = arvoLuku(MIN_HUONEET, MAX_HUONEET);
    int hintaPerYo = arvoLuku(MIN_HINTA, MAX_HINTA);

    // huoneiden varaustilanteen taulukko
    vector<bool> huoneet(huonemaara, true);

    map<int, Varaus> varaukset; // varausten tallennus varausnumerolla
    int varattujaHuoneita = 0; // laskur varatuille huoneille
    string valinta;

    cout << "Hotellin varausjarjestelm� " << endl;
    cout << "Hotellissa on yhteens� " << huonemaara << " huonetta." << endl;
    cout << "Hinta t�n� y�na on: " << hintaPerYo << " euroa." << endl;

    // P��silmukka
    while (true) {
        naytaValikko();
        cout << "Valintasi: ";
        cin >> valinta;

        // Tarkistetaan sy�te
        if (!tarkistaSyote(valinta)) {
            cout << "Virheellinen sy�te, yrit� uudelleen." << endl;
            continue;
        }

        int valintaNum = stoi(valinta);

        if (valintaNum == 1) {
            teeVaraus(huoneet, varaukset, varattujaHuoneita);
        }
        else if (valintaNum == 2) {
            haeVaraus(varaukset);
        }
        else if (valintaNum == 3) {
            cout << "Ohjelma suljetaan." << endl;
            break;
        }
        else {
            cout << "Virheellinen valinta, kokeile uudelleen." << endl;
        }
    }

    return 0;
}

// K�ytet��n simppeli� aliohjelmaa satunnaislukujen arvontaan
int arvoLuku(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Aliohjelma valikolle:
void naytaValikko() {
    cout << "\nValitse toiminto:" << endl;
    cout << "1. Tee varaus" << endl;
    cout << "2. Hae varaus" << endl;
    cout << "3. Lopeta ohjelma" << endl;
}

// Aliohjelma huoneen varauksen tarkistamiseen
bool onkoVarattu(const vector<bool>& huoneet, int huonenumero) {
    return !huoneet[huonenumero - 1];
}

// Varauksen tekemisen aliohjelma
void teeVaraus(vector<bool>& huoneet, map<int, Varaus>& varaukset, int& varattujaHuoneita) {
    if (varattujaHuoneita == huoneet.size()) {
        cout << "Kaikki huoneet ovat varattuja!" << endl;
        return;
    }

    string syote;
    int huonenumero;

    while (true) {
        cout << "Anna varattavan huoneen numero (1-" << huoneet.size() << "): ";
        cin >> syote;

        if (!tarkistaSyote(syote)) {
            cout << "Virheellinen sy�te. Anna numero v�lilt� 1-" << huoneet.size() << "." << endl;
            continue;
        }

        huonenumero = stoi(syote);

        if (huonenumero < 1 || huonenumero > huoneet.size()) {
            cout << "Huonenumero ei ole v�lilt� 1-" << huoneet.size() << ". Kokeile uudelleen." << endl;
            continue;
        }

        if (onkoVarattu(huoneet, huonenumero)) {
            cout << "Huone " << huonenumero << " on jo varattu. Valitse toinen huone." << endl;
        }
        else {
            break;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennet��n sy�tteen puskuri
    cout << "Anna varaajan nimi: ";
    string nimi;
    getline(cin, nimi);

    int varausnumero = arvoLuku(MIN_VARAUSNUMERO, MAX_VARAUSNUMERO);
    while (varaukset.find(varausnumero) != varaukset.end()) {
        varausnumero = arvoLuku(MIN_VARAUSNUMERO, MAX_VARAUSNUMERO);
    }

    huoneet[huonenumero - 1] = false;
    varaukset[varausnumero] = { nimi, huonenumero, varausnumero };
    varattujaHuoneita++;

    cout << "Varaus tehty! Varausnumero: " << varausnumero << ", Huone: " << huonenumero << ", Nimi: " << nimi << endl;
}

// Aliohjelma varauksen hakemiseen
void haeVaraus(const map<int, Varaus>& varaukset) {
    cout << "Hae varausta varausnumerolla tai nimell�." << endl;
    cout << "1. Varausnumero\n2. Nimi\nValintasi: ";
    string valinta;
    cin >> valinta;

    if (valinta == "1") {
        cout << "Anna varausnumero: ";
        string syote;
        cin >> syote;

        if (!tarkistaSyote(syote)) {
            cout << "Virheellinen varausnumero." << endl;
            return;
        }

        int varausnumero = stoi(syote);                 //Haku toiminto varausnumerolla
        auto it = varaukset.find(varausnumero);
        if (it != varaukset.end()) {
            cout << "Varaus l�ytyi! Nimi: " << it->second.nimi << ", Huone: " << it->second.huonenumero << endl;
        }
        else {
            cout << "Varausta ei l�ytynyt." << endl;
        }
    }
    else if (valinta == "2") {                //Haku nimell�
        cout << "Anna varaajan nimi: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennet��n sy�tteen puskuri
        string nimi;
        getline(cin, nimi);

        bool loytyi = false;
        for (const auto& pari : varaukset) {
            if (pari.second.nimi == nimi) {
                cout << "Varaus l�ytyi! Varausnumero: " << pari.second.varausnumero << ", Huone: " << pari.second.huonenumero << endl;
                loytyi = true;
                break;
            }
        }

        if (!loytyi) {
            cout << "Varausta ei loetynyt." << endl;
        }
    }
    else {
        cout << "Virheellinen valinta." << endl;
    }
}

// Aliohjelma sy�tteen tarkistukseen
bool tarkistaSyote(string syote) {
    for (char c : syote) {
        if (!isdigit(c)) return false;
    }
    return true;
}
