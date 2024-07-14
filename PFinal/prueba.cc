//David Pérez Sampedro - 48774775D
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <chrono>
#include <queue>
#include <tuple>
#include <sstream>

using namespace std;

struct estructura_cola {
    pair<int, int> casilla;
    int valor;
    vector<pair<int, int>> casillas_usadas;
};

class Comparar {
public:
    //ordena de menor a mayor según el pesos total del camino hasta la casilla
    bool operator()(const estructura_cola &abajo, const estructura_cola &arriba) {
        return abajo.valor > arriba.valor;
    }
};

vector<vector<int>> crearMatrizPesos(int &filas, int &columnas) {
    string linea;
    int n, i, j = i = 0;
    bool primera = true;
    vector<vector<int>> pesos;

    ifstream doc("ADA_BB-test-files/000.map");
    while(getline(doc, linea)) {
        istringstream iss(linea);
        if(primera) {
            iss >> filas;
            iss >> columnas;
            pesos.resize(filas, vector<int>(columnas));
            primera = false;
        } else {
            while(iss >> n) {
                pesos[i][j] = n;
                j++;
                if(j == columnas) {
                    j = 0;
                    i++;
                }
            }
        }
    }
    doc.close();

    return pesos;
}

bool usada(const vector<pair<int, int>> &usadas, const pair<int, int> &casilla) {
    return any_of(usadas.begin(), usadas.end(), [&casilla](const pair<int, int> &usada) {
        return usada == casilla;
    });
}

int solucion_pesimista(const int &solucion_actual, const pair<int, int> &nodo, const int &filas, const int &columnas,
                       const vector<vector<int>> &pesos, const vector<pair<int, int>> &usadas) {
    vector<pair<int, int>> en_uso(usadas);
    int sol = solucion_actual;
    int i = nodo.first;
    int j = nodo.second;
    int S1, S2, S3, S4, S5, S6, S7, S8, maximo;
    vector<int> num;
    int n;

    while(i + 1 < filas || j + 1 < columnas) {
        S1 = S2 = S3 = S4 = S5 = S6 = S7 = S8 = numeric_limits<int>::min();

        if(i - 1 >= 0 && !usada(en_uso, make_pair(i - 1, j))) {     //arriba
            S1 = sol + pesos[i - 1][j];
        }

        if(i - 1 >= 0 && j + 1 < columnas && !usada(en_uso, make_pair(i - 1, j + 1))) {     //digonal arriba derecha
            S2 = sol + pesos[i - 1][j + 1];
        }

        if(j + 1 < columnas && !usada(en_uso, make_pair(i, j + 1))) {     //derecha
            S3 = sol + pesos[i][j + 1];
        }

        if(i + 1 < filas && j + 1 < columnas && !usada(en_uso, make_pair(i + 1, j + 1))) {     //diagonal abajo derecha
            S4 = sol + pesos[i + 1][j + 1];
        }

        if(i + 1 < filas && !usada(en_uso, make_pair(i + 1, j))) {     //abajo
            S5 = sol + pesos[i + 1][j];
        }

        if(i + 1 < filas && j - 1 >= 0 && !usada(en_uso, make_pair(i + 1, j - 1))) {     //diagonal abajo izquierda
            S6 = sol + pesos[i + 1][j - 1];
        }

        if(j - 1 >= 0 && !usada(en_uso, make_pair(i, j - 1))) {     //izquierda
            S7 = sol + pesos[i][j - 1];
        }

        if(i - 1 >= 0 && j - 1 >= 0 && !usada(en_uso, make_pair(i - 1, j - 1))) {     //diagonal arriba izquierda
            S8 = sol + pesos[i - 1][j - 1];
        }

        num = {S1, S2, S3, S4, S5, S6, S7, S8};
        sort(num.begin(), num.end());

        maximo = num[3];

        //minimo = max(max(max(S1, S2), max(S3, S4)), max(max(S5, S6), max(S7, S8)));

        if(maximo == numeric_limits<int>::min()) {
            return numeric_limits<int>::max();
        }

        if(maximo == S1) {
            i--;
        } else if(maximo == S2) {
            i--;
            j++;
        } else if(maximo == S3) {
            j++;
        } else if(maximo == S4) {
            i++;
            j++;
        } else if(maximo == S5) {
            i++;
        } else if(maximo == S6) {
            i++;
            j--;
        } else if(maximo == S7) {
            j--;
        } else if(maximo == S8) {
            i--;
            j--;
        }

        en_uso.emplace_back(i, j);
        sol = maximo;
    }

    return sol;
}

bool es_hoja(const pair<int, int> &nodo, const int &filas, const int &columnas) {
    return (nodo.first == filas - 1 && nodo.second == columnas - 1);    //última casilla
}

int optimista(const int &solucion_actual, const pair<int, int> &nodo, const int &filas, const int &columnas,
                       const vector<vector<int>> &pesos) {
    int sol = solucion_actual;
    int i = nodo.first;
    int j = nodo.second;
    int S1, S2, S3, minimo;

    while(i + 1 < filas || j + 1 < columnas) {
        if(i + 1 == filas) {
            sol += pesos[i][j + 1];  //Movimiento hacia la derecha
            j++;
        } else if(j + 1 == columnas) {
            sol += pesos[i + 1][j];  //Movimiento hacia abajo
            i++;
        } else {
            S1 = sol + pesos[i + 1][j + 1];  //Movimiento diagonal
            S2 = sol + pesos[i][j + 1];  //Movimiento hacia la derecha
            S3 = sol + pesos[i + 1][j];  //Movimiento hacia abajo

            minimo = min(S1, min(S2, S3));

            if(minimo == S1) {
                sol = S1;
                i++;
                j++;
            } else if (minimo == S2) {
                sol = S2;
                j++;
            } else {
                sol = S3;
                i++;
            }
        }
    }

    return sol;
}


int mcp_bb(const vector<vector<int>> &pesos, const int &filas, const int &columnas, vector<pair<int, int>> &mejor_camino, vector<int> &estadisticas) {
    vector<pair<int, int>> usadas;  //vector de posiciones ya usadas por el camino actual

    pair<int, int> inicial(0, 0);   //nodo inicial (primera casilla)
    usadas.emplace_back(inicial);   //añadr el nodo inicial a usadas
    int mejor_actual = solucion_pesimista(pesos[0][0], inicial, filas, columnas, pesos, usadas);
    priority_queue<estructura_cola, vector<estructura_cola>, Comparar> cola;
    cola.push({inicial, pesos[0][0], usadas});

    //declaración de las variables utilizadas en el bucle
    pair<int, int> nodo_actual; //nodo de la cola del que se va a probar expansiones
    int i, j, S, sol;    //filas, columnas, solucíón hasta el momento del nodo actual, S del expandido
    bool probar;    //factible o no factible
    pair<int, int> nodo_expandido;  //nodo extendido y probado para añadir a la cola
    vector<pair<int, int>> en_uso;

    while(!cola.empty()) {
        nodo_actual = cola.top().casilla;
        S = cola.top().valor;
        usadas = cola.top().casillas_usadas;
        cola.pop();

        i = nodo_actual.first;
        j = nodo_actual.second;

        if(optimista(mejor_actual, nodo_actual, filas, columnas, pesos) > mejor_actual) {  //no es prometedor
            estadisticas[5]++;  //prometedores_descartados
            continue;
        }

        if(es_hoja(nodo_actual, filas, columnas)) {
            estadisticas[2]++;  //hojas
            if(S <= mejor_actual) {
                mejor_actual = S;
                mejor_camino = usadas;   //actualizar el mejor camino
                estadisticas[6]++;  //mejores_soluciones_desde_hojas
            }
            continue;
        }

        for(int k = 1; k <= 8; k++) {
            probar = false;
            en_uso = usadas;
            sol = S;
            switch(k) {
                case 1:     //arriba
                    nodo_expandido = make_pair(i - 1, j);
                    probar = (nodo_expandido.first >= 0); //factible
                    break;

                case 2:     //diagonal arriba derecha
                    nodo_expandido = make_pair(i - 1, j + 1);
                    probar = (nodo_expandido.first >= 0 && nodo_expandido.second <= columnas - 1); //factible
                    break;

                case 3:     //derecha
                    nodo_expandido = make_pair(i, j + 1);
                    probar = (nodo_expandido.second <= columnas - 1); //factible
                    break;

                case 4:     //diagonal abajo derecha
                    nodo_expandido = make_pair(i + 1, j + 1);
                    probar = (nodo_expandido.first <= filas - 1 && nodo_expandido.second <= columnas - 1); //factible
                    break;

                case 5:     //abajo
                    nodo_expandido = make_pair(i + 1, j);
                    probar = (nodo_expandido.first <= filas - 1); //factible
                    break;

                case 6:     //diagonal abajo izquierda
                    nodo_expandido = make_pair(i + 1, j - 1);
                    probar = (nodo_expandido.first <= filas - 1 && nodo_expandido.second >= 0); //factible
                    break;

                case 7:     //izquierda
                    nodo_expandido = make_pair(i, j - 1);
                    probar = (nodo_expandido.second >= 0); //factible
                    break;

                case 8:     //diagonal arriba izquierda
                    nodo_expandido = make_pair(i - 1, j - 1);
                    probar = (nodo_expandido.first >= 0 && nodo_expandido.second >= 0); //factible
                    break;

                default:
                    break;
            }

            if(probar) {    //evito visitar nodos que no existen
                if(!usada(en_uso, nodo_expandido)) {    //factible
                    sol += pesos[nodo_expandido.first][nodo_expandido.second];
                    en_uso.emplace_back(nodo_expandido);

                    int pesimista = solucion_pesimista(sol, nodo_expandido, filas, columnas, pesos, en_uso);
                    if (pesimista <= mejor_actual) {
                        mejor_actual = pesimista;   //poda
                        mejor_camino = en_uso;   //actualizar el mejor camino
                        estadisticas[7]++;  //mejores_soluciones_desde_pesimista
                    }

                    if(optimista(sol, nodo_expandido, filas, columnas, pesos) <= mejor_actual) {     //prometedor
                        cola.push({nodo_expandido, sol, en_uso});
                        estadisticas[1]++;  //explorados
                    } else {
                        estadisticas[4]++;  //no_prometedores
                    }
                } else {
                    estadisticas[3]++;  //no_factibles
                }
                estadisticas[0]++;  //visitados
            }
        }
    }

    return mejor_actual;
}

void mcp_parser(const vector<vector<int>> &M, vector<pair<int, int>> mejor_camino, const int &filas, const int &columnas) {
    vector<vector<char>> sol(filas + 1, vector<char>(columnas + 1, '.'));
    int total = 0;

    while(!mejor_camino.empty()) {
        total += M[mejor_camino.back().first][mejor_camino.back().second];
        sol[mejor_camino.back().first][mejor_camino.back().second] = 'x';
        mejor_camino.pop_back();
    }

    //Dibujar camino
    for (int i = 0; i <= filas; i++) {
        for (int j = 0; j <= columnas; j++) {
            cout << sol[i][j];
        }
        cout << endl;
    }
    cout << total << endl;
}

void mcp_codificacion(vector<pair<int, int>> camino) {
    int i, j = i = 0;

    cout << "<";
    while(!camino.empty()) {
        if(camino.back() == make_pair(i - 1, j)) {  //arriba - norte - 1
            i--;
            cout << 1;
        } else if(camino.back() == make_pair(i - 1, j + 1)) {   //diagonal arriba derecha - noreste - 2
            i--;
            j++;
            cout << 2;
        } else if(camino.back() == make_pair(i, j + 1)) {   //derecha - este - 3
            j++;
            cout << 3;
        } else if(camino.back() == make_pair(i + 1, j + 1)) {   //diagonal abajo derecha - sureste - 4
            i++;
            j++;
            cout << 4;
        } else if(camino.back() == make_pair(i + 1, j)) {   //abajo - sur - 5
            i++;
            cout << 5;
        } else if(camino.back() == make_pair(i + 1, j - 1)) {   //diagonal abajo izquierda - suroeste - 6
            i++;
            j--;
            cout << 6;
        } else if(camino.back() == make_pair(i, j - 1)) {   //izquierda - oeste - 7
            j--;
            cout << 7;
        } else if(camino.back() == make_pair(i - 1, j - 1)) {   //diagonal arriba izquierda - noroeste - 8
            i--;
            j--;
            cout << 8;
        }
        camino.pop_back();
    }
    cout << ">" << endl;
}

int main() {
    int filas, columnas = filas = 0;

    //Crear matriz de pesos
    vector<vector<int>> pesos = crearMatrizPesos(filas, columnas);

    vector<pair<int, int>> mejor_camino, camino_revertido;

    //visitados, explorados, hojas, no_factibles, no_prometedores,
    //prometedores_descartados, mejores_soluciones_desde_hojas, mejores_soluciones_desde_pesimista
    vector<int> estadisticas(8, 0);

    auto start = clock();
    int solucion = mcp_bb(pesos, filas, columnas, mejor_camino, estadisticas);
    auto end = clock();

    double time = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    cout << solucion << endl;

    for(int estadistica : estadisticas) {
        cout << estadistica << " ";
    }

    cout << endl << time << endl;

    mcp_parser(pesos, mejor_camino, filas - 1, columnas - 1);

    camino_revertido = mejor_camino;

    reverse(camino_revertido.begin(), camino_revertido.end());

    mcp_codificacion(camino_revertido);

    return 0;
}