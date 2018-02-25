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

LIBPATH       = $(ROOTLIBS) -L$(FASTJETDIR)/lib -L$(PYTHIA8DIR)/lib
LIBS          = -lfastjet -lfastjettools -lpythia8


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

all : $(BDIR)/ddc_matched#$(BDIR)/ktTrackEff

$(SDIR)/dict.cxx	:	$(SDIR)/ktTrackEff.hh
	cd $(SDIR);	rootcint -f dict.cxx -c -I. ./ktTrackEff.hh

$(ODIR)/dict.o	:	$(SDIR)/dict.cxx

$(ODIR)/ktTrackEff.o	:	$(SDIR)/ktTrackEff.cxx $(SDIR)/ktTrackEff.hh

$(BDIR)/ktTrackEff	:	$(ODIR)/dict.o $(ODIR)/ktTrackEff.o

#lib/libMyJetlib.so	:	$(ODIR)/JetAnalyzer.o $(ODIR)/dict.o $(ODIR)/ktTrackEff.o

$(ODIR)/analysis_funcs.o	: $(SDIR)/analysis_funcs.cxx $(SDIR)/analysis_funcs.h
$(ODIR)/ddc_matched.o		: $(SDIR)/ddc_matched.cxx $(SDIR)/analysis_funcs.h

$(BDIR)/ddc_matched		: $(ODIR)/ddc_matched.o $(ODIR)/analysis_funcs.o $(ODIR)/dict.o $(ODIR)/ktTrackEff.o

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
