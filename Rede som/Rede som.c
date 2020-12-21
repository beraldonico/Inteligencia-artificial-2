#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define nentrada  3
#define nneuronio 2   //para uma regi?o retangular de 3 neur?nios em duas linhas
#define nexemplos 3

/*
Vari?veis globais
*/
float w[nentrada][nneuronio], errodes, tau, sigma0, sigma, eta, eta0;
int x, y, cont2, contt, epocas, k, l, k1, l1;
float entradas[nentrada][nexemplos], teste[nentrada][6], d[nneuronio], l2, h, deucl;


/*
Cabe?alho das fun??ea auxiliares
*/
void confRNA();
void definePesosIniciais();
void leDadosExemplos();
void mostraPesos();




/*
Fun??o principal
*/
int main(){

  // Seta pesos iniciais
  definePesosIniciais();

  // Mostra pesos iniciais
  mostraPesos();

  // Solicita dados de treinamento
  confRNA();

  // Le exemplos para o treinamento
  leDadosExemplos();

  printf("Iniciando processo de treinamento da rede ...\n");
  for(x = 0; x < epocas; x++) {
    for(y = 0; y < nexemplos; y++) {
      for(contt = 0; contt < nneuronio; contt++) {
        d[contt] = 0;
        for(cont2 = 0; cont2 < nentrada; cont2++)
            // C?lculo da dist?ncia euclidiana
         		d[contt] = d[contt] + (entradas[cont2][y] - w[cont2][contt])*(entradas[cont2][y] - w[cont2][contt]);
      }
      // calcula a menor distancia...
      deucl = d[0];
      for(contt = 0; contt < nneuronio; contt++)
        if(deucl >= d[contt]) deucl = d[contt];

      //teste de parada
      if (deucl < errodes) break;
      for(contt = 0; contt < nneuronio; contt++)
        if(deucl == d[contt]) {
          k = (contt + 1) % nneuronio;
          if (k == 0) k = nneuronio;
          l = 1 + contt / nneuronio;
          break;
        }

      for(contt = 0; contt < nneuronio; contt++) {
        // calcula a largura efetiva
        sigma = sigma * exp(-(x * nexemplos + y) / tau);
        //calcula a dist?ncia lateral
        l2 = 0;
        k1 = (contt + 1) % nneuronio;
        if (k1 == 0) k1 = nneuronio;
        l1 = 1 + contt / nneuronio;
        l2 = pow((k - k1), 2) + pow((l - l1), 2);

        //calcula a vizinhan?a topol?gica
        h = exp(-l2 / (2 * sigma * sigma));
        //calcula a taxa de aprendizagem
        eta = eta*exp(-(x*nexemplos + y)/tau);
        //calcula o ajuste para o peso dos arcos do neur?nio
        for(cont2 = 0; cont2 < nentrada; cont2++)
          w[cont2][contt] = w[cont2][contt] + eta*h*(entradas[cont2][y] - w[cont2][contt]);
      }
    }

    //teste de parada
    if (deucl < errodes) {
      printf("Finalizado pelo erro\n");
      break;
    }
  }  // Fim do treinamento



  // Mostra pesos finais
  mostraPesos();

  printf("Testes de Entradas e Sa?das\n");
	for(cont2 = 0; cont2 < 5; cont2++)
	  for(contt = 0; contt < nentrada; contt++) {
      if(cont2 < 3) teste[contt][cont2] = entradas[contt][cont2];
        if(cont2 == 3) teste[contt][cont2] = rand()%20;
        if(cont2 == 4) {
          printf("Digite 1 entrada:");
          scanf("%f", &teste[contt][cont2]);
        }
      }

    for(x = 0; x < 5; x++) {
      for(contt = 0; contt < nneuronio; contt++) {
        d[contt] = 0;
      	for(cont2 = 0; cont2 < nentrada; cont2++)
      		d[contt] = d[contt] + (teste[cont2][x] - w[cont2][contt])*(teste[cont2][x] - w[cont2][contt]);
         printf("d = %f\n", d[contt]);
      }

      // calcula a menor distancia...
      deucl = d[0];

      for(contt = 0; contt < nneuronio; contt++)
      	if(deucl >= d[contt]) deucl = d[contt];
      printf("deucl = %f\n", deucl);
      for(contt = 0; contt < nneuronio; contt++)
      	if(deucl == d[contt]) {
         	k = (contt + 1) % nneuronio;
            if (k == 0) k = nneuronio;
            l = 1 + contt / nneuronio;
            break;
        }

      printf("Entradas:\n");
      for(y = 0; y < nentrada; y++)
      	printf("%f\t", teste[y][x]);
      printf("\nNeur?nio Ativado: %d (%d %d)\n", contt + 1, l, k);
    }
}

/*
Implementa??o das fun??ea auxiliares
*/
void confRNA()
{
  printf("Entre com o valor da taxa de aprendizagem inicial: \n");
  scanf("%f", &eta);
  printf("Entre com o erro desejado:\n");
  scanf("%f", &errodes);
  printf("Entre com a largura efetiva inicial: \n");
  scanf("%f", &sigma);
  printf("Entre com a constante de tempo tau: \n");
  scanf("%f", &tau);
  printf("Entre com o n?mero de ?pocas de treinamento:\n");
  scanf("%d", &epocas);
}

void definePesosIniciais()
{
  int x, y;

  for(x = 0; x < nentrada; x++)
    for(y = 0; y < nneuronio; y++)
      w[y][x] = rand() % 2 + .5;
}

void leDadosExemplos()
{
  int x, y;

  printf("Entre com os vetores de exemplos de treinamento de entrada:\n");
  for(x = 0; x < nentrada; x++)
    for(y = 0; y < nexemplos; y++) {
      printf("Treinamento[%d][%d]: ", x, y);
      scanf("%f", &entradas[x][y]);
    }
}

void mostraPesos()
{
  int x, y;

  for(x = 0; x < nentrada; x++)
    for(y = 0; y < nneuronio; y++)
      printf("w[%i][%i]: %f\n", x, y, w[y][x]);
}