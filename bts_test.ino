// Pin Definitions
const uint8_t L_EN = 7;
const uint8_t R_EN = 6;
const uint8_t L_PWM = 10;
const uint8_t R_PWM = 9;

int delayTime = 50;

int calcPWM(float perc_val) {
  // This function converns a percentage value (0-100) into a PWM value (0-255)
  int pwm = (perc_val / 100) * 255;
  return pwm;
}

void setup() {
  // Activate PWM pins
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  
  // Activate enable (EN) pins
  pinMode(L_EN, OUTPUT);
  pinMode(R_EN, OUTPUT);
  
  // Write 0 value to PWM motors to stop motor if they're moving
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);

  // Enable "Right" and "Left" movement
  // NOTE: KEEP THESE THE SAME VALUE (unless you want smoke)
  digitalWrite(L_EN, HIGH);
  digitalWrite(R_EN, HIGH); 

  Serial.begin(115200);
}

void loop() {
  Serial.println("\nAccelerating Forward");
  for (int i = 0; i < 100; i++) { // Accelerate forward from 0 to top speed for (100 * delayTime) milliseconds
    analogWrite(L_PWM, 0);
    analogWrite(R_PWM, calcPWM(i));
    delay(delayTime);
  }

  Serial.println("\nDecelerating Forward");
  for (int i = 0; i < 100; i++) { // Decelerate forward from top speed to 0 for (100 * delayTime) milliseconds
    analogWrite(L_PWM, 0);
    analogWrite(R_PWM, calcPWM(100-i));
    delay(delayTime);
  }

  delay(1000);

  Serial.println("\nAccelerating Backwards");
  for (int i = 0; i < 100; i++) { // Accelerate backward from 0 to top speed for (100 * delayTime) milliseconds
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, calcPWM(i));
    delay(delayTime);
     
  }

  Serial.println("\nDecelerating Backwards");
  for (int i = 0; i < 100; i++) { // Decelerate backward from top speed to 0 for (100 * delayTime) milliseconds
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, calcPWM(100-i));
    delay(delayTime);
  }
}
