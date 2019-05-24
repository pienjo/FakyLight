# FakyLight

Fancy backlight for an LG TV. 

This is very much a work-in-progress. The hardware consists of several parts:

* An LG tv (I have an LG 42UB820V), connected to a (W)LAN. There is no discovery, you'll have to make sure it has a fixed IP and adjust the source accordingly.
* A Linux system somewhere to decode the images from the tv (doesn't have to be powerful, a raspberry pi suffices)
* A string of WS2812s, attached to the back of said TV. Because this is very much a work-in-progress, the dimensions are currently a compile-time constant: 25 LEDs along the top, 14 along the sides. Pixels start in the lower left corner - I think...
* Something that connects with the WS2812 string. Either a ESP8266 running NodeLua (I used a WEMOS D1 mini), or something like a Raspberry pi (which can be the same pi that runs the decoder).

If you decide to use a Raspberry Pi: Connect the WS2812s to the SPI MOSI pin, and use the ''stream_gpio'' binary. This may not work reliably with any ARM SoC, as it requires a decent DMA-driven SPI hardware module. The raspberry pi has one, the orange pi doesn't.

Decoder resides in the 'Faky++' directory (good luck), the NodeLua code for the ESP8266 can be found in ''NodeMCU_client''.

Note that this software uses a badly documented API on your TV, and currently uses a fixed authentication key. You might have to discover what the key for your system is. It's certainly not unique to every TV, but it might vary between models. Easiest way to figure it out is by using the "LG remote" app on your phone - use the code that is shown on the screen during that authentication process.

Obligatory sample screenshots:
![img_20181101_210556832](https://user-images.githubusercontent.com/12941778/47941599-0eb39400-deef-11e8-8b68-4b6ba863c647.jpg)
![img_20181101_210624652](https://user-images.githubusercontent.com/12941778/47941600-0f4c2a80-deef-11e8-8748-03cbde5573f9.jpg)
[I created a video](https://www.youtube.com/watch?v=N8kVniX7-_A) to show the latency I get using a WS2866 to serve the LEDs. It gets a hair tighter if you use a Raspberry Pi.
