#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


#define RST_PIN 49
#define SS_PIN 53
#define len_block 18
#define nr_block 2

LiquidCrystal_I2C lcd(0x27, 20, 4);
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

const byte ROWS = 4;
const byte COLS = 4;
int x = 4;
int blockNum = nr_block;
byte blockData [len_block];
byte readBlockData[len_block];
byte bufferLen = len_block;
byte rowPINS[ROWS] = {13, 12, 11, 10};
byte colPINS[COLS] = {9, 8, 7, 6};
char key_buff = 'D'; 
int master_card1;
int master_card2;
int master_card3;
int master_card4;

byte block[8]={
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };

byte frame[8]={
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
  };

char keys [ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

Keypad keypad = Keypad(makeKeymap(keys), rowPINS, colPINS, ROWS, COLS);


void setup() {
lcd.init ();
lcd.backlight();
lcd.setCursor (0, 0);
lcd.clear();
SPI.begin();
mfrc522.PCD_Init();
lcd.createChar(0, block);
lcd.createChar(1, frame);
}

void ReadDataFromBlock(int blockNum, byte readBlockData[])
{
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
}

void WriteDataToBlock(int blockNum, byte blockData[])
{
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
}

void RFID (){
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
 
  ReadDataFromBlock(blockNum, readBlockData);

  master_card1 = readBlockData[1];
  master_card2 = readBlockData[2];
  master_card3 = readBlockData[3];
  master_card4 = readBlockData[4];

  delay(100);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void menu ()
{
  
  lcd.setCursor (0,0);
  lcd.print("A: KOD     |");
  lcd.setCursor (0,1);
  lcd.print("B: KARTA   |");
  lcd.setCursor (0,2);
  lcd.print("C: KLUCZ   |");
  lcd.setCursor (0,3);
  lcd.print("D: MENU    |");
}

void kod ()
{
 lcd.setCursor (0,0);
 lcd.print ("PODAJ KOD");
}

void karta ()
{
  RFID();
  lcd.setCursor (0,0);
  lcd.print ("ZBLIZ KARTE DOSTEPU");
  lcd.setCursor (0, 1);
  lcd.print ("DOSTEPNE:     1234");
  lcd.setCursor(14,2); 
  if (master_card1 == 'X') {lcd.write(0);} else {lcd.write(1);}
  lcd.setCursor(15,2);
  if (master_card2 == 'X') {lcd.write(0);} else {lcd.write(1);}
  lcd.setCursor(16,2);
  if (master_card3 == 'X') {lcd.write(0);} else {lcd.write(1);}
  lcd.setCursor(17,2);
  if (master_card4 == 'X') {lcd.write(0);} else {lcd.write(1);}
}

void klucz ()
{
 lcd.setCursor (0,0);
 lcd.print ("KLUCZ    "); 
}

void loop() {
 char key_push = keypad.getKey();
 if (key_push){key_buff = key_push; lcd.clear();}
 if (key_buff == 'A'){kod();}
 if (key_buff == 'B'){karta();}
 if (key_buff == 'C'){klucz();}
 if (key_buff == 'D'){menu();}


//RFID ();
}
