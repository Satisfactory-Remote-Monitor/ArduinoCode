#define Led1 2
#define ZoomPin 3
#define Led2 4
#define Led3 7
#define Led4 8
#define RedPin 9
#define GreenPin 10
#define BluePin 11

/*
pin 2 - led 1
pin 4 - led 2
pin 7 - led 3
pin 8 - led 4
*/

 int LedColor[4][3] = 
{
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

void setup() {
  Serial.begin(9600);
  pinMode(ZoomPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);

}

void loop() {
  static int BaseDataTimer = 0;
  if (BaseDataTimer >= 100){
    for (int i = 1; i < 5; i++){
      SetStatus(i, ReadBaseData(i));
    }
  }
  DynInd();
  BaseDataTimer++;


}

void SetStatus(int LedNum, int Status){
    switch (Status){
      case 1:  
        LedColor[LedNum - 1][0] = 0;
        LedColor[LedNum - 1][1]= 255;
        LedColor[LedNum - 1][2] = 0;
        break;
      case 0:
        LedColor[LedNum - 1][0] = 255;
        LedColor[LedNum - 1][1] = 0;
        LedColor[LedNum - 1][2] = 0;
        break;
    }
}


void DynInd(){
  int port[4] = {Led1, Led2, Led3, Led4};
  for (int Counter = 0; Counter < 4; Counter++) { // цикл со счетчиком
    // записываем значения цветов
    analogWrite(RedPin, LedColor[Counter][0]);
    analogWrite(GreenPin, LedColor[Counter][0]);
    analogWrite(BluePin, LedColor[Counter][0]);

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
    Serial.print(LedNum);
    Serial.println(2);
    while (Serial.peek() == 50){
      Serial.println(3);
    }
}