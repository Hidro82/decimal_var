CC 				 =	gcc

ALL_TARGETS 	 = $(TARGET) $(TEST_EXE) gcov_report
ASAN 			 = 	#-fsanitize=address
UBSAN			 =	#-fsanitize=undefined
STDFLAGS 		 =	-g -c -Wall -Wextra -Werror #-std=c11 -pedantic  -fanalyzer

LIBS 			 =	$(CHECK_LIB)
CHECK_LIB 		 = 	$(shell pkg-config --libs --cflags check)

VG 				 = valgrind $(VG_FLAGS)
VG_FLAGS 		 = --leak-check=full --show-leak-kinds=all \
				   --track-origins=yes --verbose --log-file=$(VG_LOG)
LEAKS 			 = leaks -atExit --
VG_LOG 			 = valgrind-report.txt

TARGET 			 =	s21_decimal.a
TEST_TARGET		 =	test_s21_decimal.a
TEST_EXE 		 =	test
TEST_GCOV_EXE	 =	test_gcov
GCOV_TARGET		 =	./coverage
GCOV_INFO		 =	coverage.info

MODULES 		 =	$(shell find . -type d | grep -v -E "objs|tests")
TEST_MODULES 	 =	$(shell find tests -type d | grep -v -E "objs")

SRC_DIR 		 =	$(MODULES)
TEST_SRC_DIR	 =	$(TEST_MODULES)
OBJ_DIR 		 =	./objs
GCOV_OBJ_DIR	 =	./tests/gcov_obj

SRC 			 =	$(notdir $(shell find $(SRC_DIR) -maxdepth 1 -name "s21_*.c"))
TEST_SRC 		 =  $(notdir $(shell find $(TEST_SRC_DIR) -maxdepth 1 -name "*.c"))
INC 			 =	$(shell find $(SRC_DIR) -maxdepth 1 -name "s21_*.h")
TEST_INC		 =	$(shell find $(TEST_SRC_DIR) -maxdepth 1 -name "*.h")

ALL_OBJS		 =	$(shell find . -name "*.o")
OBJS 			 =	$(notdir $(SRC:.c=.o))
TEST_OBJS 		 =	$(notdir $(TEST_SRC:.c=.o))

GCOV_OBJS		 =	$(addprefix $(GCOV_OBJ_DIR)/gcov_, $(notdir $(SRC:.c=.o)))
GCOV_TEST_OBJS 	:=	$(addprefix $(OBJ_DIR)/, $(notdir $(TEST_SRC:.c=.o)))

GCDA 			 = 	$(shell find . -name "*.gcda")
GCNO 			 = 	$(shell find . -name "*.gcno")
GCOV_FLAGS 		 =	-fprofile-arcs -ftest-coverage

vpath %.c $(MODULES) : $(TEST_MODULES)
vpath %.o $(OBJ_DIR) : $(GCOV_OBJ_DIR)

AR 	 =	ar rc
RAN  =	ranlib
RM 	 = 	rm -f
MK 	 =	mkdir -p

CFLAGS 			= 	$(STDFLAGS) $(ASAN) $(UBSAN)

all 			: clean s21_decimal.a test gcov_report

%.o 			: %.c $(INC)
				@$(MK) $(OBJ_DIR)
				@$(CC) $(CFLAGS) -o $(addprefix $(OBJ_DIR)/, $@) -c $<

$(TARGET) 		: $(OBJS)
				@$(AR) $(TARGET) $(addprefix $(OBJ_DIR)/, $(OBJS))
				@$(RAN) $(TARGET)

$(TEST_TARGET)	: $(GCOV_OBJS) $(INC)
				@$(AR) $(TEST_TARGET) $(GCOV_OBJS)
				@$(RAN) $(TEST_TARGET)

$(TEST_EXE) 	: $(TARGET) $(TEST_OBJS) $(TEST_INC)
				@$(CC) $(addprefix $(OBJ_DIR)/, $(TEST_OBJS)) $(ASAN) $(UBSAN) $(GCOV_FLAGS) -o $(TEST_EXE) $(LIBS) -L. $(TARGET)
				./test

gcov_report 	: $(GCOV_TARGET)

$(GCOV_TARGET) 	: OBJ_DIR=$(GCOV_OBJ_DIR)
$(GCOV_TARGET) 	: CFLAGS += $(GCOV_FLAGS)
$(GCOV_TARGET) 	: $(GCOV_INFO)
				genhtml $(GCOV_INFO) -o $(GCOV_TARGET)

$(GCOV_OBJ_DIR)/gcov_%.o : %.c $(INC)
				@$(MK) $(OBJ_DIR)
				@$(CC) $(CFLAGS) -o $@ -c $<

$(TEST_GCOV_EXE): $(TEST_TARGET) $(GCOV_OBJS) $(GCOV_TEST_OBJS)
				@$(CC) $(GCOV_OBJS) $(GCOV_TEST_OBJS) $(GCOV_FLAGS) -o $(TEST_GCOV_EXE) $(LIBS) -L. $(TEST_TARGET)

$(GCOV_INFO) 	: $(TEST_GCOV_EXE)
				@./$(TEST_GCOV_EXE)
				geninfo $(GCOV_OBJ_DIR) -b . -o ./$(GCOV_INFO)

open 			:
				open coverage/index.html

clean 			:
				$(RM) $(TARGET)
				$(RM) $(TEST_TARGET)
				$(RM) $(TEST_EXE)
				$(RM) $(TEST_GCOV_EXE)
				$(RM) $(GCOV_INFO)
				$(RM) $(VG_LOG)
				$(RM) -r $(GCOV_TARGET)
				$(RM) -r $(OBJ_DIR)
				$(RM) -r test.dSYM
				$(RM) -r $(GCOV_OBJ_DIR)

vg				: $(TEST_EXE)
				$(VG) ./$(TEST_EXE)

leaks			: $(TEST_EXE)
				$(LEAKS) ./$(TEST_EXE)

lint 			:
				-cp ../materials/linters/CPPLINT.cfg .
				-find . -type f -name "*.c" | xargs python3 ../materials/linters/cpplint.py --extensions=c
				-find . -type f -name "*.h" | xargs python3 ../materials/linters/cpplint.py --extensions=c
#				-find . -type f -name "*.c" | xargs cppcheck --enable=all --suppress=missingIncludeSystem
				rm -f CPPLINT.cfg

.PHONY 			: all clean open lint vg leaks