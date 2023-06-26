import paho.mqtt.client as mqtt
import json
import time
import ast
def on_connect(client, userdata, flags, rc):
    client.subscribe("F0:08:D1:CC:6F:08")
    client.subscribe("PROTORIPAGEMIFPB1")

def on_message(client, userdata, msg):
    a=str(msg.payload.decode()).replace("'",'"')
    esps={}
    b="'"
    b+=a[1:-1]
    b+="'"
    result = ast.literal_eval(b)
    result = ast.literal_eval(result)
    if result["esp"] in esps:
        esps[result["esp"]]['id']=result['esp']
        esps[result["esp"]]['status']=1
        esps[result["esp"]]['efeito']=result['efeito']
        esps[result["esp"]]['sinal']=result['sinal']
        esps[result["esp"]]['bateria']=result['bateria']
    else:
        esps[result["esp"]] = {}
        esps[result["esp"]]['id']=result['esp']
        esps[result["esp"]]['status']=1
        esps[result["esp"]]['efeito']=result['efeito']
        esps[result["esp"]]['sinal']=result['sinal']
        esps[result["esp"]]['bateria']=result['bateria']
    json_object=json.dumps(esps[result["esp"]])
    with open("esps.json", "w") as outfile:
        outfile.write(json_object)

def on_publish(client, userdata, mid):
    print("Mensaje publicado con ID: " + str(mid))

def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Desconexión inesperada")

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message
client.on_publish = on_publish
client.on_disconnect = on_disconnect

broker = "192.168.1.120"
port = 1883

client.connect(broker, port, 60)

client.loop_start()

while True:
    pass
