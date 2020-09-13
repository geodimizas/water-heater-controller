# water-heater-controller - IOT project

Decription:
In this project you are able to control a hardaware device from your mobile through wifi (in both inside and outside of your local network).
The mobile app (designed with MIT app invertor) sending a signal through ddns server (noip.com) to your router (which is setted to port-forwarding) the request in esp8266.

1) Hardware:
Open "WaterHeaterController.ino" and upload it to your esp8266. In line 7,8 enter your wife name and password and load it to your esp.
When the esp receive the message "/gpio/0" turns on the D6 for 0.5 seconds, for "/gpio/1" turns on the D4 for 0.5 seconds, 
for "/gpio/2" reads the temperature from the sensor and it send it to the back.
The port D6 is connected to the relay for turning on the heater and D4 to the relay for turning of (for more details check schematic.jpg)

2) Software:
From https://appinventor.mit.edu/ open the file "app_controller.aia". 
In the blocks area change the line "http://192.168.1.2:8000/gpio/xx" x=1,2,3 
with your local network ip and your esp port (for example my local ip is 192.168.1.2 and the esp port is 8000) 
and "http://dimihome.ddns.net:8000/gpio/x" x=1,2,3 with your ddns domain (or subdomain) and your esp port
 
3) Network setings:
In order to set up your network 
First of all you have to set your esp device to a static port. 
Also you have to set your router port forwarding to from a specific port to your esp static ip.
 
 -HOW IT WORKS?
 -Your mobile app sends the "8000/gpio/x" message to your ddns server. This server maτches your domain/subdomain to your network ip.
 When your router receives this request from the ddns server forwarding the message "gpio/x" to your local port 8000 which is setted from your router to your esp device.
