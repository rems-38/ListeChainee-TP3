exoListeChaine: exoListeChaine.o
	gcc -o $@ $^ -Wall -g

exoListeChaine.o: exoListeChaine.c
	gcc -c $< -Wall -g


clean:
	rm -rf *.o exoListeChaine