#Introduction

This arduino GPRS/GPS tracking system was originally inspired by the blog post at Tronix Stuff 
(http://tronixstuff.wordpress.com/2011/01/19/tutorial-arduino-and-gsm-cellular-part-one/).  The
tracker will obtain its geo-coordinates and upload them to a server running the provided python
script and store them in a MySQL table.

In order for this code to work you will need the following parts

* Arduino UNO - http://www.sparkfun.com/products/11021
* GPS Shield - http://www.sparkfun.com/products/10710
* EM-406A GPS unit - http://www.sparkfun.com/products/465
* SM5100B GSM Shield - http://www.sparkfun.com/products/9607
* Quad Band Cellular Antenna - http://www.sparkfun.com/products/8347
* 1 green LED
* 1 red LED
* 2 220 ohm resitors

# Configuring arduino

Open the sketch in the Arduino IDE.  By default the software is configured to send its coordinates
every 10 seconds.  If you would like to change that interval edit line 57 and change the value for
SEND_DELAY.  This value is in milliseconds.

Second, edit line 238.  The line appears as follows

    sendATCommand("AT+SDATACONF=1,\"TCP\",\"0.0.0.0\",81", 1000);

change "0.0.0.0" to the IP address of the machine that will be running the python script.  Attach your
red LED and 1 resistor to pin 12 of the Arduino.  Do the same with the green LED, attaching it to pin
13.

NOTE: If you run into issues running on battery power, disconnect the 3.3v and 5v pins of the gps from
the stack.  Then run a jumper from pin 9 to 5v on the GPS shield

# Configuring your server

First create a file called config.py that exists in the same directory as the server script.  It should 
look as followed:

    hostname = 'localhost'
    database = 'your database'
    username = 'user'
    password = 'password'

The server script requires the twisted python package to run.  Install via your prefered method.  After
you have configured your python environment import the sql script to create your database:

    mysql -u username -p mydatabase < tracker.sql

Once it is configured you can run the server with the following command

    python server.py

# Diagnostics

This sketch provides some basic diagnostics for the tracker.  When transmitting GPS data the green LED
will light up until the connection has been terminated.  It also provides the following diagnostic codes:

* 3 flashes - SIM Unavailable
* 4 flashes - Could not connect to GPRS network
* 5 flashes - Could not connect to cellular network
* 6 flashes - Could not connect to host
* 7 flashes - Unknown error

NOTE: once an error is triggered the arduino will need to be reset
