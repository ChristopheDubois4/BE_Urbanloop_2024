#include "persistence.h"
#include "Preferences.h"
#include "nvs_flash.h"

static const char* TAG = "Persistence";
static const char* Namespace = "urbanloop";

const char* KeyId = "id";

Preferences preferences;
bool initialized;

static void checkInit()
{
    if (!initialized)
    {
        ESP_LOGI(TAG, "Initialisation des preferences");

        nvs_flash_init();
        preferences.begin(Namespace, false);
        initialized = true;

        ESP_LOGI(TAG, "Preferences initialisees");
    }
}

PersistentByte::PersistentByte(const char *key, uint8_t defaultValue)
{
    checkInit();

    _key = key;
    _value =  preferences.getUChar(key, defaultValue);
};

uint8_t PersistentByte::getValue()
{
    return _value;
}

void PersistentByte::setValue(uint8_t value)
{
    _value = value;
    preferences.putUChar(_key, value);
}