# schach: Frontend + echtes Backend. schach_stub: Frontend + Attrappe (nur zum Testen der Anzeige).
schach: src/main.c src/ui.c src/backend.c
	gcc -Wall -Wextra -o schach src/main.c src/ui.c src/backend.c

schach_stub: src/main.c src/ui.c src/stub.c
	gcc -Wall -Wextra -o schach_stub src/main.c src/ui.c src/stub.c

# Automatische Tests der Spiellogik (siehe tests/test_backend.c)
test: tests/test_backend.c src/backend.c
	gcc -Wall -Wextra -Isrc -o test_backend tests/test_backend.c src/backend.c
	./test_backend

clean:
	rm -f schach schach_stub test_backend
