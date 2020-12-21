/*
** Desenvolvido com o intuito de controlar o equilibrio 
** de um "drone" utilizando lógica fuzzy
** 
** Data: 02/06/2015
**
** Desenvolvido por: Alan Kunz Cechinel      - 13103212
**                   Thiago Raulino Dal Pont - 13104518			
*/

#include <Servo.h>
#include "Fuzzy.h"
#include "FuzzyComposition.h"
#include "FuzzyInput.h"
#include "FuzzyIO.h"
#include "FuzzyOutput.h"
#include "FuzzyRule.h"
#include "FuzzyRuleAntecedent.h"
#include "FuzzyRuleConsequent.h"
#include "FuzzySet.h"

//*=================Definição de pinos e constantes=================*/
#define TRIGGER 38
#define ECHO 39
#define MOTORA 11 // Pino de funcionamento do motor A
#define MOTORB 12 // Pino de funcionamento do motor B
#define MIN 1250 // Mínimo do ESC
#define MIN2 1210
#define MAX 1310 // Máximo do ESC
#define NUMLEITURAS 1 //Número de leituras antes de mandar o dado do ultrasom


/*=====================================================*/

/*================Definição de Variáveis===============*/
Servo motorA;
Servo motorB;
Fuzzy* fuzzy = new Fuzzy();
float altura = 0, alturaAnterior = 0;
/*=====================================================*/

/*================Protótipos de Funções================*/
float calculaDistancia(int trigger, int echo);
float microsecondsToCentimeters(long microseconds);
/*=====================================================*/

void setup()
{
	Serial.begin(115200);

	pinMode(TRIGGER, OUTPUT);
	pinMode(ECHO, INPUT);

	pinMode(MOTORA, OUTPUT); // Setando pino do motor como saída
	pinMode(MOTORB, OUTPUT);
	delay(2000);
	motorA.attach(MOTORA); // Setando em qual pino os comandos seram executados
	motorB.attach(MOTORB);
	delay(100);
	motorA.writeMicroseconds(1100); // "Ligando" o ESC A
	motorB.writeMicroseconds(1100); // "Ligando" o ESC B
	delay(1000);

	FuzzyInput* distancia = new FuzzyInput(1);
	FuzzyOutput* saidaMotorA = new FuzzyOutput(1);
	FuzzyOutput* saidaMotorB = new FuzzyOutput(2);

	/*============Definindo conjunto de entrada============*/
	FuzzySet* baixo = new FuzzySet(0.1, 12.0, 12.0, 22.0);
	distancia->addFuzzySet(baixo);

  FuzzySet* poucoBaixo = new FuzzySet(20.0, 32.0, 32.0, 42.0);
  distancia->addFuzzySet(poucoBaixo);


	FuzzySet* plano = new FuzzySet(41.0, 43.0, 43.0, 45.0);
  distancia->addFuzzySet(plano);

  FuzzySet* poucoAlto = new FuzzySet(44.0, 54.0, 54.0, 64.0);
  distancia->addFuzzySet(poucoAlto);


	FuzzySet* alto = new FuzzySet(54.0, 74.0, 74.0, 94.0);
	distancia->addFuzzySet(alto);

	fuzzy->addFuzzyInput(distancia);
	/*=====================================================*/

	/*=============Definindo conjunto de saída=============*/

	FuzzySet* muitoPoucoAceleradoA = new FuzzySet(MIN, MIN+5, MIN+5, MIN+10);
  saidaMotorA->addFuzzySet(muitoPoucoAceleradoA);
  FuzzySet* poucoAceleradoA = new FuzzySet(MIN+5, MIN+20, MIN+20, MIN+40);
  saidaMotorA->addFuzzySet(poucoAceleradoA);
  FuzzySet* padraoA = new FuzzySet(MIN+20, MIN+40, MIN+40, MIN+60);
  saidaMotorA->addFuzzySet(padraoA);
  FuzzySet* aceleradoA = new FuzzySet(MIN+40, MIN+60, MIN+60, MIN+70);
  saidaMotorA->addFuzzySet(aceleradoA);
  FuzzySet* muitoAceleradoA = new FuzzySet(MIN+60, MIN+70, MIN+70, MIN+80);
  saidaMotorA->addFuzzySet(muitoAceleradoA);

  FuzzySet* muitoPoucoAceleradoB = new FuzzySet(MIN2, MIN2+5, MIN2+5, MIN2+10);
  saidaMotorB->addFuzzySet(muitoPoucoAceleradoB);
  FuzzySet* poucoAceleradoB = new FuzzySet(MIN2+5, MIN2+20, MIN2+20, MIN2+40);
  saidaMotorB->addFuzzySet(poucoAceleradoB);
  FuzzySet* padraoB = new FuzzySet(MIN2+20, MIN2+40, MIN2+40, MIN2+60);
  saidaMotorB->addFuzzySet(padraoB);
  FuzzySet* aceleradoB = new FuzzySet(MIN2+40, MIN2+60, MIN2+60, MIN2+70);
  saidaMotorB->addFuzzySet(aceleradoB);
  FuzzySet* muitoAceleradoB = new FuzzySet(MIN2+60, MIN2+70, MIN2+70, MIN2+80);
  saidaMotorB->addFuzzySet(muitoAceleradoB);

	fuzzy->addFuzzyOutput(saidaMotorA);
	fuzzy->addFuzzyOutput(saidaMotorB);
	/*=====================================================*/

	/*============Definindo conjunto de regras=============*/
	// Os consequentes seguirão a regra a seguir - Entao_saida_(função membro a)_(função membro b)

	FuzzyRuleAntecedent* Se_altura_baixo = new FuzzyRuleAntecedent();
	Se_altura_baixo->joinSingle(baixo);
	FuzzyRuleConsequent* Entao_saida_muitoAceleradoA_muitoPoucoAceleradoB = new FuzzyRuleConsequent();
	Entao_saida_muitoAceleradoA_muitoPoucoAceleradoB->addOutput(muitoAceleradoA);
	Entao_saida_muitoAceleradoA_muitoPoucoAceleradoB->addOutput(muitoPoucoAceleradoB);
	FuzzyRule* fuzzyRule1 = new FuzzyRule(1, Se_altura_baixo, Entao_saida_muitoAceleradoA_muitoPoucoAceleradoB);
	fuzzy->addFuzzyRule(fuzzyRule1);

	FuzzyRuleAntecedent* Se_altura_poucoBaixo = new FuzzyRuleAntecedent();
	Se_altura_poucoBaixo->joinSingle(poucoBaixo);
	FuzzyRuleConsequent* Entao_saida_aceleradoA_poucoAceleradoB = new FuzzyRuleConsequent();
	Entao_saida_aceleradoA_poucoAceleradoB->addOutput(aceleradoA);
	Entao_saida_aceleradoA_poucoAceleradoB->addOutput(poucoAceleradoB);
	FuzzyRule* fuzzyRule2 = new FuzzyRule(2, Se_altura_poucoBaixo, Entao_saida_aceleradoA_poucoAceleradoB);
	fuzzy->addFuzzyRule(fuzzyRule2);


	FuzzyRuleAntecedent* Se_altura_plano = new FuzzyRuleAntecedent();
	Se_altura_plano->joinSingle(plano);
	FuzzyRuleConsequent* Entao_saida_padraoA_padraoB = new FuzzyRuleConsequent();
	Entao_saida_padraoA_padraoB->addOutput(padraoA);
	Entao_saida_padraoA_padraoB->addOutput(padraoB);
	FuzzyRule* fuzzyRule3 = new FuzzyRule(3, Se_altura_plano, Entao_saida_padraoA_padraoB);
	fuzzy->addFuzzyRule(fuzzyRule3);

	FuzzyRuleAntecedent* Se_altura_poucoAlto = new FuzzyRuleAntecedent();
	Se_altura_poucoAlto->joinSingle(poucoAlto);
	FuzzyRuleConsequent* Entao_saida_poucoAceleradoA_aceleradoB = new FuzzyRuleConsequent();
	Entao_saida_poucoAceleradoA_aceleradoB->addOutput(poucoAceleradoA);
	Entao_saida_poucoAceleradoA_aceleradoB->addOutput(aceleradoB);
	FuzzyRule* fuzzyRule4 = new FuzzyRule(4, Se_altura_poucoAlto, Entao_saida_poucoAceleradoA_aceleradoB);
	fuzzy->addFuzzyRule(fuzzyRule4);

	FuzzyRuleAntecedent* Se_altura_alto = new FuzzyRuleAntecedent();
	Se_altura_alto->joinSingle(alto);
	FuzzyRuleConsequent* Entao_saida_muitoPoucoAceleradoA_muitoAceleradoB = new FuzzyRuleConsequent();
	Entao_saida_muitoPoucoAceleradoA_muitoAceleradoB->addOutput(muitoPoucoAceleradoA);
	Entao_saida_muitoPoucoAceleradoA_muitoAceleradoB->addOutput(muitoAceleradoB);
	FuzzyRule* fuzzyRule5 = new FuzzyRule(5, Se_altura_alto, Entao_saida_muitoPoucoAceleradoA_muitoAceleradoB);
	fuzzy->addFuzzyRule(fuzzyRule5);
	
	/*=====================================================*/
}

void loop()
{
	altura = 0;
	for (int i = 0; i < NUMLEITURAS; i++)
	{
		float lido = calculaDistancia(TRIGGER, ECHO);
		if (lido >= 84.0)
			lido = 84.0;

		if (lido <=  5.0)
			lido =  5.0;

		altura += lido;
	}

	altura /= NUMLEITURAS;



	//Serial.print("Altura: ");
	//Serial.println(altura);

	fuzzy->setInput(1, altura);
	fuzzy->fuzzify();

	float saidaA = fuzzy->defuzzify(1);
	float saidaB = fuzzy->defuzzify(2);
  

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
		//(10);
	
//delay(500);
	
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
float microsecondsToCentimeters(long microseconds){

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
