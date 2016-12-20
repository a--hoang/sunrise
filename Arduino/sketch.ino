#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

String on;

void setup() {
  matrix.begin();
  on = "hi";
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) {
    if(on != "ah") {
      on = Serial.readStringUntil('m');
    } else {
      on = "hi";
      Serial.readStringUntil('m');
    }
    Serial.println(on);
    Serial.println("ACK");
    delay(100);
  }

  if (on.equals("ah")) {
    matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 7));
    matrix.fillCircle(20, 8, 3, matrix.Color333(7, 7, 7));

  } else {
   matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  }

    delay(100);
  
}
