import network
import socket
from time import sleep
from picozero import pico_temp_sensor, pico_led
from machine import Pin

ssid = 'DukeOpen'
password = ''
hallSenPin = 2
motorPin1 = 4
motorPin2 = 5

motor1a = Pin(motorPin1, Pin.OUT)
motor1b = Pin(motorPin2, Pin.OUT)
hall_sens = Pin(hallSenPin, Pin.IN)

def connect():
    #Connect to WLAN
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while wlan.isconnected() == False:
        print('Waiting for connection...')
        sleep(1)
    print("Successfully connected to",ssid)
    print("IP: "+wlan.ifconfig()[0])
    
def up():
    motor1a.high()
    motor1b.low()
    while hall_sens.value()!=0:
       continue
    stop()
   
def down():
   motor1a.low()
   motor1b.high()
   while hall_sens.value()!=0:
       continue
    stop()
   
def stop():
    motor1a.low()
    motor1b.low()

# try:
#     connect()
# except KeyboardInterrupt:
#     machine.reset()

up()
