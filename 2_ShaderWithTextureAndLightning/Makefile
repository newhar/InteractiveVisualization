CC = g++
TARGET = EXE

$(TARGET) : Bitmap.o
	$(CC) -o $(TARGET) texture.cpp Bitmap.o -lGLEW -lglut -lGLU -lGL -lX11 

Bitmap.o : Bitmap.h Bitmap.cpp
	$(CC) -c -o Bitmap.o Bitmap.cpp

clean:
	rm -f *.o
	rm -f $(TARGET)


