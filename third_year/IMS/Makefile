COMPILER=g++
FLAGS=-std=c++11 -Wall -Wextra -pedantic -g -lm -lsimlib 
SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
OUT=logistika
ZIPNAME=02_xhraba12_xholop01

make: $(OBJECTS)
	$(COMPILER) $(FLAGS) $(OBJECTS) -o $(OUT)
	rm $(OBJECTS)

$(OBJECTS): $(SOURCES)
	$(COMPILER) -c $(SOURCES)	

run:
	sh run_experiments.sh > experiments_debug.info

clean:
	rm -f $(OUT)
	rm -f $(ZIPNAME).zip

zip:
	zip -j $(ZIPNAME).zip $(SOURCES) $(HEADERS) Makefile ./doc.pdf results/ run_experiments.sh

compile_script:
	sh compile.sh logistika.cpp

