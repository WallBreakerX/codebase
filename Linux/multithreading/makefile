CC=gcc
TARGET=app

$(TARGET):example.o
	$(CC) -o $@ example.o -lpthread

clean:
	rm -rf ./*.o
