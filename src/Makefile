CC = g++ -g
HEADERS = event.h temporal_relation.h composite_event.h ieminer.h ieclassifier.h
SOURCES = event.cc composite_event.cc main.cc ieminer.cc ieclassifier.cc

#OUTPUT = ieminer
#OUT_OPTION = -o $(OUTPUT)

build:
	$(CC) $(HEADERS)
	$(CC) $(SOURCES) $(OUT_OPTION)

clean:
	rm -rf *.gch
	rm -rf $(OUTPUT)
