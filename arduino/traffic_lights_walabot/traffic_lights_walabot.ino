/**************************************************************
 * This code controls a simple traffic light using an
 * Arduino MKR1000 through serial. More info:
 * https://www.hackster.io/Abysmal/traffic-control-with-walabot-6dfe7a
 * 
 * This project is made for "Walabot - Power to the Makers"
 * contest on hackster.io. More info:
 * https://www.hackster.io/contests/WalaBot
 * 
 * author: Balázs Simon
 *
 **************************************************************/

#define PED_GREEN   6
#define PED_RED     7
#define CAR_GREEN   8
#define CAR_YELLOW  9
#define CAR_RED     10

int pedLightState = 0;
int carLightState = 0;

int timePedGreen = 0;
int pedGreenOn = LOW;


void setup() {
  Serial.begin(115200);
  pinMode(PED_GREEN, OUTPUT);
  pinMode(PED_RED, OUTPUT);
  pinMode(CAR_GREEN, OUTPUT);
  pinMode(CAR_YELLOW, OUTPUT);
  pinMode(CAR_RED, OUTPUT);
}


void loop() {
  if (Serial.available() > 0) {
    char which = Serial.peek();
    /*
      What does these characters mean?
      s - getting status (Ready)
      p - setting pedestrian lights state
      c - setting car lights state
      d - sending car data
    */
    if (which == 'p') {
      Serial.read();
      pedLightState = Serial.parseInt();
    }
    else if (which == 'c') {
      carLightState = Serial.parseInt();
    }
    else if (which == 's') {
      Serial.println("Ready");
    }
    else if (which == 'd') {
      Serial.println(analogRead(A0) / 100);
    }

    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  updatePedLight(pedLightState);
  updateCarLight(carLightState);
}

void updatePedLight(int state) {

  switch (state) {
    case 0: //RED
      digitalWrite(PED_GREEN, LOW);
      digitalWrite(PED_RED, HIGH);
      break;
    case 1: //GREEN
      digitalWrite(PED_GREEN, HIGH);
      digitalWrite(PED_RED, LOW);
      break;
    case 2: //GREEN FLASHING
      if (millis() - timePedGreen > 500) {
        digitalWrite(PED_GREEN, pedGreenOn);
        pedGreenOn = ~pedGreenOn;
        timePedGreen = millis();
      }

      digitalWrite(PED_RED, LOW);
  }
}

void updateCarLight(int state) {

  switch (state) {
    case 0: //RED
      digitalWrite(CAR_GREEN, LOW);
      digitalWrite(CAR_YELLOW, LOW);
      digitalWrite(CAR_RED, HIGH);
      break;
    case 1: //RED + YELLOW
      digitalWrite(CAR_GREEN, LOW);
      digitalWrite(CAR_YELLOW, HIGH);
      digitalWrite(CAR_RED, HIGH);
      break;
    case 2: //GREEN
      digitalWrite(CAR_GREEN, HIGH);
      digitalWrite(CAR_YELLOW, LOW);
      digitalWrite(CAR_RED, LOW);
      break;
    case 3: //YELLOW
      digitalWrite(CAR_GREEN, LOW);
      digitalWrite(CAR_YELLOW, HIGH);
      digitalWrite(CAR_RED, LOW);
  }
}

