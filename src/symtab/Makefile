CC	=	g++
CFLAGS	=	-ggdb3 -Wall -Woverloaded-virtual -Wno-write-strings
#CC	=	CC
#CFLAGS	=	-g +p +w
GCFLAGS =	-g -Wno-write-strings
LDFLAGS =	
DPFLAGS =	-MM

SOURCES =	error.cc scanner.cc symtab.cc symbol.cc symtabtest.cc
HEADERS =	error.hh scanner.hh symtab.hh
OBJECTS =	$(SOURCES:%.cc=%.o)
OUTFILE =	symtab

DPFILE  =	Makefile.dependencies

all : $(OUTFILE)

$(OUTFILE) : $(OBJECTS)
	$(CC) -o $(OUTFILE) $(OBJECTS) $(LDFLAGS)

scanner.cc : scanner.l
	flex scanner.l

scanner.o : scanner.cc
	$(CC) $(GCFLAGS) -c $<

.cc.o: $(DPFILE)
	$(CC) $(CFLAGS) -c $<

clean : 
	rm -f $(OBJECTS) $(OUTFILE) core *~ scanner.cc $(DPFILE)
	touch $(DPFILE)


$(DPFILE) depend : $(SOURCES) $(HEADERS)
	$(CC) $(DPFLAGS) $(CFLAGS) $(SOURCES) > $(DPFILE)

include $(DPFILE)
