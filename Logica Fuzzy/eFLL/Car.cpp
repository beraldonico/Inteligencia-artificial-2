#include <iostream>
#include "FuzzyRule.h"
#include "FuzzyComposition.h"
#include "Fuzzy.h"
#include "FuzzyRuleConsequent.h"
#include "FuzzyOutput.h"
#include "FuzzyInput.h"
#include "FuzzyIO.h"
#include "FuzzySet.h"
#include "FuzzyRuleAntecedent.h"

using namespace std;

int main(int argc, char *argv[]) {


 Fuzzy* Car = new Fuzzy();

 FuzzyInput* Sensor1 = new FuzzyInput(1);
 FuzzyInput* Sensor2 = new FuzzyInput(2);

 FuzzySet* Longe = new FuzzySet(78, 100, 100, 120);
 FuzzySet* Longe2 = new FuzzySet(78, 100, 100, 120);


 FuzzySet* Aceitavel = new FuzzySet(38, 60, 60, 80);
 FuzzySet* Aceitavel2 = new FuzzySet(38, 60, 60, 80);


 FuzzySet* Perto = new FuzzySet(0, 20, 20, 40);
 FuzzySet* Perto2 = new FuzzySet(0, 20, 20, 40);

 Sensor1->addFuzzySet(Longe);

 Sensor1->addFuzzySet(Aceitavel);

 Sensor1->addFuzzySet(Perto);

 Sensor2->addFuzzySet(Longe2);

 Sensor2->addFuzzySet(Aceitavel2);

 Sensor2->addFuzzySet(Perto2);

 Car->addFuzzyInput(Sensor1);

 Car->addFuzzyInput(Sensor2);

 FuzzyOutput* Roda1 = new FuzzyOutput(1);
 FuzzyOutput* Roda2 = new FuzzyOutput(2);

 FuzzySet* Lento = new FuzzySet(0, 45, 45, 90);
 FuzzySet* Lento2 = new FuzzySet(0, 45, 45, 90);
 Roda1->addFuzzySet(Lento);
 Roda2->addFuzzySet(Lento2);

 FuzzySet* Moderado = new FuzzySet(85, 125, 125, 175);
 FuzzySet* Moderado2 = new FuzzySet(85, 125, 125, 175);
 Roda1->addFuzzySet(Moderado);
 Roda2->addFuzzySet(Moderado2);

 FuzzySet* Rapido = new FuzzySet(170, 215, 215, 255);
 FuzzySet* Rapido2 = new FuzzySet(170, 215, 215, 255);
 Roda1->addFuzzySet(Rapido);
 Roda2->addFuzzySet(Rapido2);

 FuzzySet* Back = new FuzzySet(-175, -125, -125, -85);
 FuzzySet* Back2 = new FuzzySet(-175, -125, -125, -85);
 Roda1->addFuzzySet(Back);
 Roda2->addFuzzySet(Back2);

 Car->addFuzzyOutput(Roda1);
 Car->addFuzzyOutput(Roda2);

 FuzzyRuleAntecedent* ifSensorLongeLonge = new FuzzyRuleAntecedent();
 ifSensorLongeLonge->joinWithAND(Longe, Longe2);
 FuzzyRuleConsequent* thenRodaRapidoRapido = new FuzzyRuleConsequent();
 thenRodaRapidoRapido->addOutput(Rapido);
 thenRodaRapidoRapido->addOutput(Rapido2);

  FuzzyRuleAntecedent* ifSensorLongeAceitavel = new FuzzyRuleAntecedent();
 ifSensorLongeAceitavel->joinWithAND(Longe, Aceitavel2);
 FuzzyRuleConsequent* thenRodaModeradoRapido = new FuzzyRuleConsequent();
 thenRodaModeradoRapido->addOutput(Moderado);
 thenRodaModeradoRapido->addOutput(Rapido2);

  FuzzyRuleAntecedent* ifSensorAceitavelLonge = new FuzzyRuleAntecedent();
 ifSensorAceitavelLonge->joinWithAND(Aceitavel, Longe2);
 FuzzyRuleConsequent* thenRodaRapidoModerado = new FuzzyRuleConsequent();
 thenRodaRapidoModerado->addOutput(Rapido);
 thenRodaRapidoModerado->addOutput(Moderado2);

  FuzzyRuleAntecedent* ifSensorAceitavelAceitavel= new FuzzyRuleAntecedent();
 ifSensorAceitavelAceitavel->joinWithAND(Aceitavel, Aceitavel2);
 FuzzyRuleConsequent* thenRodaModeradoModerado = new FuzzyRuleConsequent();
 thenRodaModeradoModerado->addOutput(Moderado);
 thenRodaModeradoModerado->addOutput(Moderado2);

 FuzzyRuleAntecedent* ifSensorPertoAceitavel= new FuzzyRuleAntecedent();
 ifSensorPertoAceitavel->joinWithAND(Perto, Aceitavel2);
 FuzzyRuleConsequent* thenRodaModeradoLento = new FuzzyRuleConsequent();
 thenRodaModeradoLento->addOutput(Moderado);
 thenRodaModeradoLento->addOutput(Lento2);

 FuzzyRuleAntecedent* ifSensorAceitavelPerto= new FuzzyRuleAntecedent();
 ifSensorAceitavelPerto->joinWithAND(Aceitavel, Perto2);
 FuzzyRuleConsequent* thenRodaLentoModerado = new FuzzyRuleConsequent();
 thenRodaLentoModerado->addOutput(Lento);
 thenRodaLentoModerado->addOutput(Moderado2);

 FuzzyRuleAntecedent* ifSensorPertoPerto= new FuzzyRuleAntecedent();
 ifSensorPertoPerto->joinWithAND(Perto, Perto2);
 FuzzyRuleConsequent* thenRodaBackBack = new FuzzyRuleConsequent();
 thenRodaBackBack->addOutput(Back);
 thenRodaBackBack->addOutput(Back2);
 FuzzyRuleConsequent* thenMeiaVolta = new FuzzyRuleConsequent();
 thenMeiaVolta->addOutput(Back);
 thenMeiaVolta->addOutput(Rapido2);


 FuzzyRuleAntecedent* ifSensorPertoLonge = new FuzzyRuleAntecedent();
 ifSensorPertoLonge->joinWithAND(Perto, Longe2);
 FuzzyRuleConsequent* thenRodaLentoBack = new FuzzyRuleConsequent();
 thenRodaLentoBack->addOutput(Lento);
 thenRodaLentoBack->addOutput(Back2);


 FuzzyRuleAntecedent* ifSensorLongePerto = new FuzzyRuleAntecedent();
 ifSensorLongePerto->joinWithAND(Longe, Perto2);
 FuzzyRuleConsequent* thenRodaBackLento = new FuzzyRuleConsequent();
 thenRodaBackLento->addOutput(Back);
 thenRodaBackLento->addOutput(Lento2);

 FuzzyRule* regra1 = new FuzzyRule(1, ifSensorLongeLonge, thenRodaRapidoRapido);
 Car->addFuzzyRule(regra1);
 FuzzyRule* regra2 = new FuzzyRule(2, ifSensorLongeAceitavel, thenRodaModeradoRapido);
 Car->addFuzzyRule(regra2);
 FuzzyRule* regra3 = new FuzzyRule(3, ifSensorAceitavelLonge, thenRodaRapidoModerado);
 Car->addFuzzyRule(regra3);
 FuzzyRule* regra4 = new FuzzyRule(4, ifSensorAceitavelAceitavel, thenRodaModeradoModerado);
 Car->addFuzzyRule(regra4);
 FuzzyRule* regra5 = new FuzzyRule(5, ifSensorPertoAceitavel, thenRodaModeradoLento);
 Car->addFuzzyRule(regra5);
 FuzzyRule* regra6 = new FuzzyRule(6, ifSensorAceitavelPerto, thenRodaLentoModerado);
 Car->addFuzzyRule(regra6);
 FuzzyRule* regra7 = new FuzzyRule(7, ifSensorPertoPerto, thenRodaBackBack);
 Car->addFuzzyRule(regra7);
 FuzzyRule* regra8 = new FuzzyRule(8, ifSensorLongePerto, thenRodaLentoBack);
 Car->addFuzzyRule(regra8);
 FuzzyRule* regra9 = new FuzzyRule(9, ifSensorPertoLonge, thenRodaBackLento);
 Car->addFuzzyRule(regra9);


int var;

 while (var != -1){


    for (int i=0; i<2; i++){

        cout << "Sensor " << i+1 << ":" << endl;
        cin >> var;
        Car->setInput(i+1, var);

    }

    Car->fuzzify();

    float output1 = Car->defuzzify(1);
    float output2 = Car->defuzzify(2);
    cout << "Roda1: " << output1 << " --- Roda2: " << output2 << endl;
 }

  return 0;
}
