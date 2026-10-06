# schach: Frontend + Backend.
schach: src/main.c src/ui.c src/backend.c
	gcc -Wall -Wextra -o schach src/main.c src/ui.c src/backend.c

# Automatische Tests der Spiellogik (siehe tests/test_backend.c)
test: tests/test_backend.c src/backend.c
	gcc -Wall -Wextra -Isrc -o test_backend tests/test_backend.c src/backend.c
	./test_backend

clean:
	rm -f schach test_backend
