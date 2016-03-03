CC=gcc
CXX=g++
CFLAGS=-g -Wall -I. $(shell pkg-config --cflags qpid)

LIBS=-L. $(shell pkg-config --libs qpid)

bOBJ = \
  broadcast_receiver.o

rOBJ = \
  request_response.o

b10OBJ = \
  10_broadcast_receiver.o

r10OBJ = \
  10_request_response.o


%.o: %.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

all: broadcast_receiver request_response 10_broadcast_receiver 10_request_response

broadcast_receiver: $(bOBJ)
	$(CXX) -o $@ $^ $(LIBS)

request_response: $(rOBJ)
	$(CXX) -o $@ $^ $(LIBS)

10_broadcast_receiver: $(b10OBJ)
	$(CXX) -o $@ $^ $(LIBS)

10_request_response: $(r10OBJ)
	$(CXX) -o $@ $^ $(LIBS)


.PHONY: clean

clean:
	rm -f *.o *~ broadcast_receiver request_response 10_broadcast_receiver 10_response_receiver

