/*
Algoritmo gen?tico para resolver o problema do caixeiro viajante
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NR_CIDADES    5
#define NR_GERACOES   1000
#define GENES         NR_CIDADES
#define NR_IND_POP    10
#define PMUT          30        // 30%
#define PREC          100       // 100%


/*
Tipos de dados
*/
typedef unsigned int u_int;

typedef struct individuo {
  u_int id;
  float fitness;
  u_int cromossomo[GENES];
} ind_t;

typedef struct filhos {
  ind_t filhos[2];
} filhos_t;

/*
Estrutura de dados
*/
u_int distancias[NR_CIDADES][NR_CIDADES];
ind_t populacao[NR_IND_POP];



/*
Cabe?alho das fun??es auxiliares
*/
void cria_populacao_inicial();
void calcula_fitness(ind_t *individuo);
void calcula_fitness_pop();
u_int seleciona_melhor();
filhos_t cruzamento(ind_t ind_1, ind_t ind_2);
void mutacao(ind_t *individuo);
void mostra_cromossomo(ind_t individuo);
void mostra_populacao();
void ordena_pop_fitness();


/*
Fun??o principal
*/
int main()
{
  int geracoes = 0;

  // Gera a sementinha do mal
  srand(time(NULL));

  // Cria uma populacao de individuos
  cria_populacao_inicial();

  do {
    // Avalia os individuos (calcula fitness)
    calcula_fitness_pop();

    // Ordena a populacao
    ordena_pop_fitness();

    // Seleciona os x melhores

    // Muta alguns individuos (prob. p)
    for (i = 0; i < NR_IND_POP; i++)
      mutacao(&populacao[i]);

    geracoes++;
  } while (geracoes <= NR_GERACOES);

  return 0;
}

/*
Implementa??o das fun??es auxiliares
*/
void cria_populacao_inicial()
{
  int i, j;
  for (i = 0; i < NR_IND_POP; i++) {
    populacao[i].id       = i+1;
    populacao[i].fitness  = 0.0;
    for (j = 0; j < GENES; j++) {
      populacao[i].cromossomo[j] = rand() % NR_CIDADES;
    }
  }
}

void calcula_fitness(ind_t *individuo)
{
  int i;

  for (i = 0; i < GENES-1; i++) {
    individuo->fitness += abs(individuo->cromossomo[i] - individuo->cromossomo[i+1]);
  }

}

void calcula_fitness_pop()
{
  int i;

  for (i = 0; i < NR_IND_POP; i++)
    calcula_fitness(&populacao[i]);
}

u_int seleciona_melhor()
{

}

filhos_t cruzamento(ind_t ind_1, ind_t ind_2)
{
  int ponto_cruzamento = rand() % (NR_CIDADES - 1);
  int gene, i, j;
  filhos_t filhos;

  // Inverte os genes de ind_1 com ind_2
  for (i = 0; i <= ponto_cruzamento; i++) {
    gene = ind_1.cromossomo[i];
    ind_1.cromossomo[i] = ind_2.cromossomo[i];
    ind_2.cromossomo[i] = gene;
  }

  // Gera os filhos
  filhos.filhos[0] = ind_1;
  filhos.filhos[1] = ind_2;

  return filhos;

}

void mutacao(ind_t *individuo)
{
  int pmut = rand() % 100;

  if (pmut >= PMUT) {
    // Escolhe o ponto de muta??o
    int ponto_de_mutacao = rand() % GENES;  // 0 .. GENES - 1
    individuo->cromossomo[ponto_de_mutacao] = rand() % NR_CIDADES + 1;
  }

}

void mostra_cromossomo(ind_t individuo)
{
  int i;

  printf("Cromossomo do individuo %d\n", individuo.id);
  for (i = 0; i < GENES; i++) {
    printf("%d ", individuo.cromossomo[i]);
  }
  printf("Fitness: %.2f\n\n", individuo.fitness);
}

void mostra_populacao()
{
  int i;
  float media_fitness = 0.0;

  printf("Populacao...........\n");
  for (i = 0; i < NR_IND_POP; i++) {
    mostra_cromossomo(populacao[i]);
    media_fitness += populacao[i].fitness;
  }

  printf("Media de fitness da populacao: %.2f\n", media_fitness / NR_IND_POP);

}

void ordena_pop_fitness()
{
  int i, j;
  ind_t ind;

  for (i = 0; i < NR_IND_POP; i++) {
    for (j = 1; j < NR_IND_POP-1; j++) {
      if (populacao[i].fitness < populacao[j].fitness) {
        ind = populacao[i];
        populacao[i] = populacao[j];
        populacao[j] = ind;
      }
    }
  }
}
