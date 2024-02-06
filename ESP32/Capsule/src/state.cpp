#include "state.h"

uint8_t Vitesse = 128;
int Sens = 0;

double Distance;

PersistentByte IdEntite{KeyId, DefaultId};

PersistentByte IdBoucleDestination{"boucleId", 1};
PersistentByte IdStationDestination{"stationId", 141}; // La station actuelle a l'ID 141, mettre 142 pour pod1, 143 pour pod 2, 144 pour pod 3, 150 est une destination infinie
PersistentByte IdBoucleActuelle{"actuelleId", 1};

uint8_t State = 0;  // 0 : OFF
                    // 1 : ON
