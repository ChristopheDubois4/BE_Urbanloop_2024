#include <Arduino.h>
#include <stdio.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_rom_gpio.h>
#include "esp_log.h"

#include "state.h"

#include "oled.h"
#include "Irda.h"
#include "persistence.h"

// Paramètres LEDs
#define pin_LED_Vitesse GPIO_NUM_14
#define pin_LED_State GPIO_NUM_27

// Paramètre des tâches
#define PERIOD_SEND_ID 50
#define PERIOD_LED_INDICATION 100

// TAG pour les logs
static const char *TAG_IRDA = "Communication IRDA";

// Définition de l'IRDA
Irda irda{IdEntite.getValue()};

struct irda_packet donnees_recues;

//=======================================//

// Tâche envoie id
// La capsule envoie son identifiant et sa destination en permanence
void vTaskSendId(void *params)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t period = pdMS_TO_TICKS(PERIOD_SEND_ID);

	ESP_LOGI(TAG_IRDA, "Tache envoie ID sur le coeur %d", xPortGetCoreID());

	for (;;)
	{
		irda.send_annonce_capsule(IdEntite.getValue(), IdStationDestination.getValue(), IdBoucleDestination.getValue(), IdBoucleActuelle.getValue());

		ESP_LOGD(TAG_IRDA, "Paquet IRDA envoyé: IDPOD : %u, Station: %u, Boucle: %u", IdEntite.getValue(), IdStationDestination.getValue(), IdBoucleDestination.getValue());

		vTaskDelayUntil(&xLastWakeTime, period);
	}
}

// Tâche reception vitesse par un radar
void vTaskReceiveIrda(void *params)
{
	ESP_LOGI(TAG_IRDA, "Tache reception IrDA sur le coeur %d", xPortGetCoreID());

	for (;;)
	{
		// Pas d'erreur lors de la réception
		if (irda.receive(&donnees_recues))
		{
			ESP_LOGD(TAG_IRDA, "Type message reçu: %u", donnees_recues.typeMessage);

			switch (donnees_recues.typeMessage)
			{
				case Irda_Vitesse:
					Vitesse = donnees_recues.data1;
					ESP_LOGI(TAG_IRDA, "Nouvelle valeur de vitesse : %u", Vitesse);
					break;
			}
		}
	}
}

// Tâche contrôle des LEDs d'indication
void vTaskLightLEDs(void *params)
{
	// Initialisation des LED
	pinMode(pin_LED_State, OUTPUT);
	pinMode(pin_LED_Vitesse, OUTPUT);

	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t period = pdMS_TO_TICKS(PERIOD_LED_INDICATION);

	for (;;)
	{
		// OFF
		if (State == 0)
		{
			digitalWrite(pin_LED_State, LOW);
			analogWrite(pin_LED_Vitesse, Vitesse);
		}
		// ON
		else if (State == 1)
		{
			digitalWrite(pin_LED_State, HIGH);
			analogWrite(pin_LED_Vitesse, Vitesse);
		}

		vTaskDelayUntil(&xLastWakeTime, period);
	}
}

void setup()
{
	// Configuration du service d'interruptions
	gpio_install_isr_service(0);

	// Passage de l'IRDA en 115200
	irda.init_115200();

	// Création de la tâche d'envoie d'id
	xTaskCreatePinnedToCore(
		vTaskSendId,	   /* Function to implement the task */
		"Tâche envoie id", /* Name of the task */
		10000,			   /* Stack size in words */
		NULL,			   /* Task input parameter */
		1,				   /* Priority of the task */
		NULL,			   /* Task handle. */
		1				   /* Core where the task should run */
	);

	// Création de la tâche de récupération de la distance
	xTaskCreatePinnedToCore(
		vTaskReceiveIrda,			 /* Function to implement the task */
		"Tâche reception vitesse radar", /* Name of the task */
		10000,							 /* Stack size in words */
		NULL,							 /* Task input parameter */
		3,								 /* Priority of the task */
		NULL,							 /* Task handle. */
		1								 /* Core where the task should run */
	);

	// Création de la tâche d'allumage des LEDs gauche/droite
	xTaskCreatePinnedToCore(
		vTaskLightLEDs,	  /* Function to implement the task */
		"vTaskLightLEDs", /* Name of the task */
		1000,			  /* Stack size in words */
		NULL,			  /* Task input parameter */
		1,				  /* Priority of the task */
		NULL,			  /* Task handle. */
		1				  /* Core where the task should run */
	);
}

void loop()
{
	// On fait tout dans des tâches
	vTaskDelay(portMAX_DELAY);
}