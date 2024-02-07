#include <Arduino.h>
#include <NewPing.h>

// Function declarations
unsigned int measureTimeOfFlight();
void echoReceived();  // Callback function declaration
float calculateFlowRate(unsigned int duration);

const int trigTransmit = 2;  // Trigger pin for the transmitting ultrasonic sensor
const int echoTransmit = 3;  // Echo pin for the transmitting ultrasonic sensor
const int trigReceive = 4;   // Trigger pin for the receiving ultrasonic sensor
const int echoReceive = 5;   // Echo pin for the receiving ultrasonic sensor

NewPing sonarTransmit(trigTransmit, echoTransmit);
NewPing sonarReceive(trigReceive, echoReceive);

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(1000);  // Delay for stability

  // Measure the time taken for the ultrasonic pulse to travel from the transmit sensor to the receive sensor
  unsigned int duration = measureTimeOfFlight();

  // Calculate flow rate based on the measured time of flight
  float flowRate = calculateFlowRate(duration);

  // Print the flow rate
  Serial.print("Flow Rate: ");
  Serial.print(flowRate, 2);  // Print flow rate with 2 decimal places
  Serial.println(" liters per minute");
}

unsigned int measureTimeOfFlight() {
  unsigned int duration;
  unsigned int maxDistance = 200;  // Maximum expected distance between sensors in centimeters

  // Trigger the transmit sensor
  sonarTransmit.ping_timer(echoReceived, maxDistance * 2 * 29); // Trigger the transmit sensor and set the timeout

  // Wait for the signal to travel between sensors
  duration = sonarReceive.ping_median(5);  // Take the median of 5 ping readings from the receiving sensor

  // Ensure the duration is within a reasonable range
  if (duration > maxDistance * 2 * 29) {
    duration = 0;
  }

  return duration;
}

void echoReceived() {
  // Empty callback function for echoReceived
}

float calculateFlowRate(unsigned int duration) {
  // Speed of sound in air at 20 degrees Celsius is approximately 343 meters per second
  float speedOfSound = 343.0 / 100.0;  // Convert to centimeters per second
  float distance = duration * 0.5 * speedOfSound / 100.0;  // Convert duration to centimeters

  // Assume a known pipe diameter in centimeters
  const float pipeDiameter = 5.0;

  // Calculate flow rate based on the time of flight and pipe diameter
  float area = PI * pow((pipeDiameter / 2.0), 2);
  float velocity = distance / (duration / 1000000.0);  // Convert duration to seconds
  float flowRate = area * velocity;

  return flowRate;
}
