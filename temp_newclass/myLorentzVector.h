#ifndef myLorentzVector_h
#define myLorentzVector_h

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include <iostream>

class myLorentzVector : public TLorentzVector {
 private:
  std::vector<int> tracks;
 public:
  //constructors & destructor
 myLorentzVector() : TLorentzVector(){}
  myLorentzVector(const double px, const double py, const double pz, const double E);
    myLorentzVector(const double px, const double py, const double pz, const double E, const int nTracks);

  //getters & setters
  std::vector<int> GetTracks();
  int GetMultiplicity();

  void AddTrack(); //Counting from 1!!! I.e. multiplicity = 2 => track #1 and track #2
  void AddTracks(int n);

  ClassDef(myLorentzVector, 1); // the class title
};

#endif //myLorentzVector_h
