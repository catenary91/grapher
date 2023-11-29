CC = g++
PARSER_HEADERS = token.hpp exceptions.hpp parser.hpp
OBJS = tokenizer.o syntax_checker.o parser.o evaluator.o main.o
TARGET = grapher
CXXFLAGS = -Wall -O2 -std=c++17

grapher : $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -lpng -o $(TARGET)

tokenizer.o : $(PARSER_HEADERS) tokenizer.cpp
	$(CC) $(CXXFLAGS) -c tokenizer.cpp

syntax_checker.o : $(PARSER_HEADERS) syntax_checker.cpp
	$(CC) $(CXXFLAGS) -c syntax_checker.cpp

parser.o : $(PARSER_HEADERS) parser.cpp
	$(CC) $(CXXFLAGS) -c parser.cpp

evaluator.o : $(PARSER_HEADERS) evaluator.cpp
	$(CC) $(CXXFLAGS) -c evaluator.cpp

main.o : $(PARSER_HEADERS) image.hpp graph.hpp options.hpp main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

clean:
	rm -f $(OBJS) $(TARGET)