#include <Statistic.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 7

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

Statistic myStat;

#define VERSION_SW    "v0.2"
float actualTemp = 1;
 

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
 
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(1000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  testdrawtriangle();  // Draw triangles (outlines)

  testscrolltext();    // Draw scrolling text
  display.clearDisplay();

  myStat.clear();
  
  sensors.begin(); delay(1000);
}

void loop() {
  sensors.requestTemperatures();
  actualTemp = sensors.getTempCByIndex(0);
  //Serial.println(actualTemp);
  myStat.add(actualTemp);
  display.drawRoundRect(0,0,128,16,1,SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(3,5);
  display.print("Actuel: ");
  display.setTextSize(1);
  display.setCursor(70,5);
  display.print(actualTemp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("C");

  if ( (myStat.count() % 2) == 0) { 
    display.fillCircle(124, 3, 2, SSD1306_WHITE);  
  }
  
  display.drawRoundRect(0,17,64,35,1,SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(18,19);
  display.print("Min.: ");
  display.setTextSize(2);
  display.setCursor(3,35);
  display.print(myStat.minimum());

  display.drawRoundRect(65,17,63,35,1,SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(82,19);
  display.print("Max.: ");
  display.setTextSize(2);
  display.setCursor(68,35);
  display.print(myStat.maximum());

  display.setTextSize(1);
  display.setCursor(0,57);
  display.print("AVG");
  display.setCursor(20,57);
  display.print(myStat.average());
  display.setCursor(55,57);
  display.print("+-");
  display.setCursor(65,57);
  display.print(myStat.pop_stdev());

  //Display Version
  display.setCursor(100,57);
  display.print(VERSION_SW);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(1000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("CaveTemp"));
  display.setCursor(12, 30);
  display.println(F("By M4T"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(1000);
  display.stopscroll();
  delay(100);
  display.startscrollleft(0x00, 0x0F);
  delay(1000);
  display.stopscroll();
  delay(500);
}
