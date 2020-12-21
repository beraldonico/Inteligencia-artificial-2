#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ENTRADAS        6
#define SAIDAS          1
#define NR_AMOSTRAS     26
#define NR_NEURON_O     8
#define EPOCAS          100000
#define TX_APRENDIZADO  0.8

#define ZERO_TEST       0


/*
Dados para o treinamento da rede
*/
float cj_treinamento[NR_AMOSTRAS][ENTRADAS+SAIDAS] = {{1, 0, 0, 0, 0, 0, 0.006555},//a
                                                      {1, 0, 1, 0, 0, 0, 0.006655},//b
                                                      {1, 1, 0, 0, 0, 0, 0.006755},//c
                                                      {1, 1, 0, 1, 0, 0, 0.006855},//d
                                                      {1, 0, 0, 1, 0, 0, 0.006955},//e
                                                      {1, 1, 1, 0, 0, 0, 0.007055},//f
                                                      {1, 1, 1, 1, 0, 0, 0.007155},//g
                                                      {1, 0, 1, 1, 0, 0, 0.007255},//h
                                                      {0, 1, 1, 0, 0, 0, 0.007355},//i
                                                      {0, 1, 1, 1, 0, 0, 0.007455},//j
                                                      {1, 0, 0, 0, 1, 0, 0.007555},//k
                                                      {1, 0, 1, 0, 1, 0, 0.007655},//l
                                                      {1, 1, 0, 0, 1, 0, 0.007755},//m
                                                      {1, 1, 0, 1, 1, 0, 0.007855},//n
                                                      {1, 0, 0, 1, 1, 0, 0.007955},//o
                                                      {1, 1, 1, 0, 1, 0, 0.008055},//p
                                                      {1, 1, 1, 1, 1, 0, 0.008155},//q
                                                      {1, 0, 1, 1, 1, 0, 0.008255},//r
                                                      {0, 1, 1, 0, 1, 0, 0.008355},//s
                                                      {0, 1, 1, 1, 1, 0, 0.008455},//t
                                                      {1, 0, 0, 0, 1, 1, 0.008555},//u
                                                      {1, 0, 1, 0, 1, 1, 0.008655},//v
                                                      {0, 1, 1, 1, 0, 1, 0.008755},//w
                                                      {1, 1, 0, 0, 1, 1, 0.008855},//x
                                                      {1, 1, 0, 1, 1, 1, 0.008955},//y
                                                      {1, 0, 0, 1, 1, 1, 0.009055}//z
                                                     };


/*
Vari�veis globais
*/
double w_e_o[ENTRADAS+1][NR_NEURON_O];
double w_o_s[NR_NEURON_O+1][SAIDAS];
double saida_o[NR_NEURON_O];
double saida_s[SAIDAS];
double delta_saida[SAIDAS];
double gradiente_oculta[NR_NEURON_O];
double delta_oculta[NR_NEURON_O];
double media_erro = 0.0;


/*
Cabe�alho das fun��es auxiliares
*/
void inicializa_sinapses();
int gera_nr_aleatorios();
void mostrar_sinapses();
double f_sigmoid(double net);
void calcular_saidas(double entradas[ENTRADAS]);
void treinar_RNA();
double calcular_erro(double desejado, double saida);
void menu();
void calcular_delta_saida(double desejado);
void calcular_delta_oculta();
void calcular_gradiente_oculta();
void ajustar_pesos_sinapticos(double entradas[ENTRADAS]);
void gravar_pesos_sinapticos();
void restaurar_pesos_sinapticos();


/*
Fun��o principal
*/
int main()
{
  srand(time(NULL));

  while (1) {

    menu();

  }

  return 0;
}

void inicializa_sinapses()
{
  int i, j;

  // Inicializa pesos sin�pticos da entrada para a camada oculta
  for (i = 0; i < ENTRADAS+1; i++) {
    for (j =0; j < NR_NEURON_O; j++) {
     #if ZERO_TEST
      w_e_o[i][j] = 0.0;
     #else
      w_e_o[i][j] = gera_nr_aleatorios();
     #endif
    }
  }

  // Inicializa pesos sin�pticos da camada oculta para a sa�da
  for (i = 0; i < NR_NEURON_O+1; i++) {
    for (j =0; j < SAIDAS; j++) {
     #if ZERO_TEST
      w_o_s[i][j] = 0.0;
     #else
      w_o_s[i][j] = gera_nr_aleatorios();
     #endif
    }
  }

}

int gera_nr_aleatorios()
{
  int numeros[2] = {-1, 1};
  // Retorna -1 ou 1
  return (numeros[rand() % 2]);
}

void mostrar_sinapses()
{
  int i, j;

  // Inicializa pesos sin�pticos da entrada para a camada oculta
  for (i = 0; i < ENTRADAS+1; i++) {
    for (j =0; j < NR_NEURON_O; j++) {
      printf("w_e_o[%d][%d]: %f ", i, j, w_e_o[i][j]);
    }
    printf("\n");
  }

  // Inicializa pesos sin�pticos da camada oculta para a sa�da
  for (i = 0; i < NR_NEURON_O+1; i++) {
    for (j =0; j < SAIDAS; j++) {
      printf("w_o_s[%d][%d]: %f ", i, j, w_o_s[i][j]);
    }
    printf("\n");
  }
}

double f_sigmoid(double net)
{
  return 1 / (1 + exp(-net));
}

void calcular_saidas(double entradas[ENTRADAS])
{
  int i, j;

  // Calcular os nets da entrada para a camada oculta
  for (i = 0; i < NR_NEURON_O; i++) {
    saida_o[i] = 0.0;
    saida_o[i] += w_e_o[0][i] * 1;  // Calcula saida para bias

    for (j = 1; j < ENTRADAS+1; j++) {
     saida_o[i] += w_e_o[j][i] * entradas[j-1];
    }

    // Calcular a saida de saida_o[i]
    saida_o[i] = f_sigmoid(saida_o[i]);
  }

  // Calcular os nets da camada oculta para a sa�da
  for (i = 0; i < SAIDAS; i++) {
    saida_s[i] = 0.0;
    saida_s[i] += w_o_s[0][i] * 1;  // Calcula saida para bias

    for (j = 1; j < NR_NEURON_O+1; j++) {
      saida_s[i] += w_o_s[j][i] * saida_o[j-1];
    }

    saida_s[i] = f_sigmoid(saida_s[i]);
  }

}

void treinar_RNA()
{
  int i, j;
  double entradas[ENTRADAS];

  for (i = 1; i <= EPOCAS; i++) {

    for (j = 0; j < NR_AMOSTRAS; j++) {
      entradas[0] = cj_treinamento[j][0];
      entradas[1] = cj_treinamento[j][1];
      entradas[2] = cj_treinamento[j][2];
      entradas[3] = cj_treinamento[j][3];
      entradas[4] = cj_treinamento[j][4];
      entradas[5] = cj_treinamento[j][5];
      // Feedforward
      calcular_saidas(entradas);

      // Backward (backpropagation)
      calcular_delta_saida(cj_treinamento[j][6]);
      calcular_gradiente_oculta();
      calcular_delta_oculta();
      ajustar_pesos_sinapticos(entradas);
    }

  }

  // Mostra media dos erros
  printf("RNA TREINADA - Media dos erros: %lf\n", media_erro);
}

double calcular_erro(double desejado, double saida)
{
  return desejado - saida;
}

void menu()
{
  int opcao;
  double entradas[ENTRADAS];

  printf("Rede Neural Perceptron de Multiplas Camadas\n");
  printf("Calcular a letra em Braile de acordo com a tabela\n");
  printf("*******************************************\n\n");
  printf("1.Treinar a rede\n");
  printf("2.Usar a rede\n");
  printf("3.Ver pesos sinapticos\n");
  printf("4.Sair\n");
  printf("Opcao? ");
  scanf("%d", &opcao);

  switch (opcao) {

    case 1: inicializa_sinapses();
            treinar_RNA();
            break;

    case 2: printf("Entrada 1: ");
            scanf("%lf", &entradas[0]);
            printf("Entrada 2: ");
            scanf("%lf", &entradas[1]);
            printf("Entrada 3: ");
            scanf("%lf", &entradas[2]);
            printf("Entrada 4: ");
            scanf("%lf", &entradas[3]);
            printf("Entrada 5: ");
            scanf("%lf", &entradas[4]);
            printf("Entrada 6: ");
            scanf("%lf", &entradas[5]);
            calcular_saidas(entradas);
            printf("%lf", saida_s[0]);
            saida_s[0]=saida_s[0]*10000;
            int hue = saida_s[0];
            printf("\nResposta da RNA: %d %c\n\n\n", hue, hue);
            break;

    case 3: mostrar_sinapses();
            break;

    case 4: exit(0);

  }
}

void calcular_delta_saida(double desejado)
{
  int i;
  for (i = 0; i < SAIDAS; i++)
    delta_saida[i] = calcular_erro(desejado, saida_s[i]) * (1 - saida_s[i] * saida_s[i]);
}

void calcular_gradiente_oculta()
{
  int i, j;

  for (i = 0; i < SAIDAS; i++) {
    for (j = 1; j < NR_NEURON_O+1; j++) {
      gradiente_oculta[j-1] = delta_saida[i] * w_o_s[j][i];
    }
  }
}

void calcular_delta_oculta()
{
  int i;
  for (i = 0; i < NR_NEURON_O; i++)
    delta_oculta[i] = gradiente_oculta[i] * saida_o[i] * (1 - saida_o[i]);
}

void ajustar_pesos_sinapticos(double entradas[ENTRADAS])
{
  int i, j;

  // Ajusta os pesos sin�pticos da camada intermedi�ria para a camada de sa�da
  for (i = 0; i < SAIDAS; i++) {
    w_o_s[0][i] = w_o_s[0][i] + TX_APRENDIZADO * delta_saida[i] * 1;

    for (j = 1; j < NR_NEURON_O+1; j++) {
      w_o_s[j][i] = w_o_s[j][i] + TX_APRENDIZADO * delta_saida[i] * saida_o[j-1];
    }

  }

  // Ajusta os pesos sin�pticos da camada de saida para a camada intermedi�ria
  for (i = 0; i < NR_NEURON_O; i++) {
    w_e_o[0][i] = w_e_o[0][i] + TX_APRENDIZADO * delta_oculta[i] * 1;

    for (j = 1; j < ENTRADAS+1; j++) {
     w_e_o[j][i] = w_e_o[j][i] + TX_APRENDIZADO * delta_oculta[i] * entradas[j-1];
    }
  }

}

