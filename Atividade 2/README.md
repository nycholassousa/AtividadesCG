# Passo a Passo do Pipeline Gráfico

## Atividade Proposta
O objetivo deste trabalho é familiarizar os alunos com a estrutura e o funcionamento do pipeline gráfico através da implementação de um pipeline completo, capaz de transformar vértices descritos no espaço do objeto em primitivas rasterizadas no espaço de tela.[1] 

## O que é um Pipeline Gráfico
TODO

## Dificuldades Encontradas
Uma das dificuldades encontradas foi encontrar uma forma de como o color buffer poderia ser limpo sem usar funções do OpenGL, assim, ficando o erro da imagem abaixo:

![Imagem sem limpar o color buffer](screenshots/clean_color_buffer.jpg)

Para corrigir isso, notei que poderia usar o **memset**[2], da seguinte maneira:
	
	memset(FBptr, 0, IMAGE_WIDTH * IMAGE_HEIGHT * 4)

Com isso, o FBprt foi preenchido com 0, no tamanho da conta feito pela tela, ou seja, a tela foi limpa com a cor preta a cada frame.

## Referências

[1] Definição do Trabalho Proposto para cadeira de ICG, Professor Christian A. P.
[2] http://en.cppreference.com/w/cpp/string/byte/memset