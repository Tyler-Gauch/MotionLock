int debug = D7;
int trigPin = D5;
int echoPin = D6;
int servoPin = A7;

int patternLength = 6;
int numberOfBlocks = 0;
int pattern[6] = {0,0,0,1,1,1};
int testPattern[6] = {-1,-1,-1,-1,-1,-1};
bool reading = false;
int currentIndex = 0;

void setup()
{
  pinMode(debug, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);

  if(inches < 5)
  {
    numberOfBlocks++;

    if(!reading)
    {
      if(numberOfBlocks == 15)
      {
        blinkLed(128,128,0,5);
      }
      else if(numberOfBlocks == 20)
      {
        blinkLed(0,128,128,5);
      }
      else if(numberOfBlocks > 20)
      {
        blinkLed(255,0,0,5);
        numberOfBlocks = 0;
      }
    }
  }
  else
  {
    if(reading)
    {
      if(currentIndex >= patternLength)
      {
        blinkLed(128,128,0,5);
        numberOfBlocks = 0;
        reading = false;
        currentIndex = 0;
        bool correct = true;
        printPattern(testPattern);
        printPattern(pattern);
        for(int i = 0; i < patternLength; i++)
        {
          if(pattern[i] != testPattern[i])
          {
            correct = false;
            break;
          }
        }
        if(correct)
        {
            blinkLed(0,255,0,20);
            unlock();
        }
        else
        {
            blinkLed(255,0,0,20);
            lock();
        }
        currentIndex = 0;
      }
      else if(numberOfBlocks > 0 && numberOfBlocks < 5)
      {
        testPattern[currentIndex] = 0;
        blinkLed(0,0,255,1);
        numberOfBlocks = 0;
        currentIndex++;
      }
      else if(numberOfBlocks > 5 && numberOfBlocks < 15)
      {
        testPattern[currentIndex] = 1;
        blinkLed(0,255,0,1);
        numberOfBlocks = 0;
        currentIndex++;
      }
    }
    else{
      if(numberOfBlocks == 15)
      {
        numberOfBlocks = 0;
        reading = true;
        currentIndex = 0;
      }
      else if(numberOfBlocks == 20)
      {
        numberOfBlocks = 0;
        lock();
      }
    }
  }
  delay(100);
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

void blinkLed(int r, int g, int b, int flashes)
{
  RGB.control(true);
  for(int i = 0; i < flashes; i++)
  {
    RGB.color(r,g,b);
    delay(100);
    RGB.color(0,0,0);
    delay(100);
  }
  RGB.control(false);
}

void printPattern(int * p)
{
  for(int i = 0; i < patternLength; i++)
  {
    Serial.print(p[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void lock()
{
  analogWrite(servoPin, 180);
  delay(5000);
  analogWrite(servoPin, 200);
}

void unlock()
{
  analogWrite(servoPin, 240);
  delay(5000);
  analogWrite(servoPin, 200);
}
