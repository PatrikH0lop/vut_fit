all: bin

.PHONY:
	clean bin pack

bin:
	mkdir temp
	cd temp && qmake -spec win32-g++ ../FiboCalc.pro && make
	rmdir /s /q temp

pack: clean doc
	cd zip && 7za.exe a xholop01_xchoch07_xkapou04_xkaras34.zip  ../../../../xholop01_xchoch07_xkapou04_xkaras34
	
run: bin
	cd bin && FiboCalc.exe

test: bin
	cd bin && FiboCalc.exe -DT

doc:
	doxygen Doxyfile

clean:
	-rmdir /s /q bin
	-rmdir /s /q html
	-rmdir /s /q latex