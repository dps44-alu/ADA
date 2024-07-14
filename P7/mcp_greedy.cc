//David Pérez Sampedro - 48774775D
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>
#include<sstream>
using namespace std;

void mensajeError(const string &error) {
    if(!error.empty()) {
        cerr << "ERROR: " << error << endl;
    }
    cerr << "Usage:" << endl << "mcp_greedy [--p2D] -f file" << endl;
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
        if(args[i] != "--p2D" && args[i] != "-f") {
            mensajeError("unknown option " + args[i] + ".");
        }
    }
}

void validaArgumentos(const int argc, const vector<string> &args, bool &p2D) {
    for(int i = 1; i < argc - 2; i++) {
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

    ifstream doc(args[argc - 1]);
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

int minimo(const int n1, const int n2, const int n3) {
    if(n1 <= n2) {
        if(n1 <= n3) {
            return n1;
        }
        return n3;
    }
    if(n2 <= n3) {
        return n2;
    }
    return n3;
}

int mcp_greedy(const vector<vector<int>> &pesos, const int filas, const int columnas, bool first) {
    if(first) {
        int i, j = i = 0;
        int min, S1, S2, S3;
        int sol = pesos[i][j];
        while (i != filas - 1 ||  j != columnas - 1) {
            if(i + 1 == filas) {
                sol += pesos[i][j + 1];  //Movimiento hacia la derecha
                j += 1;
            } else if(j + 1 == columnas) {
                sol += pesos[i + 1][j];  //Movimiento hacia abajo
                i += 1;
            } else {
                S1 = sol + pesos[i + 1][j + 1];  //Movimiento diagonal
                S2 = sol + pesos[i][j + 1];  //Movimiento hacia la derecha
                S3 = sol + pesos[i + 1][j];  //Movimiento hacia abajo

                min = minimo(S1, S2, S3);

                if (min == S1) {
                    sol = S1;
                    i += 1;
                    j += 1;
                } else if (min == S2) {
                    sol = S2;
                    j += 1;
                } else {
                    sol = S3;
                    i += 1;
                }
            }
        }
        return sol;
    } else {
        int i = filas - 1;
        int j = columnas - 1;
        int min, S1, S2, S3;
        int sol = pesos[i][j];
        while (i != -1 && j != -1) {
            if(i - 1 == -1) {
                sol += pesos[i][j - 1];  //Movimiento hacia la izquierda
                j -= 1;
            } else if(j - 1 == -1) {
                sol += pesos[i - 1][j];  //Movimiento hacia arriba
                i -= 1;
            } else {
                S1 = sol + pesos[i - 1][j - 1];  //Movimiento diagonal
                S2 = sol + pesos[i][j - 1];  //Movimiento hacia la izquierda
                S3 = sol + pesos[i - 1][j];  //Movimiento hacia arriba

                min = minimo(S1, S2, S3);

                if (min == S1) {
                    sol = S1;
                    i -= 1;
                    j -= 1;
                } else if (min == S2) {
                    sol = S2;
                    j -= 1;
                } else {
                    sol = S3;
                    i -= 1;
                }
            }
        }
        return sol;
    }
}

int minimo_mcp_parser(const vector<vector<int>> &M, const int i, const int j, const int filas, const int columnas, const int first, string &movimiento) {
    if(first) {
        if (i == filas) {                         //Límite del tablero por debajo
            movimiento = "horizontal";
            return M[i][j + 1];
        } else if (j == columnas) {               //Límite del tablero por la derecha
            movimiento = "vertical";
            return M[i + 1][j];
        } else {
            int min = minimo(M[i + 1][j + 1], M[i][j + 1], M[i + 1][j]);
            if(min == M[i + 1][j + 1]) {
                movimiento = "diagonal";
            } else if (min == M[i][j + 1]) {
                movimiento = "horizontal";
            } else {
                movimiento = "vertical";
            }
            return min;
        }
    } else {
        if (i == 0) {                         //Límite del tablero por arriba
            movimiento = "horizontal";
            return M[i][j - 1];
        } else if (j == 0) {               //Límite del tablero por la izquierda
            movimiento = "vertical";
            return M[i - 1][j];
        } else {
            int min = minimo(M[i - 1][j - 1], M[i][j - 1], M[i - 1][j]);
            if(min == M[i - 1][j - 1]) {
                movimiento = "diagonal";
            } else if (min == M[i][j - 1]) {
                movimiento = "horizontal";
            } else {
                movimiento = "vertical";
            }
            return min;
        }
    }
}

//Solución mejor camino
void mcp_parser(const vector<vector<int>> &M, const int filas, const int columnas, const bool first) {
    vector<vector<char>> sol(filas + 1, vector<char>(columnas + 1));
    int min, last_i, last_j = last_i = min = 0;
    string movimiento; //diagonal, horizontal, vertical
    if(first) {
        for (int i = 0; i <= filas; i++) {
            for (int j = 0; j <= columnas; j++) {
                //Primera casilla
                if (i == 0 && j == 0) {
                    sol[i][j] = 'x';
                    min = minimo_mcp_parser(M, i, j, filas, columnas, true, movimiento);
                    last_i = i;
                    last_j = j;
                } else {
                    //Si la casilla actual es el mínimo
                    if (min == M[i][j]) {
                        //Comprueba que la casilla sea adyacente a la anterior del camino
                        if ((i == last_i && j - 1 == last_j && movimiento == "horizontal")
                            || (i - 1 == last_i && j == last_j && movimiento == "vertical")
                            || (i - 1 == last_i && j - 1 == last_j && movimiento == "diagonal")) {
                            sol[i][j] = 'x';
                            min = minimo_mcp_parser(M, i, j, filas, columnas, true, movimiento);
                            last_i = i;
                            last_j = j;
                        } else {
                            sol[i][j] = '.';
                        }
                    } else {
                        sol[i][j] = '.';
                    }
                }
            }
        }
    } else {
        for (int i = filas; i >= 0; i--) {
            for (int j = columnas; j >= 0; j--) {
                //Última casilla
                if (i == filas && j == columnas) {
                    sol[i][j] = 'x';
                    min = minimo_mcp_parser(M, i, j, filas, columnas, false, movimiento);
                    last_i = i;
                    last_j = j;
                } else {
                    //Si la casilla actual es el mínimo
                    if (min == M[i][j]) {
                        //Comprueba que la casilla sea adyacente a la anterior del camino
                        if ((i == last_i && j + 1 == last_j && movimiento == "horizontal")
                            || (i + 1 == last_i && j == last_j && movimiento == "vertical")
                            || (i + 1 == last_i && j + 1 == last_j && movimiento == "diagonal")) {
                            sol[i][j] = 'x';
                            min = minimo_mcp_parser(M, i, j, filas, columnas, false, movimiento);
                            last_i = i;
                            last_j = j;
                        } else {
                            sol[i][j] = '.';
                        }
                    } else {
                        sol[i][j] = '.';
                    }
                }
            }
        }
    }

    //Dibujar camino
    for(int i = 0; i <= filas; i++) {
        for(int j = 0; j <= columnas; j++) {
            cout << sol[i][j];
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    bool p2D = false;
    int filas, columnas = filas = 0;
    int first, last;
    vector<string> args(argv, argv + argc);

    //Comprovación y validación de argumnetos
    compruebaArgumentos(argc, args);
    validaArgumentos(argc, args, p2D);

    //Crear matriz de pesos
    vector<vector<int>> pesos = crearMatrizPesos(argc, args, filas, columnas);

    first = mcp_greedy(pesos, filas, columnas, true);
    last = mcp_greedy(pesos, filas, columnas, false);

    cout << first << " " << last << endl;

    if(p2D) {
        if(first <= last) {
            mcp_parser(pesos, filas - 1, columnas - 1, true);
            cout << first << endl;
        } else {
            mcp_parser(pesos, filas - 1, columnas - 1, false);
            cout << last << endl;
        }
    }

    return 0;
}