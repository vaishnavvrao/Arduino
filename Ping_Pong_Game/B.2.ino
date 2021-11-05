const byte ledPinL = 4;
const byte ledPinR = 13;
const byte switchL = 2;
const byte switchR = 3;

const byte red1= 6;
const byte red2= 8;
const byte red3= 10;
const byte red4= 12;
int tspeed= 1000; // sets the speed of lighting up
int timeout= 1000; 

int t1, t2;  //to measure time differences

volatile byte flagL=LOW; //variables go high when buttons are pressed
volatile byte flagR=LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);   //setting up output
  pinMode(red3, OUTPUT);
  pinMode(red4, OUTPUT);

  pinMode(ledPinL, OUTPUT);  
  pinMode(ledPinR, OUTPUT);
  
  pinMode(switchL, INPUT_PULLUP);
  pinMode(switchR, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(switchL), isr_L, FALLING); //interrupt for left button press
  attachInterrupt(digitalPinToInterrupt(switchR), isr_R, FALLING); //interrupt for right button press
  Serial.begin(9600); //initialises serial monitor
}

void forward() {
  for(int i=6; i<= 12; i=i+2) {
    digitalWrite(i,HIGH);  //forward sequence of lighting
    delay(tspeed);
    digitalWrite(i,LOW);
  }
  t1= millis(); 
}

void backward() {
  for(int i=12; i>= 6; i=i-2) {
    digitalWrite(i,HIGH);  //backward sequence of lighting
    delay(tspeed);
    digitalWrite(i,LOW);
  }
  t1= millis(); 
}
void loop() {

  forward();  //ball moves left to right
  if(flagR) {flagR=LOW;}  //if right button has been pressed in between, reset it to not get negative times
  delay(timeout);  // waits for this much time for user to press right button
  
  if(flagR) {  //if right button has been pressed in the duration of timeout
    Serial.println(t2-t1);  //prints time difference between right button press and right LED going off
    flagR=LOW; //reset right button flag
  }
  
  backward();  //ball moves right to left
  if(flagL) {flagL=LOW;}  //if left button has been pressed in between, reset it to not get negative times
  delay(timeout); // waits for this much time for user to press left button

  if(flagL) {  //if left button has been pressed in the duration of timeout
    Serial.println(t2-t1);  //prints time difference between left button press and left LED going off
    flagL=LOW;  //reset left button flag
  }
  
}

void isr_L() {
  flagL= HIGH; //interrupt routine for left button press
  t2=millis();  // notes time of left button press
}

void isr_R() {
  flagR= HIGH; //interrupt routine for right buton press
  t2=millis();  //notes time of right button press
}
