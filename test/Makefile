NAME = test_gnl
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c ../get_next_line.c ../get_next_line_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -D BUFFER_SIZE=42 $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test_all: fclean
	@echo "\n🔍 Testing with BUFFER_SIZE=1"
	$(CC) $(CFLAGS) -D BUFFER_SIZE=1 $(SRCS) -o $(NAME)
	./$(NAME)
	@echo "\n🔍 Testing with BUFFER_SIZE=42"
	$(CC) $(CFLAGS) -D BUFFER_SIZE=42 $(SRCS) -o $(NAME)
	./$(NAME)
	@echo "\n🔍 Testing with BUFFER_SIZE=10000000"
	$(CC) $(CFLAGS) -D BUFFER_SIZE=10000000 $(SRCS) -o $(NAME)
	./$(NAME)

.PHONY: all clean fclean re test_all