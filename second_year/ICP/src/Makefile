########################################
# Authors: Matej Hrabal, Patrik Holop  #
# Logins:  xhraba12, xholop01          #  
########################################
# changed qt5-qmake -> qmake -qt=5

all: bin

.PHONY:
	clean bin pack

bin:
	mkdir -p tmp
	cd tmp && qmake-qt5 -spec linux-g++ ../src.pro && make && mv ./src ../../blockeditor
	rm -rf tmp

run: bin
	../blockeditor

doxygen:
	doxygen Doxyfile

pack:
	zip -rq ../xholop01-xhraba12.zip ../src/ ../Makefile ../examples ../doc ../README.txt

clean:
	rm -rf ../doc/* ../blockeditor
