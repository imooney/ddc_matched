os = $(shell uname -s)

# Include the configuration.
-include Makefile.inc

INCFLAGS      = -I`root-config --incdir` -I$(ROOTSYS)/include -I$(FASTJETDIR)/include -I$(PYTHIA8DIR)/include -I/opt/local/include -I/usr/local/include

ifeq ($(os),Linux)
CXXFLAGS      = -std=c++11
else
CXXFLAGS      = -O -fPIC -pipe -Wall -Wno-deprecated-writable-strings -Wno-unused-variable -Wno-unused-private-field -Wno-gnu-static-float-init -std=c++11
## for debugging:
# CXXFLAGS      = -g -O0 -fPIC -pipe -Wall -Wno-deprecated-writable-strings -Wno-unused-variable -Wno-unused-private-field -Wno-gnu-static-float-init
endif

ifeq ($(os),Linux)
LDFLAGS       = -g
LDFLAGSS      = -g --shared 
else
LDFLAGS       = -O -Xlinker -bind_at_load -flat_namespace
LDFLAGSS      = -flat_namespace -undefined suppress
LDFLAGSSS     = -bundle
endif

ifeq ($(os),Linux)
CXX          = g++ 
else
CXX          = clang
endif


ROOTLIBS      = $(shell root-config --libs)

LIBPATH       = $(ROOTLIBS) -L$(FASTJETDIR)/lib
LIBS          = -lfastjet -lfastjettools -lpythia8 -L./ -lmyLorentzVector


# for cleanup
SDIR          = src
ODIR          = src/obj
BDIR          = bin


###############################################################################
################### Remake when these headers are touched #####################
###############################################################################


###############################################################################

# standard rules
$(ODIR)/%.o : $(SDIR)/%.cxx $(INCS)
	    @echo 
	    @echo COMPILING
	    $(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

$(BDIR)/%  : $(ODIR)/%.o 
	   @echo 
	   @echo LINKING
	   $(CXX) $(LDFLAGS) $(LIBPATH) $(LIBS) $^ -o $@

###############################################################################

###############################################################################
############################# Main Targets ####################################
###############################################################################

all : $(BDIR)/matchedjetdiffs

$(ODIR)/matchedjetdiffs.o	: $(SDIR)/matchedjetdiffs.cxx

$(BDIR)/matchedjetdiffs		: $(ODIR)/matchedjetdiffs.o ../libmyLorentzVector.so

###############################################################################
##################################### MISC ####################################
###############################################################################

clean :
	@echo
	@echo CLEANING
	rm -vf $(ODIR)/*.o
	#rm -vf out/*.root
	rm -vf $(BDIR)/*
	#rm -vf lib/*
