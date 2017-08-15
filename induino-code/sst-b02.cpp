#include<LiquidCrystal.h>

LiquidCrystal lcd(8,9,10,11,12,13);

int R=5;    // REED SWITCH
int P=7;    // POWER --not needed
int X=6;    // PIR
int T=A3;     // TEMPERATURE - A2 has already been defined as Analog Pin 2 in
int H=A5;   //HUMIDITY
int L=A4;   //LIGHT

int M1=3;   //MOTOR PIN #1
int M2=4;   //MOTOR PIN #2

int BUZ=2;

int ctr=0;

char buffer[10];

float CELSIUS, HUM;
int LIGHT;

String sensordata [6];
void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);

  pinMode(P, INPUT);
  pinMode(R, INPUT);
  pinMode(X, INPUT);
  pinMode(T, INPUT);
  pinMode(H, INPUT);
  pinMode(L, INPUT);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(BUZ, OUTPUT);

  digitalWrite(BUZ, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);

  displayName();
}

// A Function to locate a given search string in a given base string
boolean find_string(String base, String search)
{
   int len = search.length(); // find the length of the base string
   for(int m = 0; m<((base.length()-len)+1);m++)// Iterate from the beginning of the base string till the end minus length of the substring
   {
   if(base.substring(m,(m+len))==search) // Check if the extracted Substring Matches the Search String
   {
   return true; // if it matches exit the function with a true value
 }
}
   return false; // if the above loop did not find any matches, control would come here and return a false value
 }

  // A Function to locate a given search character in a given base string and return its position
 boolean find_char_loc(String base, char search)
 {
    for(int m = 0; m < base.length();m++)// Iterate from the beginning of the base string till the end minus length of the substring
    {
      if(base[m]==search) // Check if the character Matches the Search character
      {
        return m; // if it matches exit the function with the current location value
      }
    }
  }

//////////***TEMPERATURE***///////////
void TEMPERATURE()
{
  const int len1 = 5; // no of times for the loop to run
  float samples[len1], tempc=0.0, tempf = 0.0;
  Serial.print("Getting Temp\t");
  lcd.clear();
  // for taking mean value of len1 readings
  for(int i = 0;i<len1;i++){
    samples[i] = ( 5.0 * analogRead(T) * 100.0) / 1024.0;
    tempc = tempc + samples[i];
    delay(2000/len1);
  }
  CELSIUS=tempc/len1; // setting mean value here
  tempf = (CELSIUS * 9)/ 5 + 32; // converting to farheniet (if needed)
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(CELSIUS);
  Serial.print("T:\t");
  Serial.print(CELSIUS);
  Serial.print("\t");

  // dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
  // converting float to string and setting in sensordata array
  sensordata[0] = dtostrf(CELSIUS, 5, 2, buffer);

}
//////////***HUMIDITY***///////////
void HUMIDITY()
{
  Serial.print("Getting Hum ");
  float valb = analogRead(H); // humidity calculation
  delay(10);
  float prehum = (valb/5);
  float humconst = (0.16/0.0062);
  float humi = prehum - humconst;
  float pretruehumconst = 0.00216*CELSIUS;
  float pretruehum = 1.0546-pretruehumconst;
  float truehum = humi/pretruehum ;
  HUM = truehum;
  lcd.setCursor(9,0);
  lcd.print("H:");
  lcd.print(HUM);
  Serial.print("H: ");
  Serial.print(HUM); Serial.print("\t");
  //sensordata[1] = String(HUM);
  sensordata[1] = dtostrf(HUM, 5, 2, buffer);
}
//////////***LIGHT***///////////
void LIG()
{
  Serial.print("Getting Lig ");
  int value_lig=analogRead(L);
  delay(10);
  LIGHT=value_lig;
  lcd.setCursor(0,1);
  lcd.print("L:");
  lcd.print(LIGHT);
  Serial.print("L: ");
  Serial.print(LIGHT);Serial.println("\t");
  //sensordata[2] = String(LIGHT);
  sensordata[2] = String(LIGHT);
  delay(2000);
  lcd.clear();
}
//////////***POWER***///////////
void POWER()
{
  Serial.print("PWR STATUS\t");
  /*if(digitalRead(P)==LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("P:OFF");
    Serial.print("P:OFF");Serial.print("\t");
    sensordata[3] = "OFF";

  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("P:ON");
    Serial.print("P:ON");Serial.print("\t");
    sensordata[3] = "ON";
  }*/
    lcd.setCursor(0,0);
    lcd.print("P:ON");
    Serial.print("P:ON");Serial.print("\t");
    sensordata[3] = "ON";
}
//////////***REED SWITCH***///////////
void REED()
{
  Serial.print("        REED STATUS ");
  if(digitalRead(R)==LOW)
  {
    lcd.setCursor(6,0);
    lcd.print("R:OPEN");
    Serial.print("R:OPEN");Serial.print("\t");
    sensordata[4] = "OPEN";

    lcd.clear();
    lcd.print("WARNING!!");
    lcd.setCursor(0,1);
    lcd.print("DOOR BREACHED!!!");
    delay(200);
    digitalWrite(BUZ,HIGH);
    delay(10000);
    lcd.clear();
    digitalWrite(BUZ,LOW);
  }
  else
  {
    lcd.setCursor(6,0);
    lcd.print("R:CLOSE");
    Serial.print("R:CLOSE");Serial.print("\t");
    sensordata[4] = "CLOSE";
  }
}
//////////***PIR SENSOR***///////////
void PIR()
{
  Serial.print("PIR STATUS ");
  if(digitalRead(X)==HIGH)
  {
    lcd.setCursor(0,1);
    lcd.print("X:YES");
    Serial.print("X:YES");Serial.println("\t");
    sensordata[5] = "YES";

    digitalWrite(M1,HIGH);
    for(int k=0; k<3; k++) {
      lcd.clear();
      lcd.print("WARNING!!");
      lcd.setCursor(0,1);
      lcd.print("MOTION");
      delay(1000);
      lcd.clear();
      lcd.print("WARNING!!");
      lcd.setCursor(0,1);
      lcd.print("DETECTED!!");
      delay(1000);
    }
    lcd.clear();
    digitalWrite(M1,LOW);
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("X:NO ");
    Serial.print("X:NO");Serial.println("\t");
    sensordata[5] = "NO";
  }
  delay(2000);
}

/////***LCD TEST FUNCTION***////////
void lcd_test(int msg_no)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEST MSG ");
  lcd.print(msg_no);
  delay(1000);
  lcd.clear();
}

void lcd_test(String msg)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEST MSG ");
  lcd.print(msg);
  delay(1000);
  lcd.clear();
}

//////////***MAIN LOOP***///////////
void loop()
{
  //lcd_test(0);
  TEMPERATURE();
  HUMIDITY();
  LIG();
  POWER();
  REED();
  PIR();
  Serial.println();

  if(ctr == 5){
    printResult();
    displayName();
    ctr = 0;
  }
  ctr++;
}


void displayName() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sidhi's ");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("SMART TRACKER");
  delay(1000);
  lcd.clear();
}


void printResult() {
  Serial.println("");
  Serial.print("RESULT ");
  Serial.print("T:"); Serial.print(sensordata[0]); Serial.print(",");
  Serial.print("H:"); Serial.print(sensordata[1]); Serial.print(",");
  Serial.print("L:"); Serial.print(sensordata[2]); Serial.print(",");
  Serial.print("P:"); Serial.print(sensordata[3]); Serial.print(",");
  Serial.print("R:"); Serial.print(sensordata[4]); Serial.print(",");
  Serial.print("X:"); Serial.print(sensordata[5]); Serial.println("END");

}
