CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SRC = Main.c Jeu.c Combat.c Grille.c Alignements.c Entraves.c Niveaux.c Livre-des-ames.c Table-des-types.c
OBJ = $(SRC:.c=.o)

all: dreamsool

dreamsool: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_detection: tests/test_detection.o Alignements.o Grille.o Entraves.o Table-des-types.o Livre-des-ames.o Niveaux.o
	$(CC) $(CFLAGS) -o $@ $^

test_entraves: tests/test_entraves.o Alignements.o Grille.o Entraves.o Combat.o Table-des-types.o Livre-des-ames.o Niveaux.o
	$(CC) $(CFLAGS) -o $@ $^

check: test_detection test_entraves
	./test_detection
	./test_entraves

clean:
	rm -f dreamsool test_detection test_entraves $(OBJ) tests/*.o

.PHONY: all check clean
