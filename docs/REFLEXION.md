# Reflexionsaufgabe

> ENTWURF. Die Antworten beziehen sich auf unseren Code, muessen aber vor der Abgabe
> noch in eigenen Worten ueberarbeitet werden.

## 1. Warum eignet sich ein zweidimensionales Array fuer ein Schachbrett?
Ein Schachbrett hat Zeilen und Spalten, genau wie ein 2D-Array. Das Feld `e2` entspricht
direkt `spielfeld[6][4]`. Dadurch lassen sich Zuege mit Zahlen rechnen: Ein Turm bleibt in
der gleichen Zeile oder Spalte, ein Laeufer aendert Zeile und Spalte um gleich viel. In
`backend.c` pruefen wir das mit `abs(zz - sz) != abs(zs - ss)`. Mit Schleifen koennen wir
das ganze Brett ausgeben, speichern und laden (`spielfeld_ausgeben`, `spielstand_speichern`).

## 2. Welche Vorteile ergeben sich durch die Verwendung von Funktionen?
- Jede Funktion hat eine Aufgabe, z.B. `feld_ermitteln` wandelt nur Text in Indizes um.
- Code wird nicht doppelt geschrieben: `weg_ist_frei` nutzen Turm, Laeufer und Dame.
- Man kann Teile einzeln testen. Unsere 45 Tests rufen die Spiellogik ohne Oberflaeche auf.
- Zwei Personen koennen parallel arbeiten: Frontend (`ui.c`) und Backend (`backend.c`)
  sind nur ueber `backend.h` verbunden.

## 3. Welche Probleme entstehen bei der Umwandlung von `a1` in Array-Indizes?
- Die Spalte ist ein Buchstabe: `'a' - 'a' = 0` ... `'h' - 'a' = 7`.
- Die Zeile ist verdreht: Auf dem Brett steht 8 oben, im Array ist Zeile 0 oben. Deshalb
  gilt `zeile = 8 - (ziffer)`. Also ist `a1` = `[7][0]` und nicht `[0][0]`.
- Das Array zaehlt ab 0, das Brett ab 1. Das fuehrt leicht zu Off-by-one-Fehlern.
- Die Eingabe kann falsch sein (`z9`, `abc`, leer). Erst pruefen, dann umrechnen.

## 4. Warum sollte ein Programm Benutzereingaben ueberpruefen?
Benutzer tippen Fehler oder absichtlich Unsinn. Ohne Pruefung wuerde `z9` ausserhalb des
Arrays lesen oder schreiben, das Programm koennte abstuerzen oder falsche Daten
speichern. Deshalb prueft `feld_ermitteln` jede Eingabe, und `figur_bewegen` prueft den
Zug, bevor das Brett veraendert wird. Bei einem Fehler bleibt der Spielstand unveraendert
und es kommt eine verstaendliche Meldung.

## 5. Welche Informationen muessen mindestens gespeichert werden, damit ein Spielstand vollstaendig wiederhergestellt werden kann?
- das Spielfeld (alle 64 Felder mit Figur und Farbe)
- wer am Zug ist (`aktuellerSpieler`)
- die Zugnummer

Wir speichern zusaetzlich das Zugprotokoll, damit auch der Verlauf zurueckkommt. Nicht
gespeichert sind Informationen, die fuer Rochade und en passant noetig waeren (ob Koenig
oder Turm sich schon bewegt haben). Diese Regeln haben wir nicht eingebaut.

## 6. Welche Probleme wuerden entstehen, wenn statt eines Arrays nur 64 einzelne Variablen verwendet wuerden?
- Keine Schleifen: Ausgabe, Speichern und Laden muessten 64 Mal einzeln geschrieben werden.
- Man kann nicht mit Zahlen rechnen. Ein Zug `e2` -> `e4` waere eine riesige Fallunterscheidung.
- Pruefen, ob der Weg frei ist, ginge nicht mit einer Schleife.
- Der Code waere sehr lang und fehleranfaellig, schon ein Tippfehler im Namen faellt kaum auf.

## 7. Wie koennte das Programm erweitert werden, damit zwei Spieler ueber ein Netzwerk gegeneinander spielen koennen?
Ein Programm wird zum Server, das Spielfeld und die Regeln liegen nur dort. Zwei Clients
verbinden sich ueber Sockets (TCP). Ein Client schickt seinen Zug als Text, z.B.
`e2 e4`. Der Server prueft ihn mit `figur_bewegen`, aendert das Brett und schickt den
neuen Stand an beide Clients. Dank der Trennung in Frontend und Backend bleibt die
Spiellogik gleich, nur die Ein- und Ausgabe wird durch Netzwerkaufrufe ersetzt. Zusaetzlich
braucht es: Pruefung, dass nur der Spieler am Zug sendet, und Behandlung von
Verbindungsabbruch (Spielstand speichern).
