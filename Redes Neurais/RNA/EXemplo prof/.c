/*
Rede Neural Perceptron
*/

#include <stdio.h>
#include <stdlib.h>

#define ENTRADAS    2
#define SAIDAS      1
#define NR_AMOSTRAS 4
#define EPOCAS      1000
#define TX_APR      0.8

int dados_treinamento[NR_AMOSTRAS][ENTRADAS+SAIDAS] = {{0, 0, 0},
                                                       {0, 1, 0},
                                                       {1, 0, 0},
                                                       {1, 1, 1}};

double w[ENTRADAS+1] = {0, 0, 0};

/*
Fun??es auxiliares
*/
int erro(int desejado, int saida);
int f_degrau(double saida);
double calcular_net(int entradas[2]);
void mostrar_pesos_sinapticos();
int calcular_saida(double net);
void treinar_RNA();
void ajustar_w(int entradas[2], int erro);



/*
Fun??o principal
*/
int main()
{
  int entradas[2];

  // Treinar a rede
  treinar_RNA();

  // Mostra pesos sinapticos
  mostrar_pesos_sinapticos();

  do {
    // Ler entradas
    printf("\nEntrada 1: ");
    scanf("%d", &entradas[0]);
    printf("Entrada 2: ");
    scanf("%d", &entradas[1]);

    // Usar RNA
    printf("\nSaida da RNA: %d\n", calcular_saida(calcular_net(entradas)));

  } while (entradas[0] != -100 && entradas[1] != - 100);



  return 0;
}

/*
Fun??es auxiliares
*/
int erro(int desejado, int saida)
{
  return desejado - saida;
}

int f_degrau(double saida)
{
  if (saida > 0) return 1;
  return 0;
}

double calcular_net(int entradas[2])
{
  double net = 0;

  // Para bias
  net = w[0] * 1;
  // Para entrada 1
  net += w[1] * entradas[0];
  // Para entrada 2
  net += w[2] * entradas[1];

  return net;
}

void mostrar_pesos_sinapticos()
{
  int i;
  for (i = 0; i < ENTRADAS + 1; i++)
    printf("w[%d]: %.2f\n", i, w[i]);
}

int calcular_saida(double net)
{
  return f_degrau(net);
}

void treinar_RNA()
{
  int i, j, entradas[2], saida, vlr_erro;

  for (i = 0; i < EPOCAS; i++) {
    for (j = 0; j < NR_AMOSTRAS; j++) {
      entradas[0] = dados_treinamento[j][0];
      entradas[1] = dados_treinamento[j][1];

      // Calcular saida
      saida = calcular_saida(calcular_net(entradas));
      // Verificar erro
      vlr_erro = erro(dados_treinamento[j][2], saida);
      if (vlr_erro)
        // Ajusta pesos sinapticos
        ajustar_w(entradas, vlr_erro);
    }
  }

}

void ajustar_w(int entradas[2], int erro)
{
  int i;
  // Para w bias
  w[0] = w[0] + TX_APR * erro * 1;
  // Para w entradas
  for (i = 1; i < ENTRADAS+1; i++)
    w[i] = w[i] + TX_APR * erro * entradas[i-1];
}
