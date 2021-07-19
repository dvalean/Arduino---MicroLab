/*
 Content:
  -> DEFINE
    -> MOTORS
    -> ULTRASONIC
    -> LIGHTS
  -> FUNCTIONS
    -> MOVE 
    -> MANUAL CONTROL
    -> DISTANCE MEASURE
    -> EMERGENCY STOP
    -> FOLLOW
      -> VEHICLE
      -> ROAD
      -> TUNNEL
  -> void setup()
    -> SERIAL
    -> MOVE PINS&INIT
    -> ULTRASONIC PINS&INIT
    -> LIGHTS PINS&INIT
  -> void loop()
*/

// <--------------- DEFINE --------------->
// <----- MOTORS ----->
#define LEFTMOTOR_IN1 2
#define LEFTMOTOR_IN2 3
#define LEFTMOTOR_EN  4

#define RIGHTMOTOR_IN1 5
#define RIGHTMOTOR_IN2 6
#define RIGHTMOTOR_EN  7

#define STEERINGWHEEL_IN1 8
#define STEERINGWHEEL_IN2 9
#define STEERINGWHEEL_EN  10

// <----- ULTRASONIC ----->
#define triggerFLL 30 // Front far left
#define echoFLL    31 
#define triggerFL  32 // Front center left
#define echoFL     33
#define triggerFR  34 // Front center right
#define echoFR     35
#define triggerFRR 36 // Frint far right
#define echoFRR    37

#define triggerBRR 38 // Back far right
#define echoBRR    39
#define triggerBR  40 // Back center right
#define echoBR     41
#define triggerBL  42 // Back center left
#define echoBL     43 
#define triggerBLL 44 // Back far left
#define echoBLL    45

int trigger[] = {triggerFLL, triggerFL, triggerFR, triggerFRR, triggerBRR, triggerBR, triggerBL, triggerBLL};
int echo[] = {echoFLL, echoFL, echoFR,echoFRR, echoBRR, echoBR, echoBL, echoBLL};

// <----- LIGHTS ----->
#define lightsLeft 23
#define lightsRight 22

// <--------------- FUNCTIONS --------------->
// <----- MOVE ----->
void LeftMotor(int i) // if 1 move forward, if -1 move backwards, else stop.
{
  if (i == 1)
  {
    digitalWrite(LEFTMOTOR_IN1, HIGH);
    digitalWrite(LEFTMOTOR_IN2, LOW);
  }
  else if (i == -1)
  {
    digitalWrite(LEFTMOTOR_IN1, LOW);
    digitalWrite(LEFTMOTOR_IN2, HIGH);
  }
  else
  {
    digitalWrite(LEFTMOTOR_IN1, LOW);
    digitalWrite(LEFTMOTOR_IN2, LOW);
  }
}

void RightMotor(int i)
{
  if (i == 1)
  {
    digitalWrite(RIGHTMOTOR_IN1, HIGH);
    digitalWrite(RIGHTMOTOR_IN2, LOW);
  }
  else if (i == -1)
  {
    digitalWrite(RIGHTMOTOR_IN1, LOW);
    digitalWrite(RIGHTMOTOR_IN2, HIGH);
  }
  else
  {
    digitalWrite(RIGHTMOTOR_IN1, LOW);
    digitalWrite(RIGHTMOTOR_IN2, LOW);
  }
}

void SteeringWheel(int i)
{
  if (i == 1)
  {
    digitalWrite(STEERINGWHEEL_IN1, HIGH);
    digitalWrite(STEERINGWHEEL_IN2, LOW);
  }
  else if (i == -1)
  {
    digitalWrite(STEERINGWHEEL_IN1, LOW);
    digitalWrite(STEERINGWHEEL_IN2, HIGH);
  }
  else
  {
    digitalWrite(STEERINGWHEEL_IN1, LOW);
    digitalWrite(STEERINGWHEEL_IN2, LOW);
  }
}

void Forward()
{
  LeftMotor(1);
  RightMotor(1);
}

void Backward()
{
  LeftMotor(-1);
  RightMotor(-1);
}

void Left()
{
  digitalWrite(lightsLeft, LOW);
  digitalWrite(lightsRight, HIGH);
  SteeringWheel(1); 
}

void Right()
{
  digitalWrite(lightsLeft, HIGH);
  digitalWrite(lightsRight, LOW);
  SteeringWheel(-1);
}

void Stop()
{
  digitalWrite(lightsRight, LOW);
  digitalWrite(lightsLeft, LOW);
  LeftMotor(0);
  RightMotor(0);
  SteeringWheel(0);
}

void ForwardLeft()
{
  Forward();
  Left();
}

void ForwardRight()
{
  Forward();
  Right();
}

void BackwardLeft()
{
  Backward();
  Left();
}

void BackwardRight()
{
  Backward();
  Right();
}

// <----- MANUAL CONTROL ----->
void ManualControl()
{  
  if (Serial2.available()) // Checks if it's connected to the remote
  {
    char car_move = Serial2.read();

    switch (car_move)
    {
      case 'F':
        Forward();
        Serial.print(car_move);
        break;

      case 'B':
        Backward();
        Serial.print(car_move);
        break;
        
      case 'L':
        Left();
        Serial.print(car_move);
        break;
        
      case 'R':
        Right();
        Serial.print(car_move);
        break;

      case 'S':
        Stop();
        Serial.print(car_move);
        break;

      case 'G':
        ForwardLeft();
        Serial.print(car_move);
        break;

      case 'I':
        ForwardRight();
        Serial.print(car_move);
        break;
        
      default: 
        Serial.print(car_move);
    }
  }
  return;
}

// <----- DISTANCE MEASURE ----->
int DistanceMeasure(int sensorTrigger, int sensorEcho)
{
  int distance;
  long duration;

  digitalWrite(sensorTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensorTrigger, LOW);

  duration = pulseIn(sensorEcho, HIGH);
  distance = duration * 0.034 / 2;
  
  return distance;
}

// <----- EMERGENCY STOP ----->
int stopDistance = 30; // Distance in mm.
void EmergencyStop()
{
  int distSensor1 = DistanceMeasure(triggerFL, echoFL);
  int distSensor2 = DistanceMeasure(triggerFR, echoFR);
  
  if (distSensor1 <= stopDistance + 10 || distSensor2 <= stopDistance + 10) // +10 - Error of measure.
    Stop();
  else
    Forward();
}

// <----- FOLLOW ----->
void FollowVehicle()
{
  int distSensor1 = DistanceMeasure(triggerFL, echoFL);
  int distSensor2 = DistanceMeasure(triggerFR, echoFR);

  if (distSensor1 <= stopDistance + 10 || distSensor2 <= stopDistance + 10)
    Stop();
  else if (abs(distSensor1 - distSensor2) <= 50 && (distSensor1 >= 500 && distSensor1 <= 1000))
    Forward();
  else if (distSensor1 < distSensor2)
    ForwardLeft();
  else if (distSensor2 < distSensor1)
    ForwardRight();
}

void FollowRoad()
{
  int distSensor1 = DistanceMeasure(triggerFL, echoFL);
  int distSensor2 = DistanceMeasure(triggerFR, echoFR);
  int distSensor0 = DistanceMeasure(triggerFLL, echoFLL);
  int distSensor3 = DistanceMeasure(triggerFRR, echoFRR);
  
  if (distSensor1 <= stopDistance + 5 || distSensor2 <= stopDistance + 10)
    Stop();
  else if ((distSensor0 + 10) < distSensor3)
    ForwardRight();
  else if (distSensor3 < (distSensor0 + 5))
    ForwardLeft();  
}

void FollowTunnel()
{
  int distSensor0 = DistanceMeasure(triggerFLL, echoFLL);
  int distSensor3 = DistanceMeasure(triggerFRR, echoFRR);

  if (abs(distSensor0 - distSensor3) <= 50)
    Forward();
  else if (distSensor0 < distSensor3)
    ForwardLeft();
  else if (distSensor3 < distSensor3)
    ForwardRight();  
}

void setup()
{
  // <----- SERIAL ----->
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  
  // <----- MOVE PINS&INIT ----->
  pinMode(LEFTMOTOR_IN1, OUTPUT);
  pinMode(LEFTMOTOR_IN2, OUTPUT);
  pinMode(LEFTMOTOR_EN, OUTPUT);

  pinMode(RIGHTMOTOR_IN1, OUTPUT);
  pinMode(RIGHTMOTOR_IN2, OUTPUT);
  pinMode(RIGHTMOTOR_EN, OUTPUT);

  pinMode(STEERINGWHEEL_IN1, OUTPUT);
  pinMode(STEERINGWHEEL_IN2, OUTPUT);
  pinMode(STEERINGWHEEL_EN, OUTPUT);

  digitalWrite(LEFTMOTOR_IN1, LOW);
  digitalWrite(LEFTMOTOR_IN2, LOW);
  digitalWrite(LEFTMOTOR_EN, HIGH);

  digitalWrite(RIGHTMOTOR_IN1, LOW);
  digitalWrite(RIGHTMOTOR_IN2, LOW);
  digitalWrite(RIGHTMOTOR_EN, HIGH);

  digitalWrite(STEERINGWHEEL_IN1, LOW);
  digitalWrite(STEERINGWHEEL_IN2, LOW);
  digitalWrite(STEERINGWHEEL_EN, HIGH);  

  // <----- ULTRASONIC PINS&INIT ----->
  for (int i = 0; i < 8; i++)
    pinMode(trigger[i], OUTPUT);

  for (int i = 0; i < 8; i++)
    pinMode(echo[i], INPUT);

  // <----- LIGHTS PINS&INIT ----->
  pinMode(lightsLeft, OUTPUT);
  pinMode(lightsRight, OUTPUT);
}

void loop()
{
  ManualControl();
  //EmergencyStop();
  //FollowVehicle();
  //FollowRoad();
  //FollowTunnel();
}
