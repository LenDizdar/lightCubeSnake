
 void setup()
 {
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
 } 

boolean inputable = true;
int xAxis = 0;
int yAxis = 0;
int pos[] = {0,0,0,1,0,0,2,0,0,2,1,0};
int newPos[] = {0,0,0};
boolean changed = true;
int resetCounter = 200;
int pin = 0;
boolean toggled = false;
int roller = 0;
//NOTICE! LIGHTS[Z][X][Y]
boolean lights[3][3][3] = {{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}}};
int counter = 0;
int tempSnake = 0;
     void loop()
 { 
    //reading definitions
    xAxis = analogRead(A0);
    yAxis = analogRead(A1);

    //check if input, if so change position
    if (inputable) {
      if (xAxis > 600) {
        newPos[0] ++;
        if (newPos[0] == 3) {
          newPos[0] = 0;
        }
        inputable = false;
        changed = true;
      } else if (xAxis < 400) {
        newPos[0] --;
        if (newPos[0] == -1) {
          newPos[0] = 2;
        }
        inputable = false;
        changed = true;
      } else if (yAxis > 600) {
        newPos[1] --;
        if (newPos[1] == -1) {
          newPos[1] = 2;
        }
        inputable = false;
        changed = true;
      } else if (yAxis < 400) {
        newPos[1] ++;
        if (newPos[1] == 3) {
          newPos[1] = 0;
        }
        changed = true;
        inputable = false;
      }
    } else if (!(xAxis < 400) && !(xAxis > 600) && !(yAxis < 400) && !(yAxis > 600)) {
      inputable = true;
    }
    //check input but z change
    if (digitalRead(1) == LOW) {
      if (!toggled) {
        toggled = true;
        newPos[2] ++;
        if (newPos[2] == 3) {
          newPos[2] = 0;
        }
        changed = true;
      }
    } else {
      toggled = false;
    }
    
    //if input happened, display it
    if (changed) {
      changed = false;
      //shiftback
      for (int i = 4; i > 1; i--) {
        for (int j = 0; j < 3; j++) {
          pos[i*3-j-1] = pos[(i-1)*3-j-1];
        }
      }
      
      for (int i = 0; i < 3; i++) {
        pos[i] = newPos[i];
      }
      
      lights[pos[11]][pos[9]][pos[10]] = 0;
      lights[pos[2]][pos[0]][pos[1]] = 1;

      
    }
    
    
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
    
    
 }
