#include <queue>
#include "simlib.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

queue<double> waitingInBa;
Facility Ke_KNH("Košice-Kostoľany nad Hornádom");
Facility KNH_Ky("Kostoľany nad Hornádom-v");
Facility Ky_VR("Kysak-Výh. Ružín");
Facility VR_Marg("Výh. Ružín-Margecany");
Facility Marg_Kr("Margecany_Krompachy");
Facility Kr_SV("Krompachy-Spišské Vlachy");
Facility SV_Mar("Spišské Vlachy-Markušovce");
Facility Mar_SNV("Markušovce-Spišská Nová Ves");
Facility SNV_Vy("Spišská Nová Ves-Vydrník");
Facility Vy_Pp("Vydrník-Poprad-Tatry");
Facility Pp_Svit("Poprad-Tatry-Svit");
Facility Svit_Strba("Svit-Štrba");
Facility Strba_Vych("Štrba-Východná");
Facility Vych_KL("Východná-Kráľova Lehota");
Facility KL_LH("Kráľova Lehota-Liptovský Hrádok");
Facility LH_LM("Liptovský Hrádok-Liptovský Mikuláš");
Facility LM_VP("Liptovský Mikuláš-Výh. Paludza");
Facility VP_LT("Výh. Paludza-Liptovská Teplá");
Facility LT_Rk("Liptovská Teplá-Ružomberok");
Facility Rk_Lub("Ružomberok-Ľubochňa");
Facility Lub_Kra("Ľubochňa-Kraľovany");
Facility Kra_Tur("Kraľovany-Turany");
Facility Tur_Vr("Turany-Vrútky");
Facility Vr_Zi("Vrútky-Žilina");
Facility Zi_DH("Žilina-Dolný Hričov");
Facility DH_By("Dolný Hričov-Bytča");
Facility By_PB("Bytča-Považská Bystrica");
Facility PB_Pu("Považská Bystrica-Púchov");
Facility Pu_La("Púchov-Ladce");
Facility La_TT("Ladce-Trenčianska Teplá");
Facility TT_Tn("Trenčianska Teplá-Trenčín");
Facility Tn_VN("Trenčín-Výh. Nivy");
Facility VN_TB("Výh. Nivy-Trenčianske Bohuslavice");
Facility TB_NMNV("Trenčianske Bohuslavice-Nové Mesto nad Váhom");
Facility NMNV_Pie("Nové Mesto nad Váhom-Piešťany");
Facility Pie_VK("Piešťany-Veľké Kostoľany");
Facility VK_Le("Veľké Kostoľany-Leopoldov");
Facility Le_Bre("Leopoldov-Brestovany");
Facility Bre_Tr("Brestovany-Trnava");
Facility Tr_Ci("Trnava-Cífer");
Facility Ci_Se("Cífer-Šenkvice");
Facility Se_VSJ("Šenkvice-Výh. Svätý Jur");
Facility VSJ_Vi("Výh. Svätý JurOdb.-Vinohrady");
Facility Vi_Ba("Vinohrady-Bratislava hl stanica");

Facility Sturovo_NoveZamky("Sturovo-NoveZamky");
Facility NoveZamky_Bratislava ("NoveZamky-Bratislava");

 Store Sturovo (2);
 Store NoveZamky (2);

 Store Ky (4);
 Store Ma (2);
 Store SNV (2);
 Store Pp (3);
 Store St(2);
 Store LH (2);
 Store LM (2);
 Store Rk (2);
 Store Kr (2);
 Store Vr (3);
 Store Zi (3);
 Store PB (2);
 Store Pu (2);
 Store TT (2);
 Store Tn (3);
 Store NMNV (2);
 Store Pie (2);
 Store Le (2);
 Store Tr (3);
 Store Vi (2);
 Store Ba (6);


Stat trainTime("Time which take path from Ke    to Ba");
Stat timeInBAStation("Time which travelers spend in Bratislava ");
Stat HungaryTrainTime("Time which take path from Bydapest to Ba");

double propability = 0;
int timeOfDelay [6] = {0,0,0,0,0,0};
string station [6];

double ran; // for saving Random() values
double suicideOrBreakDown;

class Train1: public Process {
  double timeT = Time; // ulozime si hodnotu casu, aby sme vedeli neskor vypocitat
  string startStation;
  double delayTime;
  int inputDelay = 0;
  int trainNumber;

  public: void setValues(int delay, string s) {
    startStation = s;
    delayTime = delay;
    inputDelay = 1;

  }
  public: void setNumber(int i) {
    trainNumber = ++i;
  }

  void Behavior() {
    // pravdepodobnost ze vlak sa zdrzi v zaciatocnej stanici
    // Kosice

    if(!inputDelay) {
      if((ran = Random()) <= 0.06 ) {
        Wait(Uniform(5*60,15*60));
      }
    }
    if(!startStation.compare("Ke")) {
       inputDelay = 0;
       Wait(delayTime);
    }

    Seize(Ke_KNH);
    if(!inputDelay && ((ran = Random()) <= 0.2228 )) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(7*60 + Uniform(60,240) );
      }
    }
    else {

      Wait(7*60);
    }
    Release(Ke_KNH);

    Seize(KNH_Ky);
    if(!inputDelay && ((ran = Random()) <= 0.45) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(5*60 + Uniform(60,165) );
      }
    }
    else {
      Wait(5*60);
    }
    Release(KNH_Ky);

    //Kysak
    Enter(Ky,1);
    if(((ran = Random()) <= 0.02) && !inputDelay) { // moznost cakania na pripoj z PO
      if ((ran = Random()) < 0.8)
      {
        Wait(Uniform(3,8));
      }
      else {
        Wait(Uniform(9,15));
      }
    }
    else {
      if(!startStation.compare("Ky")) {
         inputDelay = 0; Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(Ky,1);

    Seize(Ky_VR);
    if(!inputDelay && ((ran = Random()) <= 0.30) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(7*60 + Uniform(-60,180) );
      }
    }
    else {
      Wait(7*60);
    }
    Release(Ky_VR);

    Seize(VR_Marg);
    if(!inputDelay && ((ran = Random()) <= 0.05) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(7*60 + Uniform(60,120) );
      }
    }
    else {
      Wait(7*60);
    }
    Release(VR_Marg);

    //Margecany
    Enter(Ma,1);
    if(!startStation.compare("Ma")) {
       inputDelay = 0; Wait(delayTime);
    }
    Wait(2*60);
    Leave(Ma,1);

    Seize(Marg_Kr);
    if(!inputDelay && ((ran = Random()) <= 0.044) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 + Uniform(60,120) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Marg_Kr);

    Seize(Kr_SV);

    if (!inputDelay && (Random() <= 0.0003) ){
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else {
      Wait(4*60);
    }
    Release(Kr_SV);

    Seize(SV_Mar);
    if(!inputDelay && ((ran = Random()) <= 0.01) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(8*60 + Uniform(30,90) );
      }
    }
    else {
      Wait(8*60);
    }
    Release(SV_Mar);

    Seize(Mar_SNV);
    if( !inputDelay && (timeT - Time > 48*60)) {//tzn. ma meskanie
      if((ran = Random()) <= 0.05 ) {
        if (ran <= 0.0003) {
          suicideOrBreakDown = Exponential(20*60);
          if ( suicideOrBreakDown < 10*60) {
            suicideOrBreakDown+= 10*60;
          }
          Wait(suicideOrBreakDown);
        }
        else {
          Wait(7*60 - Uniform(50,70) );
        }
      }
    }
    else {
      Wait(7*60);
    }
    Release(Mar_SNV);

    //Spiska Nova Ves
    Enter(SNV,1);
    if(!inputDelay &&((ran = Random())) <= 0.65) {
      Wait(Uniform(60,180) + 2*60);
    }
    else {
      if(!startStation.compare("SNV")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(SNV,1);

    Seize(SNV_Vy);
    if(!inputDelay && ((ran = Random()) <= 0.025) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(9*60 + Uniform(60,120) );
      }
    }
    else {
      Wait(9*60);
    }
    Release(SNV_Vy);

    Seize(Vy_Pp);
    if(!inputDelay && ((ran = Random()) <= 0.36) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(9*60 + Uniform(60,180) );
      }
    }
    else {
      Wait(9*60);
    }
    Release(Vy_Pp);

    //Poprad
    Enter(Pp,1);
    if(!inputDelay &&((ran = Random()) <= 0.05) ){
      Wait(Uniform(60,120) + 2*60);
    }
    else {
      if(!startStation.compare("Pp")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(Pp,1);

    Seize(Pp_Svit);
    if (!inputDelay && (Random() <= 0.0003) ){
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    Wait(6*60);
    Release(Pp_Svit);

    Seize(Svit_Strba);
    if(!inputDelay && ((ran = Random()) <= 0.03) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(9*60 + Uniform(30,60) );
      }
    }
    else {
      Wait(9*60);
    }
    Release(Svit_Strba);

    //Strba
    Enter(St,1);
    if( !inputDelay &&((ran = Random()) ) <= 0.25) {
      Wait(Uniform(30,60) + 60);
    }
    else {
      if(!startStation.compare("St")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(60);
    }
    Leave(St,1);

    Seize(Strba_Vych);
    if(!inputDelay && ((ran = Random()) <= 0.03) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(9*60 + 60);
      }
    }
    else {
      Wait(9*60);
    }
    Release(Strba_Vych);

    Seize(Vych_KL);
    if(!inputDelay && ((ran = Random()) <= 0.22) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 + Uniform(60,180) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Vych_KL);

    Seize(KL_LH);
    if(!inputDelay && ((ran = Random()) <= 0.0345) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(4*60 + 30 );
      }
    }
    else {
      Wait(4*60);
    }
    Release(KL_LH);

    //Liptovsky Hradok
    Enter(LH,1);
    if(!inputDelay &&((ran = Random()) <= 0.05) ){
      Wait(Uniform(30,60) + 60);
    }
    else {
      if(!startStation.compare("LH")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(60);
    }
    Leave(LH,1);

    Seize(LH_LM);
    if(!inputDelay && ((ran = Random()) <= 0.15) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(8*60 + 120 );
      }
    }
    else {
      Wait(8*60);
    }
    Release(LH_LM);

    //Liptovsky Mikulas
    Enter(LM,1);
    Wait(2*60);
    if(!startStation.compare("LM")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Leave(LM,1);

    Seize(LM_VP);
    if(!inputDelay && ((ran = Random()) <= 0.021) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(5*60 + Uniform(30,90) );
      }
    }
    else {
      Wait(5*60);
    }
    Release(LM_VP);

    Seize(VP_LT);
    if(!inputDelay && ((ran = Random()) <= 0.05) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 + Uniform(30,60) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(VP_LT);

    Seize(LT_Rk);
    if(!inputDelay && ((ran = Random()) <= 0.075) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(5*60 + Uniform(120,240) );
      }

    }
    else {
      Wait(5*60);
    }
    Release(LT_Rk);

    //Ruzomberok
    Enter(Rk,1);
    if(!inputDelay &&((ran = Random()) <= 0.032)) {
      Wait(Uniform(30,60) + 60);
    }
    else {
      if(!startStation.compare("Rk")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(60);
    }
    Leave(Rk,1);

    Seize(Rk_Lub);
    if(!inputDelay && ((ran = Random()) <= 0.025) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(10*60);
      }
    }
    else {
      Wait(9*60);
    }
    Release(Rk_Lub);

    Seize(Lub_Kra);
    if(!inputDelay && ((ran = Random()) <= 0.026) ) {
      Wait(6*60 + Uniform(30,60) );
    }
    else {
      Wait(6*60);
    }
    Release(Lub_Kra);

    //Kralovany
    Enter(Kr,1);
    Wait(2*60);
    if(!startStation.compare("Kr")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Leave(Kr,1);

    Seize(Kra_Tur);
    if(!inputDelay && ((ran = Random()) <= 0.003) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 + Uniform(30,90) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Kra_Tur);

    Seize(Tur_Vr);
    if(!inputDelay && ((ran = Random()) <= 0.028) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(7*60);
      }

    }
    else {
      Wait(6*60);
    }
    Release(Tur_Vr);

    //Vrutky
    Enter(Vr,1);
    Wait(2*60);
    if(!startStation.compare("Vr")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Leave(Vr,1);

    Seize(Vr_Zi);
    if(!inputDelay && ((ran = Random()) <= 0.022) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(16*60 + Uniform(60,180) );
      }
    }
    else {
      Wait(16*60);
    }
    Release(Vr_Zi);

    //Zilina
    Enter(Zi,1);
    if ( (Time - timeT > (187*60) ) && ( (ran = Random()) < 0.79) ) { //ma meskanie
      Wait(4*60 - Uniform(30,60));
    }
    else {
      if((ran >= 0.98) && (!inputDelay)) {
        Wait(Uniform(60,120) + 4*60);
      }
      else {
        Wait(4*60);
      }
    }
    if(!startStation.compare("Zi")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Leave(Zi,1);

    Seize(Zi_DH);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (191*60) )  && ( ran < 0.89 ) && !inputDelay){
      Wait(14*60 - Uniform(90,240) );
    }
    else {
      Wait(14*60);
    }
    Release(Zi_DH);
    Seize(DH_By);
    if (!inputDelay &&(ran <= 0.0003) ){
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else {
      Wait(4*60);
    }
    Release(DH_By);

    Seize(By_PB);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (209*60) )  && ( ran < 0.72 )&& !inputDelay){
      Wait(15*60 - Uniform(60,240) );
    }
    else {
      Wait(15*60);
    }
    Release(By_PB);

    //Povazska Bystrica
    Enter(PB,1);
    if(!inputDelay &&((ran = Random()) <= 0.03)) {
      Wait(Uniform(60,120) + 2*60);
    }
    else {
      if(!startStation.compare("PB")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(PB,1);

    Seize(PB_Pu);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (226*60) )  && ( ran < 0.61 )&& !inputDelay){
      Wait(13*60 - Uniform(60,120) );
    }
    else {
      Wait(13*60);
    }
    Release(PB_Pu);

    //Puchov
    Enter(Pu,1);
    if(!startStation.compare("Pu")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Wait(2*60);
    Leave(Pu,1);

    Seize(Pu_La);

    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (226*60) )  && ( ran < 0.043 )&& !inputDelay){
      Wait(7*60 - Uniform(30,90) );
    }
    else {
      Wait(7*60);
    }
    Release(Pu_La);

    Seize(La_TT);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (248*60) )  && ( ran < 0.72 ) && !inputDelay){
      Wait(9*60 - Uniform(30,60) );
    }
    else {
      Wait(9*60);
    }
    Release(La_TT);

    //Trencianska Tepla
    Enter(TT,1);
    if(!inputDelay &&((ran = Random()) <= 0.032)) {
      Wait(Uniform(60,120) + 2*60);
    }
    else {
      if(!startStation.compare("TT")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(TT,1);

    Seize(TT_Tn);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (259*60) )  && ( ran < 0.77 ) && !inputDelay){
      Wait(6*60 - Uniform(30,60) );
    }
    else {
      Wait(6*60);
    }
    Release(TT_Tn);

    //Trencin
    Enter(Tn,1);
    if ( (Time - timeT > (265*60) ) && ( (ran = Random()) < 0.73) && !inputDelay ) {
      Wait(2*60  - Uniform(30,60));
    }
    else {
      if(!startStation.compare("Tn")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(Tn,1);

    Seize(Tn_VN);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (267*60) )  && ( ran < 0.82 ) && !inputDelay){
      Wait(7*60 - Uniform(60,120) );
    }
    else {
      Wait(7*60);
    }
    Release(Tn_VN);

    Seize(VN_TB);
    if (!inputDelay &&(Random() <= 0.0003) ){
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else {
      Wait(5*60);
    }
    Release(VN_TB);

    Seize(TB_NMNV);
    if(((ran = Random()  <= 0.0003) && !inputDelay)) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else if((Time - timeT > (279*60) )  && ( ran < 0.822 )&& !inputDelay){
      Wait(5*60 - Uniform(30,60) );
    }
    else {
      Wait(5*60);
    }
    Release(TB_NMNV);

    //Nove Mesto nad Vahom
    Enter(NMNV,1);
    if(!inputDelay &&((ran = Random()) <= 0.04)) {
      Wait(Uniform(30,60) + 2*60);
    }
    else {
      if(!startStation.compare("NMNV")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(2*60);
    }
    Leave(NMNV,1);

    Seize(NMNV_Pie);
    if ( (Time - timeT > (296*60) ) && ( (ran = Random()) < 0.75) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(11*60 - Uniform(30,60) );
      }
    }
    else {
      Wait(11*60);
    }
    Release(NMNV_Pie);

    //Piestany
    Enter(Pie,1);
    if(!startStation.compare("Pie")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Wait(2*60);
    Leave(Pie,1);

    Seize(Pie_VK);
    if(!inputDelay && ((ran = Random()) <= 0.05) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(7*60 + Uniform(60,120) );
      }
    }
    else {
      Wait(7*60);
    }
    Release(Pie_VK);

    Seize(VK_Le);
    if(!inputDelay && ((ran = Random()) <= 0.01) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(4*60 + Uniform(30,60) );
      }
    }
    else {
      Wait(4*60);
    }
    Release(VK_Le);

    //Leopoldov
    Enter(Le,1);
    if(!startStation.compare("Le")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Wait(2*60);
    Leave(Le,1);

    Seize(Le_Bre);
    if ( (Time - timeT > (312*60) ) && ( (ran = Random()) < 0.735) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 - Uniform(30,60) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Le_Bre);

    Seize(Bre_Tr);
    if ( (Time - timeT > (318*60) ) && ( (ran = Random()) < 0.84) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 - Uniform(30,90) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Bre_Tr);

    //Trnava
    Enter(Tr,1);
    if(!startStation.compare("Tr")) {
       inputDelay = 0;
       Wait(delayTime);
    }
    Wait(2*60);
    Leave(Tr,1);


    Seize(Tr_Ci);
    if ( (Time - timeT > (326*60) ) && ( (ran = Random()) < 0.75) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 - Uniform(40,80) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Tr_Ci);

    Seize(Ci_Se);
    if ( (Time - timeT > (332*60) ) && ( (ran = Random()) < 0.75) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 - Uniform(40,80) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Ci_Se);

    Seize(Se_VSJ);
    if ( (Time - timeT > (338*60) ) && ( (ran = Random()) < 0.75) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 - Uniform(40,80) );
      }
    }
    else {
      Wait(6*60);
    }
    Release(Se_VSJ);

    Seize(VSJ_Vi);
    if(!inputDelay && ((ran = Random()) <= 0.09) ) {
      if (ran <= 0.0003) {
        suicideOrBreakDown = Exponential(20*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(6*60 + Uniform(60,120) );
      }

    }
    else {
      Wait(6*60);
    }
    Release(VSJ_Vi);

    //Bratislava Vinohrady
    Enter(Vi,1);
    if((ran = Random()) <= 0.12) {
      Wait(Uniform(30,60) + Uniform(120,420));
    }
    else {
      if(!startStation.compare("Vi")) {
         inputDelay = 0;
         Wait(delayTime);
      }
      Wait(60);
    }
    Leave(Vi,1);

    Seize(Vi_Ba);
    if (ran <= 0.0003) {
      suicideOrBreakDown = Exponential(20*60);
      if ( suicideOrBreakDown < 10*60) {
        suicideOrBreakDown+= 10*60;
      }
      Wait(suicideOrBreakDown);
    }
    else {
      Wait(6*60);
    }

    Release(Vi_Ba);

    //Bratislava hlavna stanica
    Enter(Ba,1);
    trainTime(Time - timeT);
    waitingInBa.push(trainNumber);
    // not finsh yet
    printf("%d Slovak time: %f\n",trainNumber,Time );
  }
};

class Train2 : public Process {
  int trainNumber;
  double timeT;
  public: void SetNumber(int i) {
    trainNumber = ++i;
  }
  void Behavior() {
    timeT = Time;
    if((ran = Random() ) <= 0.10)
    {
      if(ran <= 0.02) {
        Wait(Exponential(10*60));
      }
      Wait(Uniform(3*60,7*60));
    }
    Enter(Sturovo,1);
    Wait(3*60);
    Leave(Sturovo,1);

    Seize(Sturovo_NoveZamky);
    if((ran = Random() ) <= 0.15) {
      if (ran <= 0.002) {
        suicideOrBreakDown = Exponential(10*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(Uniform(27*60, 30*60));
      }
    }
    else {
      Wait(25*60);
    }
    Release(Sturovo_NoveZamky);

    Enter(NoveZamky,1);
    Wait(60*2);
    Leave(NoveZamky,1);

    Seize(NoveZamky_Bratislava);
    if((ran = Random() ) <= 0.13) {
      if (ran <= 0.002) {
        suicideOrBreakDown = Exponential(10*60);
        if ( suicideOrBreakDown < 10*60) {
          suicideOrBreakDown+= 10*60;
        }
        Wait(suicideOrBreakDown);
      }
      else {
        Wait(Uniform(53*60, 55*60));
      }
    }
    else {
      Wait(51*60);
    }
    Release(NoveZamky_Bratislava);

    Enter(Ba,1);
    Wait(Uniform(140,220));
    HungaryTrainTime(Time - timeT);
    Leave(Ba,6 - Ba.Free());
    while (!waitingInBa.empty()) {
      if(waitingInBa.front() ==  trainNumber){
        propability++;
      }
      //timeInBAStation(Time - waitingInBa.front());
      waitingInBa.pop();
    }
    printf("%d Hungary time: %f\n",trainNumber,Time );
  }

};

class  GeneratorKe : public Event {
  int i = 0;
  Train1 *train1;
  void Behavior() {
    train1 = (new Train1);
    if(timeOfDelay[i] != 0) {
      train1->setValues(timeOfDelay[i],station[i]);
    }
    train1->setNumber(i);
    i++;
    train1->Activate();
    if(Time < 50880  ) {
      Activate(Time + 7200);
    }
  }
};

class GeneratorHungary : public Event {
  int i = 0;
  Train2 *train2;
  void Behavior() {
    train2 = (new Train2);
    train2->SetNumber(i++);
    train2->Activate();
    if(Time < 67560  ) {
      Activate(Time + 7200);
    }
  }
};

int main(int argc, char** argv) {
  string help;
  string line;

  if(argc != 2) {
    std::cerr << "bad number of arguments" << '\n';
    return 0;
  }
  ifstream inputfile;
  inputfile.open(argv[1]);
  if(!inputfile.is_open()) {
    std::cerr << "error in opening input file" << '\n';
    return 0;
  }
  getline(inputfile,line);
  int  numberOfIterations = stoi(line);
  for (int i =0; i < 6; i++)
  {
    getline(inputfile,line);
    if (line.compare("0")) {
      timeOfDelay[i] = stoi(line);
      station[i] = line.substr(line.find(" ")+1);
    }
  }
  ofstream myfile;
  myfile.open("ZilinaStatistiky", std::ios::app);
  for (int i = 0; i < numberOfIterations; i++) {
  RandomSeed(time(NULL) + i);
  Init(0, 86400); // 1 den
  //timeInBAStation.Clear();
  HungaryTrainTime.Clear();
  trainTime.Clear();
  Ke_KNH.Clear();
  KNH_Ky.Clear();
  Ky_VR.Clear();
  VR_Marg.Clear();
  Marg_Kr.Clear();
  Kr_SV.Clear();
  SV_Mar.Clear();
  Mar_SNV.Clear();
  SNV_Vy.Clear();
  Vy_Pp.Clear();
  Pp_Svit.Clear();
  Svit_Strba.Clear();
  Strba_Vych.Clear();
  Vych_KL.Clear();
  KL_LH.Clear();
  LH_LM.Clear();
  LM_VP.Clear();
  VP_LT.Clear();
  LT_Rk.Clear();
  Rk_Lub.Clear();
  Lub_Kra.Clear();
  Kra_Tur.Clear();
  Tur_Vr.Clear();
  Vr_Zi.Clear();
  Zi_DH.Clear();
  DH_By.Clear();
  By_PB.Clear();
  PB_Pu.Clear();
  Pu_La.Clear();
  La_TT.Clear();
  TT_Tn.Clear();
  Tn_VN.Clear();
  VN_TB.Clear();
  TB_NMNV.Clear();
  NMNV_Pie.Clear();
  Pie_VK.Clear();
  VK_Le.Clear();
  Le_Bre.Clear();
  Bre_Tr.Clear();
  Tr_Ci.Clear();
  Ci_Se.Clear();
  Se_VSJ.Clear();
  VSJ_Vi.Clear();
  Vi_Ba.Clear();

  Sturovo_NoveZamky.Clear();
  NoveZamky_Bratislava .Clear();

  Ky.Clear();
  Ma.Clear();
  SNV.Clear();
  Pp.Clear();
  St.Clear();
  LH.Clear();
  LM.Clear();
  Rk.Clear();
  Kr.Clear();
  Vr.Clear();
  Zi.Clear();
  PB.Clear();
  Pu.Clear();
  TT.Clear();
  Tn.Clear();
  NMNV.Clear();
  Pie.Clear();
  Le.Clear();
  Tr.Clear();
  Vi.Clear();
  Ba.Clear();

  Sturovo.Clear();
  NoveZamky.Clear();

  (new GeneratorKe)->Activate(14880);
  (new GeneratorHungary)->Activate(31560  );
  Run();
  HungaryTrainTime.Output();
  trainTime.Output();
  }
  myfile <<timeOfDelay[1];
  myfile <<"\t";
  myfile <<propability/(numberOfIterations*6);
  myfile <<"\n";
}
