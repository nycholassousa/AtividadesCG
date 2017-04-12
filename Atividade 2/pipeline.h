#ifndef GRAPHIC_PIPELINE_H
#define GRAPHIC_PIPELINE_H

#include <cmath>
#include "matrixlib.h"

typedef MatrixLib Vector;
typedef MatrixLib Matrix;

//Estrutura que representa um ponto (Ponto != Pixel)
struct Point
{
    int x;
    int y;
};

/*****************************************************************/
double norm(Matrix * mtx);
void cross(Matrix * v1, Matrix * v2, Matrix * res);
void init();
void scaleGL(double x, double y, double z);
void shearGL(double x, double y, double z);
void rotateGL(double angle, double x, double y, double z);
void translateGL(double dx, double dy, double dz);
void createCamera(double pos_x, double pos_y, double pos_z, double lookat_x, double lookat_y, double lookat_z, double up_x, double up_y, double up_z);
void viewportGL(int x, int y, size_t width, size_t height);
void viewPlaneDGL(double dist);
Point createPipeline(double x, double y, double z);
/*****************************************************************/

//Criando as matrizes básicas para o pipeline
Matrix matrix_model(4, 4);
Matrix matrix_view(4, 4);
Matrix matrix_projection(4, 4);
Matrix matrix_viewport(4, 4);

//Normalização do vetor
double norm(Vector * vec)
{
    return sqrt(
                vec->getValue(0, 0) * vec->getValue(0, 0) +
                vec->getValue(1, 0) * vec->getValue(1, 0) +
                vec->getValue(2, 0) * vec->getValue(2, 0)
    );
}

//Produto vetorial
void cross(Vector * v1, Vector * v2, Vector * res)
{
    res->setValue(0, 0, v1->getValue(1, 0) * v2->getValue(2, 0) - v2->getValue(1, 0) * v1->getValue(2, 0));
    res->setValue(1, 0, v1->getValue(2, 0) * v2->getValue(0, 0) - v2->getValue(2, 0) * v1->getValue(0, 0));
    res->setValue(2, 0, v1->getValue(0, 0) * v2->getValue(1, 0) - v2->getValue(0, 0) * v1->getValue(1, 0));
}

//Inicializãção das matrizes básicas
void init()
{
    matrix_model.loadIdentity();
    matrix_view.loadIdentity();
    matrix_projection.loadIdentity();
    matrix_viewport.loadIdentity();
}

//Matriz de escala
void scaleGL(double x, double y, double z)
{
    Matrix scale(4, 4);
    
    scale.loadIdentity();
    
    scale.setValue(0, 0, x);
    scale.setValue(1, 1, y);
    scale.setValue(2, 2, z);
    
    matrix_model.mult(matrix_model, scale);
}

//Matriz de shear
void shearGL(double x, double y, double z)
{
    Matrix shear(4, 4);
    shear.loadIdentity();

    shear.setValue(0, 1, x);
    shear.setValue(0, 2, x);

    shear.setValue(1, 0, y);
    shear.setValue(1, 2, y);

    shear.setValue(2, 0, z);
    shear.setValue(2, 1, z);

    matrix_model.mult(matrix_model, shear);
}

//Matriz de rotação
void rotateGL(double angle, double x, double y, double z)
{
    Matrix rotate(4, 4);
    
    rotate.loadIdentity();
    
    if(x > 0.0) {
        rotate.setValue(1, 1, cos(angle));
        rotate.setValue(1, 2, -sin(angle));
        rotate.setValue(2, 1, sin(angle));
        rotate.setValue(2, 2, cos(angle));
    }
    
    if(y > 0.0) {
        rotate.setValue(0, 0, cos(angle));
        rotate.setValue(0, 2, sin(angle));
        rotate.setValue(2, 0, -sin(angle));
        rotate.setValue(2, 2, cos(angle));
    }
    
    if(z > 0.0) {
        rotate.setValue(0, 0, cos(angle));
        rotate.setValue(0, 1, -sin(angle));
        rotate.setValue(1, 0, sin(angle));
        rotate.setValue(1, 1, cos(angle));
    }
    
    matrix_model.mult(matrix_model, rotate);
}

//Matriz de translação
void translateGL(double dx, double dy, double dz)
{
    Matrix translate(4, 4);
    
    translate.loadIdentity();
    
    translate.setValue(0, 3, dx);
    translate.setValue(1, 3, dy);
    translate.setValue(2, 3, dz);
    
    matrix_model.mult(matrix_model, translate);
}

//Criação da Câmera
void createCamera(double pos_x, double pos_y, double pos_z, 
    double lookat_x, double lookat_y, double lookat_z, 
    double up_x, double up_y, double up_z)
{
    Vector x_cam(3, 1);
    Vector y_cam(3, 1);
    Vector z_cam(3, 1);

    Matrix Bt(4, 4);
    Matrix T(4, 4);
    
    Vector aux(3, 1);
    Vector up(3, 1);

    double vec_pl[] = {pos_x - lookat_x, pos_y - lookat_y, pos_z - lookat_z};
    double vec_up[]  = {up_x, up_y, up_z};
    
    aux.setValues(vec_pl);
    up.setValues(vec_up);
    
    z_cam.div(aux, norm(&aux));

    cross(&up, &z_cam, &aux);
    
    x_cam.div(aux, norm(&aux));
    
    cross(&z_cam, &x_cam, &aux);
    y_cam.div(aux, norm(&aux));
    
    double bt_mtx[] = {
        x_cam.getValue(0, 0), x_cam.getValue(1, 0), x_cam.getValue(2, 0), 0,
        y_cam.getValue(0, 0), y_cam.getValue(1, 0), y_cam.getValue(2, 0), 0,
        z_cam.getValue(0, 0), z_cam.getValue(1, 0), z_cam.getValue(2, 0), 0,
                0,                    0,                    0,            1
    };
    
    Bt.setValues(bt_mtx);
    
    T.loadIdentity();
    T.setValue(0, 3, -pos_x);
    T.setValue(1, 3, -pos_y);
    T.setValue(2, 3, -pos_z);
    
    matrix_view.loadIdentity();

    matrix_view.mult(Bt, T);
}

//Criação da ViewPort
void viewportGL(int x, int y, size_t width, size_t height)
{
    Matrix S1(4, 4);
    Matrix S2(4, 4);
    Matrix T(4, 4);

    S1.loadIdentity();
    S2.loadIdentity();
    T.loadIdentity();
    
    S1.setValue(1, 1, -1);
    
    S2.setValue(0, 0, (width - 1) / 2.0f);
    S2.setValue(1, 1, (height - 1) / 2.0f);
    
    T.setValue(0, 3, 1);
    T.setValue(1, 3, 1);
    
    matrix_viewport.loadIdentity();
    
    matrix_viewport.mult(T, S1);
    matrix_viewport.mult(S2, matrix_viewport);
}

//Setando distancia D do View Plane
void viewPlaneDGL(double dist)
{
    double d = dist;
    
    matrix_projection.setValue(2, 3, d);
    matrix_projection.setValue(3, 2, (-1) / d);
    matrix_projection.setValue(3, 3, 0);
}

//Criação do Pipeline em si
Point createPipeline(double x, double y, double z)
{
    double vert[] = {x, y, z, 1};
    Matrix vertice(4, 1);
    vertice.setValues(vert);

    vertice.mult(matrix_model, vertice);
    vertice.mult(matrix_view, vertice);
    vertice.mult(matrix_projection, vertice);
    vertice.div(vertice, vertice.getValue(3, 0));
    vertice.mult(matrix_viewport, vertice);

    Point p = {.x = (int)vertice.getValue(0, 0), .y = (int)vertice.getValue(1, 0)};
    
    return p;
}

#endif








