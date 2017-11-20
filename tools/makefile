#the project configures
DIR_SRC = .
DIR_OBJ = .
DIR_BIN = .
DIR_LIB = .
DIR_TEST = .

_lllsuffix = cpp

TARGET = test
BIN_TARGET= $(DIR_BIN)/${TARGET}

#define
PLATFORM=LINUX		
BITSFLAGS= -D__64BIT__

#option
VERSION = DEBUG
TYPE = EXE

#compile information
CXX=g++
LINK=$(CXX)
LINKSL=$(CXX)

#compile argument
CFLAGS = -Wall $(BITSFLAGS) -I. -D$(PLATFORM) 

#modify info by the configures
ifeq ($(VERSION),DEBUG)
#CFLAGS += -g
endif

ifeq ($(BITSFLAGS),D__32BIT__)
CFLAGS += -m32
endif

ifeq ($(TYPE),DLL)
CFLAGS += -fPIC
SLFLAGS = -shared -L.
endif

#CPPFLAGS = $(CFLAGS)

tmpfile := makefile.lll

All: $(tmpfile) ${BIN_TARGET}
	rm -f $(tmpfile)


include $(tmpfile)

SOURCE := $(wildcard $(DIR_SRC)/*.$(_lllsuffix)) 	#read the ".c"
OBJS := $(patsubst %.$(_lllsuffix),%.o,$(notdir $(SOURCE)))	#analy the all ".o", so the make will make the all .c in the src.

LIBS := $(patsubst lib%.so,-l%,$(notdir $(wildcard $(DIR_LIB)/*.so))) #read the all .so, then translate to so in the linux
SLIBS = -L${DIR_LIB} $(LIBS) -lpthread

#create the .o
${tmpfile}:
	gcc -MM *.$(_lllsuffix) > $(tmpfile)

${BIN_TARGET}:${OBJS}
	$(LINKSL) $(SLFLAGS) $(EXTRA_SLFLAGS) $^ $(SLIBS) -o $@ 
%.o:%.c
	$(CXX) -c  $(CFLAGS) $< -o $@
%.o:%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@
%.o:%.cc
	$(CXX) -c $(CFLAGS) $< -o $@
clean:
	rm -rf $(DIR_OBJ)/*.o
copy:
	cp $(BIN_TARGET) $(DIR_TEST)
compare:
	diff ./src ./vs_code
init:
	mkdir ./bin ./src ./obj ./lib ./test 
