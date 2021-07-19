#define MOTOR_IN1 8
#define MOTOR_IN2 9
#define MOTOR_EN  10

#define FWD 'F'
#define BWD 'B'

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);

  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_EN, HIGH);
}

void loop() {
  if(Serial2.available())
  {
    char wmove = Serial2.read();
    //Serial.println(wmove);
    switch(wmove)
    {
      case FWD:
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        delay(50);
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
      break;

      case BWD:
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        delay(50);
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
      break;

      default: Serial.println(wmove);
    }
  }
}
