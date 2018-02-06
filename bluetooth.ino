//----------------------------------------------------
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

//----------------------------------------------------
#define BUFSIZE                        128
#define VERBOSE_MODE                   true
//----------------------------------------------------
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4
//----------------------------------------------------
#define BLE_READPACKET_TIMEOUT         500 
//----------------------------------------------------
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
//----------------------------------------------------
// the packet buffer
extern uint8_t packetbuffer[];
//----------------------------------------------------
int32_t charid_string;
int32_t charid_number;
//----------------------------------------------------

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

boolean isConnected = false;

void setupBLE(){
  
  if ( !ble.begin(VERBOSE_MODE) ){ DEBUG_PRINT_LN(F("Failed to initialize BLE")); }
  // ---------------------------------------
  ble.verbose(false);
  DEBUG_PRINT_LN(F("setup ble"));
  // ---------------------------------------
  ble.sendCommandCheckOK( F("AT+GATTADDSERVICE=uuid=0x1234") );
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2345,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_string);
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x6789,PROPERTIES=0x08,MIN_LEN=4,MAX_LEN=4,DATATYPE=INTEGER,DESCRIPTION=number,VALUE=0"), &charid_number);
  // ---------------------------------------
  ble.reset();
  // ---------------------------------------
  ble.echo(false);
  // ---------------------------------------
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);
  // ---------------------------------------
  ble.setBleUartRxCallback(BleUartRX);
  ble.setBleGattRxCallback(charid_string, BleGattRX);
  ble.setBleGattRxCallback(charid_number, BleGattRX);
  // ---------------------------------------
}

void connected(){
  DEBUG_PRINT_LN(F("Connected"));
  DEBUG_PRINT_LN( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);
  isConnected = true;
}

void disconnected(){
  DEBUG_PRINT_LN(F("Disconnected"));
  isConnected = false;
}

void BLELoop(){
  ble.update(0);
}

void BleUartRX(char data[], uint16_t len){
//  Serial.print( F("[BLE UART RX]" ) );
//  Serial.write(data, len);
//  Serial.println();

  if (data[1] == 'C') {
    uint8_t red = data[2];
    uint8_t green = data[3];
    uint8_t blue = data[4];
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);
  }
  if (data[1] == 'B') {
    uint8_t buttnum = data[2] - '0';
    boolean pressed = data[3] - '0';
    Serial.print ("Button "); Serial.print(buttnum);
    if (pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }
  }
}

void BleGattRX(int32_t chars_id, uint8_t data[], uint16_t len){
  // ---------------------------------------
  Serial.print( F("[BLE GATT RX] (" ) );
  Serial.print(chars_id);
  Serial.print(") ");
  // ---------------------------------------
  if (chars_id == charid_string){  
    Serial.write(data, len);
    Serial.println();
  }else if (chars_id == charid_number){
    int32_t val;
    memcpy(&val, data, len);
    Serial.println(val);
  }
  // ---------------------------------------
}

