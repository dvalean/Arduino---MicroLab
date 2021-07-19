// <--------------- MANUAL MOVEMENT --------------->
// Defining motors' pins.
#define MOVE_LEFTMOTOR_IN1  2
#define MOVE_LEFTMOTOR_IN2  3
#define MOVE_LEFTMOTOR_EN   4

#define MOVE_RIGHTMOTOR_IN1 5
#define MOVE_RIGHTMOTOR_IN2 6
#define MOVE_RIGHTMOTOR_EN  7

#define MOVE_STEERWHEEL_IN1 8
#define MOVE_STEERWHEEL_IN2 9
#define MOVE_STEERWHEEL_EN  10

// Movement input from the phone app.
#define CMD_FWD   'F'
#define CMD_BWD   'B'
#define CMD_RIGHT 'R'
#define CMD_LEFT  'L'

void Forward(void) {
  digitalWrite(MOVE_LEFTMOTOR_IN1, HIGH);
  digitalWrite(MOVE_LEFTMOTOR_IN2, LOW);
  digitalWrite(MOVE_RIGHTMOTOR_IN1, HIGH);
  digitalWrite(MOVE_RIGHTMOTOR_IN2, LOW);
}

void Backward(void) {
  digitalWrite(MOVE_LEFTMOTOR_IN1, LOW);
  digitalWrite(MOVE_LEFTMOTOR_IN2, HIGH);
  digitalWrite(MOVE_RIGHTMOTOR_IN1, LOW);
  digitalWrite(MOVE_RIGHTMOTOR_IN2, HIGH);
}

void Right(void) {
  digitalWrite(MOVE_STEERWHEEL_IN1, LOW);
  digitalWrite(MOVE_STEERWHEEL_IN2, HIGH);
}

void Left(void) {
  digitalWrite(MOVE_STEERWHEEL_IN1, HIGH);
  digitalWrite(MOVE_STEERWHEEL_IN2, LOW);
}

void MovementSet(void) {
  pinMode(MOVE_LEFTMOTOR_IN1,  OUTPUT);
  pinMode(MOVE_LEFTMOTOR_IN2,  OUTPUT);
  pinMode(MOVE_LEFTMOTOR_EN,   OUTPUT);
  pinMode(MOVE_RIGHTMOTOR_IN1, OUTPUT);
  pinMode(MOVE_RIGHTMOTOR_IN2, OUTPUT);
  pinMode(MOVE_RIGHTMOTOR_EN,  OUTPUT);
  pinMode(MOVE_STEERWHEEL_IN1, OUTPUT);
  pinMode(MOVE_STEERWHEEL_IN2, OUTPUT);
  pinMode(MOVE_STEERWHEEL_EN,  OUTPUT);

  digitalWrite(MOVE_LEFTMOTOR_IN1,  LOW);
  digitalWrite(MOVE_LEFTMOTOR_IN2,  LOW);
  digitalWrite(MOVE_LEFTMOTOR_EN,   HIGH);
  digitalWrite(MOVE_RIGHTMOTOR_IN1, LOW);
  digitalWrite(MOVE_RIGHTMOTOR_IN2, LOW);
  digitalWrite(MOVE_RIGHTMOTOR_EN,  HIGH);
  digitalWrite(MOVE_STEERWHEEL_IN1, LOW);
  digitalWrite(MOVE_STEERWHEEL_IN2, LOW);
  digitalWrite(MOVE_STEERWHEEL_EN,  HIGH);
}

void Movement(void) {
  if (Serial2.available())
  {
    char car_move = Serial2.read();

    switch (car_move)
    {
      case CMD_FWD:
        Forward();
        break;

      case CMD_BWD:
        Backward();        
        break;

      case CMD_RIGHT:
        Right();
        break;

      case CMD_LEFT:
        Left();
        break;

      case 'S':
        digitalWrite(MOVE_LEFTMOTOR_IN1, LOW);
        digitalWrite(MOVE_LEFTMOTOR_IN2, LOW);
        digitalWrite(MOVE_RIGHTMOTOR_IN1, LOW);
        digitalWrite(MOVE_RIGHTMOTOR_IN2, LOW);
        digitalWrite(MOVE_STEERWHEEL_IN1, LOW);
        digitalWrite(MOVE_STEERWHEEL_IN2, LOW);
        break;

      default: Serial.print(car_move);
    }
  }
}

// <--------------- END MANUAL MOVEMENT --------------->

// <---------------

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // Bluetooth driver port
  Serial2.begin(9600); // App port

  MovementSet();
}

void loop() {
  Movement();
}
