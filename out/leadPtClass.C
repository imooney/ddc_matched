#define leadPtClass_cxx
#include "leadPtClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void leadPtClass::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L leadPtClass.C
//      Root > leadPtClass t
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
    
    string path = "../histos/leadPt/";
    string type = "leadPt";
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    
    TH1 * uncutleadPt = new TH1D(("uncut" + type).c_str(), "Lead jet p_{T} spectrum raw", 100, 0, 40);
    TH1 * cuncutleadPt = new TH1D(("cuncut" + type).c_str(), "Lead jet p_{T} spectrum raw (charged)", 100, 0, 40);
    TH1 * cutleadPt = new TH1D(("cut" + type).c_str(), "Lead jet p_{T} spectrum after particle cuts", 100, 0, 40);
    TH1 * ccutleadPt = new TH1D(("ccut" + type).c_str(), "Lead jet p_{T} spectrum after particle cuts (charged)", 100, 0, 40);
    TH1 * cut2leadPt = new TH1D(("cut2" + type).c_str(), "Lead jet p_{T} spectrum after all cuts", 100, 0, 40);
    TH1 * ccut2leadPt = new TH1D(("ccut2" + type).c_str(), "Lead jet p_{T} spectrum after particle cuts (charged)", 100, 0, 40);
    TH1 * efficleadPt = new TH1D(("effic" + type).c_str(), "Lead jet p_{T} spectrum after cuts + efficiency", 100, 0, 40);
    TH1 * cefficleadPt = new TH1D(("ceffic" + type).c_str(), "Lead jet p_{T} spectrum after cuts + efficiency (charged)", 100, 0, 40);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        uncutleadPt->Fill(uncut_leadPt, weight);
        cuncutleadPt->Fill(c_uncut_leadPt, weight);
        cutleadPt->Fill(cut_leadPt,weight);
        ccutleadPt->Fill(c_cut_leadPt, weight);
        cut2leadPt->Fill(cut2_leadPt, weight);
        ccut2leadPt->Fill(c_cut2_leadPt, weight);
        efficleadPt->Fill(effic_leadPt, weight);
        cefficleadPt->Fill(c_effic_leadPt, weight);
        
    }
    Int_t nEntries = uncutleadPt->GetEntries();
    
    uncutleadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    uncutleadPt->Scale(1/(double) nEntries); c1->SetLogy(); uncutleadPt->DrawCopy(); c1->SaveAs((path + "uncut_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    cuncutleadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    cuncutleadPt->Scale(1/(double) nEntries); c1->SetLogy(); cuncutleadPt->DrawCopy(); c1->SaveAs((path + "c_uncut_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    cutleadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    cutleadPt->Scale(1/(double) nEntries); c1->SetLogy(); cutleadPt->DrawCopy(); c1->SaveAs((path + "cut_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    ccutleadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    ccutleadPt->Scale(1/(double) nEntries); c1->SetLogy(); ccutleadPt->DrawCopy(); c1->SaveAs((path + "c_cut_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    cut2leadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    cut2leadPt->Scale(1/(double) nEntries); c1->SetLogy(); cut2leadPt->DrawCopy(); c1->SaveAs((path + "cut2_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    ccut2leadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    ccut2leadPt->Scale(1/(double) nEntries); c1->SetLogy(); ccut2leadPt->DrawCopy(); c1->SaveAs((path + "c_cut2_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    efficleadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    efficleadPt->Scale(1/(double) nEntries); c1->SetLogy(); efficleadPt->DrawCopy(); c1->SaveAs((path + "effic_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    cefficleadPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    cefficleadPt->Scale(1/(double) nEntries); c1->SetLogy(); cefficleadPt->DrawCopy(); c1->SaveAs((path + "c_effic_" + type + ".pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    delete c1; c1 = NULL;
}
