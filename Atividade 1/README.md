# Familiarização com Algoritmos de Rasterização

## Atividade Proposta
Implementar as funções de desenhos de pontos, retas e triângulos com interpolação linear das cores dos vértices e dos pontos iniciais e finais das retas.

## O que é rasterização
Rasterização, é a tarefa de tomar uma imagem descrita em um imagem vetorial e convertê-la em uma imagem raster (pixels ou pontos) para a saída em vídeo ou impressora.[1]

## Início da rasterização
Para começar a rasterização, deve-se levar em conta a primitiva de uma reta, que é o ponto, e de forma computacional, o pixel é considerado um ponto. Com isso, antes de criar uma reta, a primeira coisa que deve-se criar é um ponto.

### Criando um ponto
Na rasterização, um ponto é composto por suas coordenadas, além disso, possui as informações de cores, o RGBA.
Para isso, criei uma *struct* chamada Pixel, que guarda as informações do pixel:

	struct Pixel
	{
		int pos_x;
		int pos_y;

		unsigned char color[4];
	}
Foi-se usado *unsigned* pois na representação de cores, usamos apenas os valores de 0 a 255, que é o tamanho de um *char* sem sinal.

Com isso, foi-se criado um *enum*, onde ficaria mais fácil indicar com qual elemento do array é referenciado a qual elemento do RGBA;

	enum colors
	{
		RED   = 0,
    	GREEN = 1,
    	BLUE  = 2,
    	ALPHA = 3
	};

Pronto, assim temos toda a estrutura pra começar a criar um pixel, porém, ainda precisamos inicializar-lo, assim, foi feito da seguinte maneira:

	Pixel createPixel(int pos_x, int pos_y, char red, unsigned char green, unsigned char blue, unsigned char alpha, Pixel pixel)
	{
		pixel.pos_x = pos_x; //Coordenada X do Pixel
		pixel.pos_y = pos_y; //Coordenada Y do Pixel

		pixel.color[RED]   = red; //Posição 0 do array representa o red
		pixel.color[GREEN] = green; //Posição 1 do array representa o green
		pixel.color[BLUE]  = blue; //Posição 2 do array representa o blue
		pixel.color[ALPHA] = alpha; //Posição 3 do array representa o alpha

		return pixel;
	}

Considere esse createPixel como um construtor.

Com o pixel criado, nos resta inseri-lo na tela, para que possamos ver. Assim, foi implementado a função putPixel, descrita abaixo:

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

Mostrando o resultado da função, podemos ver o pixel na tela, como mostra na imagem abaixo:

![Pixels adicionados na tela, usando zoom para ver melhor](screenshots/putPixel.jpg)

#### Curiosidades sobre o Pixel/Ponto na tela

Como mostrado na cadeira de computação gráfica, na área da memória, os bytes que correspondem a cada componente do RGBA fica lado a lado, e é interessante ver que as luzes dos pixels na tela também fica lado a lado, veja, por exemplo, um screenshot feito na tela do meu computador:

![Alinhamento dos pixels na tela](screenshots/alignment.JPG)


Deu pra notar que não ficam alinhados? Bem, pra facilitar mais ainda, usei uma câmera com um zoom considerável, pra fazer o "tira-teima":

![Alinhamento dos pixels na tela com zoom](screenshots/alignment_camera.jpeg)

Agora podemos notar mais ainda a diferença. Interassante ver que parece desalinhado pra nós, ou em outro pixel, porém, todos estão no mesmo pixel. Caso ainda não concorde, veja abaixo o código feito pra fazer este teste:

	Pixel tempPixel;
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
	}

### Criando uma linha
Com o ponto/pixel criado, podemos fazer uma reta, que é um conjunto de pontos/pixels alinhados. Para isso, utilizaremos a função *drawLine*, e a base do algoritmo se dá da seguinte forma: baseia-se no argumento de que um segmento de reta, ao ser destacado, deve ser contínuo. O algoritmo baseia-se no critério do ponto médio. Onde, para cada coluna de píxeis existem dois píxeis que se encontram mais próximos da reta, um abaixo e outro acima desta. A escolha do pixel a ser destacado, é feita através da distância da interseção da reta com a coluna de píxeis a cada um dos dois píxeis, escolhendo-se então o pixel mais próximo da interseção. Os pixels que compõem um segmento de reta devem ser vizinhos, o que permite visualizar quais pontos numa matriz de base quadriculada que deve ser destacados para atender o grau de inclinação da reta dy/dx. Se o grau do ângulo for de 0º – 45º, nunca poderá destacar 2 pixels na mesma coluna, caso seja de 45º – 90º, nunca poderá destacar 2 pixels na mesma linha. Em outras palavras, a aproximação é feita estipulando um ponto médio, onde o pixel que mais se aproxima da reta será o próximo destacado.[2]

Como exemplo, veja a imagem abaixo:

![Exemplo de como fica o algoritmo de bresenham pra um angulo de 22º](screenshots/bresenham22o.png)

Esta imagem representa de como a linha fica quando a reta possui 22º

Começando o algoritmos de desenhar linha, primeiro, devemos fazer umas contas simples pra que a interpolação ocorra e para termos a variação de X e Y:

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

   
Após as definições, a primeira coisa que é feita, é a impressão da linha em forma de coluna, da seguinte maneira:

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

Esse trecho imprime os pixels em forma de coluna, como pode ser visto, em nenhum momento a posição X é alterada, apenas o Y, seja de "baixo para cima" ou de "cima para baixo".

![DrawLine em forma de coluna (delta_X == 0)](screenshots/drawline_coluna.JPG)

Com a coluna feita, podemos fazer a impressão agora sendo realmente uma linha, ou seja, não há variação em Y, só há variação em X, veja o trecho do código que corresponde a isso:

	if (delta_y == 0){ //linha
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

Similar à impressão de coluna, esse trecho imprime os pixels lado a lado, mantendo o valor de Y e alterando apenas o valor de X.

![DrawLine em forma de linha (delta_Y == 0)](screenshots/drawline_linha.JPG)


## Referências
[1] https://pt.wikipedia.org/wiki/Rasteriza%C3%A7%C3%A3o

[2] http://www.univasf.edu.br/~jorge.cavalcanti/comput_graf04_prim_graficas2.pdf