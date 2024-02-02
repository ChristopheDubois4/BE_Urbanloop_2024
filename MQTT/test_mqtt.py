import paho.mqtt.client as paho
from paho import mqtt
import time

# Informations de connexion
cluster_url = "cf9b375e8064414397c16aacf9b8e9cb.s2.eu.hivemq.cloud"
port = 8883

# Variables d'authentification
username = "urban"
password = "Urbanl00p2023"

# Fonction de connexion au broker MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connexion établie avec succès")
    else:
        print(f"Échec de la connexion, code de retour : {rc}")
        
# Fonction de callback lors de la publication
def on_publish(client, userdata, mid, properties=None):
    print("\nVitesse publiée | mid: " + str(mid))


# Création d'une instance du client MQTT
client = paho.Client(client_id="", userdata=None, protocol=paho.MQTTv5)
client.on_connect = on_connect

# enable TLS for secure connection
client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)

# Configuration des informations de connexion
client.username_pw_set(username, password)

# Connexion au broker MQTT
client.connect(cluster_url, port, 60)

# Callback de on_publish()
client.on_publish = on_publish

# Boucle principale
try:
    vitesse = 0
    while True:
        # Publication de la donnée sur le topic "/speed"
        client.publish("/speed", vitesse)
        print(str(vitesse) + " km/h")
        # Augmentation de la vitesse
        vitesse += 2
        # Attente d'une seconde
        time.sleep(1)

except KeyboardInterrupt:
    print("Arrêt du script par l'utilisateur")
    client.disconnect()

