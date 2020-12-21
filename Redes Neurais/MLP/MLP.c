/*
Rede Neural Multilayer Peceptron - Problema do XOR
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ENTRADAS    2
#define SAIDAS      1
#define BIAS        1
#define NR_AMOSTRAS 4
#define NR_NEURO_OC 3
#define TX_APREND   0.8
#define EPOCAS      10000


int dados_treinamento[NR_AMOSTRAS][ENTRADAS+SAIDAS] = { {0, 0, 0},
                                                        {0, 1, 1},
                                                        {1, 0, 1},
                                                        {1, 1, 0}
};

float w_entrada_oculta[ENTRADAS+BIAS][NR_NEURO_OC];
float w_oculta_saida[NR_NEURO_OC+BIAS][SAIDAS];
float sinal_saida[SAIDAS];
float sinal_oculta[NR_NEURO_OC];
float delta_saida[SAIDAS];
float delta_oculta[NR_NEURO_OC];
float gradiente_oculta[NR_NEURO_OC];


float erro(float desejado, float obtido);
float sigmoid(float sinal);
void inicializa_pesos_sinapticos();
void mostra_pesos_sinapticos();
void treinar_rede();
void feedforward(int entradas[ENTRADAS]);
void calcular_delta_saida(float desejado);
void calcular_gradiente_oculta();
void calcular_delta_oculta();
void corrigir_pesos_sinapticos(int entradas[ENTRADAS]);


int main()
{
    int opc, entradas[ENTRADAS];

    inicializa_pesos_sinapticos();

    while (1)
    {
        printf("Rede MLP ... XOR\n");
        printf("1.Treinar a rede\n");
        printf("2.Usar a rede\n");
        printf("3.Mostrar pesos sinapticos\n");
        printf("4.Sair do programa\n");
        printf("Opcao: ");
        scanf("%d", &opc);

        switch(opc)
        {
        case 1:
            treinar_rede();
            break;
        case 2:
            printf("Entrada 1.....: ");
            scanf("%f", &entradas[0]);
            printf("Entrada 2.....: ");
            scanf("%f", &entradas[1]);
            feedforward(entradas);
            printf("Saida da RNA..: %.2f\n", sinal_saida[0]);
            break;
        case 3:
            mostra_pesos_sinapticos();
            break;
        case 4:
            exit(0);
        }
        printf("\n\n");
    }

    return 0;
}

float erro(float desejado, float obtido)
{
    return desejado - obtido;
}

float sigmoid(float sinal)
{
    return 1 / (1 + exp(-sinal));
}

void inicializa_pesos_sinapticos()
{
    int i, j;
    for (i = 0; i < ENTRADAS+BIAS; i++)
        for (j = 0; j < NR_NEURO_OC; j++)
            w_entrada_oculta[i][j] = 0.0;

    for (i = 0; i < NR_NEURO_OC+BIAS; i++)
        for (j = 0; j < SAIDAS; j++)
            w_oculta_saida[i][j] = 0.0;

}

void mostra_pesos_sinapticos()
{
    int i, j;
    for (i = 0; i < ENTRADAS+BIAS; i++)
    {
        for (j = 0; j < NR_NEURO_OC; j++)
            printf("w[%d][%d]%.2f ", i, j, w_entrada_oculta[i][j]);
        printf("\n");
    }

    for (i = 0; i < NR_NEURO_OC+BIAS; i++)
    {
        for (j = 0; j < SAIDAS; j++)
             printf("w[%d][%d]%.2f ", i, j, w_oculta_saida[i][j]);
        printf("\n");
    }
}

void treinar_rede()
{
    int i, j, k, entradas[ENTRADAS];

    for (i = 0; i < EPOCAS; i++)
    {
        for (j = 0; j < NR_AMOSTRAS; j++)
        {
            // Carrega dados de entrada
            for (k = 0; k < ENTRADAS; k++)
            {
                entradas[k] = dados_treinamento[j][k];
            }
            // Calcula sinal de saida de todos os neuronios da camada de saida
            feedforward(entradas);
            // Calcular delta da saida
            calcular_delta_saida(dados_treinamento[j][k]);
            // Calcular gradiente da sa�da
            calcular_gradiente_oculta();
            // Calcular delta oculta
            calcular_delta_oculta();
            // Ajustar pesos sin�pticos
            corrigir_pesos_sinapticos(entradas);
        }
    }
}


void feedforward(int entradas[ENTRADAS])
{
    int i, j;

    for (j = 0; j < NR_NEURO_OC; j++)
    {
        sinal_oculta[j] = 0.0;
        // Calcula saida bias
        sinal_oculta[j] += w_entrada_oculta[0][j] * BIAS;
        for (i = 1; i < ENTRADAS+BIAS; i++)
        {
            sinal_oculta[j] += w_entrada_oculta[i][j] * entradas[i-1];
        }
        sinal_oculta[j] = sigmoid(sinal_oculta[j]);
    }

    // Calcular sa�da da camada de sa�da
    for (j = 0; j < SAIDAS; j++)
    {
        sinal_saida[j] = 0.0;
        // Calcula saida bias
        sinal_saida[j] += w_oculta_saida[0][j] * BIAS;
        for (i = 1; i < NR_NEURO_OC+BIAS; i++)
        {
            sinal_saida[j] += w_oculta_saida[i][j] * sinal_oculta[i-1];
        }
        sinal_saida[j] = sigmoid(sinal_saida[j]);
    }
}

void calcular_delta_saida(float desejado)
{
    int i;

    for (i = 0; i < SAIDAS; i++)
    {
        delta_saida[i] = erro(desejado, sinal_saida[i]) * (1 - sinal_saida[i] * sinal_saida[i]);
    }
}

void calcular_gradiente_oculta()
{
    int i, j;

    for (i = 0; i < SAIDAS; i++)
    {
        for (j = 1; j < NR_NEURO_OC+BIAS; j++)
        {
            gradiente_oculta[j-1] = delta_saida[i] * w_oculta_saida[j][i];
        }
    }
}

void calcular_delta_oculta()
{
    int i;

    for (i = 0; i < NR_NEURO_OC; i++)
        delta_oculta[i] = gradiente_oculta[i] * sinal_oculta[i] * (1 - sinal_oculta[i]);
}

void corrigir_pesos_sinapticos(int entradas[ENTRADAS])
{
    int i, j;

    // Ajustar pesos sin�pticos da sa�da para a camada oculta
    for (j = 0; j < SAIDAS; j++)
    {
        // Ajusta w para bias
        w_oculta_saida[0][j] =  w_oculta_saida[0][j] + TX_APREND * delta_saida[j] * BIAS;
        for (i = 1; i < NR_NEURO_OC+BIAS; i++)
        {
            // Ajusta w para os demais neur�nios
            w_oculta_saida[i][j] =  w_oculta_saida[i][j] + TX_APREND * delta_saida[j] * sinal_oculta[i-1];
        }
    }

    // Ajustar pesos sin�pticos da camada oculta para a camada de entrada
    for (j = 0; j < NR_NEURO_OC; j++)
    {
        // Ajusta w para bias
        w_entrada_oculta[0][j] = w_entrada_oculta[0][j] + TX_APREND * delta_oculta[j] * BIAS;
        for (i = 1; i < ENTRADAS+BIAS; i++)
        {
            // Ajusta w para os demais neur�nios
            w_entrada_oculta[i][j] = w_entrada_oculta[i][j] + TX_APREND * delta_oculta[j] * entradas[i-1];
        }
    }
}
