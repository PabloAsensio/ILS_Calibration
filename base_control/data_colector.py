from read_json import json2csv
import paho.mqtt.client as paho
import geojson
import pandas as pd

READING_TOPIC = "EXAMPLE_TOPIC"
READING_TOPIC_2 = "ENVIO_DE_DRON"

def on_connect(client, userdata, flags, rc):
    print("Connected (%s)" % client._client_id)
    client.subscribe(topic=READING_TOPIC)
    client.subscribe(topic=READING_TOPIC_2)


def on_message(client, userdata, message):
    global run
    if message.topic == READING_TOPIC_2:

        run = run + 1
        geojson_raw = geojson.loads(message.payload)
        json_properties = geojson_raw["properties"]
        json2csv(json_properties, run)
        print("qos %s" % message.qos)

    if message.topic == "status":

        if message.payload == "-4":
            print("Overflow")
        if message.payload == "0":
            print("SDR not running. Killing program ...")

    if message.topic == READING_TOPIC:

        print(message.topic)
        print(message.payload)



def main():
    global run
    run = 0
    client = paho.Client(client_id="pablo", clean_session=False)
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(username="receivedrone", password="Rec3iv3Dr0n3")
    client.connect(host="193.201.107.70", port=1883)
    client.loop_forever()


if __name__ == "__main__":
    main()
