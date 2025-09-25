
#define LED_RED 12
#define LED_YELLOW 13 

void setup() {   
  // initialize inbuilt LED pin as an output.
  Serial.begin(115200);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT); 
  Serial.println("Wearable Alert System - Phase 2 Test");
  Serial.println("Send a number (1-4) via the Serial Monitor to test alerts.");
  Serial.println("1: Fire Alert");
  Serial.println("2: Siren Alert");
  Serial.println("3: Dog Bark Alert");
  Serial.println("4: All Off");
}

// loop function runs over and over  again forever
void loop() {
  if (Serial.available() > 0){
    char command = Serial.read();
    if(command == '1'){
      Serial.println("Exceuting FIRE alert");
      alert_fire(); 
    }
    else if ( command == '2' ){
      Serial.println(" Executing SIREN alert");
      alert_siren(); 
    }
    else if ( command == '3' ){
      Serial.println(" Executing DOG BARK alert");
      alert_dog(); 
    }
    else if ( command == '4' ){
      Serial.println(" Turning all alerts OFF ");
      all_off(); 
    }
  }
}


void alert_fire(){
  for (int i = 0; i<3 ; i++){
    digitalWrite(LED_RED, HIGH);
    delay(250);
    digitalWrite(LED_RED, LOW);
    delay(250); 
  }
}


void alert_siren(){
  for (int i = 0; i<3 ; i++){
    digitalWrite(LED_YELLOW, HIGH);
    delay(250);
    digitalWrite(LED_YELLOW, LOW);
    delay(250); 
  }
}

void alert_dog(){
  for (int i = 0; i<3 ; i++){
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, HIGH);    
    delay(250);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    delay(250);
  }
}

void all_off(){
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

