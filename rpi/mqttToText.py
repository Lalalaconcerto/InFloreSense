counter = 0
messageTriplet = ''

import paho.mqtt.client as mqttClient
import time

def on_connect(client, userdata, flags, rc):

    if rc == 0:

        print("Connected to broker")

        global Connected                #Use global variable
        Connected = True                #Signal connection

    else:

        print("Connection failed")

def on_message(client, userdata, message):
    #int_msg = int.from_bytes(message.payload, "little")
    #print ("Message received: "  + str(int_msg))
    global messageTriplet
    global counter
    
    if counter == 3:
        with open('/home/pi/Desktop/test.txt','r+') as f:
            f.truncate(0)
            f.write(messageTriplet) #todo fix
        counter = 0
        messageTriplet = ''
    unformattedPayload = str(message.payload)
    formattedPayload = unformattedPayload[2:-1]
    messageTriplet = messageTriplet + str(formattedPayload) + ','
    counter += 1

Connected = False   #global variable for the state of the connection

broker_address= "mqtt.cetools.org"  #Broker address
port = 1884                         #Broker port
user = "student"                    #Connection username
password = "ce2021-mqtt-forget-whale"            #Connection password

client = mqttClient.Client("Python")               #create new instance
client.username_pw_set(user, password=password)    #set username and password
client.on_connect= on_connect                      #attach function to callback
client.on_message= on_message                      #attach function to callback
client.connect(broker_address,port,60) #connect
client.subscribe("student/CASA0021/InfloreSense/#") #subscribe
client.loop_forever() #then keep listening forever