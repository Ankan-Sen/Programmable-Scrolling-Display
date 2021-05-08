/*Wiring:
 * pin D2 of esp8266 --> DIN pin of WS2812 LED Matrix
 * 3.3V pin ---> 5V of matrix
 * GND pin ----> GND of matrix
 */


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

uint8_t LED_Pin = 4;            //Data will be sent through D2 pin of ESP8266
String textIn;
int y;

char auth[] = "PutBlynkAuthTokenHere";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PutWifiNameHere";
char pass[] = "PutWifiPasswordHere";

BLYNK_WRITE(V0)
{
  textIn = param.asStr();            //Get the text from blynk
  y = textIn.length();
}


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, LED_Pin,
                            NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);            //Declaring Matrix parameters

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)
};     // Text colors to be displayed can be put here (R, G, B)

void setup()
{
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  delay(2000);
  Serial.print("Blynk Ready\n");
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix.setTextColor(colors[0]);
}  


int x    =  matrix.width();
int p = 0;

void loop()
{
  Blynk.run();
  Serial.print(textIn + "\n");
  matrix.fillScreen(0);    //Turn off all the LEDs
  matrix.setCursor(x, 0);
  matrix.print(textIn);

  if ( --x < -6 * y) {
    x = matrix.width();       // Change cursor location to make the text scroll

    if (++p >= 8) p = 0;
    matrix.setTextColor(colors[p]); // Loop through all the colors in the list
  }
  matrix.show();
  delay(30);       //How fast the text will scroll, smaller number => higher speed
 
}
