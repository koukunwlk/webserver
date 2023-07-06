CC	=	c++
CFLAGS = -Wall -Wextra -Werror
TFLAGS = -l gtest -l pthread

NAME 		= webserver

INC 		=	includes
OBJ_D		=	obj

SRC_D		=	src
INC_D		=	includes

RAW_H		=	webserver.hpp

RAW_C		=	main.cpp

OBJ			=	$(addprefix $(OBJ_D)/,$(RAW_C:.cpp=.o))
INCLUDES	=	$(addprefix $(INC_D)/,$(RAW_H))

TEST_D		=	test
TEST_C		=	test.cpp
TEST_OBJ	=	$(addprefix $(TEST_D)/,$(TEST_C:.cpp=.o))


$(NAME): $(OBJ) $(INCLUDES)
	@echo -n "Compiling $(NAME)... "
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo Done!
	@echo run ./webserver

$(OBJ_D)/%.o : $(SRC_D)/%.cpp
	@echo -n "Compiling $@... "
	@mkdir -p $(OBJ_D)
	@$(CC) $(CFLAGS) -c $< -I $(INC_D) -I $(INC) -o $@
	@echo Done!
	
test: $(TEST_OBJ) $(INCLUDES)
	@echo -n "Compiling tests... "
	@$(CC) $(CFLAGS) $(TFLAGS) $(TEST_OBJ) -o webtest
	@echo Done!
	@echo run ./webtest to execute the tests

$(TEST_D)/%.o : $(SRC_D)/%.cpp $(SRC_D)/%.cpp
	@echo -n "Compiling $@... "
	@$(CC) $(CFLAGS) -c $< -I $(INC_D) -I $(INC) -o $@
	@echo Done!

all: $(NAME)

deps: 
	@chmod +x install_deps.sh
	@./install_deps.sh
clean:
	@echo -n "Cleaning... "
	@$(RM) $(OBJ)
	@$(RM) $(TEST_OBJ)
	@echo Done!

fclean:	clean
	@$(RM) $(NAME)
	@$(RM) webtest

re: fclean all

.PHONY: clean fclean re