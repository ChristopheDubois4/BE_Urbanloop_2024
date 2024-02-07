#include <Arduino.h>
#include <stdio.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_rom_gpio.h>

#include "state.h"

#include "Irda.h"
#include "oled.h"

static const char* TAG = "RADAR";

// Définition de l'IRDA
Irda irda{IdRadar.getValue()};

// Données
struct irda_packet donnees_Capsule_recues;

// Tâches
TaskHandle_t ecouteCapsule_Handle;

#define PERIOD_AFFICHAGE_COMPTEUR 2000

// Historique du passage des capsules
int CompteurCapsules[51];

// Fonction auxiliaires

// Envoie de la consigne de vitesse par IrDA
void consigneVitesse(uint8_t speed, uint8_t destination)
{
	irda.send_vitesse(speed, destination);
}

// Les tâches
void ecouteCapsule(void *parameter)
{
	TickType_t lastSeenTime = 0;
	uint8_t idCapsule;

	for (;;)
	{
		// On lit le paquet envoyé par la capsule

		if (irda.receive(&donnees_Capsule_recues) && donnees_Capsule_recues.typeMessage == Irda_AnnonceCapsule)
		{
			// Filtrage pour l'incrémentation du compteur et l'envoie du message au serveur
			TickType_t currentTime = xTaskGetTickCount();

			if (pdMS_TO_TICKS(currentTime - lastSeenTime) > 200)
			{
				// Historique du passage des capsules
				idCapsule = donnees_Capsule_recues.data1;
				CompteurCapsules[idCapsule] += 1;

				// Envoie du message de détection au serveur
				ESP_LOGI(TAG, "Passage d'une capsule: %u", idCapsule);
			}
			lastSeenTime = currentTime;

			// Envoie de la consigne de vitesse du radar à la capsule
			consigneVitesse(Speed, idCapsule);

			Speed += 4;
		}
	}
}

void changementVitesse(void *parameter)
{
	int nouvelleVitesse;

	ESP_LOGI(TAG, "Serial timeout: %ul", Serial.getTimeout());

	for (;;)
	{
		// On lit la nouvelle consigne de vitesse depuis le moniteur série
		if (Serial.available() > 0)
		{
			nouvelleVitesse = Serial.parseInt();
			if (nouvelleVitesse >= 0 && nouvelleVitesse <= 4095)
			{
					Speed = nouvelleVitesse;

					ESP_LOGI(TAG, "Nouvelle consigne de vitesse: %d", nouvelleVitesse);
			}
		}

		vTaskDelay(pdMS_TO_TICKS(300));
	}
}

void affichageCompteurCapsules(void *params)
{
	const TickType_t period = PERIOD_AFFICHAGE_COMPTEUR;
	TickType_t lastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		// On lit le paquet envoyé par la capsule
		displayIntArray("Capsule", CompteurCapsules, 1, 4);

		vTaskDelayUntil(&lastWakeTime, period);
	}
}

void setup()
{
	// Initialisation de la communication IRDA en 115200
	irda.init_115200();

	xTaskCreatePinnedToCore(
		ecouteCapsule,		   // Task function.
		"ecouteCapsule",	   // name of task.
		10000,				   // Stack size of the task
		NULL,				   // parameter of the task
		1,					   // priority of the task
		&ecouteCapsule_Handle, // Task handle to keep track of created task
		0					   // pin task to core 0
	);

	xTaskCreatePinnedToCore(
		changementVitesse,	   // Task function.
		"changementVitesse",   // name of task.
		10000,				   // Stack size of the task
		NULL,				   // parameter of the task
		2,					   // priority of the task
		NULL,			       // Task handle to keep track of created task
		0					   // pin task to core 0
	);

	xTaskCreatePinnedToCore(
		affichageCompteurCapsules,		   // Task function.
		"affichageCompteurCapsules",	   // name of task.
		10000,				   // Stack size of the task
		NULL,				   // parameter of the task
		5,					   // priority of the task
		NULL,				   // Task handle to keep track of created task
		1					   // pin task to core 1
	);
}

void loop()
{
	// On fait tout dans des tâches
	vTaskDelay(portMAX_DELAY);
}
