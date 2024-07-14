//David Pérez Sampedro - 48774775D
#include<iostream>
#include<iomanip>
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
    cerr << "Usage:" << endl << "mcp [-t] [--p2D] [--ignore-naive] -f file" << endl;
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
        if(args[i] != "-t" && args[i] != "--p2D" && args[i] != "--ignore-naive" && args[i] != "-f") {
            mensajeError("unknown option " + args[i] + ".");
        }
    }
}

void validaArgumentos(const int argc, const vector<string> &args, bool &t, bool &p2D, bool &ignore_naive) {
    for(int i = 1; i < argc - 2; i++) {
        if(args[i] == "-t") {
            t = true;
        }
        if(args[i] == "--ignore-naive") {
            ignore_naive = true;
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

double minimo(const double n1, const double n2, const double n3) {
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

bool primeroFila(int posicion, int columnas) {
    for(int j = posicion; j >= 0; j = j - columnas) {
        if(j == 0) {
            return true;
        }
    }
    return false;
}

double minimo_mcp_parser(const vector<vector<double>> &M, const int i, const int j, const int filas, const int columnas) {
    if(filas - i == 0) {                         //Límite del tablero por arriba
        return M[filas - i][columnas - j - 1];
    } else if(columnas - j == 0) {               //Límite del tablero por la izquierda
        return M[filas - i - 1][columnas - j];
    } else {
        return minimo(M[filas - i - 1][columnas - j - 1], M[filas - i - 1][columnas - j], M[filas - i][columnas - j - 1]);
    }
}

//Recursivo sin almacén
double mcp_naive(const vector<vector<int>> &pesos, int i, int j, const int filas, const int columnas) {
    //Última casilla
    if(i == filas - 1 && j == columnas - 1) {
        //return pesos[pesos.size() - 1];
        return pesos[i][j];
    }

    //Fuera del tablero
    if(i == filas || j == columnas) {
        return -1;
    }

    //Movimiento hacia abajo
    double camino_S1 = mcp_naive(pesos, i + 1, j, filas, columnas);
    double S1 = pesos[i][j] + camino_S1;

    //Movimiento hacia la derecha
    double camino_S2 = mcp_naive(pesos, i, j + 1, filas, columnas);
    double S2 = pesos[i][j] + camino_S2;

    //Movimiento diagonal
    double camino_S3 = mcp_naive(pesos, i + 1, j + 1, filas, columnas);
    double S3 = pesos[i][j] + camino_S3;

    if(camino_S1 == -1) {
        if(camino_S3 == -1) {
            return S2;
        }
        return min(S2, S3);
    } else if(camino_S2 == -1) {
        if(camino_S3 == -1) {
            return S1;
        }
        return min(S1, S3);
    }
    return minimo(S1, S2, S3);
}

//Memoización
double mcp_memo(vector<vector<double>> &M, const vector<vector<int>> &pesos, int i, int j, const int filas, const int columnas) {
    double sol;

    //Última casilla
    if(i == filas - 1 && j == columnas - 1) {
        sol = pesos[i][j];
        M[i][j] = sol;
        return sol;     //Última casilla
    }

    //Fuera del tablero
    if(i == filas || j == columnas) {
        return -1;  //Fuera del tablero
    }

    //Si ya se ha hecho, devolver el resultado directamente
    if(M[i][j] != -1) {
        return M[i][j];
    }

    //Movimiento hacia abajo
    double camino_S1 = mcp_memo(M, pesos, i + 1, j, filas, columnas);
    double S1 = pesos[i][j] + camino_S1;

    //Movimiento hacia la derecha
    double camino_S2 = mcp_memo(M, pesos, i, j + 1, filas, columnas);
    double S2 = pesos[i][j] + camino_S2;

    //Movimiento diagonal
    double camino_S3 = mcp_memo(M, pesos, i + 1, j + 1, filas, columnas);
    double S3 = pesos[i][j] + camino_S3;

    if(camino_S1 == -1) {
        if(camino_S3 == -1) {
            sol = S2;
        } else {
            sol = min(S2, S3);
        }
    } else if(camino_S2 == -1) {
        if(camino_S3 == -1) {
            sol = S1;
        } else {
            sol = min(S1, S3);
        }
    } else {
        sol = minimo(S1, S2, S3);
    }

    M[i][j] = sol;
    return sol;
}

//Iterativo con matriz
double mcp_it_matrix(vector<vector<double>> &M, const vector<vector<int>> &pesos, const int filas, const int columnas) {
    double S1, S2, S3;
    for(int i = 0; i <= filas; i++) {
        for(int j = 0; j <= columnas; j++) {
            //Primera posición
            if (i == 0 && j == 0) {
                M[0][0] = pesos[0][0];
            } else {
                if(i - 1 < 0) {
                    M[i][j] = pesos[i][j] + M[i][j - 1];
                } else if(j - 1 < 0) {
                    M[i][j] = pesos[i][j] + M[i - 1][j];
                } else {
                    S1 = pesos[i][j] + M[i - 1][j];
                    S2 = pesos[i][j] + M[i][j - 1];
                    S3 = pesos[i][j] + M[i - 1][j - 1];

                    M[i][j] = minimo(S1, S2, S3);
                }
            }
        }
    }
    return M[filas][columnas];
}

//Iterativo con vector
double mcp_it_vector(const vector<double> &pesos, const int columnas) {
    double S1, S2, S3;
    vector<double> v(pesos.size());
    for(int i = 0; i < (int) pesos.size(); i++) {
        //Primera posición
        if (i == 0) {
            v[i] = pesos[0];
        } else {
            if(i - columnas < 0) {
                v[i] = pesos[i] + v[i - 1];
            } else if(primeroFila(i, columnas)) {
                v[i] = pesos[i] + v[i - columnas];
            } else {
                S1 = pesos[i] + v[i - columnas];
                S2 = pesos[i] + v[i - 1];
                S3 = pesos[i] + v[i - columnas - 1];

                v[i] = minimo(S1, S2, S3);
            }
        }
    }
    return v.back();
}

//Solución mejor camino
void mcp_parser(const vector<vector<double>> &M, const int filas, const int columnas) {
    double min, last_i, last_j = last_i = min = 0;
    vector<vector<char>> sol(filas + 1, vector<char>(columnas + 1));
    for(int i = 0; i <= filas; i++) {
        for(int j = 0; j <= columnas; j++) {
            //Primera casilla
            if(i == 0 && j == 0) {
                sol[filas][columnas] = 'x';
                min = minimo_mcp_parser(M, i, j, filas, columnas);
                last_i = filas - i;
                last_j = columnas - j;
            } else {
                //Si la casilla actual es el mínimo
                if(min == M[filas - i][columnas - j]) {
                    //Comprueba que la casilla sea adyacente a la anterior del camino
                    if((filas - i == last_i && columnas - j + 1 == last_j)
                    || (filas - i + 1 == last_i && columnas - j == last_j)
                    || (filas - i + 1 == last_i && columnas - j + 1 == last_j)) {
                        sol[filas - i][columnas - j] = 'x';
                        min = minimo_mcp_parser(M, i, j, filas, columnas);
                        last_i = filas - i;
                        last_j = columnas - j;
                    } else {
                        sol[filas - i][columnas - j] = '.';
                    }
                } else {
                    sol[filas - i][columnas - j] = '.';
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
    cout << M[filas][columnas] << endl;
}

int main(int argc, char *argv[]) {
    bool t, p2D, ignore_naive = p2D = t = false;
    int filas, columnas = filas = 0;
    vector<string> args(argv, argv + argc);

    //Comprovación y validación de argumnetos
    compruebaArgumentos(argc, args);
    validaArgumentos(argc, args, t, p2D, ignore_naive);

    //Crear matriz de pesos
    vector<vector<int>> pesos = crearMatrizPesos(argc, args, filas, columnas);

    //Crear almacén de mcp_memo y mcp_it_matrix
    //El resultado que queda almacenado al final es el de mcp_it_matrix
    vector<vector<double>> memo(filas, vector<double>(columnas, -1));

    //Matriz de pesos en forma de vector
    vector<double> pesosVector;
    for(const vector<int> &fila : pesos) {
        pesosVector.insert(pesosVector.end(), fila.begin(), fila.end());
    }

    if(ignore_naive) {
        cout << "-";
    } else {
        cout << mcp_naive(pesos, 0, 0, filas, columnas);
    }

    cout << " " << mcp_memo(memo, pesos, 0, 0, filas, columnas);
    cout << " " << mcp_it_matrix(memo, pesos, filas - 1, columnas - 1);
    cout << " " << mcp_it_vector(pesosVector, columnas) << endl;

    if(p2D) {
        mcp_parser(memo, filas - 1, columnas - 1);
    }

    if(t) {
        //Saltos de línea para las columnas
        int digitos = log10(memo[filas - 1][columnas - 1]) + 1;
        if(digitos == 1) {
            digitos = 2;
        }

        //Dibujar matriz
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                cout << std::setw(digitos) << memo[i][j] << " ";
            }
            cout << endl;
        }
    }

    return 0;
}