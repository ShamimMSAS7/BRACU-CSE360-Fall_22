int a = 10;
int b = 5;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Addition: ");
  Serial.print(a+b);
  Serial.println();
  delay(2000);
  Serial.print("Subtraction: ");
  Serial.print(a-b);
  Serial.println();
  delay(2000);

}
