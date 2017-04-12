# Passo a Passo do Pipeline Gráfico

## Atividade Proposta

## O que é um Pipeline Gráfico

## Dificuldades Encontradas
Uma das dificuldades encontradas foi encontrar uma forma de como o color buffer poderia ser limpo sem usar funções do OpenGL, assim, ficando o erro da imagem abaixo:

![Imagem sem limpar o color buffer](screenshots/clean_color_buffer.jpg)

Para corrigir isso, notei que poderia usar o *memset*[1], da seguinte maneira:
	
	memset(FBptr, 0, IMAGE_WIDTH * IMAGE_HEIGHT * 4)

Com isso, o FBprt será preenchido com 0, no tamanho da conta feito pela tela, ou seja, a tela ficará com a cor preta.

## Referências

[1] http://en.cppreference.com/w/cpp/string/byte/memset