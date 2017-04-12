#include "matrixlib.h"

//Construtor
//Cria a matriz baseado no numero de linhas e colunas informado no parâmetro
MatrixLib::MatrixLib(unsigned int num_lines, unsigned int num_columns) {
    _lines   = num_lines;
    _columns = num_columns;
    _matrix  = new double*[_lines];
    
    for(int i = 0; i < _lines; i++)
        _matrix[i] = new double[_columns];
}

//Destrutor
//Apaga os valores em cada posição da matriz, após isso apaga a matriz em si
MatrixLib::~MatrixLib() {
    for(int i = 0; i < _lines; i++)
        delete[] _matrix[i];

    delete[] _matrix;
}

//Setter
//Seta um valor na posição informada
void MatrixLib::setValue(unsigned int line, unsigned int column, double value) {
    _matrix[line][column] = value;
}

//Getter
//Obtem um valor na posição informada
double MatrixLib::getValue(unsigned int line, unsigned int column) {
    return _matrix[line][column];
}

//Setter 2
//Seta um conjunto de valores de um array na matriz
void MatrixLib::setValues(double values[]) {
    size_t size = _columns * sizeof(double);
    
    for(int l = 0; l < _lines; l++)
        memcpy(_matrix[l], values + (l * _columns), size);
}

//Getter Line
//Obtem o numero de linhas da matriz
unsigned int MatrixLib::numLines() {
    return _lines;
}

//Getter Column
//Obtem o numero de colunas da matriz
unsigned int MatrixLib::numColumns() {
    return _columns;
}

//Carrega a identidade na matriz
void MatrixLib::loadIdentity() {
    size_t size = sizeof(double) * _columns;
    
    for(int l = 0; l < _lines; l++) {
        memset(_matrix[l], 0, size);
        _matrix[l][l] = 1;
    }
}

//Multiplica 2 matrizes
void MatrixLib::mult(MatrixLib& m1, MatrixLib& m2) {  
    double res[_lines * _columns];
    
    for(int l = 0; l < _lines; l++) {
        for(int c = 0; c < _columns; c++) {
            res[_columns * l + c] = 0.0;
            
            for(int cl = 0; cl < m1.numColumns(); cl++) {
                res[_columns * l + c] += m1.getValue(l, cl) * m2.getValue(cl, c);
            }
        }
    }
    
    this->setValues(res);
}

//Divide 2 matrizes
void MatrixLib::div(MatrixLib& m1, double value) {
    for(int l = 0; l < _lines; l++)
        for(int c = 0; c < _columns; c++)
            this->setValue(l, c, m1.getValue(l, c) / value);
}