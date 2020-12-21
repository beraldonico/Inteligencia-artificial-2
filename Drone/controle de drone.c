#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ENTRADAS        1
#define SAIDAS          2
#define NR_AMOSTRAS     15
#define NR_NEURON_O     5
#define CAMADAS_O       2
#define EPOCAS          100000000 //14:05      //21:18
#define TX_APRENDIZADO  0.7
                                                         //altura/Motor1/Motor2
double cj_treinamento[NR_AMOSTRAS][ENTRADAS+SAIDAS+1] = {{0.26, 0.1300, 0.1232},
                                                         {0.28, 0.1298, 0.1233},
                                                         {0.30, 0.1296, 0.1234},
                                                         {0.32, 0.1294, 0.1235},
                                                         {0.34, 0.1292, 0.1236},
                                                         {0.38, 0.1286, 0.1238},
                                                         {0.40, 0.1284, 0.1239},
                                                         {0.42, 0.1282, 0.1240},//ideal
                                                         {0.51, 0.1280, 0.1242},
                                                         {0.60, 0.1278, 0.1245},
                                                         {0.69, 0.1276, 0.1247},
                                                         {0.78, 0.1274, 0.1250},
                                                         {0.87, 0.1272, 0.1252},
                                                         {0.96, 0.1270, 0.1255},
                                                         {1.05, 0.1268, 0.1260}};

double w_e_o[ENTRADAS+1][NR_NEURON_O];    //sinapses entrada para oculta 1
double w_o_o[NR_NEURON_O+1][NR_NEURON_O]; // sinapses oculta 1 para oculta 2
double w_o_s[NR_NEURON_O+1][SAIDAS];	  // sinapses oculta 2 para saida

double saida_o[NR_NEURON_O][CAMADAS_O];   //saidas das camadas ocultas

double saida_s[SAIDAS];					  //saida final

double delta_saida[SAIDAS];
double gradiente_oculta[NR_NEURON_O][CAMADAS_O]; // gradiente

double delta_oculta[NR_NEURON_O][CAMADAS_O];  //delta




/*
Cabecalho das funcoes auxiliares
*/
void inicializa_sinapses();
int gera_nr_aleatorios();
void mostrar_sinapses();
double f_sigmoid(double net);
void calcular_saidas(double entradas[ENTRADAS]);
void treinar_RNA();
double calcular_erro(double desejado, double saida);
void menu();
void calcular_delta_saida(double desejado, double desejado2);
void calcular_delta_oculta();
void calcular_delta_oculta2();
void calcular_gradiente_oculta();
void calcular_gradiente_oculta2();
void ajustar_pesos_sinapticos(double entradas[ENTRADAS]);
void salvaSinapses();

/*
Função principal
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

  // Inicializa pesos sinapticos da entrada para a camada oculta
  for (i = 0; i < ENTRADAS+1; i++)
    for (j =0; j < NR_NEURON_O; j++)
      w_e_o[i][j] = gera_nr_aleatorios();

  //inicializa pesos sinapticos da camda oculta 1 para camada oculta 2
  for (i = 0; i < NR_NEURON_O+1; i++)
    for (j =0; j < NR_NEURON_O; j++)
        w_o_o[i][j] = gera_nr_aleatorios();

  // Inicializa pesos sinapticos da camada oculta para a saida
  for (i = 0; i < NR_NEURON_O+1; i++)
    for (j =0; j < SAIDAS; j++)
      w_o_s[i][j] = gera_nr_aleatorios();
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

  // Inicializa pesos sinapticos da entrada para a camada oculta
	for (i = 0; i < ENTRADAS+1; i++) {
		for (j =0; j < NR_NEURON_O; j++)
		  printf("w_e_o[%d][%d]: %f \n", i, j, w_e_o[i][j]);

		printf("\n");
	}

	//mostra pesos sinapticos da 1camada oculata para a 2camada oculta
	for (i = 0; i < NR_NEURON_O+1; i++) {
	    for (j =0; j < NR_NEURON_O; j++)
			printf("w_o_o[%d][%d]: %f \n", i, j, w_o_o[i][j]);

		printf("\n");
	}

  // Inicializa pesos sinapticos da camada oculta para a saida
	for (i = 0; i < NR_NEURON_O+1; i++) {
    	for (j =0; j < SAIDAS; j++)
      		printf("w_o_s[%d][%d]: %f \n", i, j, w_o_s[i][j]);

   		printf("\n");
 	}
}

double f_sigmoid(double net){
  return 1 / (1 + exp(-net));
}

void calcular_saidas(double entradas[ENTRADAS])
{

}

void treinar_RNA()
{

  }

}

double calcular_erro(double desejado, double saida)
{
  return desejado - saida;
}

void menu()
{
  int opcao, i;
  double entradas[ENTRADAS];

  printf("Rede Neural Perceptron de Multiplas Camadas\n");
  printf("Gangorra\n");
  printf("*******************************************\n\n");
  printf("1.Treinar a rede\n");
  printf("2.Usar a rede\n");
  printf("3.Ver pesos sinpaticos\n");
  printf("4.Sair\n");
  printf("Opcao? ");
  scanf("%d", &opcao);

  switch (opcao) {

    case 1: inicializa_sinapses();
            treinar_RNA();
            salvaSinapses();
            break;

    case 2: for(i = 0; i < ENTRADAS; i++){
    			printf("Entrada %d: ", i+1);
            	scanf("%lf", &entradas[i]);
       		 }
            calcular_saidas(entradas);
            for(i = 0; i < SAIDAS; i++)
            	printf("\nMotor %d: %lf\n", i+1, saida_s[i]);
            printf("\n\n\n");
            break;

    case 3: mostrar_sinapses();
            break;

    case 4: exit(0);

  }
}

void calcular_delta_saida(double desejado, double desejado2)
{

}

void calcular_gradiente_oculta()
{

}

void calcular_delta_oculta()
{

}

void calcular_gradiente_oculta2()
{

}

void calcular_delta_oculta2()
{

}

void ajustar_pesos_sinapticos(double entradas[ENTRADAS])
{

}
void salvaSinapses(){

  int i, j;

  FILE *fp;

  fp = fopen("sinapses.txt", "w+");



    fprintf(fp,"double w_e_o[ENTRADAS+1][NR_NEURON_O] = {");
    for (i = 0; i < ENTRADAS +1; ++i){
        fprintf(fp,"{");
        for (j = 0; j < NR_NEURON_O; ++j){
            if (j<NR_NEURON_O-1)
                fprintf(fp,"%lf, ", w_e_o[i][j]);
            else
                if (i<ENTRADAS)
                    fprintf(fp,"%lf},\n                                        ", w_e_o[i][j]);
                else
                    fprintf(fp,"%lf}};\n\n", w_e_o[i][j]);
        }
    }

  fprintf(fp,"double w_o_o[NR_NEURON_O+1][NR_NEURON_O] = {");
  for (int i = 0; i < NR_NEURON_O+1; ++i){
    fprintf(fp,"{");
    for (int j = 0; j < NR_NEURON_O; ++j){
      if (j<NR_NEURON_O-1)
        fprintf(fp,"%lf, ", w_o_o[i][j]);
      else
        if (i < NR_NEURON_O)
          fprintf(fp,"%lf},\n                                            ", w_o_o[i][j]);
        else
          fprintf(fp,"%lf}};\n\n", w_o_o[i][j]);
    }
  }

  fprintf(fp,"double w_o_s[NR_NEURON_O+1][SAIDAS] = {");
    for (i = 0; i < NR_NEURON_O+1; ++i){
        fprintf(fp,"{");
        for (j = 0; j < SAIDAS; ++j){
            if (j<SAIDAS-1)
               fprintf(fp,"%lf, ", w_o_s[i][j]);
            else
                if (i<NR_NEURON_O)
                    fprintf(fp,"%lf},\n                                       ", w_o_s[i][j]);
                else
                    fprintf(fp,"%lf}};\n\n", w_o_s[i][j]);
        }
    }


        fprintf(fp,"\n");


    fclose(fp);

}