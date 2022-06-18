#include <Wire.h>
#include <MPU6050.h>

// Define digital pins for each motor
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;

// [UNUSED] Define pins for speed control
// int motor1_PWM= 9;
// int motor2_PWM= 10;

// Define variable for MPU6050
MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  Serial.println("Initialize MPU6050");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  // [OPTIONAL] Set gyroscope offsets
  // mpu.setGyroOffsetX(0);
  // mpu.setGyroOffsetY(0);
  // mpu.setGyroOffsetZ(0);

  mpu.calibrateGyro(); // Calibrate gyroscope while at rest.
  
  mpu.setThreshold(3); // Set threshold sensivty

  checkSettings();

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  
  // pinMode(motor1_PWM, OUTPUT);
  // pinMode(motor2_PWM, OUTPUT);
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:      ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  } 
  
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());
  
  Serial.println();
}

void syncMotors(void)
/* This function causes motor2 to move in the same direction as motor1 */
{
  digitalWrite(motor2pin1, digitalRead(motor1pin1));
  digitalWrite(motor2pin2, digitalRead(motor1pin2));
  
  // Output Motor Status
  Serial.print("\nMotor Status: "); 
  if (digitalRead(motor1pin1) == 1 && digitalRead(motor1pin2) == 0)
  {
    Serial.print("Forward");
  }
  else if (digitalRead(motor1pin1) == 0 && digitalRead(motor1pin2) == 1)
  {
    Serial.print("Backward");
  }
  else
  {
    Serial.print("At Rest");
  }
  
  delay(1000); // 1 second time delay between inputs
}

void moveForward(void) 
/* This function moves the object forward if at rest and stops it if it's moving backward */
{
  if (digitalRead(motor1pin1) == LOW && digitalRead(motor1pin2) == LOW) // Move forward if at rest
  {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
  }
  else if (digitalRead(motor1pin1) == LOW && digitalRead(motor1pin2) == HIGH) // Stop if moving backward
  {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
  }
  syncMotors();
}

void moveBackward(void)
/* This function moves the object backward if at rest and stops it if it's moving forward */
{
  if (digitalRead(motor1pin1) == LOW && digitalRead(motor1pin2) == LOW) // Move backward if at rest
  {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
  }
  else if (digitalRead(motor1pin1) == HIGH) // Stop if moving forward
  {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
  }
  syncMotors();
}

void loop()
{
  // Read raw/normalized accel/gyro data and print to serial monitor
  Vector Gyro = mpu.readNormalizeGyro();
  float X = Gyro.XAxis, Y = Gyro.YAxis, Z = Gyro.ZAxis;
  
  // Motor Control
  if (Y > 100)
  {
    moveForward();
  }
  else if (Y < -100)
  {
    moveBackward();
  }
  
  delay(10); // 10 ms time delay
}
