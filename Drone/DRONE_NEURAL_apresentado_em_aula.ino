#include <math.h>
#include <Servo.h>

//dados da RNA
#define ENTRADAS        1                                                                
#define SAIDAS          2																 
#define NR_NEURON_O     5
#define CAMADAS_O       2

//configuracao sensor ultrasonico
#define TRIGGER 38
#define ECHO 39

//configuracao motores
#define MOTORA 11 // Pino de funcionamento do motor A
#define MOTORB 12 // Pino de funcionamento do motor B

//valores de seguranca
#define MIN 1100 // Minimo do ESC  
#define MAX 1450 // Maximo do ESC
#define NUMLEITURAS 5 //Numero de leituras antes de mandar o dado do ultrasom

//diferença mecanica
#define DIF  05       
#define DIF2 0        


//VARIAVEIS GLOBAIS;
Servo motorA;
Servo motorB;

//sinapses treinadas;
double w_e_o[ENTRADAS+1][NR_NEURON_O] = {{1.098026, 1.255222, -0.762470, 1.225690, -0.863524},
                                        {0.614540, -1.661055, 0.819144, -1.639193, -1.786923}};

double w_o_o[NR_NEURON_O+1][NR_NEURON_O] = {{-0.875570, -1.124995, 0.506570, 1.377800, 0.740659},
                                            {1.030944, -1.032429, 0.978878, 1.015448, -1.028357},
                                            {-1.032429, 1.098214, 1.063360, 0.954283, 1.065743},
                                            {0.978878, 1.063360, -0.958616, -1.029618, 1.042645},
                                            {1.015448, 0.954283, -1.029618, 1.023899, -1.031168},
                                            {-1.028357, -0.934257, -0.957355, 0.968832, -0.953253}};

double w_o_s[NR_NEURON_O+1][SAIDAS] = {{-3.002281, -0.738583},
                                       {-0.926601, -0.146892},
                                       {-2.789558, 1.148594},
                                       {0.344814, 0.724331},
                                       {3.348338, -2.566194},
                                       {-1.330737, 0.661115}};

float saidaA; 
float saidaB; 
double saida_s[SAIDAS];
double saida_o[NR_NEURON_O][CAMADAS_O];
double altura = 0, alturaAnterior = 0;
                                            
//PROTOTIPOS--------------------------------------------------------------
double f_sigmoid(double net);
void calcular_saidas(double entradas[ENTRADAS]);
float microsecondsToCentimeters(long microseconds);
float calculaDistancia(int trigger, int echo);
//------------------------------------------------------------------------

void setup()
{
	Serial.begin(115200);     //velocidade de transmissão

	pinMode(TRIGGER, OUTPUT); //sensor 
	pinMode(ECHO, INPUT);     //sensor

	pinMode(MOTORA, OUTPUT);  // Setando pino do motorA como saida
	pinMode(MOTORB, OUTPUT) ; // Setando pino do motorB como saida
	delay(2000);
	motorA.attach(MOTORA);    //Setando em qual pino os comandos serao executados
	motorB.attach(MOTORB);
	delay(100);
	motorA.writeMicroseconds(MIN); // "Ligando" o ESC A
	motorB.writeMicroseconds(MIN); // "Ligando" o ESC B
	delay(1000);

}

void loop()
{
	altura = 0;
	for (int i = 0; i < NUMLEITURAS; i++)
	{
		float lido = calculaDistancia(TRIGGER, ECHO);
		if (lido >= 112.0)
			lido = 112.0;

		else if (lido <=  26.0)
			lido =  26.0;

		altura += lido;
	}

	altura /= NUMLEITURAS;



	//Serial.print("Altura: ");
	//Serial.println(altura);

	if ((abs(altura - alturaAnterior) > 2))
	{
		calcular_saidas(altura/100);


		saidaA = saida_s[0]*10000 + DIF;
		saidaB = saida_s[1]*10000 + DIF2;

   

		if (saidaA < MIN) {
        	saidaA = MIN;        
		}

        if (saidaB < MIN) {
        	saidaB = MIN;        
        }

    	if (saidaA > MAX) {
        	saidaA = MAX;        
    	}

    	if (saidaB > MAX) {
      	    saidaB = MAX;        
    	}
			

		

		Serial.print((int)altura);
		Serial.print(" | ");
		Serial.print((int)saidaA);
		Serial.print(" | ");
		Serial.println((int)saidaB);

		motorA.writeMicroseconds((int)saidaA);
		motorB.writeMicroseconds((int)saidaB);

		alturaAnterior = altura;
		delay(100);
	}
//delay(500);
	
}

void calcular_saidas(double entradas)
{
  int i, j;

  // Calcular os nets da entrada para a camada oculta
	for (i = 0; i < NR_NEURON_O; i++) {
		saida_o[i][0] = 0.0;
		saida_o[i][0] += w_e_o[0][i] * 1;  // Calcula saida para bias

		for (j = 1; j < ENTRADAS+1; j++)
			saida_o[i][0] += w_e_o[j][i] * entradas;

   	 // Calcular a saida de saida_o[i][0]
    	saida_o[i][0] = f_sigmoid(saida_o[i][0]);
    }

   // Calcular net da camada oculta 1 para camada oculta 2
	for (i = 0; i < NR_NEURON_O; i++) {
    	saida_o[i][1] = 0.0;
    	saida_o[i][1] += w_o_o[0][i] * 1;  // Calcula saida para bias

    	for (j = 1; j < NR_NEURON_O+1; j++)
      		saida_o[i][1] += w_o_o[j][i] * saida_o[j-1][0];

    	saida_o[i][1] = f_sigmoid(saida_o[i][1]);
	}

  // Calcular os nets da camada oculta 2 para a saida
	for (i = 0; i < SAIDAS; i++) {
    	saida_s[i] = 0.0;
    	saida_s[i] += w_o_s[0][i] * 1;  // Calcula saida para bias

    	for (j = 1; j < NR_NEURON_O+1; j++)
      		saida_s[i] += w_o_s[j][i] * saida_o[j-1][1];

      	saida_s[i] = f_sigmoid(saida_s[i]);
    }
        
}

double f_sigmoid(double net){
  return 1 / (1 + exp(-net));
}

float calculaDistancia(int trigger, int echo)
{
	// Envia um sinal de 10ms ao sensor
	digitalWrite(trigger, LOW);
	delayMicroseconds(2);
	digitalWrite(trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigger, LOW);

	// O sensor calcula o tempo gasto entre o envio e o recebimento
	// do sinal e retorna um pulso com esta dura��o
	long duration = pulseIn(echo, HIGH);

	// Converte o tempo para distancia em centimetros
	float cm = microsecondsToCentimeters(duration);

	// retorna a dist�ncia 
	return cm;
}

float microsecondsToCentimeters(long microseconds)
{

	// Converte o tempo de microsegundos para segundos
	float seconds = (float)microseconds / 1000000.0;
	// Com a velocidade do som de 340m/s calcula-se a
	// distancia percorrida
	float distance = seconds * 340;
	// Divide o resultado por dois pois o tempo � calculado
	// considerando a ida e a volta do sinal  
	distance = distance / 2;
	// Converte o resultado em metros para centimetros
	distance = distance * 100;

	return distance;
}

