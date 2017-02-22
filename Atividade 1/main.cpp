#include "main.h"

void bresenhamDemo(){
    Pixel pixel1  = createPixel((IMAGE_WIDTH-1)/2, 0, 255, 0, 0, 255, pixel1);
    Pixel pixel2  = createPixel((IMAGE_WIDTH-1)/2, IMAGE_HEIGHT-1, 255, 255, 0, 255, pixel2);

    Pixel pixel3  = createPixel(0, (IMAGE_HEIGHT-1)/2, 0, 255, 0, 255, pixel3);
    Pixel pixel4  = createPixel(IMAGE_WIDTH-1, (IMAGE_HEIGHT-1)/2, 0, 255, 255, 255, pixel4);

    Pixel pixel5  = createPixel(0, 0, 0, 255, 255, 255, pixel5);
    Pixel pixel6  = createPixel(IMAGE_WIDTH-1, IMAGE_HEIGHT-1, 0, 0, 255, 255, pixel6);

    Pixel pixel7  = createPixel(0, IMAGE_HEIGHT-1, 0, 0, 255, 255, pixel7);
    Pixel pixel8  = createPixel(IMAGE_WIDTH-1, 0, 255, 0, 255, 255, pixel8);

    drawLine(pixel1, pixel2);
    drawLine(pixel3, pixel4);
    drawLine(pixel5, pixel6);
    drawLine(pixel7, pixel8);
}

void triangleDemo(){
    Pixel pixel1  = createPixel((IMAGE_WIDTH-1)/2, 0, 255, 0, 0, 255, pixel1);
    Pixel pixel2  = createPixel((IMAGE_WIDTH-1)/2, IMAGE_HEIGHT-1, 255, 255, 0, 255, pixel2);
    Pixel pixel3  = createPixel(0, (IMAGE_HEIGHT-1)/2, 0, 255, 0, 255, pixel3);

    drawTriangle(pixel1, pixel2, pixel3);
}

void drawFilledCircleDemo(){
    Pixel center = createPixel(IMAGE_WIDTH/2, IMAGE_HEIGHT/2, 255, 255, 0, 255, center);
    drawFilledCircle(center, 100);
}

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as funções do mygl.h
	//*************************************************************************

    //bresenhamDemo();
    //triangleDemo();
    drawFilledCircleDemo();

}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

