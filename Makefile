# Baut das Frontend mit der Attrappe. Fuer das echte Backend stub.c durch dessen Datei ersetzen.
schach: src/main.c src/ui.c src/stub.c
	gcc -Wall -Wextra -o schach src/main.c src/ui.c src/stub.c

clean:
	rm -f schach
