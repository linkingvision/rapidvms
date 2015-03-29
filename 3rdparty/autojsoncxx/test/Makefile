INCLUDE ?= -I../include -I../rapidjson/include -I../catch/single_include
TARGET ?= autojsoncxx_catch

PYTHON_INTERP ?= python
GENERATOR_SCRIPT = ../autojsoncxx.py
DEFINITION_FILE ?= ../examples/userdef.json
GENERATED_HPP ?= userdef.hpp
CODE_TEMPLATE = ../code_template

all: $(TARGET)

$(TARGET): main.o test.o $(GENERATED_HPP)
	$(CXX) $(CXXFLAGS) main.o test.o -o $(TARGET)
	
$(GENERATED_HPP): $(DEFINITION_FILE) $(GENERATOR_SCRIPT) $(CODE_TEMPLATE)
	$(PYTHON_INTERP) $(GENERATOR_SCRIPT) --check --output=$(GENERATED_HPP) --input=$(DEFINITION_FILE)

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c main.cpp -o main.o

test.o: test.cpp $(GENERATED_HPP)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c test.cpp -o test.o
	
.PHONY: clean test

test: $(TARGET)
	./$(TARGET) --success

clean:
	rm *.o $(TARGET) $(GENERATED_HPP)
