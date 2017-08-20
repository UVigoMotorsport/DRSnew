#include <Servo.h>
#define ABSMAX 2500
#define ABSMIN 500

#define BRAKE 3
#define DRS 2
#define STEERING A5
#define MINPRESS 50

#define UPRIGHT 12
#define UPLEFT 11
#define DOWNRIGHT A0
#define DOWNLEFT A1

#define MIN_RIGHT 910
#define MIN_LEFT 2000

#define MAX_RIGHT 2000
#define MAX_LEFT 910

Servo UpRight;
Servo UpLeft;
Servo DownRight;
Servo DownLeft;

#define CLOSE_UpRight 2050
#define OPEN_UpRight 1560

#define CLOSE_UpLeft 970
#define OPEN_UpLeft  1440


#define CLOSE_DownRight 1070
#define OPEN_DownRight 1450

#define CLOSE_DownLeft 1920
#define OPEN_DownLeft 1550

int debouncedrs = 0;
unsigned int debouncetimedrs = 0;
int debouncebrake = 0;
unsigned int debouncetimebrake = 0;
int releasebut = 0;
int skipone = 0;

#define CLOSED 1
#define OPEN 2

char DRS_STATE = CLOSED;

void setup()
{
  pinMode(DRS, INPUT_PULLUP);
  pinMode(BRAKE, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  UpRight.attach(UPRIGHT);
  UpLeft.attach(UPLEFT);
  DownRight.attach(DOWNRIGHT);
  DownLeft.attach(DOWNLEFT);
  UpRight.writeMicroseconds (CLOSE_UpRight);
  UpLeft.writeMicroseconds (CLOSE_UpLeft);
  DownRight.writeMicroseconds (CLOSE_DownRight);
  DownLeft.writeMicroseconds (CLOSE_DownLeft);
  Serial.begin(9600);

  delay(50);
  int rightinc = 0;
  int leftinc = 0;
  if (digitalRead(DRS) == 0)
  {
    skipone = 1;
  }
  while (digitalRead(DRS) == 1 && !skipone)
  {
    UpRight.writeMicroseconds (MAX_RIGHT + rightinc);
    UpLeft.writeMicroseconds (MAX_LEFT  + leftinc);
    DownRight.writeMicroseconds (MAX_LEFT + leftinc);
    DownLeft.writeMicroseconds (MAX_RIGHT  + rightinc);
    rightinc -= 10;
    leftinc += 10;
    delay(30);
    if (MAX_RIGHT + rightinc <= MIN_RIGHT || MAX_LEFT + leftinc >= MIN_LEFT)
    {
      break;
    }
  }
  if (digitalRead(DRS) == 0)
  {
    skipone = 1;
  }
  delay(50);
  rightinc = 0;
  leftinc = 0;
  while (digitalRead(DRS) == 1 && !skipone)
  {
    UpRight.writeMicroseconds (MIN_RIGHT + rightinc);
    UpLeft.writeMicroseconds (MIN_LEFT  + leftinc);
    DownRight.writeMicroseconds (MIN_LEFT + leftinc);
    DownLeft.writeMicroseconds (MIN_RIGHT  + rightinc);
    rightinc += 10;
    leftinc -= 10;
    delay(30);
    if (MIN_RIGHT + rightinc >= MAX_RIGHT || MIN_LEFT + leftinc <= MAX_LEFT)
    {
      break;
    }
  }
  delay(100);
}

void loop()
{
  if (DRS_STATE == CLOSED)
  {
    digitalWrite(LED_BUILTIN, 0);
    UpRight.writeMicroseconds (CLOSE_UpRight);
    UpLeft.writeMicroseconds (CLOSE_UpLeft);
    DownRight.writeMicroseconds (CLOSE_DownRight);
    DownLeft.writeMicroseconds (CLOSE_DownLeft);
  }
  else if (DRS_STATE == OPEN)
  {
    digitalWrite(LED_BUILTIN, 1);
    UpRight.writeMicroseconds (OPEN_UpRight);
    UpLeft.writeMicroseconds (OPEN_UpLeft);
    DownRight.writeMicroseconds (OPEN_DownRight);
    DownLeft.writeMicroseconds (OPEN_DownLeft);
    delay(100);
    UpRight.writeMicroseconds (0);
    UpLeft.writeMicroseconds (0);
    DownRight.writeMicroseconds (0);
    DownLeft.writeMicroseconds (0);
  }

  if (digitalRead(DRS) == 0)
  {
    if (debouncedrs == 0)
    {
      debouncedrs = 1;
      debouncetimedrs = millis();
    }
    else if (debouncedrs == 1 && (millis() - debouncetimedrs) > MINPRESS && releasebut == 0)
    {
      if (DRS_STATE == CLOSED)
      {
        DRS_STATE = OPEN;
      }
      else
      {
        DRS_STATE = CLOSED;
      }
      debouncedrs = 0;
      releasebut = 1;
    }
  }
  else
  {
    releasebut = 0;
    debouncedrs = 0;
  }

  if (digitalRead(BRAKE) == 0)
  {
    if (debouncebrake == 0)
    {
      debouncebrake = 1;
      debouncetimebrake = millis();
    }
    else if (debouncebrake == 1 && (millis() - debouncetimebrake) > MINPRESS)
    {
      debouncebrake = 0;
      Serial.println("Brakeclose");
      DRS_STATE = CLOSED;
    }
  }
  else
  {
    debouncebrake = 0;
  }
}
