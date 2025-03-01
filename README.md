# terrarium-mister

### Information
This automated terrarium mister moderates the humidity of a small enclosure, which is especially beneficial for reptiles. After not being able to find a product that accomplishes my needs on the market, I decided to build a prototype myself. Instead of foggers that constantly run, this mister self-regulates a specified humidity in the enclosure with a DHT11 sensor. Depending on how you decide to set it up, it can be used inside or outside of the enclosure.

### Hardware Needed
* ESP8266 or ESP32
* L298N motor driver
* DHT11 temperature/humidity sensor
* Jumper wires
* 5V/12V power supply (both work with the L298N)
* 5V diaphragm water pump
* Silicon or polyethylene tubing 1/4" outer diameter
* 1/4" mister attachment

### Hardware Setup
Hardware setup will vary depending on chosen power supply and whether you use an ESP32/ESP8266. For an ESP8266 and 12 volt power supply, connect pins as follows.
1. Pin D5 to IN1
2. Pin D6 to IN2
3. Pin D1 to ENA
4. Pin D2 to DHT11 data pin
5. 12V positive and negative from power supply to 12V and GND on L298N
6. 5V from L298N to ESP Vin (also ground the ESP!)
* **Important Note:** Do **not** plug in the USB port while the ESP board is powered by an external supply.
7. 3v3 on ESP to Vin on the DHT11 (also ground the DHT11!)
8. OUT1 on the L298N to the positive terminal of the pump
9. OUT2 on the L298N to the negative terminal of the pump
#### Schematic
![alt-text](https://github.com/rayk04/terrarium-mister/blob/main/Mister%20schematic.png)
### Software Setup
You will need to make some modifications to the main code before flashing it to your ESP board.
1. Set "YOUR_SSID" to your network SSID.
2. Set "YOUR_PASSWORD" to your network password.
3. Run "ipconfig" in your command prompt and adjust the local_IP, gateway, and subnet objects as needed.
4. Modify the motorSpeed variable if your pump does not draw water or draws too much. 130 is a safe starting point.
5. Upload the code to the ESP board through Arduino IDE.
6. Enter the IP address you specified in step 3 into a web browser to view the interface and set the humidity.
### Additional Modifications
* You can 3D print an enclosure for the components for a cleaner look. I've included the STL file in the repository.
### Demo
See the video for a demonstration of the device.
* [Video link](To be added)
