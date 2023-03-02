#include <Wire.h>
#include <LiquidCrystal_I2C_Hangul.h>
#include <TinyGPS++.h>
LiquidCrystal_I2C_Hangul lcd(0x3F, 16, 2);

const int buzer = 25;
const int ledyellowpin = 15;
const int ledredpin = 13;
const int ledgreenpin = 12;
const int ledbluepin = 14;
const int irSensor1 = 26;
const int irSensor2 = 32;
const int irSensor3 = 35;
const int mq3 = 33;
const int motor1Pin1 = 2;
const int motor1Pin2 = 4;
const int motor2Pin1 = 18;
const int motor2Pin2 = 19;
boolean irstatus1;
boolean irstatus2;
boolean irstatus3;
float latt = 1.2365;
float longg = 2.6536;

TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
  delay(100);
  Serial.println("WELCOME");
  delay(100);
  pinMode(buzer, OUTPUT);
  pinMode(ledyellowpin, OUTPUT);
  pinMode(ledredpin, OUTPUT);
  pinMode(ledgreenpin, OUTPUT);
  pinMode(ledbluepin, OUTPUT);
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(irSensor3, INPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
  delay(100);
  buzzon();
  TestMessage();
}
void forward()
{
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  //delay(3000);
  //digitalWrite(motor1Pin1, LOW);
  //digitalWrite(motor1Pin2, LOW);
  //digitalWrite(motor2Pin1, LOW);
  //digitalWrite(motor2Pin2, LOW);
  //delay(1000);

}
void backward() {
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(3000);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);
}
void left()
{
  Serial.println("Moving left");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(5000);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);
}

void right()
{
  Serial.println("Moving right");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(5000);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);
}

void stop1()
{
  Serial.println("Stop");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);
}
void buzzonmedium()
{
  digitalWrite(buzer, HIGH);
  delay(3000);
  digitalWrite(buzer, LOW);
  delay(500);
}
void buzzonhigh()
{
  digitalWrite(buzer, HIGH);
  delay(5000);
  digitalWrite(buzer, LOW);
  delay(100);
}
void buzzon()
{
  digitalWrite(buzer, HIGH);
  delay(500);
  digitalWrite(buzer, LOW);
  delay(500);
}
void ledyellow()
{
  digitalWrite(ledyellowpin, HIGH);
  delay(500);
  digitalWrite(ledyellowpin, LOW);
  delay(500);
  digitalWrite(ledyellowpin, HIGH);
  delay(500);
  digitalWrite(ledyellowpin, LOW);
  delay(500);
  digitalWrite(ledyellowpin, HIGH);
  delay(500);
  digitalWrite(ledyellowpin, LOW);
}

void readir()
{
  irstatus1 = digitalRead(irSensor1);
  irstatus2 = digitalRead(irSensor2);
  irstatus3 = digitalRead(irSensor3);
  if (irstatus1 == LOW || irstatus2 == LOW)
  {
    stop1();
    Serial.println("Obstacle found");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Obstacle found");
    buzzon();
    ledyellow();
    delay(1000);
  }
  else
  {
    Serial.println("No obstacle");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("No obstacle");
    digitalWrite(ledgreenpin, HIGH);
    delay(500);
  }

}
void autoparkingmodemedium()
{
  left();
  forward();
  stop1();
  ledyellow();
}
void autoparkingmodehigh()
{
  left();
  forward();
  stop1();
  ledyellow();
}

void mq3read()
{
  float mq3val = analogRead(mq3);
  Serial.print("Alcohol Value: ");
  Serial.println(mq3val);
  // Firebase.setFloat("Gas", gasval);
  delay(500);
  if (mq3val > 1800)
  {
    Serial.print("alcohol is High ");//RED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("alcohol is High");
    delay(500);
    digitalWrite(ledredpin, HIGH);
    //Firebase.setString("alcohol", "High");
    stop1();
    TestMessage();
    buzzonhigh();
    autoparkingmodehigh();
    //buzzon();
  }
  else if (mq3val > 1000 && mq3val < 1800)
  {
    Serial.print("alcohol is Medium");//Yellow
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("alcohol - Medium");
    //Firebase.setString("alcohol", "High");
    stop1();
    buzzonmedium();
    delay(1000);
    digitalWrite(ledbluepin, HIGH);
    TestMessage2();
    autoparkingmodemedium();

  }
  else
  {
    Serial.print("alcohol not found");//Green
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No alcohol");
    //Firebase.setString("alcohol", "Optimum");
    forward();
    delay(500);
  }

}

void gpslocation()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
    {
      Serial.println("Displaying info!");
      displayInfo();
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    latt = (gps.location.lat(), 6);
    Serial.print(latt);

    //    Firebase.setFloat("Lattitude", gps.location.lat());
    Serial.print(F(","));
    longg = (gps.location.lng(), 6);
    Serial.println(longg);
    //Firebase.setFloat("Longitude", gps.location.lng());
  }
  else
  {
    Serial.print("Lattitude: ");
    Serial.print(latt);
    Serial.print("Longitude: ");
    Serial.println(longg);
  }
  Serial.println();
}

void TestMessage()
{
  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial2.println("AT+CMGS=\"+918310038503\"\r"); // Replace x with mobile number
  delay(1000);
  Serial2.println("alcohol detected high");// The SMS text you want to send
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Test MSG Sent");
  delay(100);
}
void TestMessage2()
{
  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial2.println("AT+CMGS=\"+918310038503\"\r"); // Replace x with mobile number
  delay(1000);
  Serial2.println("alcohol detected-medium");// The SMS text you want to send
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Test MSG Sent");
  delay(100);
}
void loop()
{
  readir();
  forward();
  mq3read();
  //gpslocation();
  
}
