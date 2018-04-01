//  test of reading in myLorentzVector class objects
//  Created by Isaac Mooney on 8/29/17.

#include "TROOT.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TH3.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TLatex.h"
#include "TMathText.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <iostream>
#include <vector>

#include "myLorentzVector.h"

//ClassImp(myLorentzVector)

using namespace std;

int main () {
  
    gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();

    TFile *input = new TFile("test.root", "READ");
    TTree *test_tree = (TTree*) input->Get("t");
    
    TClonesArray *out_tca = new TClonesArray("myLorentzVector");
    test_tree->SetBranchAddress("b",&out_tca);
    
    unsigned nEntries = test_tree->GetEntries(); //number of jets
    
    for (unsigned i = 0; i < nEntries; ++i) {
        out_tca->Clear();
        test_tree->GetEntry(i);
	
        for (unsigned j = 0; j < out_tca->GetEntriesFast(); ++ j) {
            myLorentzVector *out_jet = (myLorentzVector*) out_tca->At(j);
	    cout << out_jet->GetMultiplicity() << '\n';
	    cout << out_jet->Px() << " " << out_jet->Py() << " " << out_jet->Pz() << " " << out_jet->E() << '\n';
	    for (int k = 0; k < out_jet->GetTracks().size(); ++ k) {
	      cout << out_jet->GetTracks()[k] << " ";
	    }
	    cout << '\n';
        }
    }
    
    return 0;
}
