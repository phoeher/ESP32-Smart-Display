#include "display_client.h"

#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

const int EPD_CS   = D10;
const int EPD_DC   = D9;
const int EPD_RST  = D8;
const int EPD_BUSY = D7;
const int EPD_PWR  = D6;

const char HelloWorld[] = "Hello World!";

GxEPD2_BW<
    GxEPD2_420_GDEY042T81,
    GxEPD2_420_GDEY042T81::HEIGHT
> display(
    GxEPD2_420_GDEY042T81(
        EPD_CS,
        EPD_DC,
        EPD_RST,
        EPD_BUSY
    )
);

void initializeDisplay() {
    pinMode(EPD_PWR, OUTPUT);
    digitalWrite(EPD_PWR, HIGH);

    delay(100);

    display.init(115200, true, 2, false);
    display.setRotation(2);
}

void showHelloWorld(){ 
    display.setFont(&FreeSans9pt7b);
    display.setTextColor(GxEPD_BLACK);
    int16_t tbx, tby; uint16_t tbw, tbh;
    display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center the bounding box by transposition of the origin:
    uint16_t x = ((display.width() - tbw) / 2) - tbx;
    uint16_t y = ((display.height() - tbh) / 2) - tby;
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(x, y);
        display.print(HelloWorld);
    }
    while (display.nextPage());
}

void clearDisplay(){
  display.setFullWindow();

    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
    }
    while (display.nextPage());
}

void hibernateDisplay(){
    display.hibernate();
}
