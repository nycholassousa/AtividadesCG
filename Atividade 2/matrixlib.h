#ifndef MY_MATRIX_H
#define MY_MATRIX_H

#include <cstring>
#include <cstdlib>

class MatrixLib {
    private:
        double**     _matrix;
        unsigned int _lines;
        unsigned int _columns;
        
    public:
        MatrixLib(unsigned int num_lines, unsigned int num_columns);
        virtual ~MatrixLib();
        
        void setValue(unsigned int line, unsigned int column, double value);
        double getValue(unsigned int line, unsigned int column);
        
        void setValues(double values[]);
        
        unsigned int numLines();
        unsigned int numColumns();
        
        void loadIdentity();
        
        void mult(MatrixLib& m1, MatrixLib& m2);
        void div(MatrixLib& m1, double value);
};

#endif