/*
              Location Display
   In this example we are using ESP8266 wifi module and OLED display
  to display location using your IP address.

*/



#include "U8glib.h"
#include <ELClient.h>
#include <ELClientRest.h>
char buff[32];
char buff2[32];
String bitcoin_value = "";
String bitcoin_value_change = "";
unsigned long lastUpdated = millis();
unsigned long samplingTime = 3600;

#define down_arrow_width 45
#define  down_arrow_height 45
//static unsigned char  down_arrow_bits[] = {
static unsigned char  down_arrow_bits[] U8G_PROGMEM = {
   0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0xFF, 0x1F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 
  0xC0, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 
  0x00, 0xFC, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0x07, 0x00, 
  0x00, 0xF8, 0xFF, 0xFF, 0x43, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0x01, 0x00, 
  0x00, 0xE0, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x7F, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0x00, 0x00, 0x00,};


#define up_arrow_width 45
#define  up_arrow_height 45
//static unsigned char  up_arrow_bits[] = {
static unsigned char  up_arrow_bits[] U8G_PROGMEM = {
     0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0x07, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0xC0, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x7F, 0x00, 0x00, 
  0x00, 0xE0, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0x01, 0x00, 
  0x00, 0xF8, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x03, 0x00, 
  0x00, 0xFC, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 
  0x80, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 
  0x80, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0x1F, 0x00, 0x00, };
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
// Initialize a connection to esp-link using the normal hardware serial port both for
// SLIP and for debug messages.
ELClient esp(&Serial, &Serial);

// Initialize a REST client on the connection to esp-link
ELClientRest rest(&esp);

boolean wifiConnected = false;

// Callback made from esp-link to notify of wifi status changes
// Here we print something out and set a global flag
void wifiCb(void *response)
{
  ELClientResponse *res = (ELClientResponse*)response;
  if (res->argc() == 1)
  {
    uint8_t status;
    res->popArg(&status, 1);

    if (status == STATION_GOT_IP)
    {
      Serial.println("WIFI CONNECTED");  //Wifi gets connected at this place
      wifiConnected = true;
    }
    else
    {
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_timB10);
        u8g.drawStr(1, 13, "WIFI DISCONNECTED!!");
      } while ( u8g.nextPage() );
      Serial.print("WIFI NOT READY: ");//Wifi not connected,check connection
      Serial.println(status);
      wifiConnected = false;


    }
  }
}

void setup()
{
  Serial.begin(9600);   // the baud rate here needs to match the esp-link config
  Serial.println("EL-Client starting!");

  u8g.setColorIndex(1);
  // Sync-up with esp-link, this is required at the start of any sketch and initializes the
  // callbacks to the wifi status change callback. The callback gets called with the initial
  // status right after Sync() below completes.
  esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
  bool ok;
  do
  {
    ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
    if (!ok) Serial.println("EL-Client sync failed!");
  } while (!ok);
  Serial.println("EL-Client synced!");

  // Get immediate wifi status info for demo purposes. This is not normally used because the
  // wifi status callback registered above gets called immediately.
  esp.GetWifiStatus();
  ELClientPacket *packet;
  if ((packet = esp.WaitReturn()) != NULL)
  {
    Serial.print("Wifi status: ");
    Serial.println(packet->value);
  }

  // Set up the REST client to talk to idiotware.herokuapp.com, this doesn't connect to that server,
  // it just sets-up stuff on the esp-link side
  int err = rest.begin("api.thingspeak.com");
  if (err != 0)
  {
    Serial.print("REST begin failed: ");
    Serial.println(err);
    while (1) ;
  }
  Serial.println("EL-REST ready");

  //get_bitcoinValue();




}

void loop()
{

  esp.Process();

  if ((millis() - lastUpdated) > samplingTime)
  {
    //get_ETHValue();
    get_ETHChange();
    lastUpdated = millis();
  }

}


void get_ETHValue()
{
  sprintf(buff, "/apps/thinghttp/send_request?api_key=5XPSYZB0Q363X1KF");
  // process any callbacks coming from esp_link
  //esp.Process();


  // if we're connected make an HTTP request
  if (wifiConnected)
  {
    // Request /utc/now from the previously set-up server
    rest.get((const char*)buff);

    char response[20];
    uint16_t code = rest.waitResponse(response, 20);
    if (code == HTTP_STATUS_OK)    //check for response for HTTP request
    {
      Serial.println("ARDUINO: GET successful:");
      Serial.println(response);
      bitcoin_value = response;
      String bitcoinValue = bitcoin_value.substring(0, 6);
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_timB10);
        u8g.drawStr(1, 10, "ETH value:");
        u8g.setFont(u8g_font_timB12);
        u8g.setPrintPos(5, 50);
        u8g.print(bitcoinValue);
        //u8g.drawXBMP( 34, 2, down_arrow_width, down_arrow_height, down_arrow_bits);
      } while ( u8g.nextPage() );
    }
    else
    {
      Serial.print("ARDUINO: GET failed: ");
      Serial.println(code);
    }
    //delay(5000);
  }

}



void get_ETHChange()
{
  sprintf(buff, "/apps/thinghttp/send_request?api_key=TKZPYOPSRBC2KEKT");
  // process any callbacks coming from esp_link
  //esp.Process();


  // if we're connected make an HTTP request
  if (wifiConnected)
  {
    // Request /utc/now from the previously set-up server
    rest.get((const char*)buff);

    char response[20];
    uint16_t code = rest.waitResponse(response, 20);
    if (code == HTTP_STATUS_OK)    //check for response for HTTP request
    {
      Serial.println("ARDUINO: GET successful:");
      Serial.println(response);
      bitcoin_value_change = response;
      String bitcoinValueChange = bitcoin_value_change.substring(0, 1);

      if (bitcoinValueChange.toInt() < 0)
      {
        u8g.firstPage();
        do {
          u8g.drawXBMP( 34, 2, down_arrow_width, down_arrow_height, down_arrow_bits);


        } while ( u8g.nextPage() );

      }
      if (bitcoinValueChange.toInt() < 0)
      {

        u8g.firstPage();
        do {

          u8g.drawXBMP( 34, 2, up_arrow_width, up_arrow_height, up_arrow_bits);

        } while ( u8g.nextPage() );
      }
    }
    else
    {
      Serial.print("ARDUINO: GET failed: ");
      Serial.println(code);
    }
    //delay(5000);
  }

}


