//David Pérez Sampedro - 48774775D
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>
#include<sstream>
#include<limits>
#include<algorithm>
using namespace std;

void mensajeError(const string &error) {
    if(!error.empty()) {
        cerr << "ERROR: " << error << endl;
    }
    cerr << "Usage:" << endl << "mcp_bt [-p] [--p2D] -f file" << endl;
    exit(EXIT_FAILURE);
}

void compruebaArgumentos(const int argc, vector<string> &args) {
    int posicion_f = 0;

    //Guardar última vez que aparece -f
    for(int i = 1; i < argc; i++) {
        if(args[i] == "-f") {
            posicion_f = i;
        }
    }

    //Si se ha pasado o no un fichero
    if(posicion_f < argc - 1) {
        ifstream doc(args[posicion_f + 1]);
        if (!doc) {
            mensajeError("can't open file: " + args[posicion_f + 1] + ".");
        }
        doc.close();

        //Mover -f y file a las posiciones penúltima y última
        args.emplace_back("-f");
        args.emplace_back(args[posicion_f + 1]);
        args.erase(args.begin() + posicion_f);
        args.erase(args.begin() + posicion_f);
    } else {
        //mcp
        if(argc == 1) {
            mensajeError("");
        }

        //mcp -f
        mensajeError("missing filename.");
    }

    //Se ignora mcp, -f y fichero_entrada
    for(int i = 1; i < argc - 2; i++) {
        if(args[i] != "-p" && args[i] != "--p2D" && args[i] != "-f") {
            mensajeError("unknown option " + args[i] + ".");
        }
    }
}

void validaArgumentos(const int argc, const vector<string> &args, bool &p, bool &p2D) {
    for(int i = 1; i < argc - 2; i++) {
        if(args[i] == "-p") {
            p = true;
        }
        if(args[i] == "--p2D") {
            p2D = true;
        }
    }
}

vector<vector<int>> crearMatrizPesos(const int argc, const vector<string> &args, int &filas, int &columnas) {
    string linea;
    int n, i, j = i = 0;
    bool primera = true;
    vector<vector<int>> pesos;

    ifstream doc("ADA_BT-test-files/002.map");
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

bool usada(const vector<pair<int, int>> &usadas, pair<int, int> casilla) {
    for(const pair<int, int> &usada : usadas) {
        if(usada == casilla) {
            return true;
        }
    }
    return false;
}

void mcp_bt(vector<vector<int>> &M, vector<vector<int>> &sol, const vector<vector<int>> &pesos, vector<pair<int, int>> &usadas, vector<pair<int, int>> &mejor_camino,
            vector<pair<int, int>> &camino_actual, int i, int j, const int &filas, const int &columnas, int &mejor_actual, int &visitados,
            int &explorados, int &no_factibles, int &no_prometedores, int &hojas) {
    if(visitados == 0) {
        visitados++;
        explorados++;
    }

    camino_actual.emplace_back(i, j);

    if(i == filas - 1 && j == columnas - 1) {   //nodo hoja
        hojas++;
        if(sol[i][j] < mejor_actual) {  //mejora la solución actual
            M[i][j] = sol[i][j];
            mejor_actual = sol[i][j];
            mejor_camino = camino_actual;
        }
        camino_actual.pop_back();
        return;
    }

    int nueva_i, nueva_j, S = nueva_i = nueva_j = 0;
    bool probar;
    for(int k = 1; k <= 8; k++) {
        probar = false;
        switch (k) {
            case 1:     //arriba
                if(!usada(usadas, make_pair(i - 1, j))) {
                    visitados++;
                    if (i - 1 >= 0) {    //factible
                        probar = true;
                        nueva_i = i - 1;
                        nueva_j = j;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 2:     //diagonal arriba derecha
                if(!usada(usadas, make_pair(i - 1, j + 1))) {
                    visitados++;
                    if (i - 1 >= 0 && j + 1 <= columnas - 1) {    //factible
                        probar = true;
                        nueva_i = i - 1;
                        nueva_j = j + 1;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 3:     //derecha
                if(!usada(usadas, make_pair(i, j + 1))) {
                    visitados++;
                    if (j + 1 <= columnas - 1) {    //factible
                        probar = true;
                        nueva_i = i;
                        nueva_j = j + 1;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 4:     //diagonal abajo derecha
                if(!usada(usadas, make_pair(i + 1, j + 1))) {
                    visitados++;
                    if (i + 1 <= filas - 1 && j + 1 <= columnas - 1) {    //factible
                        probar = true;
                        nueva_i = i + 1;
                        nueva_j = j + 1;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 5:     //abajo
                if(!usada(usadas, make_pair(i + 1, j))) {
                    visitados++;
                    if (i + 1 <= filas - 1) {    //factible
                        probar = true;
                        nueva_i = i + 1;
                        nueva_j = j;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 6:     //diagonal abajo izquierda
                if(!usada(usadas, make_pair(i + 1, j - 1))) {
                    visitados++;
                    if (i + 1 <= filas - 1 && j - 1 >= 0) {    //factible
                        probar = true;
                        nueva_i = i + 1;
                        nueva_j = j - 1;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 7:     //izquierda
                if(!usada(usadas, make_pair(i, j - 1))) {
                    visitados++;
                    if (j - 1 >= 0) {    //factible
                        probar = true;
                        nueva_i = i;
                        nueva_j = j - 1;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            case 8:     //diagonal arriba izquierda
                if(!usada(usadas, make_pair(i - 1, j - 1))) {
                    visitados++;
                    if (i - 1 >= 0 && j - 1 >= 0) {    //factible
                        probar = true;
                        nueva_i = i - 1;
                        nueva_j = j - 1;
                        S = sol[i][j] + pesos[nueva_i][nueva_j];
                    } else {
                        no_factibles++;
                    }
                }
                break;

            default:
                break;
        }

        if(probar) {    //si se ha descartado como factible no puede ser prometedor en primer lugar
            if (S <= mejor_actual) {     //prometedor
                explorados++;
                sol[nueva_i][nueva_j] = S;
                usadas.emplace_back(i, j);
                M[i][j] = sol[i][j];
                if(M[nueva_i][nueva_j] == -1 || sol[nueva_i][nueva_j] <= M[nueva_i][nueva_j]) {
                    mcp_bt(M, sol, pesos, usadas, mejor_camino, camino_actual, nueva_i, nueva_j, filas, columnas, mejor_actual, visitados, explorados, no_factibles,
                           no_prometedores, hojas);
                }
                usadas.pop_back();
            } else {
                no_prometedores++;
            }
        }
    }
    camino_actual.pop_back();
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

int main(int argc, char *argv[]) {
    bool p, p2D = p = false;
    int filas, columnas = filas = 0;

    vector<string> args(argv, argv + argc);

    //Comprovación y validación de argumnetos
    //compruebaArgumentos(argc, args);
    //validaArgumentos(argc, args, p, p2D);

    auto start = clock();

    //Crear matriz de pesos
    vector<vector<int>> pesos = crearMatrizPesos(argc, args, filas, columnas);

    vector<vector<int>> M(filas, vector<int>(columnas, -1));

    vector<vector<int>> sol(pesos);

    vector<pair<int, int>> usadas;

    int visitados, explorados, no_factibles, no_prometedores, hojas = visitados = explorados = no_factibles = no_prometedores = 0;
    int i, j = i = 0;
    int mejor_actual = numeric_limits<int>::max();
    vector<pair<int, int>> mejor_camino, camino_actual;

    mcp_bt(M, sol, pesos, usadas, mejor_camino, camino_actual, i, j, filas, columnas, mejor_actual, visitados, explorados, no_factibles, no_prometedores, hojas);

    auto end = clock();
    double time = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    cout << mejor_actual << endl;
    cout << visitados << " " << explorados << " " << hojas << " " << no_factibles << " " << no_prometedores << endl;
    cout << time << endl;

    camino_actual = mejor_camino;
    reverse(camino_actual.begin(), camino_actual.end());

    if(p2D) {
        mcp_parser(pesos, mejor_camino, filas - 1, columnas - 1);
    }

    if(p) {
        mcp_codificacion(camino_actual);
    }

    return 0;
}