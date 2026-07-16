#include "display_client.h"

#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>

#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
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

void drawCenteredText(const String& text, int y, const GFXfont* font) {
    display.setFont(font);

    int16_t textX;
    int16_t textY;
    uint16_t textWidth;
    uint16_t textHeight;

    display.getTextBounds(
        text,
        0,
        0,
        &textX,
        &textY,
        &textWidth,
        &textHeight
    );

    int x = ((display.width() - textWidth) / 2) - textX;

    display.setCursor(x, y);
    display.print(text);
}
 
void showIdleScreen(
    const String& time,
    const String& date,
    const String& meridiem,
    float currentTemperature,
    float highTemperature,
    float lowTemperature,
    const String& condition,
    int precipitationProbability,
    float uvIndex,
    const unsigned char* icon
) {

    display.init(115200, true, 2, false);
    display.setRotation(2);

    display.setFullWindow();

    display.firstPage();

    do {
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);

        // Date
        display.setFont(&FreeSans9pt7b);
        display.setCursor(15, 30);
        display.print(date);

        // Location
        display.setFont(&FreeSans9pt7b);
        display.setCursor(display.width()-130, 30);
        display.print("Yorktown, VA");

        // Time
        display.setFont(&FreeSansBold24pt7b);
        display.setCursor(15, 86);
        display.print(time);

        // Meridiem
        display.setFont(&FreeSansBold12pt7b);
        display.setCursor(15, 116);
        display.print(meridiem);

        // Horizontal divider
        display.drawLine(15, 260, display.width() - 15, 260, GxEPD_BLACK);

        // Temperature
        display.setFont(&FreeSansBold24pt7b);
        display.setCursor(210, 86);
        display.print(String(currentTemperature, 0));
        display.print(" F");

        // Weather condition
        display.setFont(&FreeSansBold12pt7b);
        display.setCursor(210, 128);
        display.print(condition);

        // Weather icon
        display.drawBitmap(315, 45, icon, 64, 64, GxEPD_BLACK);

        // High and low
        display.setFont(&FreeSans9pt7b);
        display.setCursor(210, 170);
        display.print("H: ");
        display.print(String(highTemperature, 0));
        display.print("  L: ");
        display.print(String(lowTemperature, 0));

        // UV or precipitation
        display.setCursor(210, 190);

        if (
            condition.indexOf("Rain") >= 0 ||
            condition.indexOf("Drizzle") >= 0 ||
            condition.indexOf("Thunderstorm") >= 0 ||
            condition.indexOf("Snow") >= 0
        ) {
            display.print("Rain: ");
            display.print(precipitationProbability);
            display.print("%");
        }
        else {
            display.print("UV Max: ");
            display.print(String(uvIndex, 1));
        }

        // Temporary Box
        display.drawRect(15, 128, 180, 117, GxEPD_BLACK);


    }
    while (display.nextPage());
}

void updateTime(const String& time){
    const int x = 10;
    const int y = 52;
    const int width = 190;
    const int height = 39;

    display.init(115200, false, 2, false);
    display.setRotation(2);

    display.setPartialWindow(x, y, width, height);

    display.firstPage();
    do {
        // Clear only the clock region
        display.fillRect(x, y, width, height, GxEPD_WHITE);

        display.setTextColor(GxEPD_BLACK);

        display.setFont(&FreeSansBold24pt7b);
        display.setCursor(15, 86);
        display.print(time);
    }
    while (display.nextPage());

    display.hibernate();
}
