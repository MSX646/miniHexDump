SRC = hexdump.c

NAME = hexDump

CC = clang

all: $(NAME)

$(NAME):
	$(CC) -g3 $(SRC) -o $(NAME)

re: clean all

clean: 
	rm $(NAME)
