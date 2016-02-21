void cambiaOra() {
  whileReading = false;
  while (whileReading == false) {
    lcd.clear();
    lcd.home();
    lcd.print("IMPOSTA CAMBIO ORA:");
    lcd.setCursor(0, 1);
    lcd.print("'OK' = CAMBIA TRA");
    lcd.setCursor(0, 2);
    lcd.print("       OGGI E DOMANI");
    lcd.setCursor(0, 2);
    lcd.print("'FINE' = ESCI");
    
    now = rtc.now();
    if (digitalRead(puls_ok) == HIGH) { // cambia ora e fornisci qualche dettaglio
      lcd.clear();
      lcd.home();
      lcd.print("L'ORA CAMBIERA' TRA");
      lcd.setCursor(0, 1);
      lcd.print("LE DUE E LE TRE DEL");
      lcd.setCursor(0, 2);
      puls_track = now.day() + 1; // puls_track in questo caso serve a indicare il giorno successivo a quello attuale, ovvero il giorno in cui l'ora verrà cambiata
      if (puls_track < 10){ // aggiungi uno zero davanti
        lcd.print("0");
      }
      lcd.print(puls_track, DEC);
      lcd.print("/");
      if (now.month() < 10){ // aggiungi uno zero davanti
          lcd.print("0");
      }
      lcd.print(now.month(), DEC);
      delay(3000);
      ricordaCambiaOra = true;
      whileReading = true;
    }
    if (digitalRead(puls_fine) == HIGH) { // esci (anche dalle impostazioni)
      delay(500);
      whileReading = true;
    }
  }
}
