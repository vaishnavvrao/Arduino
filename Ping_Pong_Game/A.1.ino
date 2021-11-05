const byte ledPinL = 4;
const byte ledPinR = 13;
const byte switchL = 2;
const byte switchR = 3;

volatile byte flagL=LOW; //variables go high when buttons are pressed
volatile byte flagR=LOW;

int t1=millis(); //this is t=0
int t2;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPinL, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  
  pinMode(switchL, INPUT_PULLUP);
  pinMode(switchR, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(switchL), isr_L, FALLING); //interrupt for left button press
  attachInterrupt(digitalPinToInterrupt(switchR), isr_R, FALLING); //interrupt for right button press
  Serial.begin(9600); //initialises serial monitor

}

void loop() {
  if(flagL) {
    //t2=millis(); //time when left button is pressed
    Serial.println(t2-t1); //outputs time between left button press and t=0
    
    digitalWrite(ledPinL, HIGH); //lights up left LED momentarily if flagL=HIGH
    delay(300);
    digitalWrite(ledPinL, LOW); 
    flagL= LOW; //resets flagL
  }
  
  if(flagR) {
    //t2=millis(); //time when right button is pressed
    Serial.println(t2-t1);  //outputs time between right button press and t=0
    
    digitalWrite(ledPinR, HIGH); //lights up right LED momentarily if flagR=HIGH
    delay(300);
    digitalWrite(ledPinR, LOW);
    flagR= LOW; //resets flagR
  }
}

void isr_L() {
  flagL= HIGH; //interrupt routine for left button press
  t2=millis();
}

void isr_R() {
  flagR= HIGH; //interrupt routine for right buton press
  t2=millis();
}
