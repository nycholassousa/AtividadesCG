#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include <algorithm> //std::swap
#include <cmath> //sqrt

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

//enum colors
enum colors
{
	RED   = 0,
    GREEN = 1,
    BLUE  = 2,
    ALPHA = 3
};

//Pixel scructure
struct Pixel
{
	int pos_x;
	int pos_y;
	unsigned char color[4];
};

//Create Pixel
Pixel createPixel(int pos_x, int pos_y, 
	unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha,
	Pixel pixel)
{
	pixel.pos_x = pos_x;
	pixel.pos_y = pos_y;

	pixel.color[RED]   = red;
	pixel.color[GREEN] = green;
	pixel.color[BLUE]  = blue;
	pixel.color[ALPHA] = alpha;

	return pixel;
}

//Draw Pixel
void putPixel(Pixel pixel)
{
	int offset = 4 * pixel.pos_x + 4* pixel.pos_y * IMAGE_WIDTH;

	//Caso o pixel passe do limite da tela, não deve ser exibido
	if(pixel.pos_x < 0 || pixel.pos_y < 0 || pixel.pos_x > IMAGE_HEIGHT || pixel.pos_y > IMAGE_WIDTH)
        return;

    //Mostra o pixel na tela
    FBptr[offset + RED]   = pixel.color[RED];
    FBptr[offset + GREEN] = pixel.color[GREEN];
    FBptr[offset + BLUE]  = pixel.color[BLUE];
    FBptr[offset + ALPHA] = pixel.color[ALPHA];
}

//Draw Line
void drawLine(Pixel pixelInitial, Pixel pixelFinal) {
    int delta_x = pixelFinal.pos_x - pixelInitial.pos_x;
    int delta_y = pixelFinal.pos_y - pixelInitial.pos_y;
    int d, incDPos, incDNeg;
     
    if(delta_x < 0) {     // exchange points if order reversed
        std::swap(pixelInitial, pixelFinal);
        delta_x = -delta_x;
        delta_y = -delta_y;
    }

     
    int dySig = (delta_y < 0)?(-1):1;    // delta_y signal
     
    Pixel pixelAuxiliar = pixelInitial;        // buffer for current pixel
    putPixel(pixelInitial);
     
    if( dySig*delta_y <= delta_x ) {               //  |m| <= 1
     
        if(delta_y < 0) {
            d = 2*delta_y + delta_x;            // d_first
            while(pixelAuxiliar.pos_x < pixelFinal.pos_x) {       // for each x
                if(d < 0) {                 // midpoint ABOVE the line
                    d += 2*(delta_y + delta_x);
                    pixelAuxiliar.pos_x++;
                    pixelAuxiliar.pos_y--;
                } else {                    // midpoint BELOW the line
                    d += 2*delta_y;
                    pixelAuxiliar.pos_x++;
                }
                for(int i = RED; i <= ALPHA; i++) {    // for each component, interpolate
                    pixelAuxiliar.color[i] = (unsigned char)(pixelInitial.color[i] + (pixelAuxiliar.pos_x - pixelInitial.pos_x)*(pixelFinal.color[i] - pixelInitial.color[i])/delta_x);
                }
                putPixel(pixelAuxiliar);
            }
        } else {
            d = 2*delta_y - delta_x;            // d_first
            while(pixelAuxiliar.pos_x < pixelFinal.pos_x) {       // for each x
                if(d < 0) {                 //midpoint BELOW the line
                    d += 2*delta_y;
                    pixelAuxiliar.pos_x++;
                } else {                    // midpoint ABOVE the line
                    d += 2*(delta_y - delta_x);
                    pixelAuxiliar.pos_x++;
                    pixelAuxiliar.pos_y++;
                }
                for(int i = RED; i <= ALPHA; i++) {    // for each component, interpolate
                    pixelAuxiliar.color[i] = (unsigned char)(pixelInitial.color[i] + (pixelAuxiliar.pos_x - pixelInitial.pos_x)*(pixelFinal.color[i] - pixelInitial.color[i])/delta_x);
                }
                putPixel(pixelAuxiliar);
            }
        }
    } else {            // |m| > 1        
        if(delta_y < 0) {
            d = delta_y + 2*delta_x;              // d_first
            while( pixelAuxiliar.pos_y > pixelFinal.pos_y ) {       // for each y
                if(d < 0) {                 // midpoint ABOVE the line
                    d += 2*delta_x;
                    pixelAuxiliar.pos_y--;
                } else {                    // midpoint BELOW the line
                    d += 2*(delta_y + delta_x);
                    pixelAuxiliar.pos_x++;
                    pixelAuxiliar.pos_y--;
                }
                for(int i = RED; i <= ALPHA; i++) {    // for each component, interpolate
                    pixelAuxiliar.color[i] = (unsigned char)(pixelInitial.color[i] + (pixelAuxiliar.pos_y - pixelInitial.pos_y)*(pixelFinal.color[i] - pixelInitial.color[i])/delta_y);
                }
                putPixel(pixelAuxiliar);
            }
        } else {
            d = delta_y -2*delta_x;             // d_first
            while( pixelAuxiliar.pos_y < pixelFinal.pos_y ) {       // for each y
                if(d < 0) {                 // midpoint BELOW the line
                    d += 2*(delta_y - delta_x);
                    pixelAuxiliar.pos_x++;
                    pixelAuxiliar.pos_y++;
                } else {                    // midpoint ABOVE the line
                    d += -2*delta_x;
                    pixelAuxiliar.pos_y++;
                }
                for(int i = RED; i <= ALPHA; i++) {    // for each component, interpolate
                    pixelAuxiliar.color[i] = (unsigned char)(pixelInitial.color[i] + (pixelAuxiliar.pos_y - pixelInitial.pos_y)*(pixelFinal.color[i] - pixelInitial.color[i])/delta_y);
                }
                putPixel(pixelAuxiliar);
            }
        }
    }
     
    putPixel(pixelFinal);
}

//Draw Triangle
void drawTriangle(Pixel pixelFinal, Pixel p2, Pixel p3)
{
    drawLine(pixelFinal, p2);
    drawLine(p2, p3);
    drawLine(p3, pixelFinal);
}

//DrawFilled Circle
void drawFilledCircle(Pixel centerPos, int radius)
{

    float sinus = 0.70710678118;

    Pixel pixelAuxiliar;

    //This is the distance on the axis from sin(90) to sin(45). 
    int range = radius/(2*sinus);
    for(int i = radius ; i >= range ; --i)
    {
        int j = sqrt(radius*radius - i*i);
        for(int k = -j ; k <= j ; k++)
        {
            //We draw all the 4 sides at the same time.
            pixelAuxiliar = createPixel(centerPos.pos_x - k, centerPos.pos_y + i, centerPos.color[RED], centerPos.color[GREEN], centerPos.color[BLUE], centerPos.color[ALPHA], pixelAuxiliar);
            putPixel(pixelAuxiliar);
            pixelAuxiliar = createPixel(centerPos.pos_x - k, centerPos.pos_y - i, centerPos.color[RED], centerPos.color[GREEN], centerPos.color[BLUE], centerPos.color[ALPHA], pixelAuxiliar);
            putPixel(pixelAuxiliar);
            pixelAuxiliar = createPixel(centerPos.pos_x + i, centerPos.pos_y + k, centerPos.color[RED], centerPos.color[GREEN], centerPos.color[BLUE], centerPos.color[ALPHA], pixelAuxiliar);
            putPixel(pixelAuxiliar);
            pixelAuxiliar = createPixel(centerPos.pos_x - i, centerPos.pos_y - k, centerPos.color[RED], centerPos.color[GREEN], centerPos.color[BLUE], centerPos.color[ALPHA], pixelAuxiliar);
            putPixel(pixelAuxiliar);
        }
    }
    //To fill the circle we draw the circumscribed square.
    range = radius*sinus;
    for(int i = centerPos.pos_x - range + 1 ; i < centerPos.pos_x + range ; i++)
    {
        for(int j = centerPos.pos_y - range + 1 ; j < centerPos.pos_y + range ; j++)
        {
            pixelAuxiliar = createPixel(i, j, centerPos.color[RED], centerPos.color[GREEN], centerPos.color[BLUE], centerPos.color[ALPHA], pixelAuxiliar);
            putPixel(pixelAuxiliar);
        }
    }
}

#endif // _MYGL_H_

