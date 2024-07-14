#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
using namespace std;

vector<vector<int>> crearMatrizPesos(int &filas, int &columnas) {
    string linea;
    int n, i, j = i = 0;
    bool primera = true;
    vector<vector<int>> pesos;

    ifstream doc("tests/08.map");
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
    for(int j = posicion; j >= 0; j = j / columnas) {
        if(j == 0) {
            return true;
        }
    }
    return false;
}

//Iterativo con tabla
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

double minimo_mcp_parser(const vector<vector<double>> &M, const int i, const int j, const int filas, const int columnas) {
    if(filas - i == 0) {                         //Límite del tablero por arriba
        return M[filas - i][columnas - j - 1];
    } else if(columnas - j == 0) {               //Límite del tablero por la izquierda
        return M[filas - i - 1][columnas - j];
    } else {
        return minimo(M[filas - i - 1][columnas - j - 1], M[filas - i - 1][columnas - j], M[filas - i][columnas - j - 1]);
    }
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
                if(min == M[filas - i][columnas - j]) {     //Si la casilla actual es el mínimo
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
    int filas, columnas = filas = 0;

    //Crear matriz de pesos
    vector<vector<int>> pesos = crearMatrizPesos(filas, columnas);

    //Crear almacén de mcp_memo y mcp_it_matrix
    vector<vector<double>> memo(filas, vector<double>(columnas, -1));

    cout << mcp_it_matrix(memo, pesos, filas - 1, columnas - 1) << endl;

    mcp_parser(memo, filas - 1, columnas - 1);

    return 0;
}