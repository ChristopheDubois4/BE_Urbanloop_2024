#pragma once

#include "Arduino.h"
#include "driver/uart.h"
#include "esp_rom_gpio.h"

#define IRDA_RX_BUF_SIZE 128

#define IRDA_DEFAULT_UART_NUM 1
#define IRDA_DEFAULT_TX GPIO_NUM_32
#define IRDA_DEFAULT_RX GPIO_NUM_35
#define IRDA_PIN_MOD GPIO_NUM_33

#define IRDA_UART_TIMEOUT portMAX_DELAY

// Indicateurs de début de paquet
const uint8_t StartIndicator1 = 0xA5;
const uint8_t StartIndicator2 = 0xBE;

// Id du broadcast: envoie le paquet à toutes les entités
const uint8_t BroadcastId = 255;

// Différents types de messages Irda
typedef enum irda_message_type : uint8_t
{
    Irda_AnnonceCapsule,
    Irda_Vitesse,
    Irda_Direction,
    Irda_DepartStation,
    Irda_VitesseTousSauf,
    Irda_MessageArret
} irda_message_type;

struct irda_packet
{
    irda_message_type typeMessage;
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
};

class Irda
{
private:
    uint8_t _entityId;
    int _uartNum;

    uint8_t _startBuffer;

    uint8_t _receivePacketBuffer[sizeof(irda_packet) + 2]; // Il faut aussi compter la destination et la checksum
    uint8_t _sendPacketBuffer[sizeof(irda_packet) + 4]; // Il faut aussi compter les deux octet de début, la destination et la checksum

    irda_packet send_packet;

    int receive(uint8_t *packet);
    int send(uint8_t packet);

    void send(struct irda_packet *packet, uint8_t id_destination = BroadcastId);

    void waitStartIndicator();

public:
    Irda(uint8_t entityId, int uartNum = IRDA_DEFAULT_UART_NUM, gpio_num_t pinRx = IRDA_DEFAULT_RX, gpio_num_t pinTx = IRDA_DEFAULT_TX);
    void init_115200();

    bool receive(struct irda_packet *packet);

    void send_annonce_capsule(uint8_t identifiant_capsule, uint8_t identifiant_station_destination, uint8_t identifiant_boucle_destination, uint8_t identifiant_boucle_actuelle, uint8_t id_destination = BroadcastId);
    void send_vitesse(uint8_t vitesse, uint8_t id_destination = BroadcastId);
    void send_direction(uint8_t direction, uint8_t nouvelleBoucle = 0, uint8_t id_destination = BroadcastId);
    void send_depart_station(uint8_t vitesse, uint8_t station_destination, uint8_t boucle_destination, uint8_t id_destination = BroadcastId);
    void send_vitesse_tous_sauf(uint8_t vitesse, uint8_t exclue_1, uint8_t exclue_2, uint8_t id_destination = BroadcastId);
    void send_arret(uint8_t direction, uint16_t delai, uint8_t id_destination = BroadcastId);

    void resend_last(uint8_t id_destination = BroadcastId);
};
