from flask import Flask, jsonify, request
import time
import paho.mqtt.client as mqtt
import json
import ast
import os
esps={}
app = Flask(__name__)
mqtt_ip='192.168.1.120'
class SingletonMeta(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            instance = super().__call__(*args, **kwargs)
            cls._instances[cls] = instance
        return cls._instances[cls]
class SingletonMeta_cont(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            instance = super().__call__(*args, **kwargs)
            cls._instances[cls] = instance
        return cls._instances[cls]
class Singleton(metaclass=SingletonMeta):
    def __init__(self) -> None:
       self.dic=[]
    def reset(self):
       self.dic=[]
class baloes():
    def __init__(self,id,sinal,efeito,status,bateria):
        self.id=id
        self.efeito=efeito
        self.signal=sinal
        self.status=status
        self.battery=bateria
    def set_status(self,status):
        self.status=status
    def set_id(self,id):
        self.id=id
    def set_sinal(self,sinal):
        self.signal=sinal
    def set_efeito(self,efeito):
        self.efeito=efeito
    def get_id(self):
        return self.id
    def get_sinal(self):
        return self.signal
    def get_efeito(self):
        return self.efeito
    def get_status(self):
        return self.status
class controller(metaclass=SingletonMeta_cont):
    def __init__(self):
        self.conjunto={}
        self.conjunto['F4:CF:A2:EF:9E:A8']= baloes('F4:CF:A2:EF:9E:A8',-100,-1,0,0)
    def verificar(self,dic):
        l=[]
        for i in dic:
            l.append(i['id'])
            if i['id'] in self.conjunto:
                self.conjunto[i['id']].set_status(i['status'])
                self.conjunto[i['id']].set_efeito(i['efeito'])
                self.conjunto[i['id']].set_sinal(i['sinal'])
                self.conjunto[i['id']].set_bateria(i['bateria'])
            else:
                self.conjunto[i['id']]=baloes(dic[i]['id'],dic[i]['sinal'],dic[i]['efeito'],dic[i]['status'],dic[i]['bateria'])
        diferentes = list(set(l) ^ set(self.conjunto))
        for j in diferentes:
            self.conjunto[j].set_status(0)
            self.conjunto[j].set_efeito(-1)
            self.conjunto[j].set_sinal(-99)
        return self.conjunto
def on_publish(client, userdata, mid):
    print("message published")
def on_connect(client, userdata, flags, rc):
   global flag_connected
   flag_connected = 1
   client_subscriptions(client)
def callback_rpi_broadcast(client, userdata, msg):
    list=Singleton()
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
    list.dic.append(esps[result["esp"]])
def client_subscriptions(client):
    client.subscribe("esp32/#")
    client.subscribe("rpi/broadcast")
    client.subscribe("esp32/sensor1")
    client.subscribe("PrototipagemIFPB1")
def on_disconnect(client, userdata, rc):
   global flag_connected
   flag_connected = 0
def recive():
    client = mqtt.Client("rpi_client4")
    flag_connected = 0
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.message_callback_add('PrototipagemIFPB1',callback_rpi_broadcast)
    client.connect(mqtt_ip,1883)
    client.loop_start()
    client_subscriptions(client)
    client.disconnect()
def pub(acao):
    client = mqtt.Client("rpi_client3")
    client.on_publish = on_publish
    client.connect(mqtt_ip,1883)
    client.loop_start()
    k='api'
    msg =str(k)
    pubMsg = client.publish(
            topic='PrototipagemIFPB',
            payload=msg.encode('utf-8'),
            qos=0,
        )
    pubMsg.wait_for_publish()
    print(pubMsg.is_published())
    client.disconnect()
def efeito(acao):
    client = mqtt.Client("rpi_client3") 
    client.on_publish = on_publish
    client.connect(mqtt_ip,1883)
    client.loop_start()
    k=acao
    msg =str(k)
    pubMsg = client.publish(
            topic='efeito',
            payload=msg.encode('utf-8'),
            qos=0,
        )
    pubMsg.wait_for_publish()
    print(pubMsg.is_published())
    client.disconnect()
def efeito_unique(id,effect):
    client = mqtt.Client("rpi_client3")
    client.on_publish = on_publish
    client.connect(mqtt_ip,1883)
    client.loop_start()
    k=str(effect)
    msg =str(k)
    pubMsg = client.publish(
            topic=str(id),
            payload=msg.encode('utf-8'),
            qos=0,
        )
    pubMsg.wait_for_publish()
    print(pubMsg.is_published())
    client.disconnect()
def recv(acao):
    recive()
    for i in range(4):
        time.sleep(1)
        recive()
#Pra escutar a quantidade de balões
@app.route("/balloons")
def qnt():
    j=[]
    a=Singleton()
    a.reset()
    if os.path.isfile("esps.json"):
        os.remove("esps.json")
    cont=controller()
    time.sleep(1)
    try:
        with open("esps.json","r") as outfile:
            json_obj=json.load(outfile)
        a.dic.append(json_obj)
    except:
        pass
    pub("")
    f= (cont.verificar(a.dic))
    for w in f:
        j.append(f[w].__dict__)
    return jsonify(j)
#Para mandaremos o efeito selecionado    
@app.route("/balloons/effect")
def set_efeito_ballon():
    if request.is_json:
        re=request.json
    else:
        re=request.form
    id=re["id"]
    efeito=re["efeito"]
    efeito_unique(id,efeito)
@app.route("/balloons/effect/all")
def set_efeito_all():
    if request.is_json:
        re=request.json
    else:
        re=request.form
    efect=re["efeito"]
    efeito(efect)
    return 200
#Para escutarmos as mudanças de efeito
@app.route("/baloes/efeito/get")
def get_efeito():
    pass
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=False)