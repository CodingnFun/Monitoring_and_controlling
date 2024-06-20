# Monitoring_and_controlling
This code utilizes esp32 wifi module to read sensors data (temperature, humidity, LUX and gas) using respective sensors (DHT11, BH1750, MQ-5 ) 
and sending them to thingspeak for monitoring. also this code includes controlling of three devices, for this we have used three fields of thingspeak channel, 
data is sent from MIT App in the form of 1 or 0 to thingspeak fields, esp32 is going to read those fields and will turn on/off devices accordingly.
