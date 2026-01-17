from microdot import Microdot
import network
import socket
from time import sleep
import ubinascii
from machine import Pin

app = Microdot()
ssid = 'AP-HalfMasters'
password = '987654321'
wlan = network.WLAN(network.STA_IF)
authors = "Wanghley Soares Martins (@wanghley)"
duke = "Duke University - Pratt School of Engineering"

#Pins
hallSenUpPin = 2
hallSenMidPin = 3
hallSenBotPin = 5
motorPin1 = 4
motorPin2 = 5

motor1a = Pin(motorPin1, Pin.OUT)
motor1b = Pin(motorPin2, Pin.OUT)
hallSenUpPin = Pin(hallSenUpPin, Pin.IN)


@app.route('/')
def index(request):
    return """
    IP: %s
    MASK: %s
    MACADDR: %s
    
    DEVELOPER: %s
    %s
    """ % (wlan.ifconfig()[0],wlan.ifconfig()[1],format_mac_addr(ubinascii.hexlify(wlan.config('mac')).decode()),authors,duke)
    
@app.post('/up')
def goUp(request):
    __up__()

@app.route('/down',methods=['POST'])
def goUp(request):
    __down__()
    
@app.route('/stop',methods=['POST'])
def goUp(request):
    __stop__()
    
@app.errorhandler(404)
def not_found(request):
    return 'Not found'

@app.errorhandler(RuntimeError)
def runtime_error(request, exception):
    return 'Runtime error'

def __setup__():
    #Connect to WLAN
    wlan.active(True)
    wlan.connect(ssid, password)
    while wlan.isconnected() == False:
        print('Waiting for connection...')
        sleep(1)
    print("Successfully connected to",ssid)
    print("IP: "+wlan.ifconfig()[0])
    print("IP: "+wlan.ifconfig()[1])
    
def format_mac_addr(addr):

    mac_addr = addr
    mac_addr = mac_addr.upper()
    
    new_mac = ""
    
    for i in range(0, len(mac_addr),2):
        #print(mac_addr[i] + mac_addr[i+1])
        
        if (i == len(mac_addr) - 2):
            new_mac = new_mac + mac_addr[i] + mac_addr[i+1]
        else:
            new_mac = new_mac + mac_addr[i] + mac_addr[i+1] + ":"
    return new_mac

def __up__():
    motor1a.high()
    motor1b.low()
    while hallSenUpPin.value()!=0:
       continue
    __stop__()
    
def __down__():
    motor1a.low()
    motor1b.high()
    while hallSenUpPin.value()!=0:
        print(hallSenUpPin.value())
        continue
    __stop__()

def __stop__():
    motor1a.low()
    motor1b.low()

if __name__ == "__main__":
    try:
        __setup__()
    except KeyboardInterrupt:
        machine.reset()
        
    app.run(host='0.0.0.0',debug=True,port=80, ssl=None)


