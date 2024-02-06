#pragma once

#include "stdlib.h"

extern const char* KeyId;
#define DefaultId 199

class PersistentByte
{
    private:
        const char *_key;
        uint8_t _value;

    public:
        PersistentByte(const char *key, uint8_t defaultValue);

        uint8_t getValue();
        void setValue(uint8_t);
};