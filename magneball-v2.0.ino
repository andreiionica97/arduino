#include "LedControl.h" //  need the library
#include <LiquidCrystal.h>
#include <EEPROM.h>

LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define LEFT 0
#define RIGHT 1
#define V0_PIN 9 //replace potentiometer

const int buttonPinRight = 8;
const int buttonPinLeft = 13;

unsigned long dificultyUpdate = 0; //for selectDificulty
unsigned long lastUpdate = 0; //for movement
unsigned long endUpdate = 0; //for final text

String clearLcd = "                "; //replace lcd.clear()

int dificultyDelay = 200; //for selectDificulty
int mode = 0;  //switch loop
int delayTime; //ball speed
int ballCol = 0; //column coordinate of the ball
int ballRow = 1; //row coordinate of the ball
int ballDirection = 1; //1 for right, 0 for left
int upDown = 0; //ball vertical direction
int collected = 0; //point collected
int pointCol = random(1, 6);
int pointRow = random(2, 6);
int obstacleLeft = random(1, 6);
int obstacleRight = random(1, 6);
int score;
int obstacles; //on/off for obstacles - depends on dificulty
int dificulty; //level of dificulty
int stopText = 0; //stops overwriting the same text on lcd
int stopRepeat = 0; //stops unnecesary execution of some draw functions - in loop
int loadDelay = 3000; //final messages delay
int increaseSpeed = 1; //divides score to see if speed must pe raised
int highscoreAddress = 0; //eeprom highscore address

//resets global vars after hitting play again
void setResetVars() {
  delayTime = 400;
  ballCol = 0;
  ballRow = 1;
  ballDirection = 1;
  upDown = 0;
  lastUpdate = 0;
  collected = 0;
  pointCol = random(1, 6);
  pointRow = random(2, 6);
  obstacleLeft = random(1, 6);
  obstacleRight = random(1, 6);
  score = 0;
  stopText = 0;
}

//draws the 2 horizontal lines - up and down
void electromagnets () {
  for (int i = 0; i < 8; i++) {
    lc.setLed(0, 0, i, true);
    lc.setLed(0, 7, i, true);
  }  
}

void moveRight() {
  ballRow -= upDown;
  lc.setLed(0, ballRow + upDown, ballCol - 1, false);
  lc.setLed(0, ballRow + upDown, ballCol + 1, false);
  lc.setLed(0, ballRow + upDown, ballCol, false);
  endGame();
  lc.setLed(0, ballRow, ballCol, true);
  if (ballRow == pointRow && ballCol == pointCol || ballRow == pointRow - 1 && ballCol == pointCol) {
    collected = 1;
  }
  ballCol++; 
}

void moveLeft() {
  ballRow -= upDown;
  lc.setLed(0, ballRow + upDown, ballCol + 1, false);
  lc.setLed(0, ballRow + upDown, ballCol - 1, false);
  lc.setLed(0, ballRow + upDown, ballCol, false);
  endGame();
  lc.setLed(0, ballRow, ballCol, true);
  if (ballRow == pointRow && ballCol == pointCol || ballRow == pointRow - 1 && ballCol == pointCol) {
    collected = 1;
  }
  ballCol--;
}

void getDirection() {
  if (ballCol == 7) {
    ballDirection = LEFT;
  }
  if (ballCol == 0) {
    ballDirection = RIGHT;
  }
}

void endGame() {
  if (ballRow == 0 || ballRow == 7) {
    mode++;  
  }
  if (ballRow == obstacleLeft && ballCol == 0 && dificulty != 0 || ballRow == obstacleRight && ballCol == 7 && dificulty != 0) {
    mode++;
  }
}

//generates random bonus points
void bonusPoint() {
  int tempPointCol = pointCol;
  int tempPointRow = pointRow;
  if (collected == 0) {
    lc.setLed(0, pointRow, pointCol, true);
    lc.setLed(0, pointRow - 1, pointCol, true);
  } else {
    score += 10;
    lc.setLed(0, pointRow, pointCol, false);
    lc.setLed(0, pointRow - 1, pointCol, false);
    while(tempPointCol == pointCol && tempPointRow == pointRow) { //stops generating 2 consecutive points in the same spot
      pointCol = random(2, 6);
      pointRow = random(2, 6);
    }
    collected = 0;
  }
}

void printScoreAndDificulty() {
  lcd.setCursor(7, 0);
  lcd.print(score);
  lcd.setCursor(5, 1);
  lcd.print(dificulty);
  lcd.setCursor(12, 1);
  lcd.print(delayTime);
}

//generates random obstacles on the left and right vertical lines
void drawObstacle() {
  if (ballCol == 7 && ballDirection == RIGHT) {
    lc.setLed(0, obstacleLeft, 0, false);
    obstacleLeft = random(1, 6);
  }
  lc.setLed(0, obstacleLeft, 0, true);
  if (ballCol == 0 && ballDirection == LEFT) {
    lc.setLed(0, obstacleRight, 7, false);
    obstacleRight = random(1, 6);
  }
  lc.setLed(0, obstacleRight, 7, true);
}

//controls the movement of the ball - calls some functions: chesck direction, increase speed, draws obstacles, score, move right - left
void movement() {
  if (score / (increaseSpeed * 100) >= 1 && delayTime > 150) {
    delayTime -= 25;
    increaseSpeed++;
  }
  if (obstacles == 1) {
    drawObstacle(); 
  }
  getDirection();
  bonusPoint();
  if (digitalRead(buttonPinRight) == HIGH) {
   upDown = 1;  
  } else {
    upDown = -1;
  }
  if ((millis() - lastUpdate) >= delayTime) {
    lastUpdate = millis();
    if (ballDirection == RIGHT) {
      moveRight();
      score += dificulty;
      score++;
    } else {
      moveLeft();
      score += dificulty;
      score++;
    }
    upDown = -1;
    printScoreAndDificulty();
  }
}

void drawX() {
  lc.clearDisplay(0);
  lc.setLed(0, 1, 0, true);
  lc.setLed(0, 0, 1, true);
  lc.setLed(0, 0, 6, true);
  lc.setLed(0, 1, 7, true);
  lc.setLed(0, 6, 0, true);
  lc.setLed(0, 7, 1, true);
  lc.setLed(0, 7, 6, true);
  lc.setLed(0, 6, 7, true);
  for (int i = 0; i < 8; i++) {
    lc.setLed(0, i, i, true);
  }
  int j = 7;
  for (int i = 0; i < 8; i++) {
    lc.setLed(0, i, j, true);
    j--;
  }
}

void drawS() {
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 1, 3, true);
  lc.setLed(0, 1, 4, true);
  lc.setLed(0, 1, 5, true);
  lc.setLed(0, 3, 2, true);
  lc.setLed(0, 3, 3, true);
  lc.setLed(0, 3, 4, true);
  lc.setLed(0, 3, 5, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 3, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 4, 5, true);
}

void setDificulty() {
  homemadeClearLcd();
  lcd.setCursor(0, 0);
  lcd.print("Select dificulty!");
  lcd.setCursor(0, 1);
  lcd.print("LB-Slct/RB-change");
  lc.setLed(0, 4, 0, true);
  lc.setLed(0, 4, 2, true);
  lc.setLed(0, 4, 4, true);
  lc.setLed(0, 4, 6, true);
  lc.setLed(0, 3, 2, true);
  lc.setLed(0, 3, 4, true);
  lc.setLed(0, 3, 6, true);
  lc.setLed(0, 2, 4, true);
  lc.setLed(0, 2, 6, true);
  lc.setLed(0, 1, 6, true);
  int i = 0;
  lc.setLed(0, 6, i, true);
  //improvised debounce :)
  while (digitalRead(buttonPinLeft) != HIGH) {
    if ((millis() - dificultyUpdate) >= dificultyDelay) {
      lc.setLed(0, 6, i, false);
      dificultyUpdate = millis();
      if (digitalRead(buttonPinRight) == HIGH) {
        i += 2; //moves the point for selecting the dificulty
      }
      if(i == 8) {
        i = 0;
      }
      lc.setLed(0, 6, i, true);
    }
  }
  //after selecting the dificulty the parameters are set
  switch (i){
    case 0:
      obstacles = 0;
      delayTime = 550;
      dificulty = 0;
    break;
    case 2:
      obstacles = 1;
      delayTime = 550;
      dificulty = 1;
    break;
    case 4:
      obstacles = 1;
      delayTime = 400;
      dificulty = 2;
    break;
    case 6:
      obstacles = 1;
      delayTime = 250;
      dificulty = 3;
    break;
  }
  mode++;
  lc.clearDisplay(0);
  electromagnets ();
}

//after a mistake shows game over
void endText() {
  int artificialDelay = 2;
  homemadeClearLcd();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER");
  lcd.setCursor(2, 1);
  lcd.print("PLEASE WAIT");
  while (artificialDelay > 0) {
    if ((millis() - endUpdate) >= loadDelay) {
      endUpdate = millis();
      artificialDelay--;
    }
  }
}

//function for reading a long number from eeprom (not mine) - https://playground.arduino.cc/Code/EEPROMReadWriteLong
long eepromReadlong(long address) {
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

//function for reading a long number in eeprom (not mine) - https://playground.arduino.cc/Code/EEPROMReadWriteLong
void eepromWritelong(int address, long value) {
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

//if the show score button is selected, also saves the highscore if that's the case
void showScore() {
  if (score > (eepromReadlong(highscoreAddress))) {
    eepromWritelong(highscoreAddress, score);
  }
  homemadeClearLcd();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.setCursor(7, 0);
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("HScore: ");
  lcd.setCursor(8, 1);
  lcd.print(eepromReadlong(highscoreAddress));
}

//i had some problems with lcd.clear() so i've improvised this
void homemadeClearLcd() {
  lcd.setCursor(0, 0);
  lcd.print(clearLcd);
  lcd.setCursor(0, 1);
  lcd.print(clearLcd);
}

void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 0); // sets brightness (0~15 possible values)
  pinMode(buttonPinRight, INPUT);
  pinMode(buttonPinLeft, INPUT);
  pinMode(V0_PIN, OUTPUT);
  analogWrite(V0_PIN, 90); //instead of using a potentiometer
  //Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  switch (mode) {
    case 0: //start
    //stopRepeat is blocking the recall of the functions if the button is not pressed yet
    if(stopRepeat == 0) {
      setResetVars();
      homemadeClearLcd();
      lcd.setCursor(0, 0);
      lcd.print("Press right btn");
      lcd.setCursor(4, 1);
      lcd.print("to START");
      drawS();
      stopRepeat = 1;
    }
      if (digitalRead(buttonPinRight) == HIGH) {
        lc.clearDisplay(0);
        mode++;
        stopRepeat = 0;
      } 
    break;
    case 1: //dificulty selection
      setDificulty();
      homemadeClearLcd();
      lcd.setCursor(0, 0);
      lcd.print("Score: ");
      lcd.setCursor(7, 0);
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("Dif: ");
      lcd.setCursor(5, 1);
      lcd.print(dificulty);
      lcd.setCursor(7, 1);
      lcd.print("Spd: ");
      lcd.setCursor(12, 1);
      lcd.print(delayTime);
    break;
    case 2: //the game itself
        movement();
    break;
    case 3: //after a mistake is made it shows game over, after a delay you can select to see your score or you can play again
      if(stopText == 0) {
        drawX();
        endText();
        stopText = 1;
      }
      if(stopRepeat == 0) {
        homemadeClearLcd();
        lcd.setCursor(0, 0);
        lcd.print("LB - Score");
        lcd.setCursor(0, 1);
        lcd.print("RB - Play again");
        stopRepeat = 1;
      }
      if (digitalRead(buttonPinRight) == HIGH) {
        lc.clearDisplay(0);
        mode = 0;
        stopRepeat = 0;
      }
      if (digitalRead(buttonPinLeft) == HIGH) {
        showScore();
        int artificialDelay = 1;
        while (artificialDelay > 0) {
          if ((millis() - endUpdate) >= loadDelay) {
            endUpdate = millis();
            artificialDelay--;
          }
        }
        stopRepeat = 0;
      }
    break;
    }
}
