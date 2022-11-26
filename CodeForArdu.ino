
//Описываем порты
#define Led1 2
#define ZoomPin 3
#define Led2 4
#define Led3 7
#define Led4 8
#define RedPin 9
#define GreenPin 10
#define BluePin 11
#define IrPin 12

//Описываем Кнопки пульта
#define Ir1 3125149440
#define Ir2 3108437760
#define Ir3 3091726080
#define Ir4 3141861120
#define Ir5 3208707840
#define Ir6 3158572800
#define Ir7 4161273600
#define Ir8 3927310080
#define Ir9 4127850240
#define Ir0 3860463360
#define IrStar 3910598400
#define IrResh 4061003520
#define IrUp 3927310080
#define IrDown 4127850240
#define IrRight 3860463360
#define IrLeft 3910598400
#define IrCent 4061003520

//Описываем задержки
#define EXTRA_INFO_DELAY 3000
#define BASE_INFO_DELAY 3000


#include <IRremote.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimerMs.h>
#include <GyverOS.h>

LiquidCrystal_I2C lcd(0x38, 20, 4);
IRrecv irrecv(IrPin);
decode_results results;

/*
LED 1 - железо 
LED 2 - медь
LED 3 - Нефть
LED 4 - уголь
*/

 int LedColor[4][3] = 
{
  {255, 255, 255},
  {0, 255, 0},
  {255, 0, 0},
  {0, 0, 255}
};

int ExtraInfo[4][4] =
{
  {120, -1, 1440,2160},
  {320, -1, 960, 1080},
  {540, -1, 780, 7850},
  {100, 990, 340, -1}
};

void setup() {
  Serial.begin(9600);
  lcd.init();
  pinMode(ZoomPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(IrPin, INPUT);
  lcd.backlight();
    
}

void loop() {
  /*static int PlantChosen = 1;
  static int ReqestedPlant = 1;
  static uint32_t timer1;
  static uint32_t timer2;
  if(millis() - timer1 >= EXTRA_INFO_DELAY || PlantChosen != ReqestedPlant){
    timer1 = millis(); 
    ReqestedPlant = PlantChosen;
    for(int i = 1; i < 5; i++){
      if (ExtraInfo[ReqestedPlant-1][i - 1] != -1){
        ExtraInfo[ReqestedPlant-1][i - 1] = ReadExtraData(ReqestedPlant, i); 
      }
    }
  }
  if(millis() - timer1 >= BASE_INFO_DELAY){
    timer2 = millis();
    for (int i = 1; i < 5; i++){
      SetStatus(i, ReadBaseData(i));
    }
  }
  Zummer();*/
  DynInd();

  /*for (int i = 1; i < 5; i++){
    PrintInfo(i);
    delay(7000);
    lcd.clear();
  }*/
   
}

void Zummer(){
  if(LedColor[1][0] == 255 || LedColor[1][1] == 255 || LedColor[1][2] == 255){tone(ZoomPin, 4500);}
  if(LedColor[0][0] == 255 && LedColor[0][1] == 255 && LedColor[0][2] == 255) {noTone(ZoomPin);}

}
void SetStatus(int LedNum, int Status){
    switch (Status){
      case 1:  
        LedColor[LedNum - 1][0] = 0;
        LedColor[LedNum - 1][1]= 255;
        LedColor[LedNum - 1][2] = 0;
        Serial.print("LED");
        Serial.print(LedNum);
        Serial.println(" Setted GREEN");
        break;
      case 0:
        LedColor[LedNum - 1][0] = 255;
        LedColor[LedNum - 1][1] = 0;
        LedColor[LedNum - 1][2] = 0;
        Serial.print("LED");
        Serial.print(LedNum);
        Serial.println(" Setted RED");
        break;
    }
}


void DynInd(){
  int port[4] = {Led1, Led2, Led3, Led4};
  for (int Counter = 0; Counter < 4; Counter++) { // цикл со счетчиком
    // записываем значения цветов
    analogWrite(RedPin, LedColor[Counter][0]);
    analogWrite(GreenPin, LedColor[Counter][1]);
    analogWrite(BluePin, LedColor[Counter][2]);

    // подаем на нужный светодиод землю (включаем его)
    digitalWrite(port[Counter], LOW);

    delay(1);
    
    // подаем на все порты 5V, чтобы выключить все светодиоды.
    digitalWrite(port[0], HIGH);
    digitalWrite(port[1], HIGH);
    digitalWrite(port[2], HIGH);
    digitalWrite(port[3], HIGH);
  }
}


int ReadBaseData(int LedNum){
    String AnswerStr = "";

    Serial.print(1);
    Serial.println(LedNum);
    while (Serial.available() == 0) {}
    AnswerStr = Serial.readString();
    AnswerStr.trim();
    return AnswerStr.toInt();
}

int ReadExtraData(int PlantNum, int DataType){
    String AnswerStr = "";

    Serial.print(2);
    Serial.print(PlantNum);
    Serial.println(DataType);
    while (Serial.available() == 0) {}
    AnswerStr = Serial.readString();
    AnswerStr.trim();
    return AnswerStr.toInt();
}

int PlantChoosing(){
  if (irrecv.decode()) {
    switch (irrecv.decodedIRData.decodedRawData){
      case Ir1: irrecv.resume(); Serial.println(1); return 1; break;
      case Ir2: irrecv.resume(); Serial.println(2); return 2; break;
      case Ir3: irrecv.resume(); Serial.println(3); return 3; break;
      case Ir4: irrecv.resume(); Serial.println(4); return 4; break;
      
      
      default: 
      Serial.println(-1);
        return -1;
    }
  }
  else{
    return 0;
  }
}

void PrintInfo(int PlantNum){
  switch (PlantNum){
      case 1:  
        lcd.print("     IRON PLANT     ");
        lcd.print("Parts prod.:");
        lcd.print(ExtraInfo[PlantNum - 1][3]);
        lcd.print(" PPM");
        lcd.print("Energy cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][0]);
        lcd.print(" MWH");
        lcd.print("Parts cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][2]);
        lcd.print(" PPM");
        break;
      case 2:
        lcd.print("    COPPER PLANT    ");
        lcd.print("Parts prod.:");
        lcd.print(ExtraInfo[PlantNum - 1][3]);
        lcd.print(" PPM");
        lcd.print("Energy cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][0]);
        lcd.print(" MWH");
        lcd.print("Parts cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][2]);
        lcd.print(" PPM");
        break;
      case 3:
        lcd.print("     OIL  PLANT     ");
        lcd.print("Parts prod.:");
        lcd.print(ExtraInfo[PlantNum - 1][3]);
        lcd.print(" PPM");
        lcd.print("Energy cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][0]);
        lcd.print(" MWH");
        lcd.print("Parts cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][2]);
        lcd.print(" PPM");
        break;
      case 4:
        lcd.print("     COAL PLANT     ");
        lcd.print("Energy cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][0]);
        lcd.print(" MWH");
        lcd.print("Energy prod.:");
        lcd.print(ExtraInfo[PlantNum - 1][1]);
        lcd.print(" MWH");
        lcd.print("Parts cons.:");
        lcd.print(ExtraInfo[PlantNum - 1][2]);
        lcd.print(" PPM");
        break;
    }
}