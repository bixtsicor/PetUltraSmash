#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo;

#define RST_PIN  9
#define SS_PIN  10
MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial bluethooth(4, 2);
long previousMillis = 0;
void setup(
 {
  myservo.attach(7);
  Serial.begin(9600);
  bluethooth.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.flush();
}
byte ActualUID[4]; //almacenarÃƒÂ¡ el cÃƒÂ³digo del Tag leÃƒÂ­do
byte Usuario1[4]= {0x08, 0xF1, 0x37, 0xD9};
byte Usuario2[4]= {0xC1, 0x2F, 0xD6, 0x0E};
void loop() {
  static int get_num;
  if (bluethooth.available() < 1 ){
    Serial.println(get_num);
    return;
  }
  else if (bluethooth.available() > 1){
    int temp = get_num;
    get_num = bluethooth.read();
    if (get_num == 255) get_num = temp;
    Serial.println(get_num);
    }
  switch(get_num){
    case 2:
        if ( mfrc522.PICC_IsNewCardPresent()){
            Serial.print(F("Card UID:"));
            if ( mfrc522.PICC_ReadCardSerial()) {
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          ActualUID[i]=mfrc522.uid.uidByte[i];
                  }
                  if(compareArray(ActualUID,Usuario1) || compareArray(ActualUID, Usuario2)){
                    Serial.println("Acceso concedido...");
                  //  if (myservo.read() != 115)myservo.write(115);
                  while(mfrc522.PICC_ReadCardSerial()){
                    if (myservo.read() != 115) myservo.write(115);
                  }

                    // while (mfrc522.PICC_ReadCardSerial()){}
                    mfrc522.PICC_HaltA();
                    return;
                    }
                  else{
                    Serial.println("Acceso denegado...");
                    if (myservo.read() != 70){
                        unsigned long currentMillis = millis();
                        long previousMillis = currrentMillis;
                        while(currentMillis - previousMillis < 8000) {
                        currentMillis = millis();
                        }
                        myservo.write(70);
                    }
                    mfrc522.PICC_HaltA();
                    return;
                    }
            }}
            else {
              if (myservo.read() != 70) {
              unsigned long currentMillis = millis();
              long previousMillis = currrentMillis;
              while(currentMillis - previousMillis < 8000) {
               currentMillis = millis();
               }
               myservo.write(70);
               }
              return;
              }

    case 1:
        if (myservo.read() != 25){
          myservo.write(25);
        }
        return;
    case 3:
        if (myservo.read() != 115) {
          myservo.write(115);
        }
        return;
    //case 4:


    default:
        return;

}}
// 115, 70, 25
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

