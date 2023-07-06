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

TEST_D		=	tests
TEST_OBJ	=	$(addprefix $(TEST_D)/,$(TEST_C:.cpp=.o))


$(NAME): $(OBJ) $(INCLUDES)
	@echo -n "Compiling $(NAME)... "
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo Done!
	@echo run ./webserver

$(OBJ_D)/%.o : $(SRC_D)/%.cpp
	@echo -n "Compiling $@... "
	@mkdir -p $(OBJ_D)
	$(CC) $(CFLAGS) -c $< -I $(INC_D) -I $(INC) -o $@
	@echo Done!


#To execute a testfile just run make test_file FILENAME=filename without the .spec.cpp
#Our test files must be ended with .spec.cpp
.PHONY: test_file
test_file: 
	@filename="$(FILENAME)"; \
    filepath=$$(find . -name "$$filename.spec.cpp" -type f -print -quit); \
    if [ -n "$$filepath" ]; then \
        make test FILE_PATH=$$filepath FILENAME=$$filename; \
		./${TEST_D}/${FILENAME}; \
    else \
        echo "File not found"; \
    fi

.PHONY: test_all

test_all:
	@echo "Running all tests"; \
    files=$$(find . -name "*.spec.cpp" -type f); \
    for filepath in $$files; do \
        filename=$$(basename "$$filepath" .spec.cpp); \
        make test FILE_PATH="$$filepath" FILENAME="$$filename"; \
		echo "Running test for $$filepath"; \
        ./${TEST_D}/$$filename; \
		echo "Done!"; \
    done

test:$(INCLUDES)
	@$(CC) $(CFLAGS) $(FILE_PATH) $(TFLAGS)  -I $(INC_D) -o $(TEST_D)/${FILENAME}.test

all: $(NAME)

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