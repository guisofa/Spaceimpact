CFLAGS = -Wall -g    # Flags de compilação
LDFLAGS = -lm -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_main

CC = gcc             # Compilador

# Arquivos-objeto
objects = spaceimpact.o background.o player.o joystick.o arma.o balas.o inimigos.o chefes.o colisoes.o

all: spaceimpact

spaceimpact: $(objects)
	$(CC) $(CFLAGS) -o SP $(objects) $(LDFLAGS)

spaceimpact.o: spaceimpact.c
	$(CC) -c $(CFLAGS) -o spaceimpact.o spaceimpact.c

background.o: background.c background.h
	$(CC) -c $(CFLAGS) -o background.o background.c

player.o: player.c player.h
	$(CC) -c $(CFLAGS) -o player.o player.c

joystick.o: joystick.c joystick.h
	$(CC) -c $(CFLAGS) -o joystick.o joystick.c

arma.o: arma.c arma.h
	$(CC) -c $(CFLAGS) -o arma.o arma.c

balas.o: balas.c balas.h
	$(CC) -c $(CFLAGS) -o balas.o balas.c

inimigos.o: inimigos.c inimigos.h
	$(CC) -c $(CFLAGS) -o inimigos.o inimigos.c

chefes.o: chefes.c chefes.h
	$(CC) -c $(CFLAGS) -o chefes.o chefes.c

colisoes.o: colisoes.c colisoes.h
	$(CC) -c $(CFLAGS) -o colisoes.o colisoes.c

clean:
	rm -f *.o SP