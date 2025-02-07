#include <Javino.h>         //https://github.com/chon-group/javino2arduino
#include <LiquidCrystal.h>  //https://www.arduino.cc/reference/en/libraries/liquidcrystal/

// When using a physical arduino boar with the LCD Keypad Shield, uncomment the following:
  #define PinLCDCtrl01 4
  #define PinLCDCtrl02 5
  #define PinLCDCtrl03 6
  #define PinLCDCtrl04 7
  #define PinLCDRS     8
  #define PinLCDEN     9
  #define PinLCDLight  10
  #define LedBlue       3  
  #define LedRed       11
  #define LedYellow    12
  #define LedGreen     13


Javino javino;
LiquidCrystal lcd(PinLCDRS, PinLCDEN, PinLCDCtrl01, PinLCDCtrl02, PinLCDCtrl03, PinLCDCtrl04);
unsigned long lastClick = millis();
String strBtnPressed = "";
String lastLCDMsg = "";
int option = 0;
int spo2=98;

void serialEvent(){
  javino.readSerial();
}

void setup() {
 pinMode(LedBlue,OUTPUT); 
 pinMode(LedRed,OUTPUT);
 pinMode(LedGreen,OUTPUT);
 pinMode(LedYellow,OUTPUT);
 startLCDWithKeyboard();
 javino.start(9600);
}
 
void loop() {
 readLCDKeyboard();
 if(javino.availableMsg()){
  if(javino.requestPercepts()) getExogenousPerceptions();
  else if(javino.requestAction("blueOn")) led("blueOn");
  else if(javino.requestAction("redOn")) led("redOn");
  else if(javino.requestAction("yellowOn")) led("yellowOn");
  else if(javino.requestAction("greenOn")) led("greenOn");
  else if(javino.requestAction("blueOff")) led("blueOff");
  else if(javino.requestAction("redOff")) led("redOff");
  else if(javino.requestAction("yellowOff")) led("yellowOff");
  else if(javino.requestAction("greenOff")) led("greenOff");  
  else information(javino.getMsg());
 }
}

void information(String info){
    lastLCDMsg = "";
    lcd.clear();
    writeInLCD(info);
}

void getExogenousPerceptions(){
    javino.addPercept("device(arduinoWithLCDKeypadShield)");
    javino.addPercept("spo2Rate("+String(spo2)+")");
    javino.sendPercepts();
}

void startLCDWithKeyboard(){
  pinMode(PinLCDLight, OUTPUT);
  digitalWrite(PinLCDLight, HIGH);
  lcd.begin(16, 2);
  printScreen();
 }

void readLCDKeyboard(){ 
  strBtnPressed = getBtnPressed();
  if((strBtnPressed != "none" ) & (millis()>(lastClick+500))){

      lastClick = millis();

      if(strBtnPressed == "select") option=0;
      else if(strBtnPressed == "down") option++;
      else if(strBtnPressed == "up") option--;
      printScreen();
      
  }
  delay(50);
}


void printScreen(){
  if(option<=0){
    writeInLCD("Oximeter");
    writeInLCD(" ");
    option=0;
  } else if(option==1){
    static int lastSpo2 = -1;  // último valor exibido de spo2

    if(strBtnPressed=="right" && spo2 < 99) spo2++;
    if(strBtnPressed=="left" && spo2 > 0) spo2--;

    if(spo2 != lastSpo2) {  
      lcd.clear();
      writeInLCD("Saturation Rate");
      writeInLCD(String(spo2) + "%");
      lastSpo2 = spo2;  // Atualiza o último valor
    }
  }
}



String getBtnPressed(){
    int input = analogRead(A0);
    if     (input < 50) { return "right";}
    else if(input < 150){ return "up";}
    else if(input < 350){ return "down";}
    else if(input < 550){ return "left";}
    else if(input < 750){ return "select";}
    else return "none"; 
}


void writeInLCD(String strL){
  if (strL.startsWith("\"") && strL.endsWith("\"")) {
    strL.remove(0, 1);
    strL.remove(strL.length() - 1); 
  }

  int lengthStr = strL.length();
  for (int startIndex = 0; startIndex < lengthStr; startIndex += 16) {
    int endIndex = startIndex + 15;
    if (endIndex >= lengthStr) {
      endIndex = lengthStr - 1;
    }
    String chunk = strL.substring(startIndex, endIndex + 1);
    digitalWrite(PinLCDLight, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lastLCDMsg);
    lcd.setCursor(0,1);
    lcd.print(chunk);
    lastLCDMsg = chunk;
  }
}

void led(String op){
  if(op=="greenOff"){
    digitalWrite(LedGreen,LOW);
  }else if(op=="yellowOff"){
    digitalWrite(LedYellow,LOW);
  }else if(op=="redOff"){
    digitalWrite(LedRed,LOW);  
  }else if(op=="blueOff"){
    digitalWrite(LedBlue,LOW);
  }else if(op=="greenOn"){
    digitalWrite(LedGreen,HIGH);
  }else if(op=="yellowOn"){
    digitalWrite(LedYellow,HIGH);
  }else if(op=="redOn"){
    digitalWrite(LedRed,HIGH);
  }else if(op=="blueOn"){
    digitalWrite(LedBlue,HIGH);
  }
}
