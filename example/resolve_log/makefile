TARGET= ./resolve-log
CFLAGS := -Wall -w  -g -ggdb -O0 -Wno-unused -Wno-sign-compare -Wno-deprecated-declarations  -Wl,--no-as-needed -std=c++17   -I /usr/local/mysql/
cppfiles := $(shell ls *.cpp)
cfiles := $(-shell ls *.c)
OBJS := $(patsubst %.cpp,./%.o, $(cppfiles) $(cfiles))
COBJS=${patsubst %.c,./%.o,$(cfiles)}
CXXOBJS=${patsubst %.cpp,./%.o,$(cppfiles)}

DEPS=$(patsubst %.o,%.d,$(OBJS))

LIB= /usr/local/mysql/lib/libmysqlclient.a

SO_LIB= 


.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJS} ${LIB} 
	g++-7 -o $@ $^ ${LDFLAGS}${LIB} ${LIB64}  -lpthread  -lstdc++fs -lz -ldl #-lcurl
${CXXOBJS}:./%.o:./%.cpp
	g++-7 -MMD -c -o $@ $< ${CFLAGS} 

${COBJS}:./%.o:./%.c
	${CC} -MMD -c -o $@ $< ${CFLAGS} 

-include $(DEPS)

clean:
	rm -f *.rdb *.log *.temp ${OBJS} ${TARGET} ${DEPS}

show:
	@echo GPROF=$(GPROF)
	@echo CFLAGS=$(CFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo objs=$(OBJS)
	@echo cppfiels=$(cppfiles)
	@echo cfiels=$(cfiles)
	@echo DEPS=$(DEPS)
	@echo CXXOBJS=$(CXXOBJS)
	@echo COBJS=$(COBJS)

