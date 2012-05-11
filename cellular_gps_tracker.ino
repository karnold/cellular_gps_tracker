#include <SoftwareSerial.h>
#include <TinyGPS.h>

/*****************************************
 * Error codes
 */
const int ERROR_GPS_UNAVAILABLE = 0;
const int ERROR_GPS_STALE       = 1;

/******************************************
 * Pin Definitions
 */

// UART MODE
//int GPS_TX = 0;
//int GPS_RX = 1;

// DLINE mode
int GPS_TX = 2;
int GPS_RX = 3;

int LED_STATUS = 13;
int LED_ERROR = 12;

/*****************************************
 * Misc variables
 */
 
int POLL_TIME = 1000; // The amount of time in milliseconds to poll for gps data;
int PRECISION = 9; // Precision of lat/long coordinates
int DIAG_DELAY = 2000; // time to pause before replaying diagnostic codes

TinyGPS gps;
SoftwareSerial gpsSerial(GPS_TX, GPS_RX);

void setup() {
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  
  Serial.begin(115200);
  gpsSerial.begin(4800);
  
  Serial.println("Arduino powered GPS Tracker initializing");
}

void loop() {
  bool newdata = false;
  unsigned long start = millis();
  
  // Every second we print an update
  while (millis() - start < 1000)
  {
    if (gpsAvailable())
      newdata = true;
  }
  
  pollGPS(gps);
}

void pollGPS(TinyGPS &gps) {
      int sat = gps.satellites();
      unsigned long age;
      float lat, lng, speed;
      
      gps.f_get_position(&lat, &lng, &age);
      
      if (lat == TinyGPS::GPS_INVALID_F_ANGLE) {
        error(ERROR_GPS_UNAVAILABLE);
      } 
      
      Serial.print("Satellites: ");
      Serial.println(sat);
      
      Serial.print("Latitude: ");
      print_float(lat, TinyGPS::GPS_INVALID_F_ANGLE, 9, PRECISION);
      Serial.print(" : Longitude: ");
      print_float(lat, TinyGPS::GPS_INVALID_F_ANGLE, 9, PRECISION);
      Serial.println("");
}

static bool gpsAvailable() {
  while (gpsSerial.available())
  {
    if (gps.encode(gpsSerial.read()))
      return true;
  }
  return false; 
}

/**
 * Flash an LED X amount of times and write to console based on the error code given
 */
void error(const int errorCode) {
  int flashTimes = 0;
  int i = 0;
  
  switch(errorCode) {
    case ERROR_GPS_UNAVAILABLE:
      flashTimes = 2;
      Serial.println("ERROR: GPS Unavailable");
      break;
  }
  
  while (i < flashTimes) {
    digitalWrite(LED_ERROR, HIGH);
    delay(500);
    digitalWrite(LED_ERROR, LOW);
    delay(500);
    Serial.println(i);
    i++;
  }
  delay(DIAG_DELAY);

}

/**
 * Print floating point integers into printable text
 */
static void print_float(float val, float invalid, int len, int prec)
{
  char sz[32];
  if (val == invalid)
  {
    strcpy(sz, "N/A");
    sz[len] = 0;
    if (len > 0) 
      sz[len-1] = ' ';
    for (int i=7; i<len; ++i)
      sz[i] = ' ';
    Serial.print(sz);
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1);
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(" ");
  }

}
