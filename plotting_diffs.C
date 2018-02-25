//  plotting_diffs.C
//  Created by Isaac Mooney on 2/20/18.

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
#include <sstream>
#include <vector>

using namespace std;

void plotting_diffs () {
  gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();

    string filesize = "skinny", effic_type = "real";
    cout << "Please enter 'skinny' / 'fat' then 'real' / 'flat' \n"; 

    cin >> filesize >> effic_type;
    
    TFile *in = new TFile(("out/matchedout_" + filesize + "_" + effic_type + ".root").c_str(), "READ");

    TH1 *ptdiff = (TH1D*) in->Get("ptdiff");
    TH1 *ptpart = (TH1D*) in->Get("ptpart");
    TH1 *ptdet = (TH1D*) in->Get("ptdet");
    TH1 *ptcorr = (TH1D*) in->Get("ptcorr_real");
    TH1 *ptadded = (TH1D*) in->Get("ptadded");
    TH1 *numadded = (TH1D*) in->Get("numadded");
    TH1 *numdiff = (TH1D*) in->Get("numdiffmatched");

    
    ptdiff->Scale(1/ptdiff->Integral());
    ptadded->Scale(1/ptadded->Integral());
    ptpart->Scale(1/ptpart->Integral());
    ptdet->Scale(1/ptdet->Integral());
    ptcorr->Scale(1/ptcorr->Integral());
    
    numdiff->Scale(1/numdiff->Integral());
    numadded->Scale(1/numadded->Integral());
    
    ptpart->SetLineColor(1);
    ptdet->SetLineColor(6);
    ptdiff->SetLineColor(1);
    ptadded->SetLineColor(2);
    ptcorr->SetLineColor(7);

    TCanvas *c3 = new TCanvas("c3", "c3", 800, 800); //c3->SetLogy();
    c3->cd();

    ptpart->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    
    ptpart->Draw();
    ptcorr->Draw("same");
    ptdet->Draw("same");

    
    TLegend * leg2 = new TLegend(0.6,0.75,0.8,0.875);
    leg2->AddEntry("ptpart", "particle-level", "lep");
    leg2->AddEntry("ptdet", "detector-level", "lep");
    leg2->AddEntry("ptcorr_real", "corrected detector-level", "lep");
    leg2->SetBorderSize(0);
    
    leg2->Draw("same");
    
    TCanvas *c5 = new TCanvas("c5", "c5", 800, 800); c5->SetLogy();
    c5->cd();

    numdiff->GetXaxis()->SetTitle("Multiplicity difference [tracks]");

    numdiff->GetXaxis()->SetRangeUser(0., 8.);
    numadded->GetXaxis()->SetRangeUser(0., 8.);
    
    numadded->SetLineColor(kRed + 1);
    numdiff->Draw();
    numadded->Draw("same");

    TLegend * leg_funcs = new TLegend(0.6,0.75,0.8,0.875);
    leg_funcs->AddEntry("numdiffmatched", "number lost", "lep");
    leg_funcs->AddEntry("numadded", "number added", "lep");
    leg_funcs->SetBorderSize(0);
    
    leg_funcs->Draw("same");
    
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 800); c2->SetLogy();
    c2->cd();
    ptdiff->GetXaxis()->SetTitle("p_{T} [GeV/c]");

    ptdiff->GetXaxis()->SetRangeUser(0., 60.);
    ptadded->GetXaxis()->SetRangeUser(0., 60.);
    
    ptdiff->Draw(); ptadded->Draw("same");
    
    TLegend * leg3 = new TLegend(0.6,0.75,0.8,0.875);
    leg3->AddEntry("ptdiff", "pT lost", "lep");
    leg3->AddEntry("ptadded", "pT added back", "lep");
    leg3->SetBorderSize(0);
    
    leg3->Draw("same");
    
    TCanvas *c4 = new TCanvas("c4", "c4", 800, 800); c4->SetLogy();
    c4->cd();

    ptdet->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    ptdet->Draw();
    
    c2->SaveAs(("results/" + effic_type + "_effic_ptdiff_ptadded.pdf").c_str());
    c3->SaveAs(("results/" + effic_type + "_effic_ptpart_ptcorr_ptdet.pdf").c_str());
    c4->SaveAs(("results/" + effic_type + "_effic_ptdet.pdf").c_str());
    c5->SaveAs(("results/" + effic_type + "_effic_numdiff_numadded.pdf").c_str());
    
    return;

}
