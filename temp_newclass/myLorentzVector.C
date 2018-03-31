
#include "TROOT.h"
#include "myLorentzVector.h"

using namespace std;

ClassImp(myLorentzVector)

myLorentzVector::myLorentzVector(const double px, const double py, const double pz, const double E) : TLorentzVector(px, py, pz, E) {
  tracks.reserve(10);
}

myLorentzVector::myLorentzVector(const double px, const double py, const double pz, const double E, const int nTracks) : TLorentzVector(px, py, pz, E) {
    myLorentzVector::AddTracks(nTracks);
}

  //getters & setters
vector<int> myLorentzVector::GetTracks() {return tracks;}
int myLorentzVector::GetMultiplicity() {return tracks.size();}

void myLorentzVector::AddTrack() {tracks.push_back(GetMultiplicity() + 1);} //Counting from 1!!! I.e. multiplicity = 2 => track #1 and track #2
void myLorentzVector::AddTracks(int n) {for (int i = 0; i < n; ++ i) {tracks.push_back(GetMultiplicity() + 1);}}
