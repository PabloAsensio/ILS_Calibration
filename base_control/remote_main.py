import paho.mqtt.client as paho
import time

MQTT_HOST="193.201.107.70"
MQTT_PORT=1883
MQTT_KEEPALIVE=60
MQTT_TOPIC="EXAMPLE_TOPIC"


def on_connect(client,userdata,flags,rc):
    print("Connection result: " + str(rc))

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

def on_publish(mqttc, obj, mid):
    print("Order sended")

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def publish_msg(client, topic, nqos):
    msg = str(input())
    msg = msg.upper()
    client.publish(topic, msg, qos=nqos)
    try:
        publish_msg(client, topic, nqos)
    except KeyboardInterrupt:
        client.disconnect()
        print("\nDisconnected")

def main():
    # ------- Establecemos la conexion con el servidor mqtt -------#
    #Inicializamos las variables
    run_client = paho.Client()
    run_client.on_connect = on_connect
    run_client.on_message = on_message
    run_client.on_publish = on_publish
    run_client.on_subscribe = on_subscribe
    #Conectamos con el servidor
    run_client.username_pw_set(username="sendrone",password="S3nDr0n3")
    result=run_client.connect(MQTT_HOST,MQTT_PORT,MQTT_KEEPALIVE)
    if result==0:
        print("Connecting to mqtt server...")
        print("Host: %s" %MQTT_HOST)
        print("Port: %s" %str(MQTT_PORT))
    #Esperamos a que se establezca la conexion
    time.sleep(1)
    print("Enter an order [ON/OFF]")
    publish_msg(run_client, MQTT_TOPIC, 2)
    time.sleep(5)

# ----- Se ejecuta la función principal -------#
if __name__ == "__main__":
    main()
