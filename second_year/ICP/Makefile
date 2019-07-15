########################################
# Authors: Matej Hrabal, Patrik Holop  #
# Logins:  xhraba12, xholop01          #  
########################################

all: bin

.PHONY:
	clean bin pack

bin:
	make -C ./src

run: 
	make run -C ./src

doxygen:
	make doxygen -C ./src

pack:
	make pack -C ./src

clean:
	make clean -C ./src
