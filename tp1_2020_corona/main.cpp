#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

typedef char str20[21];
typedef char str15[16];
typedef char str10[11];
typedef unsigned short ushort;

const ushort MAX_PAIS = 200;
const ushort MESES = 12;

struct tsPais {
    str20 pais;
    str10 continente;
    int habitantes;
};

struct tsParteDiario {
    str20 pais;
    ushort mes;
    ushort dia;
    int hisopados;
    int infectados;
    int recuperados;
    int fallecidos;
};

struct tsCalculos {
    str20 pais;
    int habitantes;

    int hisopados[MESES];
    int infectados[MESES];
    int recuperados[MESES];
    int fallecidos[MESES];

    int totalHisopados;
    int totalInfectados;
    int totalRecuperados;
    int totalFallecidos;

    float porcHisopados;
    float porcInfectados;
    float porcRecuperados;
    float porcFallecidos;
};

typedef tsCalculos tvsListado[MAX_PAIS];

void IntCmb(tsCalculos &elem1, tsCalculos &elem2) {
    tsCalculos aux = elem1;
    elem1 = elem2;
    elem2 = aux;
}

void OrdxBurPais(tvsListado vsListado, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i < card - k; i++)
            if (strcmp(vsListado[i].pais, vsListado[i + 1].pais) > 0) {
                ordenado = false;
                IntCmb(vsListado[i], vsListado[i + 1]);
            }
    } while (!ordenado);
}

void OrdxBurHisopados(tvsListado vsListado, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i < card - k; i++)
            if (vsListado[i].totalHisopados < vsListado[i + 1].totalHisopados) {
                ordenado = false;
                IntCmb(vsListado[i], vsListado[i + 1]);
            }
    } while (!ordenado);
}

void OrdxBurInfectados(tvsListado vsListado, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i < card - k; i++)
            if (vsListado[i].totalInfectados <
                vsListado[i + 1].totalInfectados) {
                ordenado = false;
                IntCmb(vsListado[i], vsListado[i + 1]);
            }
    } while (!ordenado);
}

void OrdxBurRecuperados(tvsListado vsListado, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i < card - k; i++)
            if (vsListado[i].totalRecuperados <
                vsListado[i + 1].totalRecuperados) {
                ordenado = false;
                IntCmb(vsListado[i], vsListado[i + 1]);
            }
    } while (!ordenado);
}

void OrdxBurFallecidos(tvsListado vsListado, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i < card - k; i++)
            if (vsListado[i].totalFallecidos <
                vsListado[i + 1].totalFallecidos) {
                ordenado = false;
                IntCmb(vsListado[i], vsListado[i + 1]);
            }
    } while (!ordenado);
}

int BusBinVec(tvsListado vsListado, str20 clave, ushort card) {
    int ult = card - 1;
    int prim = 0;
    int med;

    while (prim <= ult) {
        med = (prim + ult) / 2;
        if (strcmp(vsListado[med].pais, clave) == 0)
            return med;
        else if (strcmp(vsListado[med].pais, clave) < 0)
            prim = med + 1;
        else
            ult = med - 1;
    }
    return -1;
}

void Abrir(ifstream &Paises, ifstream &ParteDiario) {
    Paises.open("Paises.txt");
    ParteDiario.open("ParteDiario.txt");
}

bool LeerPais(ifstream &Paises, tsPais &sPais) {
    Paises.get(sPais.pais, 21);
    Paises.ignore();
    Paises.get(sPais.continente, 11);
    Paises >> sPais.habitantes;
    Paises.ignore();
    return Paises.good();
}

bool LeerParteDiario(ifstream &ParteDiario, tsParteDiario &sParteDiario) {
    ParteDiario.get(sParteDiario.pais, 21);
    ParteDiario >> sParteDiario.mes >> sParteDiario.dia >>
        sParteDiario.hisopados >> sParteDiario.infectados >>
        sParteDiario.recuperados >> sParteDiario.fallecidos;
    ParteDiario.ignore();
    return ParteDiario.good();
}

void InicializarListado(tvsListado vsListado, ushort cantPaises) {
    for (ushort i = 0; i < cantPaises; i++) {
        for (ushort j = 0; j < MESES; j++) {
            vsListado[i].hisopados[j] = 0;
            vsListado[i].infectados[j] = 0;
            vsListado[i].recuperados[j] = 0;
            vsListado[i].fallecidos[j] = 0;
        }
        vsListado[i].totalHisopados = 0;
        vsListado[i].totalInfectados = 0;
        vsListado[i].totalRecuperados = 0;
        vsListado[i].totalFallecidos = 0;

        vsListado[i].porcHisopados = 0.0;
        vsListado[i].porcInfectados = 0.0;
        vsListado[i].porcRecuperados = 0.0;
        vsListado[i].porcFallecidos = 0.0;
    }
}

void ProcPaises(ifstream &Paises, tvsListado vsListado, ushort &cantPaises) {
    tsPais sPais;

    while (LeerPais(Paises, sPais) && cantPaises < MAX_PAIS) {
        strcpy(vsListado[cantPaises].pais, sPais.pais);
        vsListado[cantPaises].habitantes = sPais.habitantes;
        ++cantPaises;
    }

    InicializarListado(vsListado, cantPaises);
    OrdxBurPais(vsListado, cantPaises);
}

void ProcParteDiario(ifstream &ParteDiario, tvsListado &vsListado,
                     ushort cantPaises) {
    tsParteDiario sParteDiario;

    while (LeerParteDiario(ParteDiario, sParteDiario)) {
        int pos = BusBinVec(vsListado, sParteDiario.pais, cantPaises);
        if (pos == -1) continue;

        vsListado[pos].hisopados[sParteDiario.mes - 1] +=
            sParteDiario.hisopados;
        vsListado[pos].infectados[sParteDiario.mes - 1] +=
            sParteDiario.infectados;
        vsListado[pos].recuperados[sParteDiario.mes - 1] +=
            sParteDiario.recuperados;
        vsListado[pos].fallecidos[sParteDiario.mes - 1] +=
            sParteDiario.fallecidos;
    }

    for (ushort i = 0; i < cantPaises; i++) {
        for (ushort j = 0; j < MESES; j++) {
            vsListado[i].totalHisopados += vsListado[i].hisopados[j];
            vsListado[i].totalInfectados += vsListado[i].infectados[j];
            vsListado[i].totalRecuperados += vsListado[i].recuperados[j];
            vsListado[i].totalFallecidos += vsListado[i].fallecidos[j];
        }

        vsListado[i].porcHisopados =
            (vsListado[i].totalHisopados * 100.00) / vsListado[i].habitantes;
        vsListado[i].porcInfectados =
            (vsListado[i].totalInfectados * 100.00) / vsListado[i].habitantes;
        vsListado[i].porcRecuperados =
            (vsListado[i].totalRecuperados * 100.00) / vsListado[i].habitantes;
        vsListado[i].porcFallecidos =
            (vsListado[i].totalFallecidos * 100.00) / vsListado[i].habitantes;
    }
}

void TituloListado(str15 titulo) {
    cout << "Listado de " << titulo << "\n\n";
    cout << "Ord." << ' ' << setw(20) << left << "Pais" << ' ' << setw(10)
         << left << "Cant.Hab." << ' ' << setw(5) << left << "Ene" << ' '
         << setw(5) << left << "Feb" << ' ' << setw(5) << left << "Mar" << ' '
         << setw(5) << left << "Abr" << ' ' << setw(5) << left << "May" << ' '
         << setw(5) << left << "Jun" << ' ' << setw(5) << left << "Jul" << ' '
         << setw(5) << left << "Ago" << ' ' << setw(5) << left << "Sep" << ' '
         << setw(5) << left << "Oct" << ' ' << setw(5) << left << "Nov" << ' '
         << setw(5) << left << "Dic" << ' ' << setw(7) << left << "Tot." << ' '
         << "Porc." << '\n';
}

void Listado(tvsListado vsListado, ushort cantPaises) {
    freopen("ListadoHisopados.txt", "w", stdout);
    TituloListado("Hisopados");
    OrdxBurHisopados(vsListado, cantPaises);

    for (ushort i = 0; i < cantPaises; i++) {
        cout << setw(4) << right << (i + 1) << ' ' << setw(20) << left
             << vsListado[i].pais << ' ' << setw(10) << right
             << vsListado[i].habitantes;

        for (ushort j = 0; j < MESES; j++) {
            cout << ' ' << setw(5) << right << vsListado[i].hisopados[j];
        }

        cout << setw(7) << right << vsListado[i].totalHisopados << ' '
             << setw(7) << right << fixed << setprecision(5)
             << vsListado[i].porcHisopados << '\n';
    }

    fclose(stdout);

    freopen("ListadoInfectados.txt", "w", stdout);
    TituloListado("Infectados");
    OrdxBurInfectados(vsListado, cantPaises);

    for (ushort i = 0; i < cantPaises; i++) {
        cout << setw(4) << right << (i + 1) << ' ' << setw(20) << left
             << vsListado[i].pais << ' ' << setw(10) << right
             << vsListado[i].habitantes;

        for (ushort j = 0; j < MESES; j++) {
            cout << ' ' << setw(5) << right << vsListado[i].infectados[j];
        }

        cout << setw(7) << right << vsListado[i].totalInfectados << ' '
             << setw(7) << right << fixed << setprecision(5)
             << vsListado[i].porcInfectados << '\n';
    }

    fclose(stdout);

    freopen("ListadoRecuperados.txt", "w", stdout);
    TituloListado("Recuperados");
    OrdxBurRecuperados(vsListado, cantPaises);

    for (ushort i = 0; i < cantPaises; i++) {
        cout << setw(4) << right << (i + 1) << ' ' << setw(20) << left
             << vsListado[i].pais << ' ' << setw(10) << right
             << vsListado[i].habitantes;

        for (ushort j = 0; j < MESES; j++) {
            cout << ' ' << setw(5) << right << vsListado[i].recuperados[j];
        }

        cout << setw(7) << right << vsListado[i].totalRecuperados << ' '
             << setw(7) << right << fixed << setprecision(5)
             << vsListado[i].porcRecuperados << '\n';
    }

    freopen("ListadoFallecidos.txt", "w", stdout);
    TituloListado("Fallecidos");
    OrdxBurFallecidos(vsListado, cantPaises);

    for (ushort i = 0; i < cantPaises; i++) {
        cout << setw(4) << right << (i + 1) << ' ' << setw(20) << left
             << vsListado[i].pais << ' ' << setw(10) << right
             << vsListado[i].habitantes;

        for (ushort j = 0; j < MESES; j++) {
            cout << ' ' << setw(5) << right << vsListado[i].fallecidos[j];
        }

        cout << setw(7) << right << vsListado[i].totalFallecidos << ' '
             << setw(7) << right << fixed << setprecision(5)
             << vsListado[i].porcFallecidos << '\n';
    }

    fclose(stdout);
}

void Cerrar(ifstream &Paises, ifstream &ParteDiario) {
    Paises.close();
    ParteDiario.close();
}

int main() {
    ifstream Paises, ParteDiario;
    tvsListado vsListado;
    ushort cantPaises = 0;

    Abrir(Paises, ParteDiario);
    ProcPaises(Paises, vsListado, cantPaises);
    ProcParteDiario(ParteDiario, vsListado, cantPaises);
    Listado(vsListado, cantPaises);
    Cerrar(Paises, ParteDiario);
    return 0;
}
