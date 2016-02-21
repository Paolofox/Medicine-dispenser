void checkTime () { // chekka se bisogna dare una pastiglia

  now = rtc.now(); // variabile che immagazina l'ora, la data
  
  if ((girato == 0)&& (year[casstt] == now.year() && month[casstt] == now.month() && day[casstt] == now.day() && hour[casstt] == now.hour() && minute[casstt] == now.minute())){ // è ora di prendere la pastiglia
    casstt++; // passa alla lettura di data ora del cassettino successivo
    // GIRA LA RUOTA DI UNO SCOMPARTO:
    gira();
    girato = 1;
    digitalWrite(led_rosso, HIGH); // indica che bisogna prendere la pastiglia
    tone(piezo, 1000); // segnale sonoro
    lcd.home();
    lcd.print("ASSUMERE LA");
    lcd.setCursor(0, 1);
    lcd.print("PASTIGLIA");
  }
  
  if ((digitalRead(puls_bicchiere) == LOW) && (girato == 1)){ // appena estraggo il bicchiere
    girato = 2;
    noTone(piezo); // finisci il segnale sonoro
    digitalWrite(led_verde_2, HIGH); // indica che è stato estratto il bicchiere, ma bisogna ancora reinserirlo, è anche acceso il led rosso
    lcd.clear();
    lcd.home();
    lcd.print("BICCHIERE ESTRATTO"); // fornisci alcune info
    lcd.setCursor(0, 1);
    lcd.print("ASSUMERE LA PASTIGLIA");
    lcd.setCursor(0, 2);
    lcd.print("E REINSERIRE IL");
    lcd.setCursor(0, 3);
    lcd.print("BICCHIERE");
  }
    
  if ((digitalRead(puls_bicchiere) == HIGH) && (girato == 2)){ // appena reinserisco il bicchiere
    digitalWrite(led_rosso, LOW); // lascia acceso solo il led verde
    lcd.clear();
    lcd.home();
    lcd.print("BICCHIERE INSERITO"); // fornisci alcune info
    lcd.setCursor(0, 2);
    lcd.print("FINE");
    delay(1000);
    digitalWrite(led_verde_2, LOW); // spegni tutti i led
    girato = 0; // è possibile di nuovo girare la ruota
  }
  
  if ((digitalRead(puls_bicchiere) == LOW) && (girato == 0)){ // appena estraggo il bicchiere quando non devo
    girato = 3; // quando lo reinserisco mi avvisa. senza questa variabile mi avviserebbe continuamente
    tone(piezo, 1000); // segnale sonoro di errore
    digitalWrite(led_rosso, HIGH);
    lcd.clear();
    lcd.home();
    lcd.print("BICCHIERE ESTRATTO"); // fornisci alcune info
    lcd.setCursor(0, 2);
    lcd.print("REINSERIRE IL");
    lcd.setCursor(0, 3);
    lcd.print("BICCHIERE SUBITO");
  }
    
  if ((digitalRead(puls_bicchiere) == HIGH) && (girato == 3)){ // appena reinserico il bicchiere
    girato = 0;
    digitalWrite(led_rosso, LOW); // spegni il led rosso
    lcd.clear();
    lcd.home();
    lcd.print("BICCHIERE INSERITO"); // fornisci alcune info
    lcd.setCursor(0, 2);
    lcd.print("FINE");
    delay(1000);
  }

  
}

