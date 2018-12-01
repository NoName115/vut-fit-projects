AMPY = ampy --port /dev/ttyUSB0
RATE = 115200

ls:
	$(AMPY) ls

getboot:
	$(AMPY) get boot.py

boot:
	$(AMPY) put boot.py

getesp:
	$(AMPY) get esp_class.py

esp:
	$(AMPY) put esp_class.py

term:
	picocom /dev/ttyUSB0 -b $(RATE)
