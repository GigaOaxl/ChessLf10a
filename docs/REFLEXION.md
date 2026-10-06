# Reflexionsaufgabe

## 1. Warum eignet sich ein zweidimensionales Array für ein Schachbrett?
Ein Schachbrett hat Zeilen und Spalten. Ein zweidimensionales Array hat auch Zeilen und
Spalten. Das passt direkt zusammen: Das Feld e2 ist `spielfeld[6][4]`.

Dadurch kann man mit Zahlen rechnen. Ein Turm bleibt in derselben Zeile oder Spalte. Ein
Läufer geht in Zeile und Spalte gleich viele Schritte (`abs(zielZeile - startZeile) == abs(zielSpalte - startSpalte)` in
`laeufer_darf_ziehen`). Außerdem können wir das Brett mit zwei Schleifen ausgeben, speichern und
laden (`spielfeld_ausgeben`, `spielstand_speichern`, `spielstand_laden`).

## 2. Welche Vorteile ergeben sich durch die Verwendung von Funktionen?
- Jede Funktion macht nur eine Sache. `feld_ermitteln` wandelt nur "e2" in Zahlen um.
  `figur_bewegen` prüft und führt den Zug aus.
- Man schreibt Code nur einmal. `weg_ist_frei` wird von Turm, Läufer und Dame benutzt.
  `startfeld_pruefen` wird vom Menü und von `figur_bewegen` benutzt.
- Man kann Teile einzeln testen. Unsere Tests (`make test`) rufen nur die Spiellogik auf,
  ohne Menü.
- Wir konnten zu zweit arbeiten. Einer hat Anzeige und Menü gemacht (`ui.c`, `main.c`),
  der andere die Regeln (`backend.c`). Beide Teile sind nur über `backend.h` verbunden.
- Der Code ist leichter zu lesen, weil die Namen sagen, was passiert.

## 3. Welche Probleme entstehen bei der Umwandlung von a1 in Array-Indizes?
- Der Buchstabe wird zur Spalte: a = 0, b = 1, ... h = 7. Das geht mit `feldname[0] - 'a'`.
- Die Zahl wird zur Zeile, aber **andersherum**. Auf dem Brett ist die 8 oben. Im Array
  ist Zeile 0 oben. Darum rechnen wir `zeile = 8 - (feldname[1] - '0')`. Also ist a1 =
  `[7][0]` und a8 = `[0][0]`.
- Das Array fängt bei 0 an, das Brett bei 1. Da passieren schnell Fehler um eins.
- Die Eingabe kann falsch sein, zum Beispiel `z9`, `a9`, `x` oder `abc`. Deshalb prüft
  `feld_ermitteln` zuerst, ob es genau zwei Zeichen sind und ob sie zwischen a-h und 1-8
  liegen. Erst dann wird gerechnet.

## 4. Warum sollte ein Programm Benutzereingaben überprüfen?
Menschen tippen Fehler. Ohne Prüfung würde `z9` einen Platz außerhalb des Arrays treffen.
Dann kann das Programm abstürzen oder falsche Daten ändern.

Bei uns passiert das so:
- `feld_ermitteln` prüft, ob das Feld überhaupt existiert.
- `startfeld_pruefen` prüft, ob dort eine eigene Figur steht.
- `figur_bewegen` prüft, ob der Zug nach den Regeln erlaubt ist.
- `spielstand_laden` prüft die ganze Datei, bevor etwas überschrieben wird.

Bei einem Fehler bleibt das Spiel unverändert und der Benutzer bekommt eine klare Meldung
(`FEHLER: ...`).

## 5. Welche Informationen müssen mindestens gespeichert werden, damit ein Spielstand vollständig wiederhergestellt werden kann?
Mindestens drei Dinge:
1. das Spielfeld (alle 64 Felder mit Figur und Farbe),
2. wer am Zug ist (`aktuellerSpieler`),
3. die Zugnummer.

Ohne den Spieler am Zug wüsste man nach dem Laden nicht, wer dran ist. Wir speichern
außerdem das Zugprotokoll, damit auch der Verlauf zurückkommt.

Nicht gespeichert wird, ob König oder Turm sich schon bewegt haben. Das bräuchte man für
Rochade. Diese Regel haben wir nicht eingebaut.

## 6. Welche Probleme würden entstehen, wenn statt eines Arrays nur 64 einzelne Variablen verwendet würden?
- Man kann keine Schleifen benutzen. Ausgabe, Speichern und Laden müssten 64 Mal einzeln
  geschrieben werden.
- Man kann nicht mit Zahlen rechnen. Für einen Zug von e2 nach e4 müsste man jedes Feld
  einzeln abfragen (64 mal `if`).
- "Ist der Weg frei?" geht nicht mit einer Schleife (`weg_ist_frei`).
- Der Code wäre sehr lang. Ein kleiner Tippfehler bei einem Variablennamen fällt kaum auf.

## 7. Wie könnte das Programm erweitert werden, damit zwei Spieler über ein Netzwerk gegeneinander spielen können?
Ein Programm ist der Server. Nur dort liegen das Brett und die Regeln. Die zwei Spieler
sind Clients und verbinden sich über das Netzwerk (Sockets, TCP).

Ablauf:
1. Ein Spieler schickt seinen Zug als Text, zum Beispiel `e2 e4`.
2. Der Server prüft ihn mit den vorhandenen Funktionen (`feld_ermitteln`, `figur_bewegen`).
3. Ist der Zug gültig, schickt der Server das neue Brett an beide Spieler.
4. Ist er ungültig, bekommt nur der Absender eine Fehlermeldung.

Die Spiellogik bleibt gleich, weil sie schon von der Anzeige getrennt ist. Neu wären nur
die Ein- und Ausgabe über das Netzwerk. Man müsste außerdem prüfen, dass nur der Spieler
am Zug etwas senden darf, und was passiert, wenn eine Verbindung abbricht (zum Beispiel
den Spielstand speichern).
