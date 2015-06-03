A cross platform, C++ UDP-TCP Bridge.

Originally created to forward UDP TUIO (OSC) messages straight to TCP to be read from within Flash as binary data - but it actually works with any UDP data, not just TUIO or OSC.

This application forwards all incoming UDP messages straight to TCP without touching the data, just a straight forward.(Since version 0.2.1 there is the option to prefix the size of the packet before sending the data to comply with OSC / TCP specifications). This enables applications that don't support UDP (e.g. Flash) to receive the data. Since OSC / [TUIO](http://www.tuio.org) are generally sent via UDP, this enables Flash to recieve those messages in their raw binary form.

Settings can be edited from data/settings.xml.


## Isn't that what flosc does? ##

Nope. Flosc is specific to OSC, and converts the incoming data to XML and sends that over TCP. Flash then receives and parses that XML. While this is an ingenious solution, it isn't the most efficient way to do this currently as we now have flash.net.Socket's in flash allowing us to open a TCP Socket and read raw binary data.


## Can't I just send TUIO over TCP in CCV/tbeta? ##

While it's great that [CCV/tbeta](http://ccv.nuigroup.com/) sends data over TCP so flash clients can listen directly, and we can cut out the middle man (e.g. flosc), the data is still sent as XML, not as binary TUIO. In fact the data sent by CCV/tbeta isn't TUIO at all, it's just an XML string with the touch data.


## So how can I receive this data in Flash? ##

Using flash.net.Socket http://livedocs.adobe.com/flex/2/langref/flash/net/Socket.html


## Can I still use the NUIGroup Flash touchapi? ##

Yes, but you need to replace a few files. If you have a tracker sending TUIO over UDP (e.g. almost any tracker), and you use udp-tcp-bridge to forward that to TCP, the data being sent to TCP is raw TUIO, not an XML. So the [NUIGroup](http://www.nuigroup.com) Flash touchapi cannot parse it out of the box. Thankfully there are a few replacement classes (TUIO.as, TUIOCursor.as etc.) at http://www.touchgateway.com/?p=70 which do all the nessecary binary parsing and generates the same TUIO events so you can carry on using the rest of the NUIGroup touchapi. So if you overwrite those AS3 files with the ones in that zip, you can carry on using your flash multitouch apps seamlessly.


## Building the source ##
The application was written with [openframeworks 006](http://www.openframeworks.cc) and uses the addons:
  * ofxNetwork
  * ofxThread
  * ofxXMLSettings


## Also worth mentioning ##
[flosc](http://benchun.net/flosc/) - Java app which forwards incoming  OSC (UDP) messages to XML (over TCP).

[oscar](http://www.todo.to.it/blog/oscar/) - Native max osx app that does the same as above (osc->xml).

[toxy](http://code.google.com/p/tuio-tcp-proxy/) - native windows/linux app that forwards incoming TUIO to TCP in a custom binary format (as far as I understand).

[touchgateway](http://www.touchgateway.com/) - Java app which forwards all incoming UDP to TCP. Similar to my udp-tcp-bridge, but in Java.