#include "oled.h"

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL, GEOMETRY_128_64, I2C_TWO);

bool isInitialized = false;
char buffer[32];

void initDisplay()
{
    // Initialising the UI will init the display too.
    display.init();
    display.flipScreenVertically();

    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
}

static void checkInit()
{
    if (!isInitialized)
    {
        initDisplay();

        isInitialized = true;
    }
}

SSD1306Wire displayGet()
{
    checkInit();

    return display;
}

void displayIntArray(const char* prefix, int *array, size_t offset, size_t length)
{
    checkInit();

    display.clear();

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    for (int i = offset; i < offset + length; i++)
    {
        display.drawStringf(0, 16 * (i - offset), buffer, "%s %d: %d", prefix, i, array[i]);
    }

    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 54, "N° Passages");

    display.display();
}