# esp8266-ddns
Update DDNS using esp-01 + usb adaptor

If your router support limit DDNS provider and you want to use another free DDNS provider (such as Duckdns, Dynu)
You may think you have to turn on a PC or RPI 24 hours to update DDNS ip when your ip changed
another solution is to use a small and low power consuming ESP-01s board instread of using more power consuming like PC or RPI

Hardware:
1. ESP-01s
2. USB-TTL for ESP-01 *

I recommend "ESP LINK" USB-TTL for esp-01 it will automatic switch between working mode and programming mode.
or use USB-TTL with onboard switch to manually select working or programming mode.
If you use USB-TTL without switch you will have to connect GPIO0 to GND by hand before power on ESP01 to enter programming mode.
I suggest to soldering a switch to the board between gpio0 and Gnd for easy to select mode.
