from flask import Flask, request, json
import socket
import os
import subprocess
import re
import threading
import time
import webbrowser
import random as r

from flask.helpers import send_from_directory

app = Flask(__name__)

timerValue = "00:00"
currentState = "KeyOff"
errorCode = "0"
batteryVoltage = "12.4"
sendCommand = None
waterLevel = 100.0

batteryValueIndex = 0
batteryVoltageByDay = {}

v = 14.5
vi = 0.1


@app.route("/timer/<value>")
def timer(value):
    global sendCommand
    global timerValue

    timerValue = value
    sendCommand = f"cmd:ChangeTimer:{timerValue}"

    return value

@app.route("/generatrice/<value>")
def generatrice(value):
    global sendCommand
    global timerValue

    timerValue = value
    sendCommand = f"cmd:{value}Gen"

    return value

@app.route("/update")
def updateApp():
    global timerValue
    global currentState
    global errorCode
    global batteryVoltage
    global sendCommand
    global waterLevel


    global v
    global vi

    if v <= 6.0:
        vi = 0.1
    
    if v >= 14.5:
        vi = -0.1
    
    v = v + vi

    data = {
        'state':currentState,
        'timer':timerValue,
        'error':errorCode,
        'water':waterLevel,
        'voltage':v
    }
    response = app.response_class(
        response=json.dumps(data),
        status=200,
        mimetype='application/json'
    )
    response.headers.add('Access-Control-Allow-Origin', '*')
    return response

@app.route("/waterlevel/<value>")
def setWaterLevel(value):
    global waterLevel
    waterLevel = value


    response = app.response_class(
        response=json.dumps(value),
        status=200,
        mimetype='application/json'
    )
    response.headers.add('Access-Control-Allow-Origin', '*')

    return response

def threadSerialUSB():
    global currentState
    global errorCode
    global batteryVoltage
    global sendCommand
    global timerValue

    #import serial
    #ser = serial.Serial('device name ex: /dev/tty/ACM0', 9600)
    while True:
        #readedText = ser.readline()
        #print(readedText)
        readedText = ''
        if readedText.startswith('cmd:State:'):
            currentState = readedText.replace('cmd:State:', '')
        
        if readedText.startswith('cmd:Fail:'):
            errorCode = readedText.replace('cmd:Fail:', '')
        
        if readedText.startswith('cmd:Battery:'):
            batteryVoltage = readedText.replace('cmd:RemainingTime:', '')
        
        if readedText.startswith('cmd:Time:'):
            timerValue = readedText.replace('cmd:Battery:', '')

        if sendCommand:
            #ser.writelines(sendCommand)
            sendCommand = None
            time.sleep(1)

if __name__ == "__main__":
    #mythread = threading.Thread(name='the_name', target=open_Browser, args=())
    #mythread.start()
    #arduinoThread = threading.Thread(name='ArduinoGetData', target=threadSerialUSB, args=())
    #arduinoThread.start()
    app.run(host="0.0.0.0")
