#ifndef testing_wrapper_h
#define testing_wrapper_h

#include "Pythia8/Pythia.h"
#include "Pythia8/PhaseSpace.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

#include "TROOT.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TH3.h"
#include "TClonesArray.h"
#include "TLatex.h"
#include "TMathText.h"
#include "TProfile.h"
#include "TLorentzVector.h"

#include <iostream>
#include <sstream>
#include <random>
#include <chrono>

class myLorentzVector : public TLorentzVector {
 private:
  std::vector<int> tracks;
 public:
  //constructor & destructor
 myLorentzVector() : TLorentzVector(){}
 myLorentzVector(const double px, const double py, const double pz, const double E) : TLorentzVector(px, py, pz, E) {}

  //getters & setters
  std::vector<int> GetTracks() {return tracks;}
  int GetMultiplicity() {return tracks.size();}

  void AddTrack() {tracks.push_back(GetMultiplicity() + 1);} //Counting from 1!!! I.e. multiplicity = 2 => track #1 and track #2
  void AddTracks(int n) {for (int i = 0; i < n; ++ i) {tracks.push_back(GetMultiplicity() + 1);}}
  
};

#endif //testing_wrapper_h
