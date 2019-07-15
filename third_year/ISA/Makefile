CFLAGS=`xml2-config --cflags`
LIBS=`xml2-config --libs`


compile:
	g++ *.cpp $(CFLAGS) -o feedreader -lssl -lcrypto $(LIBS) -static-libstdc++

test:
	sh ./tests/test.sh

clean:
	rm *.o feedreader doc.aux doc.log doc.synctex.gz doc.toc
