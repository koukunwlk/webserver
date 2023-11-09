CC	=	c++
CFLAGS = -Wall -Wextra -Werror -g -gdwarf-2 -pthread
TFLAGS = -l gtest

NAME 		= webserver

RM			=	rm -rf

INC 		=	include
OBJ_D		=	obj

SRC_D		=	src
INC_D		=	include

RAW_C		=	main.cpp \
					WebServerException/WebServerException.cpp \
					WebServerException/HostingException.cpp \
					WebServerException/ParsingException.cpp \
					WebServerException/RequestValidationException.cpp \
					Handler/Handler.cpp \
					Request/Request.cpp \
					Response/Response.cpp \
					ConfigFileParser/ConfigFileParser.cpp \
					WebServer/WebServer.cpp \
					Server/Server.cpp

OBJ			=	$(addprefix $(OBJ_D)/,$(RAW_C:.cpp=.o))

TEST_D		=	tests
TEST_OBJ	=	$(addprefix $(TEST_D)/,$(TEST_C:.cpp=.o))


$(NAME): $(OBJ) $(INCLUDE)
	@echo -n "Compiling $(NAME)... "
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo Done!
	@echo "run ./webserver <config_file.conf>"

$(OBJ_D)/%.o : $(SRC_D)/%.cpp
	@echo -n "Compiling $@... "
	@mkdir -p $(OBJ_D)
	@mkdir -p $(OBJ_D)/Server
	@mkdir -p $(OBJ_D)/WebServer
	@mkdir -p $(OBJ_D)/WebServerException
	@mkdir -p $(OBJ_D)/Handler
	@mkdir -p $(OBJ_D)/Request
	@mkdir -p $(OBJ_D)/Response
	@mkdir -p $(OBJ_D)/ConfigFileParser
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
    files=$$(find src -name "*.spec.cpp" -type f); \
	echo $$files; \
	status=0; \
    for filepath in $$files; do \
        filename=$$(basename "$$filepath" .spec.cpp); \
        make test_folder FOLDER=$$filename; \
		status=$$(expr $$status + $$?); \
		echo "Done!"; \
    done;\
	exit $$status

test_folder:
	@echo "Running all tests in folder ${FOLDER}"; \
	test=$$(find src/${FOLDER} -iname "*.spec.cpp"  | tr '\n' ' '); \
	src=$$(find src -iname "*.cpp" -not -iname "*.spec.cpp" -not -iname "main.cpp" | tr '\n' ' '); \
	make test FILE_PATH="$$test $$src" FILENAME=${FOLDER}; \
	./${TEST_D}/${FOLDER};

test:$(INCLUDE)
	@mkdir -p $(TEST_D)
	@$(CC) $(CFLAGS) $(FILE_PATH) $(TFLAGS) -I $(INC_D) -o $(TEST_D)/${FILENAME}

all: $(NAME)

clean:
	@echo -n "Cleaning... "
	@$(RM) $(OBJ)
	@$(RM) $(TEST_D)/*
	@echo Done!

fclean:	clean
	@$(RM) $(NAME)
	@$(RM) webtest

re: fclean all

.PHONY: clean fclean re
