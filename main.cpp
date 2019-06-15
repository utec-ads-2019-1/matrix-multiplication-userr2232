#include <iostream>
#include <stdexcept>
#include <thread>
#include <pthread.h>
#include <cstdlib>
#include <vector>

using namespace std;

int nThreads;
int **A, **B, **C;
int ra, rb, ca, cb;

int** createMatrix(int rows, int columns) {
    int **matrix = new int*[rows];
    if(rows) {
        matrix[0] = new int[rows*columns];
        for(int i = 1; i < rows; ++i)
            matrix[i] = matrix[0] + i * columns;
    }
    return matrix;
}

void* multiply(void* index) {
    int initial = (int)(size_t) index;
    for(int i = initial; i < ra; i += nThreads) {
        for(int j = 0; j < cb; ++j) {
            C[i][j] = 0;
            for(int k = 0; k < ca; ++k) {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    
    return nullptr;
}

void fill(int** matrix, int rows, int columns) {
    for(int row = 0; row < rows; ++row)
        for(int column = 0; column < columns; ++column)
            matrix[row][column] = rand()%10;
}

void print(int** matrix, int rows, int columns) {
    cout << endl;
    for(int row = 0; row < rows; ++row) {
        for(int column = 0; column < columns; ++column)
            cout << matrix[row][column] << " ";
        cout << endl;
    }
    cout << endl;
}

int main() {
    nThreads = thread::hardware_concurrency();
    cout << "Matriz A" << endl;
    cout << "Ingrese el número de filas: ";
    cin >> ra;
    cout << "Ingrese el número de columnas: ";
    cin >> ca;
    cout << "Matriz B" << endl;
    cout << "Ingrese el número de filas: ";
    cin >> rb;
    cout << "Ingrese el número de columnas: ";
    cin >> cb;

    if(ca != rb) throw runtime_error("El numero de columnas de la primera matriz y filas de la segunda no coincide");

    A = createMatrix(ra, ca);
    B = createMatrix(rb, cb);
    C = createMatrix(ra, cb);

    fill(A, ra, ca);
    cout << "Matriz A";
    print(A, ra, ca);
    fill(B, rb, cb);
    cout << "Matriz B";
    print(B, rb, cb);

    vector<pthread_t> threads(nThreads);

    for(int i = 0; i < nThreads; ++i)
        if(pthread_create(&threads[i], nullptr, multiply, (void*)i)) {
            cout << "Ocurrió un error mientras se intentaba crear el thread." << endl;
            exit(-1);
        }

    for (int i = 0; i < nThreads; ++i)
		if(pthread_join(threads[i], nullptr)) {
            cout << "No se pudo cerrar el thread." << endl;
            exit(-1);
        }
    
    cout << "Matriz C";
    print(C, ra, cb);
    
    return 0;
}