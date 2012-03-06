CC = gcc
CFLAGS = -Wall #-ggdb

tarea5: tarea5.o camera.o bitmap.o cube.o
	$(CC) $(CFLAGS) -lm -lGL -lGLU -lglut $^ -o $@

tarea5.o: tarea5.c camera.o bitmap.o cube.o
bitmap.o: bitmap.c bitmap.h
camera.o: camera.c camera.h
cube.o: cube.c cube.h

clean:
	rm *.o tarea5

