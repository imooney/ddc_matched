#define testsClass_cxx
#include "testsClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void testsClass::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L testsClass.C
//      Root > testsClass t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
    if (fChain == 0) return;
    
    string path = "../histos/tests/";
    string type = "tests_";
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    
    TH1 * withoutleadsublead = new TH1D("withoutleadsublead", "p_{T} without leading 2 jets (after cuts & efficiency selection)", 100, 0, 40);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        withoutleadsublead->Fill(without_leadsublead);
    }
    Int_t nEntries = withoutleadsublead->GetEntries();
    
    withoutleadsublead->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    withoutleadsublead->Scale(1/(double) nEntries); c1->SetLogy(); withoutleadsublead->DrawCopy(); c1->SaveAs((path + "withoutleadsublead.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);

    
    delete c1; c1 = NULL;

}
