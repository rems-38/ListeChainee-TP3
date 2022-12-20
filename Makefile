exoListeChaine: exoListeChaine.o
	gcc -o $@ $^ -Wall -g

exoListeChaine.o: exoListeChaine.c
	gcc -c $< -Wall -g

fat: fat.o
	gcc -o $@ $^ -Wall -g

fat.o: fat.c fat.h
	gcc -c $< -Wall -g


clean:
	rm -rf *.o exoListeChaine fat