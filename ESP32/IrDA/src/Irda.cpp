#include "Irda.h"
#include "esp_log.h"

static const char* TAG = "IRDA";

Irda::Irda(uint8_t entityId, int uartNum, gpio_num_t pinRx, gpio_num_t pinTx)
{
    _entityId = entityId;
    _uartNum = uartNum;

    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // We won't use a buffer for sending data.
    //esp_rom_gpio_pad_select_gpio(pinTx);
    //esp_rom_gpio_pad_select_gpio(pinRx);
    //esp_rom_gpio_pad_select_gpio(pin_MOD);

    pinMode(pinTx, OUTPUT); // IMPORTANT
    pinMode(pinRx, INPUT);  // IMPORTANT
    pinMode(IRDA_PIN_MOD, OUTPUT);
    digitalWrite(IRDA_PIN_MOD, HIGH);

    ESP_ERROR_CHECK(uart_param_config(uartNum, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uartNum, pinTx, pinRx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(uartNum, IRDA_RX_BUF_SIZE * 2, 0, 0, NULL, 0));

    ESP_LOGI(TAG, "Irda initialise avec: id=%d, uartNum=%d, pinRx=%d, pinTx=%d", entityId, uartNum, pinRx, pinTx);
}

void Irda::init_115200()
{
    //Initialisation IRDA en 115200
	//Mettre le MOD à LOW - envoyer sur la liaison série 0x81 puis 0x11 - Mettre MOD à HIGH
    uint8_t read;

    ESP_LOGI(TAG, "init_115200");

    digitalWrite(IRDA_PIN_MOD, LOW);

    send(0x81);
    receive(&read);

    if (read == 0x81)
    {
        ESP_LOGI(TAG, "L'IRDA a bien reçu la demande de passage en 115200");
    }
    else if (read == 0x89)
    {
        ESP_LOGI(TAG, "L'IRDA est déjà en 115200");
    }
    else
    {
        ESP_LOGE(TAG, "Baudrate recu inconnu: %x, verifier les branchements!!!", read);
    }

    send(0x11);
    receive(&read);

    if (read == 0x11)
    {
        ESP_LOGI(TAG, "L'IRDA est bien passé en 115200");
    }
    else if (read == 0x99)
    {
        ESP_LOGI(TAG, "L'IRDA est bien passé en 115200 mais était déjà en 115200");
    }
    else
    {
        ESP_LOGE(TAG, "Commande recu inconnue: %x, verifier les branchements!!!", read);
    }

    digitalWrite(IRDA_PIN_MOD, HIGH);

    // Passage en 115200
    ESP_LOGI(TAG, "Changement du baudrate de la liaison serie en 115200...");

    uint32_t baudrate;
    uart_set_baudrate(_uartNum, 115200);
    uart_get_baudrate(_uartNum, &baudrate);

    ESP_LOGI(TAG, "Liaison serie ouverte, baudrate: %d", baudrate);
}

bool Irda::receive(struct irda_packet *packet)
{
    waitStartIndicator();
    ESP_LOGV(TAG, "Received data");

    // Lire le paquet (DestinationID, TypeMessage, Data1, Data2, Data3, checksum)
    if (uart_read_bytes(_uartNum, _receivePacketBuffer, sizeof(_receivePacketBuffer), IRDA_UART_TIMEOUT) == sizeof(_receivePacketBuffer))
    {
        uint8_t checksum = _receivePacketBuffer[sizeof(_receivePacketBuffer) - 1];

        ESP_LOGV(TAG, "Received packet: [dest: %u, type: %u, data1: %u, data2: %u, data3: %u, checksum: %u]", _receivePacketBuffer[0], _receivePacketBuffer[1], _receivePacketBuffer[2], _receivePacketBuffer[3], _receivePacketBuffer[4], _receivePacketBuffer[5]);

        // Vérification de la checksum
        if (checksum != (_receivePacketBuffer[0] ^ _receivePacketBuffer[1] ^ _receivePacketBuffer[2] ^ _receivePacketBuffer[3] ^ _receivePacketBuffer[4]))
        {
            // Paquet corrompu
            ESP_LOGD(TAG, "Paquet corrompu recu: [dest: %u, type: %u, data1: %u, data2: %u, data3: %u, checksum: %u]", _receivePacketBuffer[0], _receivePacketBuffer[1], _receivePacketBuffer[2], _receivePacketBuffer[3], _receivePacketBuffer[4], _receivePacketBuffer[5]);
            return false;
        }

        // Vérification de la destination
        uint8_t destinationId = _receivePacketBuffer[0];
        if (destinationId != BroadcastId && destinationId != _entityId)
        {
            // Paquet destiné à une autre capsule
            return false;
        }

        // Lecture du paquet
        memcpy(packet, &_receivePacketBuffer[1], sizeof(irda_packet));

        return true;
    }

    return false;
}

void Irda::send(struct irda_packet *packet, uint8_t id_destination)
{
    // Construction paquet (Start1, Start2, DestinationId, TypeMessage, Data1, Data2, Data3, checksum)
    _sendPacketBuffer[0] = StartIndicator1;
    _sendPacketBuffer[1] = StartIndicator2;

    _sendPacketBuffer[2] = id_destination;

    memcpy(&_sendPacketBuffer[3], packet, sizeof(irda_packet));

    _sendPacketBuffer[sizeof(_sendPacketBuffer) - 1] = _sendPacketBuffer[2] ^ _sendPacketBuffer[3] ^ _sendPacketBuffer[4] ^ _sendPacketBuffer[5] ^ _sendPacketBuffer[6];

    ESP_LOGV(TAG, "Sent packet: [dest: %u, type: %u, data1: %u, data2: %u, data3: %u, checksum: %u]", _sendPacketBuffer[2], _sendPacketBuffer[3], _sendPacketBuffer[4], _sendPacketBuffer[5], _sendPacketBuffer[6], _receivePacketBuffer[7]);

    // Envoie du paquet
    uart_write_bytes(_uartNum, _sendPacketBuffer, sizeof(_sendPacketBuffer));
}

int Irda::receive(uint8_t *packet)
{
    return uart_read_bytes(_uartNum, packet, sizeof(uint8_t), IRDA_UART_TIMEOUT);
}

int Irda::send(uint8_t packet)
{
    return uart_write_bytes(_uartNum, &packet, sizeof(uint8_t));
}

void Irda::waitStartIndicator()
{
    ESP_LOGV(TAG, "Waiting for start indicator");
    for (;;)
    {
        // Si le deuxième octet qu'on a lu est l'indicateur de début on passe directement au prochain octet
        if (_startBuffer != StartIndicator1)
        {
            receive(&_startBuffer);
        }

        ESP_LOGV(TAG, "First indicator: %X", _startBuffer);
        if (_startBuffer == StartIndicator1)
        {
            receive(&_startBuffer);
            ESP_LOGV(TAG, "Second indicator: %X", _startBuffer);

            if (_startBuffer == StartIndicator2)
            {
                return;
            }
        }
    }
}

// Fonction pré-implémentées
void Irda::send_annonce_capsule(uint8_t identifiant_capsule, uint8_t identifiant_station_destination, uint8_t identifiant_boucle_destination, uint8_t identifiant_boucle_actuelle, uint8_t id_destination)
{
    send_packet.typeMessage = Irda_AnnonceCapsule;
    send_packet.data1 = identifiant_capsule;
    send_packet.data2 = identifiant_station_destination;
    send_packet.data3 = (identifiant_boucle_actuelle << 4) + identifiant_boucle_destination;

    send(&send_packet, id_destination);
}

void Irda::send_vitesse(uint8_t vitesse, uint8_t id_destination)
{
    send_packet.typeMessage = Irda_Vitesse;
    send_packet.data1 = vitesse;

    send(&send_packet, id_destination);
}

void Irda::send_direction(uint8_t direction, uint8_t nouvelleBoucle, uint8_t id_destination)
{
    send_packet.typeMessage = Irda_Direction;
    send_packet.data1 = direction;
    send_packet.data2 = nouvelleBoucle;

    send(&send_packet, id_destination);
}

void Irda::send_depart_station(uint8_t vitesse, uint8_t station_destination, uint8_t boucle_destination, uint8_t id_destination)
{
    send_packet.typeMessage = Irda_DepartStation;
    send_packet.data1 = vitesse;
    send_packet.data2 = station_destination;
    send_packet.data3 = boucle_destination;

    send(&send_packet, id_destination);
}

void Irda::send_vitesse_tous_sauf(uint8_t vitesse, uint8_t exclue_1, uint8_t exclue_2, uint8_t id_destination)
{
    send_packet.typeMessage = Irda_VitesseTousSauf;
    send_packet.data1 = vitesse;
    send_packet.data2 = exclue_1;
    send_packet.data3 = exclue_2;

    send(&send_packet, id_destination);
}

void Irda::send_arret(uint8_t direction, uint16_t delai, uint8_t id_destination)
{
    send_packet.typeMessage = Irda_MessageArret;
    send_packet.data1 = direction;
    *((uint16_t*)&send_packet.data2) = delai;

    send(&send_packet, id_destination);
}

void Irda::resend_last(uint8_t id_destination)
{
    send(&send_packet, id_destination);
}