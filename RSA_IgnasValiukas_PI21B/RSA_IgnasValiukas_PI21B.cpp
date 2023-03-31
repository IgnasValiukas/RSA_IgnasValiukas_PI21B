#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;
//konsoles teksto spalvos funkcija
void SetColor(int ForgC){
     WORD wColor;
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}
//funkcija kuri tikrina ar pirminis skaicius
bool pirminis(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int atvirkstine(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // atvirkstine neegzistuoja
}

//funkcija kuri sugeneruoja viesaji ir privatu rakta
pair<pair<int, int>, pair<int, int>> generuotiRaktus(int p, int q) {
    if (!(pirminis(p) && pirminis(q))) {
        SetColor(13);
        throw invalid_argument("<KLAIDA> abu skaiciai turi buti pirminiai");
        SetColor(15);
    }
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = 2;
    while (gcd(e, phi) != 1) {
        e++;
    }
    int d = atvirkstine(e, phi);
    return make_pair(make_pair(n, e), make_pair(n, d));
}

//funkcija kuri uzsifruoja teksta
vector<int> sifruoti(pair<int, int> pk, string tekstas) {
    int n = pk.first;
    int e = pk.second;
    vector<int> uzsifruotasTekstas;
    for (char c : tekstas) {
        int m = static_cast<int>(c);
        int uzsifruotas_m = 1;
        for (int i = 0; i < e; i++) {
            uzsifruotas_m = (uzsifruotas_m * m) % n;
        }
        uzsifruotasTekstas.push_back(uzsifruotas_m);
    }
    return uzsifruotasTekstas;
}


//funkcija kuri desifruoja teksta
string desifruoti(pair<int, int> pk, vector<int> uzsifruotasTekstas) {
    int n = pk.first;
    int d = pk.second;
    string tekstas = "";
    for (int c : uzsifruotasTekstas) {
        int uzsifruotas_c = 1;
        for (int i = 0; i < d; i++) {
            uzsifruotas_c = (uzsifruotas_c * c) % n;
        }
        tekstas += static_cast<char>(uzsifruotas_c);
    }
    return tekstas;
}

int main() {
    SetColor(14);
    cout << "          <RSA SIFRAVIMO/DESIFRAVIMO ALGORITMAS>" << endl;
    cout << "___________________________________________________________" << endl;
    //Raktu kurimas/generavimas

    cout << "<PIRMINIU SKAICIU IVEDIMAS>" << endl;
    int p, q, pasirinkimas;
    string tekstas;
    SetColor(9);
    cout << "Iveskite p(pirminis skaicius) ir spauskite ENTER: ";
    SetColor(15);
    cin >> p;
    SetColor(9);
    cout << "Iveskite q(pirminis skaicius) ir spauskite ENTER: ";
    SetColor(15);
    cin >> q;

    auto raktai = generuotiRaktus(p, q);
    auto viesasRaktas = raktai.first;
    auto privatusRaktas = raktai.second;

    SetColor(14);
    cout << "___________________________________________________________" << endl;
    SetColor(15);
    //Pasirinkti, ar ivedame teksta, ar nuskaitome is failo
    SetColor(14);
    cout << "<MENIU>" << endl;
    cout << "1. Teksto ivedimas\n";
    cout << "2. Teksto skaitymas is failo\n";
    SetColor(9);
    cout << "Iveskite pasirinkima ir spauskite ENTER: ";
    SetColor(15);
    cin >> pasirinkimas;

    if (pasirinkimas == 1) {
        //Teksto ivedimas ir sifravimas
        SetColor(9);
        cout << "Iveskite teksta ir spauskite ENTER: ";
        SetColor(15);
        cin.ignore(); //Ignoruojame '\n' simboli, kuris liko po skaitymo is pirmo cin objekto
        getline(cin, tekstas);
        auto uzsifruotasTekstas = sifruoti(viesasRaktas, tekstas);
        auto desifruotasTekstas = desifruoti(privatusRaktas, uzsifruotasTekstas);

        //Ciphertext ir vieas raktas issaugojimas faile
        ofstream uzsifruotasFailas("uzsifruotasTekstas.txt");
        for (int c : uzsifruotasTekstas) {
            uzsifruotasFailas << c << " ";
        }
        uzsifruotasFailas.close();

        //Isvedimas
        SetColor(14);
        cout << "\n___________________________________________________________" << endl;
        cout << "<SUGENERUOTI RAKTAI>" << endl;
        cout << "Viesas raktas: (" << viesasRaktas.first << ", " << viesasRaktas.second << ")" << endl;
        cout << "Privatus raktas: (" << privatusRaktas.first << ", " << privatusRaktas.second << ")" << endl;
        cout << "___________________________________________________________" << endl;
        cout << "<ATSAKYMAS>" <<endl;
        SetColor(9);
        cout << "Uzsifruotas tekstas: ";
        SetColor(34);
        for (int c : uzsifruotasTekstas) {
            cout << c << " ";
        }
        cout << endl;
        SetColor(9);
        cout << "Desifruotas tekstas: ";
        SetColor(34);
        cout << desifruotasTekstas << endl;
        SetColor(15);
        cout << endl;
    } else if (pasirinkimas == 2) {
        //Nuskaityti uzsifruota teksta is failo
        ifstream uzsifruotasFailas("uzsifruotasTekstas.txt");
        vector<int> uzsifruotasTekstas;
        int c;
        while (uzsifruotasFailas >> c) {
            uzsifruotasTekstas.push_back(c);
        }
        uzsifruotasFailas.close();

        //Teksto desifravimas
        auto desifruotasTekstas = desifruoti(privatusRaktas, uzsifruotasTekstas);

        SetColor(14);
        cout << "\n___________________________________________________________" << endl;
        cout << "<SUGENERUOTI RAKTAI>" << endl;
        cout << "Viesas raktas: (" << viesasRaktas.first << ", " << viesasRaktas.second << ")" << endl;
        cout << "Privatus raktas: (" << privatusRaktas.first << ", " << privatusRaktas.second << ")" << endl;
        cout << "___________________________________________________________" << endl;
        cout << "<ATSAKYMAS>" <<endl;
        SetColor(9);
        cout << "Uzsifruotas tekstas: ";
        SetColor(34);
        for (int c : uzsifruotasTekstas) {
            cout << c << " ";
        }
        cout << endl;
        SetColor(9);
        cout << "Desifruotas tekstas: ";
        SetColor(34);
        cout << desifruotasTekstas << endl;
        SetColor(15);
    } else {
        SetColor(13);
        cout << "Neteisingas pasirinkimas." << endl;
        SetColor(15);
    }

    return 0;
}

