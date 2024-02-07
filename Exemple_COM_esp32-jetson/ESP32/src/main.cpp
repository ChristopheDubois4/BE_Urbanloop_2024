#include <HardwareSerial.h>
#include <Arduino.h>


// RX pin used for serial communication with the JETSON
#define RX_PIN GPIO_NUM_26
// TX pin used for serial communication with the JETSON
#define TX_PIN GPIO_NUM_25

// Initialize communication with the JETSON on UART 1
HardwareSerial SerialJetson(1);

// Latest received datas
struct Datas {
  int speed = 0;
  int distance = 0;
  int instruction = 0;
} datas;


// Protect the data update coming from the serial link
SemaphoreHandle_t mutex;

/**
 * Send content with a topic to the JETSON
 * @param topic 
 * @param content
*/
void sendData(const String& topic, const String& content) {
  String message = topic + " " + content + "\n";
  SerialJetson.print(message);
}

/**
 * Send content with a topic to the JETSON
 * @param topic 
 * @param content
*/
void sendData(const String& topic, int content) {
  String message = topic + " " + String(content) + "\n";
  SerialJetson.print(message);
}

/**
 * Updates the received data in the appropriate variable
 * @param topic 
 * @param content
*/
void processMessage(const String& topic, const String& content) {
  // Pointer to the variable to be updated
  int* data_to_update = NULL;
  // Retrieval of the variable to be updated
  if (topic.equals("speed")) {
      data_to_update = &(datas.speed);
      //Serial.println("vitesse : " + content); // -- Debug
  } else if (topic.equals("distance")) {
      data_to_update = &(datas.distance);
      //Serial.println("Distance : " + content); // -- Debug
  } else if (topic.equals("instruction")) {
      data_to_update = &(datas.instruction);
      //Serial.println("Consigne : " + content); // -- Debug
  } else {
      //Serial.println("ERREUR TOPIC "); // -- Debug
      return;
  }
  // Updating the variable
  if (data_to_update != NULL) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    (*data_to_update) = content.toInt();
    xSemaphoreGive(mutex);
  }
}

/**
 * Receives information from the serial connection
*/
void vTaskRcvJETSON(void *pvParameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for (;;){       
      // Listening to the serial port
      if (SerialJetson.available()) {
        String message = SerialJetson.readStringUntil('\n');
        //Serial.println("Message reçu : " + message); // -- Debug      
        // Extraire le topic et le content du message
        int espaceIndex = message.indexOf(' ');
        if (espaceIndex != -1) {
          String topic = message.substring(0, espaceIndex);
          String content = message.substring(espaceIndex + 1);   
          processMessage(topic, content);
        }
      }
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS( 1 ));
    }   
}

/**
 * Sends information to the serial connection
 * -- TEMPORARY
*/
void vTaskTestSEND(void *pvParameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    int data_1 = 50;
    int data_2 = 10;
    int data_3 = 30;

    for (;;){
      // Example of use: send data via the sendData() method
      sendData("data_1", data_1);
      sendData("data_2", data_2);
      sendData("data_3", data_3);
      data_1++;
      data_2++;
      data_3++;
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS( 1000 ));
    }   
}

/**
 * Displays data stored on the serial monitor
 * -- debug
*/
void vPrint(void *pvParameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for (;;){
      Serial.println("________________ ");
      xSemaphoreTake(mutex, portMAX_DELAY);
      Serial.println("STo Vitesse : " + String(datas.speed));
      Serial.println("Sto Distance : " + String(datas.distance));
      Serial.println("Sto Consigne : " + String(datas.instruction));
      xSemaphoreGive(mutex);
      Serial.println("________________ ");
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS( 1000 ));
    }   
}

/**
 * Initialize the program
 */
void setup() {
  // Start the serial communication with the serial monitor
  Serial.begin(115200);
  // Start the RS232 serial communication
  SerialJetson.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);     // Initialiser la communication serie RS232
  // Initialize the mutex
  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(vTaskRcvJETSON, "vTaskRecieve", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(vTaskTestSEND, "test", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(vPrint, "vPrint", 4096, NULL, 1, NULL, 1);

}


void loop() {}