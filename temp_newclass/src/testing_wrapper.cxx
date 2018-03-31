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
#include "TBranch.h"

#include <iostream>
#include <sstream>
#include <random>
#include <chrono>
#include <string>

#include "testing_wrapper.h"

using namespace std;

int main () {
  cout << "~~~~~~~~~~testing class myLorentzVector!~~~~~~~~~~\n";

  myLorentzVector * test = new myLorentzVector();
  myLorentzVector * test_fill = new myLorentzVector(1,2,3,4);
  cout << test_fill->Px() << '\n';
  cout << test_fill->GetMultiplicity() << '\n';
  test_fill->AddTrack();
  cout << "MULTIPLICITY: " << test_fill->GetMultiplicity() << '\n';
  vector<int> local_tracks = test_fill->GetTracks();
  cout << "SIZE: " << local_tracks.size() << " " << local_tracks[0] << '\n';

  test_fill->AddTracks(5);
  cout << "MULTIPLICITY: " << test_fill->GetMultiplicity() << '\n';
  vector<int> local_tracks_2 = test_fill->GetTracks();
  cout << "SIZE: " << local_tracks_2.size() << '\n';
  for (int i = 0; i < local_tracks_2.size(); ++ i) {
    cout << local_tracks_2[i] << '\n';
  }
  
  return 0;
}
