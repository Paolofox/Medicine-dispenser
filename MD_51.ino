/*  ARDUINO MEGA
  lcd ai pin 7, 8, 9, 10, 11, 12
  
  pulsanti:
  su -> pin 2
  giu -> pin 3
  ok -> pin 4
  avanti -> 5
  fine -> 6
  
  bicchiere -> 40
  
  led:
  rosso -> pin 14; // vari led, indica errore
  verde_1 -> pin 15; // indica che è acceso
  verde_2 -> pin 16; // indica correttezza, invio, di un comando
  anno -> pin 17; // indicano cosa stiamo programmando, sono gialli
  mese -> pin 18;
  giorno -> pin 19;
  ora -> pin 20;
  minuto -> pin 21;
  
  piezo -> pin 30

*/

#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

#include <buttonPressingTime.h>

LiquidCrystal lcd (7, 8, 9, 10, 11, 12); // lcd sui pin 7, 8, 9, 10, 11, 12

RTC_DS1307 rtc; // nome assegnato all' rtc
DateTime now; // variabile che terra segno del tempo rtc

int casstt = 1; // tiene conto del cassettino che stiamo configurando

int year[29]; // array che memorizza l' anno di tutti i cassetti (da 1 a 28)
int month[29]; // array che memorizza il mese di tutti i cassetti (da 1 a 28)
int day[29]; // array che memorizza il giorno di tutti i cassetti (da 1 a 28)
int hour[29]; // array che memorizza l'ora di tutti i cassetti (da 1 a 28)
int minute[29]; // array che memorizza i minuti di tutti i cassetti (da 1 a 28)

#define puls_su 24 // pin dei diversi pulsanti
#define puls_giu 25
#define puls_ok 26
#define puls_avanti 27
#define puls_fine 28

buttonPressingTime settings(puls_fine);

#define puls_bicchiere 40 // pulsantino indicatore se il bicchiere è correttamente inserito o meno

const int led_rosso = 23; // vari led, indica errore
const int led_verde_1 = 5; // indica che è acceso
const int led_verde_2 = 22; // indica correttezza, invio, di un comando
const int piezo = 30; // cicalino, segnale sonoro

int puls_track = 0; // tiene traccia di quale array stiamo configurando

int annoAttuale = 2015; // l'anno in cui il prodotto viene venduto

boolean whileReading = true; // serve a restare nelle while quando per "or" ||

boolean ricordaCambiaOra = false;

int girato = 0; // controlla se la pastiglia al momento giusto viene presa o meno, 0 non deve essere presa, 1 deve essere presa, 2 e' stata presa ma il bicchiere va reinserito

int ora = 0; // servono a impostare ora e data iniziali e al cambio d'ora
int minuto = 0;
int giorno = 1;
int mese = 1;
int anno = 2015;

void setup(){
  Serial.begin(57600);
  lcd.begin(20, 4); // accendi l' lcd
  
  Wire.begin();
  
  rtc.begin();

  if (! rtc.isrunning()) { // imposta correttamente il tempo
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  now = rtc.now(); // variabile che immagazina l'ora, la data
  
  
  lcd.print("prova");
  
  pinMode(puls_su, INPUT); // pin dei diversi pulsanti
  pinMode(puls_giu, INPUT);
  pinMode(puls_ok, INPUT);
  pinMode(puls_avanti, INPUT);
  pinMode(puls_fine, INPUT);
  
  pinMode(led_rosso, OUTPUT);
  pinMode(led_verde_1, OUTPUT);
  pinMode(led_verde_2, OUTPUT);
  
  digitalWrite(led_verde_1, HIGH); // rimarra acceso sempre
  digitalWrite(led_verde_2, HIGH); // accendi anche gli altri led per indicare l'accensione
  digitalWrite(led_rosso, HIGH);
  tone(piezo, 1000);
  delay(500);
  noTone(piezo);
  digitalWrite(led_verde_1, HIGH); // rimarra acceso sempre
  digitalWrite(led_verde_2, LOW); // rimane acceso solo quello rosso ad indicare di attaccare la periferica modulo
  
  lcd.clear(); // istruzioni:
  lcd.home();
  lcd.print("PREMERE 'FINE'");
  lcd.setCursor(0, 1);
  lcd.print("PER 3 SECONDI");
  
  whileReading = true;
  while (whileReading == true) { // aspetta che la periferica si attaccata bene
    if (settings.getPressingTime(HIGH) > 3000) { // clicca puls_fine per avviare la procedura iniziale
      digitalWrite (led_verde_2, HIGH);
      digitalWrite (led_rosso, LOW),
      whileReading = false;
    }
  }
  
  impostaOraData();
  // chiedi la correzione dell'ora:
  impostaCassetti(); // imposta la data e l'ora di ogni scomparto (chiamato cassettino) individualmente
  
  casstt = 1;
  Serial.println("loop");
  tone(piezo, 1000);
  delay(500);
  noTone(piezo);
}


//void impostaOraData(){ // correggi l'ora e la data se necessario cliccando puls_giu. Altrimenti cliccando puls_fine si conferma la data o l'ora mostrata
  
//}







void loop(){
  
  
  // fornisci data e ora attuali (da impostazione) e l'ora di apertura del prossimo cassettino solo se il bicchiere è correttamente inserito
  if ((digitalRead(puls_bicchiere) == HIGH) && (girato == 0)){ // e non è caduta nessuna pastiglia
    lcd.home();
    lcd.print("ORA:      DATA:"); // ora e data attuali
    lcd.setCursor(0, 1);
    if (now.hour() < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    if (now.minute() < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
    lcd.setCursor(10, 1);
    lcd.print(now.day(), DEC);
    lcd.print("/");
    if (now.month() < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);
    
    lcd.setCursor(0, 2);
    lcd.print("PROSSIMA PASTIGLIA:"); // ora e data prossima pastiglia
    lcd.setCursor(0, 3);
    if (hour[casstt] < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(hour[casstt], DEC);
    lcd.print(":");
    if (minute[casstt] < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(minute[casstt], DEC);
    lcd.setCursor(10, 3);
    if (day[casstt] < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(day[casstt], DEC);
    lcd.print("/");
    if (month[casstt] < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(year[casstt], DEC);
  }



  
  if (settings.getPressingTime(HIGH) > 2000) { // impostazioni
    whileReading = false;
    puls_track = 1;
    lcd.clear();
    while (whileReading == false) { // finchè non esco dalle impostazioni
      lcd.home();
      lcd.print("IMPOSTAZIONI");
      lcd.setCursor(2, 1);
      lcd.print("Cambio d'ora"); // prima impostazione: cambio d'ora
      lcd.setCursor(2, 2);
     // lcd.print("Cambio data e ora"); // seconda impostazione: cambio d'ora
      lcd.setCursor(0, 3);
      lcd.print("'FINE' = esci");
      lcd.setCursor(0, puls_track);
      lcd.print("-");
        
      if (digitalRead(puls_giu) == HIGH) {
        delay(500);
        puls_track++;
        if (puls_track == 2){
          puls_track = 1;
        }
      }
      
      if ((digitalRead(puls_ok) == HIGH) && (puls_track == 1)) {
        delay(500);
        cambiaOra();
      }
      
      /* NON DECOMMENTARE
       * if (digitalRead(puls_ok) == HIGH && puls_track == 2) { // cambio ora e data e anno
        delay(500);
        lcd.clear();
        lcd.home();
        lcd.print("IMPOSTAZIONE ORA/DATA:");
        lcd.setCursor(0, 1);
        lcd.print("LEGGERE ATTENTAMENTE");
        lcd.setCursor(0, 2);
        lcd.print("LE ISTRUZIONI");
        
        now = rtc.now();
        int anno = now.year();
        int mese = now.month();
        int giorno = now.day();
        int minuto = now.minute();
        int ora = now.hour();
        
        impostaOraData();
         // fai cadere tutte le pastiglie che altrimenti non cadrebbero se la nuova ora e data sono nel futuro
         // ...
      }*/
      
      if (digitalRead(puls_fine) == HIGH) {
        delay(700);
        whileReading = true;
      }
    }
  }
  
  now = rtc.now();
  if ((ricordaCambiaOra == true) && (now.hour() == 2)) { // a mezza notte cambia l'ora se necessario (ricordaCambiaORa == true)
    lcd.clear();
    lcd.home();
    lcd.print("STO CAMBIANDO");
    lcd.setCursor(0, 1);
    lcd.print("L'ORA...");
    delay(1000);
    
    int anno = now.year();
    int mese = now.month();
    int giorno = now.day();
    int minuto = now.minute();
    int ora = now.hour();

    // se delle pastiglie andavano prese tra le 2 e le 3 di notte
    while (year[casstt] == now.year() && month[casstt] == now.month() && day[casstt] == now.day() && hour[casstt] == now.hour()) {
      casstt++; // passa alla lettura di data ora del cassettino successivo
      // GIRA LA RUOTA DI UNO SCOMPARTO:
      gira();
      girato = 1;
      digitalWrite(led_rosso, HIGH); // indica che bisogna prendere la pastiglia
      tone(piezo, 1000); // segnale sonoro
      lcd.home();
      lcd.print("ASSUMERE LA/LE");
      lcd.setCursor(0, 1);
      lcd.print("PASTIGLIA/E");
    }
    
    if (now.month() < 6){
      ora = ora++;
    }
    else {
      ora = ora--;
    }
    rtc.adjust(DateTime(anno, mese, giorno, ora, minuto, 0)); // correggi l'ora dell' rtc rispetto a quanto impostato
    
    lcd.clear();
    lcd.home();
  }
  
          
  /* da aggiungere, riepilogo:
  impostazione anche dell' anno per i cassettini: FATTO
  impostazione del cambio ora FATTO
  impostazione di cambio data e ora nel durante 
  tutta la parte stepper in void gira()
  lcd blink FATTO
  tutti i pulsanti su un solo canale TOLTO
  pastiglia cade solo se la precedente e' stata presa
  avvisa se la pastiglia e' servita in ritardo
  
  TUTTO DA VERIFICARE
  
  */
  
  
  
  
}
