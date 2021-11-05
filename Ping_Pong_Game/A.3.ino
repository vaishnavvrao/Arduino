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
int t= 500; // sets the speed of lighting up
int timeout=1000; //decides the timeout for a missed shot
int dt1r, dt2r, dt1l, dt2l; //dynamic time variables to decide hit or miss
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

void forward() {                  //forward sequence of lighting
  for(int i=6; i<= 12; i=i+2) {
       digitalWrite(i,HIGH);  
       delay(t);
       digitalWrite(i,LOW);
     }
}

void backward() {                 //backward sequence of lighting
  for(int i=12; i>= 6; i=i-2) {
        digitalWrite(i,HIGH);  
        delay(t);
        digitalWrite(i,LOW);
      }  
}

void leftwin() {
  for(int j=0; j<4; j++) {        //Indicates left player has won
    digitalWrite(ledPinL, HIGH);
    delay(200);  
    digitalWrite(ledPinL, LOW);
    delay(200);
   }
  startflag=HIGH;     //resetting game variables
  dt2r=dt1r;
  dt2l=dt1l;        
  flagL=flagR=LOW;
}

void rightwin() {
  for(int j=0; j<4; j++) {        //Indicates right player has won
     digitalWrite(ledPinR, HIGH);
     delay(200);  
     digitalWrite(ledPinR, LOW);
     delay(200);
   }
  startflag=HIGH;     //resetting game variables
  dt2r=dt1r;
  dt2l=dt1l;
  flagL=flagR=LOW;
}

void loop() {

  if( ((dt2l-dt1l)> 0 && (dt2l-dt1l)<timeout) || (startflag && flagL) ) {  
    //enters the if either left player's shot is within timeout if left button is pressed at the beginning of game
    
    flagL=LOW;  //resets left button flag
    startflag=LOW; //sets to LOW to indicate game is in progress
    digitalWrite(ledPinL, HIGH); //lights up left LED momentarily to indicate successful hit
    delay(300);
    digitalWrite(ledPinL, LOW);

    forward();  // ball moves left to right
    if(flagR) {flagR=LOW;}  // if right button has been pressed during flight time of ball, it gets reset
    dt1r=millis();  
    delay(t); // buffer time for right player to press button
    
    if(flagR){dt2r=millis(); flagR=LOW;}  //if right player has pressed the button, notes down time
    else{dt2r=dt1r;}   //else sets the times equal to ensure that return of ball does not occur
  }

  else if(!startflag) {rightwin();} //right player wins if left player's shot is unsuccessful

  if( ((dt2r-dt1r)> 0 && (dt2r-dt1r)<timeout) ) { 
    // enters if right player's shot is within timeout
    
    flagR=LOW; //resets right button flag
    digitalWrite(ledPinR, HIGH); //lights up right LED momentarily to indicate successful hit
    delay(300);
    digitalWrite(ledPinR, LOW);

    backward(); //ball moves to right to left
    if(flagL) {flagL=LOW;}  //if left button has been pressed during flight time of ball, it gets reset
    dt1l=millis();
    delay(t);   //buffer time for left player to press the button
    
    if(flagL){dt2l=millis(); flagL=LOW;}  //if left player has pressed the button, notes down time
    else {dt2l=dt1l;}    //else sets the times equal to ensure that return of ball does not occur 
  }

  else if(!startflag) {leftwin();}  //left player wins if right player's shot is unsuccessful
  
}

void isr_L() {
  flagL= HIGH; //interrupt routine for left button press
}

void isr_R() {
  flagR= HIGH; //interrupt routine for right buton press
}
