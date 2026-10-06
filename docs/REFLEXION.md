# Reflexionsaufgabe

Entwurf der Antworten auf Basis unseres Projekts. Bitte vor der Abgabe in eigenen Worten prüfen und anpassen.

**1. Warum eignet sich ein zweidimensionales Array für ein Schachbrett?**
Das Brett hat feste 8 Zeilen und 8 Spalten. `spielfeld[zeile][spalte]` entspricht genau dem Gitter,
jedes Feld hat eine feste Position und ist direkt über Indizes erreichbar. Mit zwei Schleifen lässt sich
das ganze Brett ausgeben, speichern oder prüfen.

**2. Welche Vorteile ergeben sich durch die Verwendung von Funktionen?**
Jede Funktion hat eine klare Aufgabe (z. B. `spielfeld_ausgeben`, `figur_bewegen`). Der Code ist lesbarer,
wiederverwendbar und einzeln testbar (`make test`). Wir konnten die Arbeit aufteilen: Frontend und
Spiellogik sind nur über `backend.h` verbunden und unabhängig voneinander änderbar.

**3. Welche Probleme entstehen bei der Umwandlung von `a1` in Array-Indizes?**
- Die Spalte zählt von links (`a` = 0), die Zeile aber von oben (`8` = 0). Brett und Array sind in der Zeile "kopfüber": `zeile = 8 - ziffer`.
- Arrays beginnen bei 0, Schachfelder bei 1.
- Die Eingabe muss geprüft werden (Länge, Buchstabe a-h, Ziffer 1-8), sonst greift man außerhalb des Arrays zu.

**4. Warum sollte ein Programm Benutzereingaben überprüfen?**
Ungültige Eingaben wie `z9` oder `abc` würden sonst zu Zugriffen außerhalb des Arrays, Abstürzen oder
falschem Spielverlauf führen. Eine Prüfung mit klarer Fehlermeldung hält das Programm stabil und
sagt dem Benutzer, was falsch war.

**5. Welche Informationen müssen mindestens gespeichert werden, damit ein Spielstand vollständig wiederhergestellt werden kann?**
Das Spielfeld (alle 64 Felder), der aktuelle Spieler und die Zugnummer. Für das Zugprotokoll zusätzlich
die bisherigen Züge. (Nicht umgesetzt, aber bei vollständigen Regeln nötig: Rochade-Rechte und en passant.)

**6. Welche Probleme würden entstehen, wenn statt eines Arrays nur 64 einzelne Variablen verwendet würden?**
Man könnte keine Schleifen nutzen, jede Aktion (Ausgabe, Speichern, Laden, Wegprüfung) müsste 64-mal
einzeln geschrieben werden. Ein Feld über Zeile und Spalte zu berechnen wäre nicht möglich. Der Code
wäre sehr lang, fehleranfällig und kaum wartbar.

**7. Wie könnte das Programm erweitert werden, damit zwei Spieler über ein Netzwerk gegeneinander spielen können?**
Ein Programm dient als Server und hält den Spielstand, das andere ist der Client. Über einen Socket
(TCP) wird jeder Zug als Text wie `e2 e4` gesendet. Der Server prüft den Zug mit `figur_bewegen`
und schickt das Ergebnis oder das neue Brett an beide Seiten. Die Trennung von Frontend und Spiellogik
ist dafür schon eine gute Grundlage: Nur Ein- und Ausgabe müssten durch Netzwerkaufrufe ersetzt werden.
