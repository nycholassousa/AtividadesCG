#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include <cmath>

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

//Organização para o array que indica o RGBA
enum colors
{
	RED   = 0,
    GREEN = 1,
    BLUE  = 2,
    ALPHA = 3
};

//Estrutura do pixel
struct Pixel
{
	int pos_x;
	int pos_y;
	unsigned char color[4];
};

//Criação do pixel
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

//Desenha o pixel na tela
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

//Desenha a linha na tela
void drawLine(Pixel pixelInitial, Pixel pixelFinal)
{
    Pixel pixelAux = pixelInitial;

    int delta_x = pixelFinal.pos_x - pixelInitial.pos_x;
    int delta_y = pixelFinal.pos_y - pixelInitial.pos_y;

    float delta_x_red = ((float)pixelFinal.color[RED]   - pixelInitial.color[RED])/delta_x;
    float delta_x_green = ((float)pixelFinal.color[GREEN] - pixelInitial.color[GREEN])/delta_x;
    float delta_x_blue = ((float)pixelFinal.color[BLUE]  - pixelInitial.color[BLUE])/delta_x;
    float delta_x_alpha = ((float)pixelFinal.color[ALPHA] - pixelInitial.color[ALPHA])/delta_x;

    float delta_y_red = ((float)pixelFinal.color[RED]   - pixelInitial.color[RED])/delta_y;
    float delta_y_green = ((float)pixelFinal.color[GREEN] - pixelInitial.color[GREEN])/delta_y;
    float delta_y_blue = ((float)pixelFinal.color[BLUE]  - pixelInitial.color[BLUE])/delta_y;
    float delta_y_alpha = ((float)pixelFinal.color[ALPHA] - pixelInitial.color[ALPHA])/delta_y;


    if (delta_x == 0){ //coluna
        if (pixelFinal.pos_y > pixelInitial.pos_y){
            putPixel(pixelAux);
            for (int i = pixelInitial.pos_y; i <= pixelFinal.pos_y; i++){
                pixelAux.color[RED]   += (char)delta_y_red;
                pixelAux.color[GREEN] += (char)delta_y_green;
                pixelAux.color[BLUE]  += (char)delta_y_blue;
                pixelAux.color[ALPHA] += (char)delta_y_alpha;
                pixelAux.pos_y = i;
                putPixel(pixelAux);
            }
        } else {
            putPixel(pixelAux);
            for (int i = pixelFinal.pos_y; i <= pixelInitial.pos_y; i++){
                pixelAux.color[RED]   += (char)delta_y_red;
                pixelAux.color[GREEN] += (char)delta_y_green;
                pixelAux.color[BLUE]  += (char)delta_y_blue;
                pixelAux.color[ALPHA] += (char)delta_y_alpha;
                pixelAux.pos_y = i;
                putPixel(pixelAux);
            }
        }
        return;
    }
    if (delta_y == 0){ //coluna
        if (pixelFinal.pos_x > pixelInitial.pos_x){
            putPixel(pixelAux);
            for (int i = pixelInitial.pos_x; i <= pixelFinal.pos_x; i++){
                pixelAux.color[RED]   += (char)delta_x_red;
                pixelAux.color[GREEN] += (char)delta_x_green;
                pixelAux.color[BLUE]  += (char)delta_x_blue;
                pixelAux.color[ALPHA] += (char)delta_x_alpha;
                pixelAux.pos_x = i;
                putPixel(pixelAux);
            }
        } else {
            putPixel(pixelAux);
            for (int i = pixelFinal.pos_x; i <= pixelInitial.pos_x; i++){
                pixelAux.color[RED]   += (char)delta_x_red;
                pixelAux.color[GREEN] += (char)delta_x_green;
                pixelAux.color[BLUE]  += (char)delta_x_blue;
                pixelAux.color[ALPHA] += (char)delta_x_alpha;
                pixelAux.pos_x = i;
                putPixel(pixelAux);
            }
        }
        return;
    }


    int changeY = 0;
    int aux;

    if (delta_x < 0){
        aux            = pixelInitial.pos_x;
        pixelInitial.pos_x = pixelFinal.pos_x;
        pixelFinal.pos_x   = aux;

        aux            = pixelInitial.pos_y;
        pixelInitial.pos_y = pixelFinal.pos_y;
        pixelFinal.pos_y   = aux;

        delta_x = -delta_x;
        delta_y = -delta_y;
    }
    if (delta_y < 0){
        changeY = -1;
        delta_y = -delta_y;
    }

    int d  = 2 * delta_y - delta_x;
    int E  = 2 * delta_y;
    int NE = 2 * (delta_y - delta_x);

    pixelAux = pixelInitial;
    putPixel(pixelAux);

    int swap = 0;
    if (abs(delta_y) > abs(delta_x)){
        swap = 1;

        aux = pixelInitial.pos_x;
        pixelInitial.pos_x = pixelFinal.pos_y;
        pixelFinal.pos_y = aux;

        aux = pixelFinal.pos_x;
        pixelFinal.pos_x = pixelFinal.pos_y;
        pixelFinal.pos_y = aux;

        delta_x = pixelFinal.pos_x - pixelInitial.pos_x;
        delta_y = pixelFinal.pos_y = pixelInitial.pos_y;
    }

    if (swap == 1){
        int aux;
        aux = pixelAux.pos_x;
        pixelAux.pos_x = pixelAux.pos_y;
        pixelAux.pos_y = aux;
        putPixel(pixelAux);
    }
    else
        putPixel(pixelAux);

    while(pixelAux.pos_x < pixelFinal.pos_x){
        pixelAux.color[RED] += (char)delta_y_red;
        pixelAux.color[GREEN] += (char)delta_y_green;
        pixelAux.color[BLUE] += (char)delta_y_blue;
        pixelAux.color[ALPHA] += (char)delta_y_alpha;
        if (d <= 0){
            d = d + E;
            pixelAux.pos_x++;
        } else {
            d = d + NE;
            pixelAux.pos_x++;
            pixelAux.pos_y++;
        }
        putPixel(pixelAux);
    }
}

//Desenha o Triangulo na tela
void drawTriangle(Pixel p1, Pixel p2, Pixel p3)
{
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}

// Desenha um circulo na tela
void drawCircle(int pos_x, int pos_y, int radius, 
	unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    int x = radius;
    int y = 0;
    int err = 0;
    Pixel temp_pixel;

    while (x >= y)
    {
    	temp_pixel = createPixel(pos_x + x, pos_y + y, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x + y, pos_y + x, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x - y, pos_y + x, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x - x, pos_y + y, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x - x, pos_y - y, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x - y, pos_y - x, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x + y, pos_y - x, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);
    	temp_pixel = createPixel(pos_x + x, pos_y - y, red, green, blue, alpha, temp_pixel);
        putPixel(temp_pixel);

        if (err <= 0)
        {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void drawFilledCircle(int pos_x, int pos_y, int radius,
	unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{

    //This here is sin(45) but i just hard-coded it.
    float sinus = 0.70710678118;

    //Pixel auxiliar
    Pixel temp_pixel;

    //This is the distance on the axis from sin(90) to sin(45). 
    int range = radius/(2*sinus);
    for(int i = radius ; i >= range ; --i)
    {
        int j = sqrt(radius*radius - i*i);
        for(int k = -j ; k <= j ; k++)
        {
            //We draw all the 4 sides at the same time.
            temp_pixel = createPixel(pos_x - k, pos_y + i, red, green, blue, alpha, temp_pixel);
            putPixel(temp_pixel);
            temp_pixel = createPixel(pos_x - k, pos_y - i, red, green, blue, alpha, temp_pixel);
            putPixel(temp_pixel);
            temp_pixel = createPixel(pos_x + i, pos_y + k, red, green, blue, alpha, temp_pixel);
            putPixel(temp_pixel);
            temp_pixel = createPixel(pos_x - i, pos_y - k, red, green, blue, alpha, temp_pixel);
            putPixel(temp_pixel);
        }
    }
    //To fill the circle we draw the circumscribed square.
    range = radius*sinus;
    for(int i = pos_x - range + 1 ; i < pos_x + range ; i++)
    {
        for(int j = pos_y - range + 1 ; j < pos_y + range ; j++)
        {
        	temp_pixel = createPixel(i, j, red, green, blue, alpha, temp_pixel);
            putPixel(temp_pixel);
        }
    }
}

#endif // _MYGL_H_

