'''
This file is executed on every boot
(including wake-boot from deepsleep)

Author: Robert Kolcun, xkolcu00
'''
import gc
import time
from esp_class import ESPController


gc.collect()
esp_con = ESPController()

# Measure temperature every 5 seconds
while True:
    esp_con.send_temp()
    time.sleep(5)
