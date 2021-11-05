const byte red1= 6;
const byte red2= 8;
const byte red3= 10;
const byte red4= 12;
int t= 1000; // sets the speed of lighting up
void setup() {
  // put your setup code here, to run once:
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);   //setting up output
  pinMode(red3, OUTPUT);
  pinMode(red4, OUTPUT);
}

void loop() {
  for(int i=6; i<= 12; i=i+2) {
    digitalWrite(i,HIGH);  //forward sequence of lighting
    delay(t);
    digitalWrite(i,LOW);
  }
  delay(t);
  
  for(int i=12; i>= 6; i=i-2) {
    digitalWrite(i,HIGH);  //backward sequence of lighting
    delay(t);
    digitalWrite(i,LOW);
  }
  delay(t);
}
