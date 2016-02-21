void impostaCassetti(){
  Serial.println("impostaCassettini");
  
  while (casstt < 28) { // completa eventualmente il giro
    gira();
  }
  gira();// fai ancora due step prima di esserepronto ad accogliere nuove pastiglie
  gira();
  
  casstt = 1;
  
  whileReading = true;
  aggiornaLcd2 ();
  while (whileReading == true){ // finche non si clicca puls_fine vengono impostati gli orari dei cassettini
  //Serial.println("whileReading == true");
  
  
  // definisci giorno, mese, ora e minuti:
  if (puls_track == 0){ // definisci hour, l'ora
    lcd.setCursor (3, 2); // casella di lampeggiamento //digitalWrite(led_ora, HIGH);
    if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
      delay(700);
      hour[casstt]++;  
    }
    if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
      delay(700);
      lcd.clear();
      hour[casstt]--;
    }
  }
  
  else if (puls_track == 1){ // definisci minute, i minuti
    lcd.setCursor (6, 2); // casella di lampeggiamento //digitalWrite(led_minuto, HIGH);
    if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
      delay(700);
      // minute[casstt]++;  
      // oppure:
      minute[casstt] = minute[casstt] + 15;
    }
    if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
      delay(700);
      lcd.clear();
      // minute[casstt]--;
            // oppure:
      minute[casstt] = minute[casstt] - 15;
    }
  }
  
  else if (puls_track == 2){ // definisci day, il giorno
    lcd.setCursor (12, 2); // casella di lampeggiamento //digitalWrite(led_giorno, HIGH);
    if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
      delay(700);
      day[casstt]++;  
    }
    if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
      delay(700);
      lcd.clear();
      day[casstt]--;
    }
  }
  
  else if (puls_track == 3){ // definisci month, il mese
    lcd.setCursor (15, 2); // casella di lampeggiamento //digitalWrite(led_mese, HIGH);
    if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
      delay(700);
      month[casstt]++;  
    }
    if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
      delay(700);
      lcd.clear();
      month[casstt]--;
    }
  }
  
  else if (puls_track == 4){ // definisci year, l'anno
    lcd.setCursor (20, 2); // casella di lampeggiamento //digitalWrite(led_anno, HIGH);
    if (digitalRead(puls_su) == HIGH) { // incrementa la variabile di +1 se clicco
      delay(700);
      year[casstt]++;  
    }
    if (digitalRead(puls_giu) == HIGH){ // riduci la variabile di -1 se clicco
      delay(700);
      lcd.clear();
      year[casstt]--;
    }
  }


  // pulsante "OK": passa da mese a giorno a ora a minuto a mese... :
  if (digitalRead(puls_ok) == HIGH){ // se clicco "ok" voglio definire il mese (month), o il giorno o l'ora o il minuto
    digitalWrite(led_verde_2, HIGH);
    delay(700);
    digitalWrite(led_verde_2, LOW);
    lcd.clear();
    puls_track++;
    if(puls_track == 5){ // dopo l'anno torna all'impostazione dell'ora
      puls_track = 0;
    }
  }
  // pulsante "AVANTI": passa al cassettino successivo
  if (digitalRead(puls_avanti) == HIGH){ // se clicco "avanti" voglio configurare il cassettino successivo
    delay(700);
    lcd.clear();
    // casstt++;
    puls_track = 99; // dice che è la data e l'ora sono da controllare, fa entrare il programma in un if successivo, puls_track cambia utilizzo
    if (casstt == 28){ // se siamo arrivati all'ultimo, esci dalla configurazione dei cassettini, equivale a cliccare "fine", non girare piu la ruota
      whileReading = false; // esci dalla main-while
    }
  }
  
  // pulsante "FINE": esci dal while
  if (digitalRead(puls_fine) == HIGH){ // se clicco "fine" ho finito di configurare i cassettini
    delay(700);
    puls_track = 99; // dice che è la data e l'ora sono da controllare, fa entrare il programma in un if successivo
    whileReading = false; // esci dalla main-while
  }
  
  // CONTROLLO DATA e ORA e BISESTILITA'
  if (puls_track == 99){ // se necessario controlla che la data sia accettabile, rimane 99 se tutto ok, diventa 100 se c'è un errore. controlla bisestilità, ora e data nel passato, giorno inesistenti
    
    digitalWrite(led_verde_2, LOW);
    digitalWrite(led_rosso, LOW);
    
    now = rtc.now();
    
    if (month[casstt] == 2 && day[casstt] == 29){
      if ((year[casstt]%4 == 0 && year[casstt]%100 != 0) || (year[casstt]%400 == 0)){ // è bisestile, va bene
      }
      else { // non va bene, non è bisestile
        puls_track = 100;
      }
    }
        
    if (day[casstt] == 31 && (month[casstt] == 2 || month[casstt] == 4 || month[casstt] == 6 || month[casstt] == 9 || month[casstt] == 11)){ // non va bene
      puls_track = 100;
    }
    if (day[casstt] == 30 && month[casstt] == 2){ //  non va bene
      puls_track = 100;
    }
    
    // se la data o l'ora impostate è gia passata rispetto a quella attuale --> non va bene
    if (month[casstt] < now.month() && year[casstt] == now.year()){ // se il mese di quest'anno è passato, non va bene
      puls_track = 100;
    }
    if (month[casstt] == now.month() && year[casstt] == now.year()){ // se il mese è attuale e di quest'anno, verifica il giorno
      if (day[casstt] < now.day()){ // se il giorno è passato, non va bene
        puls_track = 100;
      }
      if (day[casstt] == now.day()){ // se il giorno è attuale, verifica l'ora
        if (hour[casstt] < now.hour()){ // se l'ora è passata, non va bene
          puls_track = 100;
        }
        if (hour[casstt] == now.hour()){ // se l'ora è attuale, verifica i minuti
          if (minute[casstt]-15 <= now.minute()){ // se il minuto -10 è passato o attuale, non va bene
            puls_track = 100;
          }
        }
      }
    }
    
    // se la data o l'ora impostate è gia passata rispetto al cassettino precedente --> non va bene
    int casstt_prec = casstt - 1;
    if (year[casstt] < year[casstt_prec]){ // se imposto
      puls_track = 100;
    }
    if (month[casstt] < month[casstt_prec] && year[casstt] == year[casstt_prec]){ // se il mese di quest'anno è passato rispetto al casstt precedente, non va bene
      puls_track = 100;
    }
    if (month[casstt] == month[casstt_prec] && year[casstt] == year[casstt_prec]){ // se il mese è uguale e di ques'anno rispetto al casstt precedente, verifica il giorno
      if (day[casstt] < day[casstt_prec]){ // se il giorno è passato, non va bene
        puls_track = 100;
      }
      if (day[casstt] == day[casstt_prec]){ // se il giorno è uguale rispetto al casstt precedente, verifica l'ora
        if (hour[casstt] < hour[casstt_prec]){ // se l'ora è passata rispetto al casstt precedente, non va bene
          puls_track = 100;
        }
        if (hour[casstt] == hour[casstt_prec]){ // se l'ora è uguale rispetto al casstt precedente, verifica i minuti
          if (minute[casstt] <= minute[casstt_prec]){ // se il minuto è passato o uguale rispetto al casstt precedente, non va bene
            puls_track = 100;
          }
        }
      }
    }
       
    if (puls_track == 100){ // scrivi se è errato, resta nel main-while, ritorna ad utilizzare puls_track come tracciatore di cosa si sta definendo
      digitalWrite(led_rosso, HIGH);
      lcd.setCursor(0, 3);
      lcd.print("ERRORE! RIPROVA");
      whileReading = true;
      lcd.blink();
    }
    else {
      lcd.noBlink();
      digitalWrite(led_verde_2, HIGH);
      delay(500);
      digitalWrite(led_verde_2, LOW);
      // GIRA LA RUOTA DI UNO SCOMPARTO:
      gira();
      casstt++;
    }
    puls_track = 0; 
  }
  
  // evita giorni, mesi, ore e minuti inesistenti:
  if (day[casstt] <= 0){ // evita lo zeeresimo giorno del mese che non esiste
    lcd.clear();
    if (month[casstt] == 11 || month[casstt] == 4 || month[casstt] == 6 || month[casstt] == 9){ // 30 giorni a novembre, aprile, giugno e settembre
      day[casstt] = 30;
    }
    else if (month[casstt] == 2){ // febbraio
      day[casstt] = 29;
    }
    else { // altri mesi con 31 giorni
      day[casstt] = 31;
    }
  }
  if (day[casstt] >= 32){ // evita il 32esimo giorno del mese che non esiste
    lcd.clear();
    day[casstt] = 1;
  }
  if (day[casstt] >= 31 && (month[casstt] == 11 || month[casstt] == 4 || month[casstt] == 6 || month[casstt] == 9)){ // nei mesi con 30 giorni torna al primo se il conteggio raggiunge i 31
    lcd.clear();
    day[casstt] = 1;
  }
  if (day[casstt] >= 30 && month[casstt] == 2){ // febbraio ha solo al massimo 29 giorni
    lcd.clear();
    day[casstt] = 1;
  }
  
  if (month[casstt] == 0){ // evita lo zeeresimo mese dell'anno che non esiste
    lcd.clear();
    month[casstt] = 12;
  }
  if (month[casstt] == 13){ // evita il 13esimo mese dell'anno che non esiste
    lcd.clear();
    month[casstt] = 1;
  }
  
  if (hour[casstt] == -1){ // evita l'ora -1 che non esiste
    lcd.clear();
    hour[casstt] = 23;
  }
  if (hour[casstt] == 24){ // evita l'ora 25 che non esiste
    lcd.clear();
    hour[casstt] = 0;
  }
  
  if (minute[casstt] <= -1){ // evita il minuto -1 che non esiste
    lcd.clear();
    minute[casstt] = 55;
    //    oppure:
    // minute[casstt] = 59;
  }
  if (minute[casstt] >= 60){ // evita il 60esimo minuto che non esiste e coincide con 0esimo minuto dell'ora dopo
    lcd.clear();
    minute[casstt] = 0;
  }
  
  now = rtc.now();
  if (year[casstt] < now.year()){ // non tornare indietro nel tempo!
    year[casstt] = now.year();
  }
  
  } // chiudi: while whileReading == true
  
  lcd.clear();
  lcd.home();
  lcd.print("COMPLIMENTI !!!");
  lcd.setCursor(0, 1);
  lcd.print("DATA E ORA DEGLI");
  lcd.setCursor(0, 2);
  lcd.print("SCOMPARTI IMPOSTATE");
  lcd.setCursor(0, 3);
  lcd.print("CORRETTAMENTE.");
  delay(3000);
  
  while (casstt < 28){ // se non sono stati impostati tutti i cassetti fai girare la ruota fino a primo, al completamento del giro
    // GIRA LA RUOTA FINO ALLA FINE:
    gira();
  }
  digitalWrite(led_verde_2, LOW);
}


void aggiornaLcd2 () {
  
  // tieni aggiornato l' lcd:
  lcd.noBlink();
  lcd.home();
  lcd.print("Scomparto n. ");
  if (casstt < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(casstt);
  lcd.setCursor(0, 1);
  lcd.print("ORA:      DATA:");
  lcd.setCursor(0, 2);
  if (hour[casstt] < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(hour[casstt]);
  lcd.print(":");
  if (minute[casstt] < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(minute[casstt]);
  lcd.setCursor(11, 2);
  if (day[casstt] < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(day[casstt]);
  lcd.print("/");
  if (month[casstt] < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(month[casstt]);
  lcd.print("/");
  if (year[casstt] < 10){ // aggiungi uno zero davanti
    lcd.print("0");
  }
  lcd.print(year[casstt]);
  lcd.setCursor(0, 3);
  lcd.print("DA IMPOSTARE");
  
  lcd.blink();
  
}
