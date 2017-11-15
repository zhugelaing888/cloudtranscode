.PHONY: all main test clean deps

CXX=g++
CXXFLAGS += -g -Wall

DEPS_INCLUDE_PATH=-I deps/json-cpp/include/ -I deps/http-parser/
DEPS_LIB_PATH=deps/json-cpp/lib/libjson_libmt.a deps/http-parser/libhttp_parser.a

INCLUDE_PATH=-I include/ -I /usr/local/include/ -I /usr/include/
SRC_INCLUDE_PATH=-I src
LIB_PATH=lib/libCTSserver.a
FFMPEG_LIB_PATH=-L /usr/local/lib -lavformat -lavcodec -lavutil -lswresample -lswscale -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -lmysqlcppconn
FTP_INCLUDE_PATH=-I /usr/local/include/apr-1/ -I /usr/local/include/oss_c_sdk/ -I /usr/local/apr/include/apr-1/
FTP_LIB_PATH=-L /usr/local/lib  -loss_c_sdk -lapr-1 -lcurl -lmxml -L /usr/local/apr/lib/ -laprutil-1
PTHREAD=-pthread
CURL=-lcurl

objects := $(patsubst %.cpp,%.o,$(wildcard src/*/*.cpp))

all: libCTSserver.a
	rm -rf src/*/*.o

deps:
	make -C deps/http-parser package

libCTSserver.a: deps $(objects) 
	ar -rcs lib/libCTSserver.a src/*/*.o

main: cts_main
	
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) $(DEPS_INCLUDE_PATH) $(FTP_INCLUDE_PATH) $(SRC_INCLUDE_PATH) $(FFMPEG_LIB_PATH) $(FTP_LIB_PATH) $(CURL) $< -o $@

cts_main:cts_main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(FTP_INCLUDE_PATH) $(DEPS_INCLUDE_PATH)  $< $(LIB_PATH) $(FFMPEG_LIB_PATH) $(FTP_LIB_PATH) $(PTHREAD) $(CURL) $(DEPS_LIB_PATH) -o bin/$@

test: server_test client_test putfile xml_test licence_test 
server_test:test/http_server_test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(DEPS_INCLUDE_PATH) $< $(LIB_PATH) $(DEPS_LIB_PATH) -o bin/$@
client_test:test/http_client_test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(DEPS_INCLUDE_PATH) $< $(LIB_PATH) $(DEPS_LIB_PATH) $(CURL) -o bin/$@
putfile:test/putFile_test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(FTP_INCLUDE_PATH) $(DEPS_INCLUDE_PATH) $< $(LIB_PATH) $(FTP_LIB_PATH)  $(DEPS_LIB_PATH) -o bin/$@
xml_test:test/test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(DEPS_INCLUDE_PATH) $< $(LIB_PATH) $(DEPS_LIB_PATH) -o bin/$@
licence_test:test/licence_test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(DEPS_INCLUDE_PATH) $< $(LIB_PATH) $(DEPS_LIB_PATH) -o bin/$@

clean:
	rm -rf bin/*
	rm -rf lib/libCTSserver.a

