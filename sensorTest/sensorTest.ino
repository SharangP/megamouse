/*
IR test
 */

int irPin[3] = {1, 2, 3};
int i, j;

void setup() {
  Serial.begin(9600);
  Serial.println("Begin IR Testing");
  for(i = 0; i < 3; i++){
    pinMode(irPin[i], INPUT);
  }
}



void loop() {
  Serial.println("begin 3x ir testing");
  
  float readVal[3] = {0};
  int nSamples = 10;

  for(i = 0; i < nSamples; i++){
    for(j = 0; j < 3; j++){
      readVal[j] = readVal[j] + (float)(analogRead(irPin[j]))/10;
    }
    delay(64);
  }

  Serial.print("1: ");
  Serial.print(readVal[0]);
  Serial.print("2: ");
  Serial.print(readVal[1]);
  Serial.print("3: ");
  Serial.println(readVal[2]);
}


