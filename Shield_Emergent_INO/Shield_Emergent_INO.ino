// The shild sends into UART 115200 "FLT1" in the case of the fault on the power of the first camera
// and "FLT2" in the case of the power falt of the scond camera
// send "RES1", "RES2" or "RESB" to reset camera 1, camera 2 or both cameras respectively
String cmd, num_of_pins_str, CMDcur;
uint8_t EN1 = 27;
uint8_t EN2 = 6;
uint8_t FLT1 = 26;
uint8_t FLT2 = 7;
uint8_t faultChannel = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  attachInterrupt(digitalPinToInterrupt(FLT1), Fault1, LOW);
  attachInterrupt(digitalPinToInterrupt(FLT2), Fault2, LOW);
  pinMode(FLT1, INPUT_PULLUP);// 4 Rpi Pico pull_up must be after the attachinterrupt. It's a bug.
  pinMode(FLT2, INPUT_PULLUP);// 4 Rpi Pico pull_up must be after the attachinterrupt. It's a bug.
  Serial.begin(115200);
  Serial.setTimeout(100);
//  while (!Serial) {
//    ;
//  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if  (!Serial) {
    Serial.begin(115200);
  }
  waiting_4_command();
  if (faultChannel == 1)
  {
    Serial.println("FLT1");
    faultChannel = 0;
  }
  if (faultChannel == 2)
  {
    Serial.println("FLT2");
    faultChannel = 0;
  }
}

void Fault1()
{
  faultChannel = 1;
}
void Fault2()
{
  faultChannel = 2;
}

void waiting_4_command() {
  cmd = "";
  if (Serial.available()) {
    cmd = Serial.readString();
    cmd.trim();
  }

  if (cmd.substring(0, 4) == "RES1") {
    digitalWrite(EN1, LOW);
    Serial.println("Reseting camera 1");
    delay(1000);
    digitalWrite(EN1, HIGH);
  }

  if (cmd.substring(0, 4) == "RES2") {
    digitalWrite(EN2, LOW);
    Serial.println("Reseting camera 2");
    delay(1000);
    digitalWrite(EN2, HIGH);;
  }

  if (cmd.substring(0, 4) == "RESB") {
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, LOW);
    Serial.println("Reseting both cameras");
    delay(1000);
    digitalWrite(EN1, HIGH);
    digitalWrite(EN2, HIGH);
  }

}

void I_AM_READY() {
  Serial.print("WireTester is ready\n");
}
