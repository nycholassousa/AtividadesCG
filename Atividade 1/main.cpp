#include "main.h"

//Apenas pra lembrar como inserir gif no markdown
//![](http://www.reactiongifs.us/wp-content/uploads/2013/10/nuh_uh_conan_obrien.gif)


//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as funções do mygl.h
	//*************************************************************************

Pixel tempPixel;

/* 
for(int i = 0; i < 60; i++) {       // white line
    tempPixel = createPixel(19, 30+i, 255, 255, 255, 0, tempPixel);
    putPixel(tempPixel);
}
 
for(int i = 0; i < 256; i++) {
    int j;
    for(j = 0; j < 20; j++) {        // all levels of RED
        tempPixel = createPixel(20+i, 30+j, i, 0, 0, 0, tempPixel);
        putPixel(tempPixel);
    }
     
    for( ; j < 40; j++) {        // all levels of GREEN
        tempPixel = createPixel(20+i, 30+j, 0, i, 0, 0, tempPixel);
        putPixel(tempPixel);
    }
 
    for( ; j < 60; j++) {        // all levels of BLUE
        tempPixel = createPixel(20+i, 30+j, 0, 0, i, 0, tempPixel);
        putPixel(tempPixel);
    }
} */

drawFilledCircle(150, 150, 100, 255, 255, 255, 0);

	


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

