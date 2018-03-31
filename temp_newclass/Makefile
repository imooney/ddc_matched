###################################

CXX	= g++
LD	= g++

###################################

os = $(shell uname -s)

ifeq ($(os),Linux)
CXXFLAGS	= -I$(shell root-config --incdir) -std=c++11 -fPIC
else
CXXFLAGS	= -O -std=c++11 -fPIC -pipe -Wall -dynamiclib -I$(shell root-config --incdir)
endif

ifeq ($(os),Linux)
LDFLAGS		= -g
LDFLAGSS	= -g --shared
else
LDFLAGS		= -O -Xlinker -bind_at_load -flat_namespace
LDFLAGSS  	= -dynamiclib -flat_namespace -undefined suppress
LDFLAGSSS 	= -bundle
endif

ROOTLIBS	= $(shell root-config --libs)
LIBS		= $(ROOTLIBS)

INSTALL_DIR	= /usr/local
GSL		= $(INSTALL_DIR)

CXXFLAGSGSL 	= $(CXXFLAGS)
CXXFLAGSGSL 	+= -I$(GSL)/include
CXXFLAGSROOT 	= -I$(shell root-config --incdir) -std=c++11

LIBSROOT       	= $(ROOTLIBS)
LIBSGSL	       	= $(LIBS)
LIBSGSL       	+= -L$(GSL)/lib -lgsl -lgslcblas


HDRS		= myLorentzVector.h LinkDef.h
SRCS		= myLorentzVector.C dictmyLorentzVector.C

OBJS		= $(SRCS:.C=.o)

SLIB		= libmyLorentzVector.so

$(SLIB):	$(OBJS)
		@echo "Linking $(PROGRAM) for "$(os)
		@echo $(LD) $(LDFLAGSS) $(OBJS) $(LIBS) -o $(SLIB)
		@$(LD) $(LDFLAGSS) $(OBJS) $(LIBS) -o $(SLIB)
		@echo "done"

#######################################

myLorentzVector.o:	myLorentzVector.h

dictmyLorentzVector.C:	$(HDRS)
		@echo "Generating dictionary ..."
		@$(ROOTSYS)/bin/rootcint -f dictmyLorentzVector.C -c $(HDRS)

.C.o:
		$(CXX) $(CXXFLAGS) -c $<

tests:
		g++ $(CXXFLAGSGSL) $(CXXFLAGSROOT) -c src/tests.C
		g++ $(LDFLAGS) tests.o $(LIBSGSL) $(LIBSROOT) -L./ -lmyLorentzVector -o bin/tests
		rm tests.o

reading_myLorentzVectors:
		g++ $(CXXFLAGSGSL) $(CXXFLAGSROOT) -c reading_myLorentzVectors.C
		g++ $(LDFLAGS) reading_myLorentzVectors.o $(LIBSGSL) $(LIBSROOT) -L./ -lmyLorentzVector -o bin/reading_myLorentzVectors
		rm reading_myLorentzVectors.o

clean :
		rm -f *.o
		rm -f *.o dict*.* $(SLIB)
		rm bin/*	
		rm *.root
		@echo "All cleaned-up :-)"

# DO NOT DELETE
