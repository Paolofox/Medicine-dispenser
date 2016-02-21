void impostaOraData(){
  lcd.clear();
  whileReading = true;
  while (whileReading == true){
    now = rtc.now();
    lcd.home();
    lcd.print("ORA:      DATA:");
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
    if (now.day() < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(now.day(), DEC);
    lcd.print("/");
    if (now.month() < 10){ // aggiungi uno zero davanti
        lcd.print("0");
    }
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);
    lcd.setCursor(0, 2);
    lcd.print("'FINE' = corretto");
    lcd.setCursor(0, 3);
    lcd.print("'GIU' = sbagliato");
    delay(1500);
    
    if ((digitalRead(puls_fine) == HIGH) || (digitalRead(puls_giu) == HIGH)){
      whileReading = false;
      digitalWrite(led_verde_2, HIGH);
      delay(500);
      digitalWrite(led_verde_2, LOW);
    }
  }
  
  if (digitalRead(puls_giu) == HIGH){ // se l'ora non è corretta
    lcd.clear();
    lcd.home();
    lcd.print("Impostazione manuale");
    lcd.setCursor(0, 1);
    lcd.print("dell'ora e data");
    lcd.setCursor(0, 2);
    lcd.print("Attendere ... ");
    delay(2000);
    lcd.clear();
    
    whileReading = true;
    aggiornaLcd ();
    while (whileReading == true){ // cliccando puls_fine si conferma l'impostazione dell'ora e dalla data manuale
      
      // definisci ora, minuti, giorno, mese e anno
      if (puls_track == 0){ // sto impostando l'ora
        lcd.setCursor (0, 1); // casella di lampeggiamento //digitalWrite(led_ora, HIGH);
        if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
          delay(700); // dai tempo di cliccare
          ora++;
        }
        if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
          delay(700);
          lcd.clear(); // pulisci per evitare residui sull' lcd dal passaggio da 10 a 9
          ora--;
        }
      }
      else if (puls_track == 1){ // sto impostando i minuti
        lcd.setCursor (4, 1); // casella di lampeggiamento //digitalWrite(led_ora, HIGH);
        if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
          delay(700);
          minuto++;
          // oppure:
          // minuto = minuto + 5;
        }
        if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
          delay(700);
          lcd.clear();
          minuto--;
          // oppure:
          // minuto = minuto - 5;
        }
      }
      else if (puls_track == 2){ // sto impostando il giorno
        lcd.setCursor (12, 1); // casella di lampeggiamento //digitalWrite(led_giorno, HIGH);
        if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
          delay(700);
          giorno++;
        }
        if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
          delay(700);
          lcd.clear();
          giorno--;
        }
      }
      if (puls_track == 3){ // sto impostando il mese
        lcd.setCursor (15, 1); // casella di lampeggiamento //digitalWrite(led_mese, HIGH);
        if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
          delay(700);
          mese++;
        }
        if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
          delay(700);
          lcd.clear();
          mese--;
        }
      }
      if (puls_track == 4){ // sto impostando l'anno
        lcd.setCursor (20, 1); // casella di lampeggiamento //digitalWrite(led_anno, HIGH);
        if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
          delay(700);
          anno++;
        }
        if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
          delay(700);
          lcd.clear();
          anno--;
        }
      }
      
      // pulsante "OK": passa da mese a giorno a ora a minuto a mese... :
      if (digitalRead(puls_ok) == HIGH){ // se clicco "ok" voglio definire il mese (month) o l'anno o il giorno o l'ora o i minuti
        digitalWrite(led_verde_2, HIGH);
        delay(700);
        lcd.clear();
        puls_track++;
        if (puls_track == 5){ // dopo i minuti torna all'impostazione giorno
          puls_track = 0;
        }
      }
      
        // evita giorni, mesi, ore e minuti inesistenti:
      if (giorno <= 0){ // evita lo zeeresimo giorno del mese che non esiste
        lcd.clear();
        if (mese == 11 || mese == 4 || mese == 6 || mese == 9){ // 30 giorni ha novembre, con april, giugno e settembre
          giorno = 30;
        }
        else if (mese == 2){ // di 28 ce n'è uno
          giorno = 29;
        }
        else { // tutti gli altri ne han 31
          giorno = 31;
        }
      }
      if (giorno >= 32){ // evita il 32esimo giorno del mese che non esiste
        lcd.clear();
        giorno = 1;
      }
      /*if (giorno >= 31 && (mese == 11 || mese == 4 || mese == 6 || mese == 9)){ // nei mesi con 30 giorni torna al primo se il conteggio raggiunge i 31
        lcd.clear();
        giorno = 1;
      }
      if (giorno >= 30 && mese == 2){ // febbraio ha solo al massimo 29 giorni
        lcd.clear();
        giorno = 1;
      }*/
  
  
      if (mese == 0){ // evita lo zeeresimo mese dell'anno che non esiste
        lcd.clear();
        mese = 12;
      }
      if (mese == 13){ // evita il 13esimo mese dell'anno che non esiste
        lcd.clear();
        mese = 1;
      }
  
  
      if (ora == -1){ // evita l'ora -1 che non esiste
        lcd.clear();
        ora = 23;
      }
      if (ora == 24){ // evita l'ora 25 che non esiste
        lcd.clear();
        ora = 0;
      }
  
  
      if (minuto <= -1){ // evita il minuto -1 che non esiste
        lcd.clear();
        // minuto = 55;
        //    oppure:
        minuto = 59;
      }
      if (minuto >= 60){ // evita il 60esimo minuto che non esiste e coincide con 0esimo minuto dell'ora dopo
        lcd.clear();
        minuto = 0;
      }
      if (anno < annoAttuale){ // non tornare indietro nel tempo!
        anno = annoAttuale;
      }
      
      if (digitalRead(puls_fine) == HIGH){ // è o non è bisestile
        whileReading = false;
        
        if (mese == 2 && giorno == 29){
          if ((anno%4 == 0 && anno%100 != 0) || (anno%400 == 0)){ // è bisestile, va bene
          }
          else { // non va bene, non è bisestile
            whileReading = true;
          }
        }
        
        if (giorno == 31 && (mese == 2 || mese == 4 || mese == 6 || mese == 9 || mese == 11)){ // non va bene
          whileReading = true;
        }
        if (giorno == 30 && mese == 2){ //  non va bene
          whileReading = true;
        }
        
        if (whileReading == true){ // scrivi se è errato
          digitalWrite(led_rosso, HIGH);
          lcd.setCursor(0, 2);
          lcd.print("ERRORE! DATA");
          lcd.setCursor(0, 3);
          lcd.print("INESISTENTE, RIPROVA");
          puls_track = 2;
          lcd.blink();
        }
        else {
          lcd.noBlink();
        }
        
     }
    
    } // chiudi: while puls_fine == LOW
    digitalWrite(led_rosso, LOW);
    digitalWrite(led_verde_2, HIGH); // rimane acceso perchè data e ora erano accettabili
    
    // imposta l'ora e la data corretta
    rtc.adjust(DateTime(anno, mese, giorno, ora, minuto, 0)); // correggi l'ora dell' rtc rispetto a quanto impostato
    delay(20);
    
  } // chiudi: if puls_giu == HIGH
  lcd.clear();
  lcd.home();
  lcd.print("Complimenti! Ora");
  lcd.setCursor(0, 1);
  lcd.print("e data impostate:");
  lcd.setCursor(0, 2);
  now = rtc.now();
  Serial.print("ora e data impostate: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print("   ");
  Serial.print(now.day());
  Serial.println("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.println(now.year());
  lcd.setCursor(0, 2);
  lcd.print("Ora:      data:");
  lcd.setCursor(0, 3);
  if (now.hour() < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  if (now.minute() < 10){ // aggiungi uno zero davanti
   lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  lcd.setCursor(10, 3);
  lcd.print(now.day(), DEC);
  lcd.print("/");
  if (now.month() < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 2);    
  delay(3000); // piccolo ritardo per dare il tempo di cliccare puls_fine, senza passare subito a void impostaCassettini e leggere
  digitalWrite(led_verde_2, LOW); // è rrivato il momento di spegnere anche questo
}

void aggiornaLcd () {
  lcd.noBlink();
  lcd.home();
  lcd.print("Ora:      data:");
      lcd.setCursor(0, 1);
      if (ora < 10){ // aggiungi uno zero davanti
        lcd.print("0");
      }
      lcd.print(ora);
      lcd.print(":");
      if (minuto < 10){
        lcd.print("0");
      }
      lcd.print(minuto);
      lcd.setCursor(11, 1);
      if (giorno < 10){
        lcd.print("0");
      }
      lcd.print(giorno);
      lcd.print("/");
      if (mese < 10){
        lcd.print("0");
      }
      lcd.print(mese);
      lcd.print("/");
      lcd.print(anno);
      
      lcd.blink();
}
