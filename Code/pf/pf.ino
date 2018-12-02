#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>


//Sensors ADC Value
unsigned int s0 = 0;
unsigned int s1 = 0;
unsigned int s2 = 0;
unsigned int s3 = 0;
unsigned int s4 = 0;
//Threshlod of Sensors Input
unsigned int t0 = 0;
unsigned int t1 = 0;
unsigned int t2 = 0;
unsigned int t3 = 0;
unsigned int t4 = 0;
//Digital State of Sensors
bool b0 = 0;
bool b1 = 0;
bool b2 = 0;
bool b3 = 0;
bool b4 = 0;
//Real-time Values
int intrrupt_pin = 2;
volatile bool calibrate = 0;
//////////////////////////////////////////////////////////////////////////CONFIGURE MOTOR PINS//////////////////////////////////////////////////////////////////////////////////////////
//RIGHT MOTOR
int rightMotor_pos = 11;
int rightMotor_neg = 12;
int rightMotor_en  = 13;
//LEFT MOTOR
int leftMotor_pos = 10;
int leftMotor_neg = 9;
int leftMotor_en  = 8;
//Intrrupt Routin
ISR(INT0_vect){
  calibrate = 1;
}

void setup() {
  //Serial Config
  UCSR0B = 0;//Turn of Serial to use LEDs
  //Configure sensors monitoring PINs
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  //Intial Calibration
  calibration();
  //Interrupt Configuration
  PORTD |= (1 << PORTD2);//Set INT0 PIN In pull-up mode  
  EIMSK |= ( 1 << INT0);//enable intrrupt
  //Set Sense mode in Rising-Edge Mode
  EICRA |= ( 0 << ISC00);
  EICRA |= ( 1 << ISC01);
  //Configure Motors Ports
  configure_motor();
  start();
  
}

void loop() {
  if(!calibrate ){
    read_sensors();
    navigate();
  }else{
    calibrate_sens();
  }
  
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   SSSSSSSSSSSSSSS EEEEEEEEEEEEEEEEEEEEEENNNNNNNN        NNNNNNNN   SSSSSSSSSSSSSSS      OOOOOOOOO     RRRRRRRRRRRRRRRRR      SSSSSSSSSSSSSSS //
// SS:::::::::::::::SE::::::::::::::::::::EN:::::::N       N::::::N SS:::::::::::::::S   OO:::::::::OO   R::::::::::::::::R   SS:::::::::::::::S//
//S:::::SSSSSS::::::SE::::::::::::::::::::EN::::::::N      N::::::NS:::::SSSSSS::::::S OO:::::::::::::OO R::::::RRRRRR:::::R S:::::SSSSSS::::::S//
//S:::::S     SSSSSSSEE::::::EEEEEEEEE::::EN:::::::::N     N::::::NS:::::S     SSSSSSSO:::::::OOO:::::::ORR:::::R     R:::::RS:::::S     SSSSSSS//
//S:::::S              E:::::E       EEEEEEN::::::::::N    N::::::NS:::::S            O::::::O   O::::::O  R::::R     R:::::RS:::::S            //
//S:::::S              E:::::E             N:::::::::::N   N::::::NS:::::S            O:::::O     O:::::O  R::::R     R:::::RS:::::S            //
// S::::SSSS           E::::::EEEEEEEEEE   N:::::::N::::N  N::::::N S::::SSSS         O:::::O     O:::::O  R::::RRRRRR:::::R  S::::SSSS         //
//  SS::::::SSSSS      E:::::::::::::::E   N::::::N N::::N N::::::N  SS::::::SSSSS    O:::::O     O:::::O  R:::::::::::::RR    SS::::::SSSSS    //
//    SSS::::::::SS    E:::::::::::::::E   N::::::N  N::::N:::::::N    SSS::::::::SS  O:::::O     O:::::O  R::::RRRRRR:::::R     SSS::::::::SS  //
//       SSSSSS::::S   E::::::EEEEEEEEEE   N::::::N   N:::::::::::N       SSSSSS::::S O:::::O     O:::::O  R::::R     R:::::R       SSSSSS::::S //
//            S:::::S  E:::::E             N::::::N    N::::::::::N            S:::::SO:::::O     O:::::O  R::::R     R:::::R            S:::::S//
//            S:::::S  E:::::E       EEEEEEN::::::N     N:::::::::N            S:::::SO::::::O   O::::::O  R::::R     R:::::R            S:::::S//
//SSSSSSS     S:::::SEE::::::EEEEEEEE:::::EN::::::N      N::::::::NSSSSSSS     S:::::SO:::::::OOO:::::::ORR:::::R     R:::::RSSSSSSS     S:::::S//
//S::::::SSSSSS:::::SE::::::::::::::::::::EN::::::N       N:::::::NS::::::SSSSSS:::::S OO:::::::::::::OO R::::::R     R:::::RS::::::SSSSSS:::::S//
//S:::::::::::::::SS E::::::::::::::::::::EN::::::N        N::::::NS:::::::::::::::SS    OO:::::::::OO   R::::::R     R:::::RS:::::::::::::::SS //
// SSSSSSSSSSSSSSS   EEEEEEEEEEEEEEEEEEEEEENNNNNNNN         NNNNNNN SSSSSSSSSSSSSSS        OOOOOOOOO     RRRRRRRR     RRRRRRR SSSSSSSSSSSSSSS   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////READ SENSORS/////////////////////////////////////////////////////////////////////
void read_sensors(){ 
  s0 = analogRead(1);
  s1 = analogRead(2);
  s2 = analogRead(3);
  s3 = analogRead(4);
  s4 = analogRead(5);
  if(s0 >= t0){
    digitalWrite(0,HIGH);
    b0 = 1;
  }else{
    digitalWrite(0,LOW);
    b0 = 0;
  }
  if(s1 >= t1){
    digitalWrite(1,HIGH);
    b1 = 1;
  }else{
    digitalWrite(1,LOW);
    b1 = 0;
  }
  if(s2 >= t2){
    digitalWrite(4,HIGH);
    b2 = 1;
  }else{
    digitalWrite(4,LOW);
    b2 = 0;
  }
  if(s3 >= t3){
    digitalWrite(5,HIGH);
    b3 = 1;
  }else{
    digitalWrite(5,LOW);
    b3 = 0;
  }
  if(s4 >= t4){
    digitalWrite(6,HIGH);
    b4 = 1;
  }else{
    digitalWrite(6,LOW);
    b4 = 0;
  }
  
}
/////////////////////////////////////////////////////////////////CALIBRATION/////////////////////////////////////////////////////////////////////
void calibration(){
  t0 = analogRead(1)+100;
  t1 = analogRead(2)+100;
  t2 = analogRead(3)+100;
  t3 = analogRead(4)+100;
  t4 = analogRead(5)+100;
}

void calibrate_sens(){
  pause();
  digitalWrite(0,HIGH);
  digitalWrite(1,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  delay(500);
  calibration();
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  calibrate = 0;
  start();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MMMMMMMM               MMMMMMMM     OOOOOOOOO     TTTTTTTTTTTTTTTTTTTTTTT     OOOOOOOOO     RRRRRRRRRRRRRRRRR             OOOOOOOOO     PPPPPPPPPPPPPPPPP                     //
//M:::::::M             M:::::::M   OO:::::::::OO   T:::::::::::::::::::::T   OO:::::::::OO   R::::::::::::::::R          OO:::::::::OO   P::::::::::::::::P                    //
//M::::::::M           M::::::::M OO:::::::::::::OO T:::::::::::::::::::::T OO:::::::::::::OO R::::::RRRRRR:::::R       OO:::::::::::::OO P::::::PPPPPP:::::P                   //
//M:::::::::M         M:::::::::MO:::::::OOO:::::::OT:::::TT:::::::TT:::::TO:::::::OOO:::::::ORR:::::R     R:::::R     O:::::::OOO:::::::OPP:::::P     P:::::P                  //
//M::::::::::M       M::::::::::MO::::::O   O::::::OTTTTTT  T:::::T  TTTTTTO::::::O   O::::::O  R::::R     R:::::R     O::::::O   O::::::O  P::::P     P:::::P    ssssssssss    //
//M:::::::::::M     M:::::::::::MO:::::O     O:::::O        T:::::T        O:::::O     O:::::O  R::::R     R:::::R     O:::::O     O:::::O  P::::P     P:::::P  ss::::::::::s   //
//M:::::::M::::M   M::::M:::::::MO:::::O     O:::::O        T:::::T        O:::::O     O:::::O  R::::RRRRRR:::::R      O:::::O     O:::::O  P::::PPPPPP:::::P ss:::::::::::::s  //
//M::::::M M::::M M::::M M::::::MO:::::O     O:::::O        T:::::T        O:::::O     O:::::O  R:::::::::::::RR       O:::::O     O:::::O  P:::::::::::::PP  s::::::ssss:::::s //
//M::::::M  M::::M::::M  M::::::MO:::::O     O:::::O        T:::::T        O:::::O     O:::::O  R::::RRRRRR:::::R      O:::::O     O:::::O  P::::PPPPPPPPP     s:::::s  ssssss  //
//M::::::M   M:::::::M   M::::::MO:::::O     O:::::O        T:::::T        O:::::O     O:::::O  R::::R     R:::::R     O:::::O     O:::::O  P::::P               s::::::s       //
//M::::::M    M:::::M    M::::::MO:::::O     O:::::O        T:::::T        O:::::O     O:::::O  R::::R     R:::::R     O:::::O     O:::::O  P::::P                  s::::::s    //
//M::::::M     MMMMM     M::::::MO::::::O   O::::::O        T:::::T        O::::::O   O::::::O  R::::R     R:::::R     O::::::O   O::::::O  P::::P            ssssss   s:::::s  //
//M::::::M               M::::::MO:::::::OOO:::::::O      TT:::::::TT      O:::::::OOO:::::::ORR:::::R     R:::::R     O:::::::OOO:::::::OPP::::::PP          s:::::ssss::::::s //
//M::::::M               M::::::M OO:::::::::::::OO       T:::::::::T       OO:::::::::::::OO R::::::R     R:::::R      OO:::::::::::::OO P::::::::P          s::::::::::::::s  //
//M::::::M               M::::::M   OO:::::::::OO         T:::::::::T         OO:::::::::OO   R::::::R     R:::::R        OO:::::::::OO   P::::::::P           s:::::::::::ss   //
//MMMMMMMM               MMMMMMMM     OOOOOOOOO           TTTTTTTTTTT           OOOOOOOOO     RRRRRRRR     RRRRRRR          OOOOOOOOO     PPPPPPPPPP            sssssssssss     //                                                                                                                                          
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////MOTOR OPERATIOIN HANDLER/////////////////////////////////////////////////////////////////////////////////
void configure_motor(){
  //RIGTH MOTOR
  pinMode(rightMotor_pos,OUTPUT);
  pinMode(rightMotor_neg,OUTPUT);
  pinMode(rightMotor_en,OUTPUT);
  //LEFT MOTOR
  pinMode(leftMotor_pos,OUTPUT);
  pinMode(leftMotor_neg,OUTPUT);
  pinMode(leftMotor_en,OUTPUT);
}
void navigate(){
///////////////////////////////////
// ..............x.............. //  
// .......o.............o....... //  
// o...........................o //  
//#################################
  if(b2 == 1){
    go_straight();
  }
///////////////////////////////////
// ..............x.............. //  
// .......o.............o....... //  
// o...........................o //  
//#################################
  if(b1 == 1){
    go_left_pwm();
  }
///////////////////////////////////
// ..............x.............. //  
// .......o.............o....... //  
// o...........................o //  
//#################################
  if(b3 == 1){
    go_right_pwm();
  }
///////////////////////////////////
// ..............x.............. //  
// .......o.............o....... //  
// o...........................o //  
//#################################
  if(b0 == 1){
    go_left_off();
  }
///////////////////////////////////
// ..............x.............. //  
// .......o.............o....... //  
// o...........................o //  
//#################################
  if(b4 == 1){
    go_right_off();
  }
}

void start(){
  digitalWrite(rightMotor_en,HIGH);
  digitalWrite(leftMotor_en,HIGH);
}

void pause(){
  digitalWrite(rightMotor_en,LOW);
  digitalWrite(leftMotor_en,LOW);
}
/////////////////////////////////////////////////////////////////////////MOTOR CONTROL FUNCTIONS///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ..............x..............                                     PORT STATES                                   PWM                                                       //
//    .......o.............o.......    => go_straight()           RIGHT MOTOR => +:1    -:0                           255                                                       //
//    o...........................o                               LEFT  MOTOR => +:1    -:0                           255                                                       //
//################################################################################################################################################################################
//    ..............o..............                                     PORT STATES                                   PWM                                                       //
//    .......o.............x.......    => go_left_pwm()           RIGHT MOTOR => +:1    -:0                           255                                                       //
//    o...........................o                               LEFT  MOTOR => +:50%  -:0                           127                                                       //
//################################################################################################################################################################################
//    ..............o..............                                     PORT STATES                                   PWM                                                       //
//    .......o.............o.......    => go_left_off()           RIGHT MOTOR => +:1    -:0                           255                                                       //
//    o...........................x                               LEFT  MOTOR => +:0    -:0                           0                                                         //
//################################################################################################################################################################################
//    ..............o..............                                     PORT STATES                                   PWM                                                       //
//    .......x.............o.......    => go_right_pwm()          RIGHT MOTOR => +:50%  -:0                           127                                                       //
//    o...........................o                               LEFT  MOTOR => +:1    -:0                           255                                                       //
//################################################################################################################################################################################
//    ..............o..............                                     PORT STATES                                   PWM                                                       //
//    .......o.............o.......    => go_right_off()          RIGHT MOTOR => +:0    -:0                           0                                                         //
//    x...........................o                               LEFT  MOTOR => +:1    -:0                           255                                                       //
//################################################################################################################################################################################
void go_straight(){
    //RIGHT MOTOR
    analogWrite(rightMotor_pos,255);
    digitalWrite(rightMotor_neg,LOW);
    //LEFT MOTOR
    analogWrite(leftMotor_pos,255);
    digitalWrite(leftMotor_neg,LOW);
}

void go_left_pwm(){
    //RIGHT MOTOR
    analogWrite(rightMotor_pos,255);
    digitalWrite(rightMotor_neg,LOW);
    //LEFT MOTOR
    analogWrite(leftMotor_pos,127);
    digitalWrite(leftMotor_neg,LOW);
}

void go_left_off(){
    //RIGHT MOTOR
    analogWrite(rightMotor_pos,255);
    digitalWrite(rightMotor_neg,LOW);
    //LEFT MOTOR
    analogWrite(leftMotor_pos,0);
    digitalWrite(leftMotor_neg,LOW);
}

void go_right_pwm(){
    //RIGHT MOTOR
    analogWrite(rightMotor_pos,127);
    digitalWrite(rightMotor_neg,LOW);
    //LEFT MOTOR
    analogWrite(leftMotor_pos,255);
    digitalWrite(leftMotor_neg,LOW);
}

void go_right_off(){
    //RIGHT MOTOR
    analogWrite(rightMotor_pos,0);
    digitalWrite(rightMotor_neg,LOW);
    //LEFT MOTOR
    analogWrite(leftMotor_pos,255);
    digitalWrite(leftMotor_neg,LOW);
}










