CXXFLAGS+=-O2 -std=c++11

all: spritz

%: %.o

clean:
	$(RM) spritz spritz.o

