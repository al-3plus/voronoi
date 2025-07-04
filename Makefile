compile:
		gcc -fsanitize=address,undefined -Wvla -Wpedantic -Wall -lm -I/usr/include/SDL2 -lSDL2main -lSDL2  *.c -o sain

not_sanitized:
		gcc  *.c -Wvla -Wpedantic -Wall -lm -I/usr/include/SDL2 -lSDL2main -lSDL2  -g -o3 main

transforme:
	for f in rive*.dot; do \
		[ -f "$$f" ] || continue; \
		echo "Traitement de $$f"; \
		dot -Tpng "$$f" -o "$${f%.dot}.png"; \
	done  && \
	rm -f rive*.dot

debug:
	-rm -f rive*.png &&\
	-rm -f rive*.dot &&\ 
	gcc *.c -Wvla -Wpedantic -Wall -lm -I/usr/include/SDL2 -lSDL2main -lSDL2 -g -o main && \
	./main && \
	for f in rive*.dot; do \
		[ -f "$$f" ] || continue; \
		echo "Traitement de $$f"; \
		dot -Tpng "$$f" -o "$${f%.dot}.png"; \
	done  && \
	rm -f rive*.dot

		


