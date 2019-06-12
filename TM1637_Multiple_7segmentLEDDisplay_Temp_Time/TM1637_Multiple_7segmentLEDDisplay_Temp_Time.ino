#include <Arduino.h>
#include <TM1637Display.h>

#include <NTPClient.h>
// change next line to use with another board/shield
#include <WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

#include<dht.h>

dht DHT;
#define DHT11_PIN 5


const char *ssid     = "NETGEAR84";
const char *password = "welcome2ibm";

String timestr;
String hourStamp, minStamp;
String minsec;
char buf[20];

WiFiUDP ntpUDP;

// Note the 19800 = 5.5 hrs X 3600 in the third argument.

NTPClient timeClient(ntpUDP,"europe.pool.ntp.org", 19800, 60000);

// Module connection pins (Digital Pins)
#define CLK 18
#define DIO 19

#define CLK_2 22
#define DIO_2 23

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   500

const uint8_t SEG_DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};

TM1637Display display(CLK, DIO);

TM1637Display display2(CLK_2, DIO_2);

void setup()
{
  
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop()
{
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x01);
  display2.setBrightness(0x0f);

  // All segments on, this will blink 8888. Commenting it.
  //display.setSegments(data);
  //display2.setSegments(data);
  
  delay(TEST_DELAY);

  // Selectively set different digits
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  //display.setSegments(data);

  data[0] = display2.encodeDigit(0);
  data[1] = display2.encodeDigit(1);
  data[2] = display2.encodeDigit(2);
  data[3] = display2.encodeDigit(3);
  //display2.setSegments(data);

  /* calculate the time now and display */
  /*************************************/
  
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  timestr =   timeClient.getFormattedTime();
  int splitT = timestr.indexOf(":");
  hourStamp = timestr.substring(0, splitT);
  Serial.print("Hour ");
  Serial.print (atoi(hourStamp.c_str()));
  minsec= timestr.substring ( splitT+1, timestr.length());
  //Serial.print (" minsec = ");
  //Serial.print ( minsec );
  splitT = minsec.indexOf(":");
  minStamp = minsec.substring(0, splitT);
  Serial.print(" Min ");
  Serial.println (minStamp.c_str());
  sprintf (buf, "%s%s", hourStamp.c_str() , minStamp.c_str() );
  Serial.println ( buf );

  
  int x = atoi( buf);
  display2.showNumberDec(x, true); // Expect numberin decimals, e.g: 0301 
  delay(TEST_DELAY);


  /******** . Calculate the Temperature now *************/
  int chk = DHT.read11(DHT11_PIN);

  Serial.println(" Humidity " );

  Serial.println(DHT.humidity, 1);

  Serial.println(" Temparature ");

  Serial.println(DHT.temperature, 1);
  display.showNumberDec ( DHT.temperature, true );

  delay(1000);
  
  
  /*
  for(k = 3; k >= 0; k--) {
	display.setSegments(data, 1, k);
	delay(TEST_DELAY);
	}
  

  display.clear();
  display.setSegments(data+2, 2, 2);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+2, 2, 1);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+1, 3, 1);
  delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  display.showNumberDec(0, false); // Expect: ___0
  delay(TEST_DELAY);
  display.showNumberDec(0, true);  // Expect: 0000
  delay(TEST_DELAY);
	display.showNumberDec(1, false); // Expect: ___1
	delay(TEST_DELAY);
  display.showNumberDec(1, true);  // Expect: 0001
  delay(TEST_DELAY);
  display.showNumberDec(301, false); // Expect: _301
  delay(TEST_DELAY);
  display.showNumberDec(301, true); // Expect: 0301
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(14, false, 2, 1); // Expect: _14_
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(4, true, 2, 2);  // Expect: 04__
  delay(TEST_DELAY);
  display.showNumberDec(-1, false);  // Expect: __-1
  delay(TEST_DELAY);
  display.showNumberDec(-12);        // Expect: _-12
  delay(TEST_DELAY);
  display.showNumberDec(-999);       // Expect: -999
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(-5, false, 3, 0); // Expect: _-5_
  delay(TEST_DELAY);
  display.showNumberHexEx(0xf1af);        // Expect: f1Af
  delay(TEST_DELAY);
  display.showNumberHexEx(0x2c);          // Expect: __2C
  delay(TEST_DELAY);
  display.showNumberHexEx(0xd1, 0, true); // Expect: 00d1
  delay(TEST_DELAY);
  display.clear();
  display.showNumberHexEx(0xd1, 0, true, 2); // Expect: d1__
  delay(TEST_DELAY);
  
	// Run through all the dots
	for(k=0; k <= 4; k++) {
		display.showNumberDecEx(0, (0x80 >> k), true);
		delay(TEST_DELAY);
	}

  // Brightness Test
  for(k = 0; k < 4; k++)
	data[k] = 0xff;
  for(k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }
  
  // On/Off test
  for(k = 0; k < 4; k++) {
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    display.setBrightness(7, true); // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);  
  }

 
  // Done!
  display.setSegments(SEG_DONE);
  delay(2000);
  */
}
