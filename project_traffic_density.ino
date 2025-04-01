#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Define LED pins for four signals
int red1 = 2, yellow1 = 3, green1 = 4;
int red2 = 5, yellow2 = 6, green2 = 7;
int red3 = 8, yellow3 = 9, green3 = 10;
int red4 = 11, yellow4 = 12, green4 = 13;

// Define Ultrasonic sensor pins
int trig1 = 14, echo1 = 15;
int trig2 = 16, echo2 = 17;
int trig3 = 18, echo3 = 19;
int trig4 = A9, echo4 = A8;

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Traffic Density");
    lcd.setCursor(3, 1);
    lcd.print("Monitoring...");
    delay(3000);
    lcd.clear();

    // Set LED pins as OUTPUT
    pinMode(red1, OUTPUT); pinMode(yellow1, OUTPUT); pinMode(green1, OUTPUT);
    pinMode(red2, OUTPUT); pinMode(yellow2, OUTPUT); pinMode(green2, OUTPUT);
    pinMode(red3, OUTPUT); pinMode(yellow3, OUTPUT); pinMode(green3, OUTPUT);
    pinMode(red4, OUTPUT); pinMode(yellow4, OUTPUT); pinMode(green4, OUTPUT);

    // Set Ultrasonic sensor pins
    pinMode(trig1, OUTPUT); pinMode(echo1, INPUT);
    pinMode(trig2, OUTPUT); pinMode(echo2, INPUT);
    pinMode(trig3, OUTPUT); pinMode(echo3, INPUT);
    pinMode(trig4, OUTPUT); pinMode(echo4, INPUT);

    Serial.begin(9600); // Start serial communication
}

void loop() {
    // Measure traffic density
    int density1 = getTrafficDensity(trig1, echo1);
    int density2 = getTrafficDensity(trig2, echo2);
    int density3 = getTrafficDensity(trig3, echo3);
    int density4 = getTrafficDensity(trig4, echo4);

    // Display densities on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("D1:"); lcd.print(density1); lcd.print("s ");
    lcd.print("D2:"); lcd.print(density2); lcd.print("s");
    lcd.setCursor(0, 1);
    lcd.print("D3:"); lcd.print(density3); lcd.print("s ");
    lcd.print("D4:"); lcd.print(density4); lcd.print("s");
    
    delay(2000);  

    // Control traffic lights for each signal
    trafficLight(green1, yellow1, red1, red2, red3, red4, density1);
    trafficLight(green2, yellow2, red2, red1, red3, red4, density2);
    trafficLight(green3, yellow3, red3, red1, red2, red4, density3);
    trafficLight(green4, yellow4, red4, red1, red2, red3, density4);
}

// Function to measure traffic density using an Ultrasonic sensor
int getTrafficDensity(int trig, int echo) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH);
    int distance = duration * 0.034 / 2; // Convert to cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Determine traffic density timing
    if (distance < 10) return 15; // Heavy traffic (15 sec green)
    if (distance < 30) return 10; // Medium traffic (10 sec green)
    return 5; // Low traffic (5 sec green)
}

// Function to control traffic light based on density
void trafficLight(int go, int wait, int stop, int stop2, int stop3, int stop4, int greenTime) {
    // Turn ON the Red LEDs for inactive signals
    digitalWrite(stop, LOW);
    digitalWrite(stop2, HIGH); digitalWrite(stop3, HIGH); digitalWrite(stop4, HIGH);

    // Green ON
    digitalWrite(go, HIGH);
    delay(greenTime * 1000); // Adjusted Green Light duration

    // Yellow ON, Green OFF
    digitalWrite(go, LOW);
    digitalWrite(wait, HIGH);
    delay(3000); // Yellow Light (3 sec)

    // Red ON
    digitalWrite(wait, LOW);
    digitalWrite(stop, HIGH);
    delay(1000); // Delay before next signal
}
