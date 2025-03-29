/* 
Circuit Breaker Box Puzzle

has 6 switches, sends a signal whenever any of them are flipped

signal is in the form "01101", where the ith character is the on/off state of the ith switch

the switches are ordered like:
---------
| 1   2 |
| 3   4 |
| 5   6 |
---------

*/ 


bool S1 = false;
bool S2 = false;
bool S3 = false;
bool S4 = false;
bool S5 = false;
bool S6 = false;

// pins:     3,  4,  5,  8,  9,  10
// switches: S2, S4, S6, S1, S3, S5

void setup() {
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);

  Serial.begin(9600);   // Initiate a serial communication
  while (!Serial); // wait for Serial to initiate
  Serial.println("initialized");
}

void loop() {
  // whenver anything changes from previous states: send ALL signal states
  if (S1 != digitalInput(8) || 
      S2 != digitalInput(3) || 
      S3 != digitalInput(9) || 
      S4 != digitalInput(4) || 
      S5 != digitalInput(10) || 
      S6 != digitalInput(5)) {
      
      // update all states
      S1 = digitalInput(8);
      S2 = digitalInput(3);
      S3 = digitalInput(9);
      S4 = digitalInput(4);
      S5 = digitalInput(10); 
      S6 = digitalInput(5);
      
      // send all states
      string content = "";
      content.concat(S1);
      content.concat(S2);
      content.concat(S3);
      content.concat(S4);
      content.concat(S5);
      content.concat(S6);
      Serial.println();
      delay(1000);
  }

}
