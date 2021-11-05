const byte ledPinL = 4;
const byte ledPinR = 13;
const byte switchL = 2;
const byte switchR = 3;

volatile byte flagL=LOW; //indicates left button press
volatile byte flagR=LOW; //indicates right button press

const byte red1= 6;
const byte red2= 8;
const byte red3= 10;
const byte red4= 12;
int tspeed= 1000; // sets the speed of lighting up
int timeout=1000; //decides the timeout for a missed shot
int dt1r, dt2r, dt1l, dt2l, dtr, dtl=0; //dynamic time variables to decide hit or miss
volatile byte startflag=HIGH;   //HIGH if game is not yet started; LOW if game is in progress

void setup() {
  // put your setup code here, to run once:
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);   //setting up output
  pinMode(red3, OUTPUT);
  pinMode(red4, OUTPUT);
  
  pinMode(ledPinL, OUTPUT);  //green LED pins
  pinMode(ledPinR, OUTPUT);
  
  pinMode(switchL, INPUT_PULLUP); //interrupt pins
  pinMode(switchR, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(switchL), isr_L, FALLING); //interrupt for left button press
  attachInterrupt(digitalPinToInterrupt(switchR), isr_R, FALLING); //interrupt for right button press
}

int speed_det(int dt) {    
  //function determines return speed
  float newspeed = float(tspeed) -(float(dt)*((float(tspeed)-100.0)/float(timeout))); 
  //pressing immediately sets dt between LEDs to tspeed
  //pressing after timeout sets dt between LEDs to 100ms (this is max speed of return)
  return(newspeed);
}

void forward(int dt) {                  
  //forward sequence of lighting
  int newspeed= speed_det(dt);    //calls the function to determine return speed
  // here float to int implicit type conversion has occured
  for(int i=6; i<= 12; i=i+2) {
       digitalWrite(i,HIGH);  
       delay(newspeed);
       digitalWrite(i,LOW);
     }
   dt1r=millis();
}

void backward(int dt) {                 
  //backward sequence of lighting
  int newspeed= speed_det(dt);     //calls the function to determine return speed
  // here float to int implicit type conversion has occured
  for(int i=12; i>= 6; i=i-2) {    
        digitalWrite(i,HIGH);  
        delay(newspeed);
        digitalWrite(i,LOW);
      }
   dt1l=millis();  
}

void leftwin() {
  //Indicates left player has won
  for(int j=0; j<4; j++) {        
    digitalWrite(ledPinL, HIGH);
    delay(200);  
    digitalWrite(ledPinL, LOW);
    delay(200);
   }
  //resetting game variables
  startflag=HIGH;     
  dt2r=dt1r;
  dt2l=dt1l;
  dtr=0;
  dtl=0;        
  flagL=flagR=LOW;
}

void rightwin() {
  //Indicates right player has won
  for(int j=0; j<4; j++) {        
     digitalWrite(ledPinR, HIGH);
     delay(200);  
     digitalWrite(ledPinR, LOW);
     delay(200);
   }
  //resetting game variables
  startflag=HIGH;     
  dt2r=dt1r;
  dt2l=dt1l;
  dtr=0;
  dtl=0;
  flagL=flagR=LOW;
}

void loop() {

  if( ((dtl)> 0 && (dtl)<timeout) || (startflag && flagL) ) {  
    //enters if either left player's shot is within timeout if left button is pressed at the beginning of game
    
    flagL=LOW;  //resets left button flag
    startflag=LOW; //sets to LOW to indicate game is in progress
    digitalWrite(ledPinL, HIGH); //lights up left LED momentarily to indicate successful hit
    delay(300);
    digitalWrite(ledPinL, LOW);

    forward(dtl);  // ball moves left to right
    if(flagR) {flagR=LOW;}  // if right button has been pressed during flight time of ball, it gets reset 
    delay(timeout); // buffer time for right player to press button
    
    if(flagR){dtr=dt2r-dt1r; flagR=LOW;}  //if right player has pressed the button within timeout, updates dtr
    else{dtr=0;}   //else sets dtr=0 to ensure that return of ball does not occur
  }

  else if(!startflag) {rightwin();} //right player wins if left player's shot is unsuccessful

  if( ((dtr)> 0 && (dtr)<timeout) ) { 
    // enters if right player's shot is within timeout
    
    flagR=LOW; //resets right button flag
    digitalWrite(ledPinR, HIGH); //lights up right LED momentarily to indicate successful hit
    delay(300);
    digitalWrite(ledPinR, LOW);

    backward(dtr); //ball moves to right to left
    if(flagL) {flagL=LOW;}  //if left button has been pressed during flight time of ball, it gets reset
    delay(timeout);   //buffer time for left player to press the button
    
    if(flagL){dtl=dt2l-dt1l; flagL=LOW;}  //if left player has pressed the button within timeout, updates dtl
    else {dtl=0;}    //else sets the dtl=0 to ensure that return of ball does not occur 
  }

  else if(!startflag) {leftwin();}  //left player wins if right player's shot is unsuccessful
  
}

void isr_L() {
  //interrupt routine for left button press
  flagL= HIGH; 
  dt2l=millis(); //notes time of left button press
}

void isr_R() {
  //interrupt routine for right buton press
  flagR= HIGH; 
  dt2r=millis();  //notes time of right button press
}
