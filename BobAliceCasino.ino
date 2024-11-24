int casino[5] = { 0, 0, 0, 0, 0 };
int worstCasino[5] = { 0, 0, 0, 0, 0 };
int Bob[5] = { 0, 0, 0, 0, 0 };
char selInt[] = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' };
char selDisp[] = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' };
int AliceScore = 0;
int BobScore = 0;
int selNum = 0;
int count = 0;
int BobC = 0;
int CasC = 0;
const int SIZE = 5;
int BestScore = 0;
int WorstScore = 10;
int BestBob[5] = { 0, 0, 0, 0, 0 };
int WorstBob[5] = { 0, 0, 0, 0, 0 };


void decimalToBinary(int decimal, int binaryArray[]) {
  for (int i = 0; i < SIZE; i++) {
    // Get the least significant bit and store it in the array
    binaryArray[i] = decimal % 2;
    decimal /= 2;
  }
}

void lightleds(int lednum, int ledstate) {
  if (lednum <= 4) {
    digitalWrite(3, HIGH);
    switch (lednum) {
      case 1:
        digitalWrite(4, LOW);
        break;
      case 2:
        digitalWrite(5, LOW);
        break;
      case 3:
        digitalWrite(6, LOW);
        break;
      case 4:
        digitalWrite(7, LOW);
        break;
    }
  } else if (lednum > 4 && lednum <= 8) {
    digitalWrite(4, HIGH);
    switch (lednum) {
      case 5:
        digitalWrite(3, LOW);
        break;
      case 6:
        digitalWrite(5, LOW);
        break;
      case 7:
        digitalWrite(6, LOW);
        break;
      case 8:
        digitalWrite(7, LOW);
        break;
    }
  } else if (lednum > 8 && lednum <= 10) {
    digitalWrite(5, HIGH);
    switch (lednum) {
      case 9:
        digitalWrite(3, LOW);
        break;
      case 10:
        digitalWrite(4, LOW);
        break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT_PULLUP);  //Trigger
  pinMode(9, OUTPUT);        //Bob output
  pinMode(10, OUTPUT);       //Casino output
  pinMode(11, INPUT);        //Alice input
  pinMode(13, OUTPUT);       //pulse
  int randN = 0;
  for (int i = 0; i <= 9; i++) {
    randN = round(random(0, 100));
    if (randN > 50) {
      casino[i] = 1;
    } else {
      casino[i] = 0;
    }
    Serial.print(casino[i], DEC);
  }
  Serial.print("\n");
}

void loop() {
  if (digitalRead(8)) {
    //  Serial.print("Casino: \n");
    //  for (int i = 0; i <= 9; i++) {
    //    Serial.print(casino[i], DEC);
    //  }
    //  if (digitalRead(6)) {
    //    Serial.print("\n");
    //    Serial.print("Chn");
    //    Serial.print("\n");
    //    if (digitalRead(7)) {
    //      Bob[selNum] = 1;
    //    } else {
    //      Bob[selNum] = 0;
    //    }
    //    for (int i = 0; i <= 9; i++) {
    //      selDisp[i] = selInt[i];
    //    }
    //    selDisp[selNum] = '^';
    //    Serial.print("Bob: \n");
    //    for (int i = 0; i <= 9; i++) {
    //      Serial.print(Bob[i], BIN);
    //    }
    //    Serial.print("\n");
    //    for (int i = 0; i <= 9; i++) {
    //      Serial.print(selDisp[i]);
    //    }
    //    Serial.print("\n");
    //  } else {
    //    Serial.print("\n");
    //    Serial.print("sel");
    //    Serial.print("\n");
    //    selNum++;
    //    if (selNum > 9) {
    //      selNum = 0;
    //    }
    //    for (int i = 0; i <= 9; i++) {
    //      selDisp[i] = selInt[i];
    //    }
    //    selDisp[selNum] = '^';
    //    Serial.print("Bob: \n");
    //    for (int i = 0; i <= 9; i++) {
    //      Serial.print(Bob[i], DEC);
    //      Serial.print(",");
    //    }
    //    Serial.print("\n");
    //    for (int i = 0; i <= 9; i++) {
    //      Serial.print(selDisp[i]);
    //    }
    //    Serial.print("\n");
    //  }
    //
  } else {
    Serial.print("\n");
    Serial.print("solution");
    Serial.print("\n");
    for (BobC = 0; BobC < 32; BobC++) {
      decimalToBinary(BobC, Bob);
      for (CasC = 0; CasC < 32; CasC++) {
        decimalToBinary(CasC, casino);
        Serial.print("\n");
        for (int i = 0; i <= 5; i++) {
          digitalWrite(13, HIGH);
          if (i > 0) {
            if (Bob[i - 1] == 1) {
              digitalWrite(9, HIGH);
            } else {
              digitalWrite(9, LOW);
            }
            if (casino[i - 1] == 1) {
              digitalWrite(10, HIGH);
            } else {
              digitalWrite(10, LOW);
            }
            Serial.print(Bob[i-1]);
          }
          delay(1);
          digitalWrite(13, LOW);
          delay(1);
          if (i < 5) {
            if (digitalRead(11) == casino[i]) {
              AliceScore++;
            }
          }
          if (i > 0) {
            if (casino[i] == Bob[i]) {
              BobScore++;
            }
          }
          Serial.print(AliceScore);
        }
        Serial.print("\n");
        if (AliceScore >= 5 && BobScore >= 5) {
          Serial.print("\n YOU WIN! \n");
        } else {
          Serial.print("\n YOU LOSE! \n");
        }
        //if (AliceScore > BestScore) {
        //  BestScore = AliceScore;
        //  for (int i = 0; i < 10; i++) {
        //    BestBob[i] = Bob[i];
        //  }
        //}
        if (BobScore < WorstScore && (BobScore*2) >= 5 && (AliceScore*2) >=5) {
          WorstScore = BobScore;
          for (int i = 0; i < 5; i++) {
            WorstBob[i] = Bob[i];
            worstCasino[i] = casino[i];
          }
        }
        AliceScore = 0;
        BobScore = 0;
        //for (int j=0;j<10;j++){
        //  lightleds(j+1,Bob[j]);
        //}
      }
    }
    Serial.print("\n");
    for (int i = 0; i <= 4; i++) {
      Serial.print(WorstBob[i]);
      Serial.print(",");
    }
    Serial.print("\n");
    for (int i = 0; i <= 4; i++) {
      Serial.print(worstCasino[i]);
      Serial.print(",");
    }
    Serial.print("\n");
    Serial.print(WorstScore, DEC);
    Serial.print("\n");
  }
  delay(3000);
}
