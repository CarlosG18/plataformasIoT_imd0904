#define TRIGGER_PIN  27  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     26  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

unsigned long tim, tim2, totalTime, maxTime = MAX_DISTANCE*1000000*2/34300;

long ping_cm(){

  digitalWrite(TRIGGER_PIN, HIGH);
  tim = micros();
  while(micros()-tim<10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  while(!digitalRead(ECHO_PIN));
  tim = micros();
  do{
    tim2 = micros();
  }while(digitalRead(ECHO_PIN) && tim2-tim <= maxTime);
  totalTime = tim2-tim;
  if(totalTime <= maxTime){
    long dist = 34300 * totalTime / 2000000;
    return dist;
  }
  else{
    return 0;
  }
}

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}
