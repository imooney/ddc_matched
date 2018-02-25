#define efficConsClass_cxx
#include "efficConsClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void efficConsClass::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L efficConsClass.C
//      Root > efficConsClass t
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
    
    string path = "../histos/efficCons/";
    string type = "efficCons_";
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    
    TH3 * efficCons_pxpypz = new TH3D((type + "pxpypz").c_str(), "momentum", 100, -40, 40, 100, -40, 40, 100, -90, 90);
    TH3 * efficCons_etaphipt = new TH3D((type + "etaphipt").c_str(), "eta, phi, pt", 100, -10, 10, 100, 0, 6.5, 100, 0, 40);
    TH1 * efficCons_e = new TH1D((type +"e").c_str(), "energy", 100, 0, 90);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        efficCons_pxpypz->Fill(effic_cons_px, effic_cons_py, effic_cons_pz);
        efficCons_etaphipt->Fill(effic_cons_eta, effic_cons_phi, effic_cons_Pt);
        efficCons_e->Fill(effic_cons_E);
    }
    Int_t nEntries = efficCons_e->GetEntries();
    
    efficCons_e->GetXaxis()->SetTitle("E [GeV]");
    efficCons_e->Scale(1/(double) nEntries); c1->SetLogy(); efficCons_e->DrawCopy(); c1->SaveAs((path + type + "e.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    //selecting bin ranges for projections. Change as desired.
    //pxpypz->GetXaxis()->SetRangeUser(-1, 1);
    //pxpypz->GetYaxis()->SetRangeUser(-1, 1);
    //pxpypz->GetZaxis()->SetRangeUser(-1, 1);
    //projections with above slices!
    
    //QUESTION: HOW TO SCALE A 2D HISTOGRAM???
    
    efficCons_pxpypz->Project3D("yx"); //first vertical, second horizontal
    efficCons_pxpypz_yx->GetXaxis()->SetTitle("p_{x} [GeV/c]"); efficCons_pxpypz_yx->GetYaxis()->SetTitle("p_{y} [GeV/c]");
    c1->SetLogz(); efficCons_pxpypz_yx->DrawCopy("colz"); c1->SaveAs((path + type + "pypx.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    efficCons_pxpypz->Project3D("zy");
    efficCons_pxpypz_zy->GetXaxis()->SetTitle("p_{y} [GeV/c]"); efficCons_pxpypz_zy->GetYaxis()->SetTitle("p_{z} [GeV/c]");
    efficCons_pxpypz_zy->DrawCopy("colz"); c1->SaveAs((path + type + "pzpy.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    efficCons_pxpypz->Project3D("zx");
    efficCons_pxpypz_zx->GetXaxis()->SetTitle("p_{x} [GeV/c]"); efficCons_pxpypz_zx->GetYaxis()->SetTitle("p_{z} [GeV/c]");
    efficCons_pxpypz_zx->DrawCopy("colz"); c1->SaveAs((path + type + "pzpx.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    
    efficCons_pxpypz->Project3D("x");
    efficCons_pxpypz_x->GetXaxis()->SetTitle("p_{x} [GeV/c]");
    efficCons_pxpypz_x->Scale(1/(double) nEntries); c1->SetLogy(); efficCons_pxpypz_x->DrawCopy(); c1->SaveAs((path + type + "px.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    efficCons_pxpypz->Project3D("y");
    efficCons_pxpypz_y->GetXaxis()->SetTitle("p_{y} [GeV/c]");
    efficCons_pxpypz_y->Scale(1/(double) nEntries); c1->SetLogy(); efficCons_pxpypz_y->DrawCopy(); c1->SaveAs((path + type + "py.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    efficCons_pxpypz->Project3D("z");
    efficCons_pxpypz_z->GetXaxis()->SetTitle("p_{z} [GeV/c]");
    efficCons_pxpypz_z->Scale(1/(double) nEntries); c1->SetLogy(); efficCons_pxpypz_z->DrawCopy(); c1->SaveAs((path + type + "pz.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    
    //selecting bin ranges for projections. Change as desired.
    //etaphipt->GetXaxis()->SetRangeUser(-1, 1);
    //etaphipt->GetYaxis()->SetRangeUser(0, 6.5);
    //etaphipt->GetZaxis()->SetRangeUser(-1, 1);
    //projections with above slices!
    efficCons_etaphipt->Project3D("yx");
    efficCons_etaphipt_yx->GetXaxis()->SetTitle("#eta"); efficCons_etaphipt_yx->GetYaxis()->SetTitle("#phi");
    c1->SetLogz(); efficCons_etaphipt_yx->DrawCopy("colz"); c1->SaveAs((path + type + "phieta.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    efficCons_etaphipt->Project3D("zy");
    efficCons_etaphipt_zy->GetXaxis()->SetTitle("#phi"); efficCons_etaphipt_zy->GetYaxis()->SetTitle("p_{T} [GeV/c]");
    c1->SetLogz(); efficCons_etaphipt_zy->DrawCopy("colz"); c1->SaveAs((path + type + "ptphi.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    efficCons_etaphipt->Project3D("zx");
    efficCons_etaphipt_zx->GetXaxis()->SetTitle("#eta"); efficCons_etaphipt_zx->GetYaxis()->SetTitle("p_{T} [GeV/c]");
    c1->SetLogz(); efficCons_etaphipt_zx->DrawCopy("colz"); c1->SaveAs((path + type + "pteta.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    
    efficCons_etaphipt->Project3D("x");
    efficCons_etaphipt_x->GetXaxis()->SetTitle("#eta");
    efficCons_etaphipt_x->Scale(1/(double) nEntries); c1->SetLogy(); efficCons_etaphipt_x->DrawCopy(); c1->SaveAs((path + type + "eta.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    efficCons_etaphipt->Project3D("y");
    efficCons_etaphipt_y->GetXaxis()->SetTitle("#phi");
    efficCons_etaphipt_y->Scale(1/(double) nEntries); efficCons_etaphipt_y->DrawCopy(); c1->SaveAs((path + type + "phi.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    efficCons_etaphipt->Project3D("z");
    efficCons_etaphipt_z->GetXaxis()->SetTitle("p_{T}");
    efficCons_etaphipt_z->Scale(1/(double) nEntries); c1->SetLogy(); efficCons_etaphipt_z->DrawCopy(); c1->SaveAs((path + type + "pt.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    delete c1; c1 = NULL;
}
