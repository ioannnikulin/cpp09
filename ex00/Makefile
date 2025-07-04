CPP = c++
COMPILE_FLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g -c
LINK_FLAGS = 
NAME = btc

SRCS = main.cpp BitcoinExchange.cpp
OBJS = $(SRCS:.cpp=.o)

$(NAME): $(OBJS)
	$(CPP) $(LINK_FLAGS) -o $@ $^

all: $(NAME)

%.o: %.cpp
	$(CPP) $(COMPILE_FLAGS) $^ -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) input.txt

.PHONY: all clean fclean re