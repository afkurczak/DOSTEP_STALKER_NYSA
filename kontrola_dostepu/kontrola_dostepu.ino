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
int blockNum = nr_block;
byte blockData [len_block];
byte readBlockData[len_block];
byte bufferLen = len_block;
byte rowPINS[ROWS] = {13, 12, 11, 10};
byte colPINS[COLS] = {9, 8, 7, 6};
char key_buf = "Z"; 
char read_password;

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
SPI.begin();
mfrc522.PCD_Init();
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
  for (byte j = 0; j < 16; j++){
  lcd.setCursor(j,0);
  read_password = readBlockData[j];
  lcd.print (read_password);
  }
  delay(100);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void menu ()
{
  
  lcd.setCursor (0,0);
  lcd.print("A: KOD     |");
  lcd.setCursor (0,1);
  lcd.print("B: KLUCZ   |");
  lcd.setCursor (0,2);
  lcd.print("C: KARTA   |");
  lcd.setCursor (0,3);
  lcd.print("D: RESET   |");
  
}

void kod ()
{
 lcd.setCursor (0,0);
 lcd.print ("PODAJ KOD");
// if (key_push == 'D'){key_buf = "Z";}
}
 
void loop() {
  char key_push = keypad.getKey();
  key_buf = key_push;
// if (key_push == 'Z'){menu ();}
// if (key_push == 'A'){kod ();}
  lcd.setCursor (0,0);
  lcd.print(key_push);

//RFID ();
}
