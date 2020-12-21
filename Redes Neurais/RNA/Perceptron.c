/// Redes neurais perceptron

#include <stdio.h>
#include <stdlib.h>

#define ENTRADAS    2
#define SAIDAS      1
#define NR_AMOSTRAS 4
#define EPOCAS      10000
#define TX_APR      0.7

int dados_treinamento[NR_AMOSTRAS][ENTRADAS + SAIDAS] = {{0 ,0 ,0},
                                                         {0 ,1 ,0},
                                                         {1 ,0 ,0},
                                                         {1 ,1 ,1}};

float w[ENTRADAS+1] = {0 ,0 ,0};

/// Funçoes auxiliares

int erro(int desejado, int saida);
int f_degrau(int saida);
float calcular_net(int entradas[2]);
void mostrar_pesos_sinapticos();
int calcular_saidas(float net);
void treinar_RNA();
void ajustar_w(int entradas[2], int erro);

/// Funçoes principais

int main(){
    int entradas[2];

    ///treinar  a rede
    treinar_RNA();

    ///Mostar pesos sinapticos
    mostrar_pesos_sinapticos();

    ///usar RNA
    do{
        ///ler entradas
        printf("entrada 1: ");
        scanf("%d", &entradas[0]);
        printf("entrada 2: ");
        scanf("%d", &entradas[1]);
        printf("Saida da RNA: %d\n\n", calcular_saidas(calcular_net(entradas)));
    }while(entradas[0] != -100 && entradas[1] != -100 );
    return 0;
}

int erro(int desejado, int saida){
    return desejado - saida;
}

int f_degrau(int saida){
    if(saida > 0){
        return 1;
    }
    return 0;
}

float calcular_net(int entradas[2]){
    float net = 0;
    //para bias
    net = w[0] * 1;
    //para entrada 1
    net += w[1] * entradas[0];
    //para entrada 2
    net += w[2] * entradas[1];
    return net;
}

void mostrar_pesos_sinapticos(){
    int i;
    for (i = 0; i < ENTRADAS + 1; i++ ){
        printf("w[%d]: %.2f\n", i, w[i]);
    }
}

int calcular_saidas(float net){
    return f_degrau(net);
}

void treinar_RNA(){
    int i, j;
    int entradas[2], saida, vlr_erro;
    for(i = 0; i < EPOCAS; i++){
        for(j = 0; j< NR_AMOSTRAS; j++){
            entradas[0] = dados_treinamento[j][0];
            entradas[1] = dados_treinamento[j][1];
            ///calcular saida
            saida = calcular_saidas(calcular_net(entradas));
            ///verificar erro
            vlr_erro = erro(dados_treinamento[j][2], saida);
            if(vlr_erro)
                ///ajustar pesos sinapticos
                ajustar_w(entradas, vlr_erro);
        }
    }
}

void ajustar_w(int entradas[2], int erro){
    int i;
    ///para bias
    w[0] = w[0] + TX_APR * erro * 1;
    ///para w entradas
    for (i = 1; i < ENTRADAS + 1; i++){
        w[i] = w[i] + TX_APR * erro * entradas[i-1];
    }
}
