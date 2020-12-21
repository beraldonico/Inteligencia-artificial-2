///trabalho de otimizacao de topicos
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define genes 4     // tamanho do vetor de genes
#define popsize 10  // populacao
#define mutchance 15    // ratio de 0-100 para mutar
#define muttx  0.25 // taxa de variacao na mutacao
#define geracoes 20
#define elite 2     // numero de individuos copiados para a proxima geracao sujeito a mutacao.
// tava considerando mudar a forma que a elite funciona pra: numeros de ind da elite = popsize/elite(esse elite no cado eh o numero que vai no define)
// dai pra ser 2 ind de elite numa pop de 10 o numero de elite seria 5. pra fazer 10/5 -> 2

typedef struct individuo{
    float score;
    float cromossomo[genes];
}ind;

ind pop[popsize];
ind melhor;

float ub[genes] = {100000.0, 5.0, 100.0, 10.0};
float lb[genes] = {100.0, 1, 3.0, 1.5};

void popula();
void score();
void ordena();
void cruza();
void muta();

void printpop();    // funcao auxiliar para teste e desenvolvimento

int main(){
    srand(time(NULL));
    int i = 0;
    popula();
    melhor.score = 0.0;
    while(i < geracoes){
        printf("geracao: %d\t", i+1);
        score();
        ordena();
        printf("melhor ganho atual: %f\n", pop[0].score);
        if(pop[0].score > melhor.score){
            melhor = pop[0];
        }
        printpop();
        cruza();
        muta();
        i++;
    }
    printf("Melhor ganho: %f\n", melhor.score);
    printf("Melhor [0]: %f\t", melhor.cromossomo[0]);
    printf("Melhor [1]: %f\t", melhor.cromossomo[1]);
    printf("Melhor [2]: %f\t", melhor.cromossomo[2]);
    printf("Melhor [3]: %f\t", melhor.cromossomo[3]);

    return 0;
}

void popula(){
    int i, j;
    float x;
    for(i = 0; i < popsize; i++){
        pop[i].score = 0.0;
        for(j = 0; j < genes; j++){
            x = rand()%100;
            x = x / 100;
            pop[i].cromossomo[j] = lb[j] + (ub[j] - lb[j])*x;   // igual tava iniciando no octave
        }
    }
}

void score(){
    FILE *netlist, *log;
    float ganho;
    netlist = fopen("circuito.cir", "w");
    if(netlist == NULL){
        printf("Erro!");
    }
    for(int i = 0; i < popsize; i++){
        netlist = fopen("circuito.cir", "w");
        fprintf(netlist,"* Circuit Name\n");
        fprintf(netlist,"V_gs Vin N001 AC 1\n");
        fprintf(netlist,"VGS N001 0 %f\n", pop[i].cromossomo[1]);
        fprintf(netlist,"VDD VDD 0 5\n");
        fprintf(netlist,"R1 VDD Vout %f\n", pop[i].cromossomo[0]);
        fprintf(netlist,"M1 Vout Vin 0 0 N_1u l=%fu w=%fu\n", pop[i].cromossomo[3], pop[i].cromossomo[2]);
        fprintf(netlist,".model NMOS NMOS\n");
        fprintf(netlist,".model PMOS PMOS\n");
        fprintf(netlist,".ac dec 10 1 1meg\n");
        fprintf(netlist,".include cmosedu_models.txt\n");
        fprintf(netlist,".meas AC ganho FIND v(vout) AT 1 \n");
        fprintf(netlist,".end\n");
        fclose(netlist);

        system("ltspice -b circuito.cir");

        log = fopen("circuito.log", "r");       // sei oq essas proximas linhas fazem mas n sei todos os detalhes
        char line[100], meas[9];
        fgets(line, 99, log);
        while (!feof(log)){
            fgets(line, 99, log);
            if (strstr(line, "ganho") != NULL){
                strncpy(meas, &line[16], 8);
                meas[8]='\0';
                ganho = atof(meas);
                pop[i].score = ganho;
            }
        }
        fclose(log);
    }
}

void ordena(){      // soh copiei do athila, eh bubble eu queria fazer um quick mas da pra fazer depois
    int i, j;
    ind aux;
    for(i = 0; i < popsize-1; i++){
        for(j = 0; j < popsize-i-1; j++){
            if(pop[j+1].score > pop[j].score){
                aux = pop[j+1];
                pop[j+1] = pop[j];
                pop[j] = aux;
            }
        }
    }
}

void cruza(){ // tem que testas se com outros tamanhos de popsize e genes essa aqui vai funcionar
    int i, j, k, l;
    ind aux[popsize];
    for (i = 0; i < popsize;i++){
        aux[i] = pop[i];
    }
    i = 0;
    j = 1;
    for(k = 0; k < elite; k++){
        pop[k] = aux[k];
    }
    for(; k < popsize; k++){
        for(l = 0; l < genes/2; l++){
            pop[k].cromossomo[l] = aux[i].cromossomo[l];
        }
        for(l = genes/2; l < genes; l++){
            pop[k].cromossomo[l] = aux[j].cromossomo[l];
        }
        i=i+2;
        j=j+2;
        if(i >= popsize){
            i = 1;
            j = 0;
        }
    }
}

void muta(){
    int i, j, k, l;
    float aux;
    for(i = 0; i < popsize; i++){
        for(j = 0; j < genes; j++){
            aux = pop[i].cromossomo[j];
            l = 0;
            if((rand()%100) < mutchance){
                do{
                    pop[i].cromossomo[j] = aux;
                    if(l == 0){
                        k = rand()%2;   // define se a mutacao vai ser para mais ou para menos
                    }
                    else{
                        k = 2;
                    }
                    if(k == 0 || l == 2){
                        pop[i].cromossomo[j] *= (1-muttx);
                        l = 1;
                    }
                    if(k == 1 || l == 1){
                        pop[i].cromossomo[j] *= (1+muttx);
                        l = 2;
                    }
                    printf("pop[%d].cromossomo[%d] = %f\n", i, j, pop[i].cromossomo[j]);
                }while(pop[i].cromossomo[j] < lb[j] || pop[i].cromossomo[j] > ub[j]);
            }
        }
    }
}

void printpop(){
    int i, j;
    for(i = 0; i < popsize; i++){
        printf("Ind %d\t:", i);
        for(j = 0; j < genes; j++){
            printf("[%d]: %f\t", j, pop[i].cromossomo[j]);
        }
        printf("score: %f", pop[i].score);
        printf("\n");
    }
    printf("\n");
}
