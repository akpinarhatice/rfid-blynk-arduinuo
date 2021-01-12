#define BLYNK_PRINT Serial

#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  D3
#define SS_PIN  D8

MFRC522 rfid(SS_PIN, RST_PIN);
byte ID[4] = {0, 0, 0, 0};

const char auth[] = "wbFUTcuzHTzXfQA8FxLIXBXFF8lhZc46";

// Network settings
const char ssid[] = "Hatice";
const char pass[] = "12345678";
  

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
Blynk.run();
delay(10);
 //  Serial.println("Tanımlanmış kart okundu.");
  if (! rfid.PICC_IsNewCardPresent())
    return ;
  if (!rfid.PICC_ReadCardSerial())
    return;
  if (rfid.uid.uidByte[0] == ID[0] &&     //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
      rfid.uid.uidByte[1] == ID[1] &&
      rfid.uid.uidByte[2] == ID[2] &&
      rfid.uid.uidByte[3] == ID[3] ) {
    Serial.println("Tanımlanmış kart okundu.");
    ekranaYazdir();

  } else {
    Serial.println("Tanımlanmayan kart okutuldu.");
    ekranaYazdir();

  }
  rfid.PICC_HaltA();
  delay(50);
}

int n = 0;
void ekranaYazdir(){
  Serial.print("ID NUMARASI: ");
  String id = "";
  for (int sayac = 0; sayac < 4 ; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
    id = id+String(rfid.uid.uidByte[sayac])+"-";
  }
  Blynk.virtualWrite(V1, "add", n, "ID", id);
  n++;
  Serial.println("");
}
