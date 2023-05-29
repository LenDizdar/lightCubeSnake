 void setup()
 {
    pinMode(0, OUTPUT);
    pinMode(1,INPUT_PULLUP);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    randomSeed(analogRead(4));
 }

 void(* resetFunc) (void) = 0; //declare reset function @ address 0

boolean inputable = true;
int xAxis = 0;
int yAxis = 0;
int pos[] = {0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int posLength = 11;
int applePos[] = {2,2,2};
int newPos[] = {0,0,0};
boolean changed = true;
int resetCounter = 200;
int pin = 0;
boolean toggled = false;
int roller = 0;
int lastMoved = -1;
//NOTICE! LIGHTS[Z][X][Y]
boolean lights[3][3][3] = {{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}}};
int counter = 0;
int headFlashCounter = 0;
const int flashRate = 700;
int tempSnake = 0;
boolean grown = 0;
int rewardSound = 0;
boolean startFlag = 0;
int crashBlocker = 0;

     void loop()
 {
    if (!startFlag) {
      randomApple();
      lights[applePos[2]][applePos[0]][applePos[1]] = 1;
      startFlag = 1;
    }
    //reading definitions
    xAxis = analogRead(A0);
    yAxis = analogRead(A1);

    //check if input, if so change position
    if (inputable) {
      if (xAxis > 600 && lastMoved != 1) {
        mover(0, 3, 0, 1,0);
      } else if (xAxis < 400 && lastMoved != 0) {
        mover(0,-1,2,-1,1);
      } else if (yAxis > 600 && lastMoved != 3) {
        mover(1,-1,2,-1,2);
      } else if (yAxis < 400 && lastMoved != 2) {
        mover(1,3,0,1,3);
      }
    } else if (!(xAxis < 400) && !(xAxis > 600) && !(yAxis < 400) && !(yAxis > 600)) {
      inputable = true;
    }
   
    //check input but z change
    if (digitalRead(1) == LOW) {
      if (!toggled) {
        toggled = true;
        mover(2,3,0,1,-1);
      }
    } else {
      toggled = false;
    }
   
    //if input happened, display it
    if (changed) {
      if (crashBlocker != 2) {
        crashBlocker ++;
      }
      lights[pos[2]][pos[0]][pos[1]] = 1;
      lights[applePos[2]][applePos[0]][applePos[1]] = 1;
      
      changed = false;
      //shiftback
      for (int i = (posLength+1)/3; i > 1; i--) {
        for (int j = 0; j < 3; j++) {
          pos[i*3-j-1] = pos[(i-1)*3-j-1];
        }
      }

      if (lights[newPos[2]][newPos[0]][newPos[1]] && !(newPos[0] == applePos[0] && newPos[1] == applePos[1] && newPos[2] == applePos[2]) && !(newPos[0] == pos[posLength-2] && newPos[1] == pos[posLength-1] && newPos[2] == pos[posLength]) && crashBlocker == 2) {
          for (int i = 0; i < 30; i++) {
          digitalWrite(0,HIGH);
          delay(i);
          digitalWrite(0,LOW);
          delay(i);
        }
         resetFunc();  //call reset
      }
      for (int i = 0; i < 3; i++) {
        pos[i] = newPos[i];
      }
     
      lights[pos[posLength]][pos[posLength-2]][pos[posLength-1]] = 0;
      lights[pos[2]][pos[0]][pos[1]] = 1;
     

     
    }

    if (pos[0] == applePos[0] && pos[1] == applePos[1] && pos[2] == applePos[2]) {
      posLength += 3;
      randomApple();
      lights[applePos[2]][applePos[0]][applePos[1]] = 1;
      rewardSound = 30;
      
    }

    headFlashCounter ++;
    if (headFlashCounter%(flashRate/2) == 0) {
      lights[applePos[2]][applePos[0]][applePos[1]] = !lights[applePos[2]][applePos[0]][applePos[1]];
    }
    if (headFlashCounter == flashRate) {
      lights[pos[2]][pos[0]][pos[1]] = !lights[pos[2]][pos[0]][pos[1]];
      headFlashCounter = 0;
    }
    
    //rolling display go so crazy
    counter ++;
    if (counter == 5) {
      counter = 0;
      for(int i = 2; i < 14; i++) {
        digitalWrite(i, LOW);
      }
      //write to all on positions
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if (lights[roller][i][j]) {
            digitalWrite(2+i+j*3, HIGH);  
          }  
        }
      }
      switch(roller) {
        case 0:
          digitalWrite(11, HIGH);
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          roller = 1;
        break;
        case 1:
          digitalWrite(11, HIGH);
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);    
          roller = 2;
        break;
        case 2:
          digitalWrite(11, LOW);
          digitalWrite(12, HIGH);
          digitalWrite(13, HIGH);
          roller = 0;
        break;
      }
      
    }

   if (rewardSound > 0) {
    digitalWrite(0, !digitalRead(0));
    delay(1);
    rewardSound --;
   }
   
 }
 
  void mover(int index, int over, int reset, int add, int directn) {
 
      newPos[index] += add;
      if (newPos[index] == over) {
        newPos[index] = reset;
      }
      inputable = false;
      changed = true;
      lastMoved = directn;
      randomSeed(counter);
 }

 void randomApple() {
  for (int i = 0; i < 3; i++) {
    applePos[i] = random(3);
  }
  if (lights[applePos[2]][applePos[0]][applePos[1]]) {
    randomApple();
  }
 }
