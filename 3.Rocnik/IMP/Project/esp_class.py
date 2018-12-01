'''
Script for esp8266 in micropython.

Author: Robert Kolcun, xkolcu00
'''
import network
import socket
import json
import urandom
from machine import Pin
from onewire import OneWire
from ds18x20 import DS18X20


class ESPController():

    SSID = 'HT17' #'UPCEE375A7'
    PASS = '1234567890' #'wxp57xbedmBZ'

    HOST = '192.168.43.122'
    PORT = 12345

    def __init__(self, pin=5):
        random_number = None
        try:
            urandom.seed(int(open('seed_file', 'r').read()))
        finally:
            file = open('seed_file', 'w')
            random_number = urandom.getrandbits(20)
            file.write(str(random_number))
            file.close()

        self.temp_pin = pin
        self.data = {
            'temp': None,
            'id': random_number,
        }

        self.__setup_pin()
        self.__setup_wifi()

    def __setup_pin(self):
        '''
        Initalize pin for measuring temperature
        '''
        self.temp_controller = DS18X20(OneWire(Pin(self.temp_pin)))
        self.temp_scan = self.temp_controller.scan()[0]

    def __setup_wifi(self):
        '''
        Initalize wifi connection
        '''
        sta_if = network.WLAN(network.STA_IF)
        sta_if.active(True)
        ap_if = network.WLAN(network.AP_IF)
        ap_if.active(False)

        sta_if.connect(self.SSID, self.PASS)
        while not sta_if.isconnected():
            pass

        print("Network2: " + str(sta_if.ifconfig()))

    def get_temp(self):
        '''
        Return temperature from sensor
        '''
        self.temp_controller.convert_temp()
        return self.temp_controller.read_temp(self.temp_scan)

    def send_temp(self):
        '''
        Write temperature value into socket
        '''
        self.data['temp'] = self.get_temp()

        print('Sending temp: ' + str(self.data['temp']))

        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((self.HOST, self.PORT))
            sock.write(json.dumps(self.data).encode('UTF-8'))
        except Exception as err:
            print("Sending error: " + str(err))

        print("Data sent")
