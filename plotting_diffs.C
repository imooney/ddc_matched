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

    /*
    ptdiff->Scale(1/ptdiff->Integral());
    ptadded->Scale(1/ptadded->Integral());
    ptpart->Scale(1/ptpart->Integral());
    ptdet->Scale(1/ptdet->Integral());
    ptcorr->Scale(1/ptcorr->Integral());
    */
    
    numdiff->Scale(1/numdiff->Integral());
    numadded->Scale(1/numadded->Integral());
    
    ptpart->SetLineColor(1);
    ptpart->SetMarkerColor(1);
    ptdet->SetLineColor(6);
    ptdet->SetMarkerColor(6);
    ptdiff->SetLineColor(1);
    ptdiff->SetMarkerColor(1);
    ptadded->SetLineColor(2);
    ptadded->SetMarkerColor(2);
    ptcorr->SetLineColor(7);
    ptcorr->SetMarkerColor(7);
    
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
    numadded->SetMarkerColor(kRed + 1);
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
    /*
    c2->SaveAs(("results/" + effic_type + "_effic_ptdiff_ptadded.pdf").c_str());
    c3->SaveAs(("results/" + effic_type + "_effic_ptpart_ptcorr_ptdet.pdf").c_str());
    c4->SaveAs(("results/" + effic_type + "_effic_ptdet.pdf").c_str());
    c5->SaveAs(("results/" + effic_type + "_effic_numdiff_numadded.pdf").c_str());
    */
    
    const string path = "out/";
    const string histos = "compare/";
        
        gStyle->SetOptStat(0);
        
        TH1::SetDefaultSumw2();
        TH2::SetDefaultSumw2();
        TH3::SetDefaultSumw2();
    
    TFile *real = TFile::Open((path + "matchedout_" + filesize + "_" + effic_type + ".root").c_str());
        
        TCanvas * c = new TCanvas("c", "canvas", 800, 800);
        c->cd();
        TH1D *partlevel = (TH1D*)real->Get("ptpart");
        TH1D *corrected_real = (TH1D*)real->Get("ptcorr_real");
        
        TPad * pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
        pad1->SetBottomMargin(0);
        pad1->SetGridx();
        pad1->Draw();
        pad1->cd();
        pad1->SetLogy();
        partlevel->SetStats(0);
    
    partlevel->SetMarkerStyle(21);
    corrected_real->SetMarkerStyle(24);
    
    corrected_real->GetXaxis()->SetRangeUser(15,30);
    partlevel->GetXaxis()->SetRangeUser(15,30);
    
        partlevel->Draw();
        corrected_real->Draw("same");
        
        TLegend * test = new TLegend(0.6,0.75,0.8,0.875);
        TLegendEntry *eleg = test->AddEntry("ptpart", "Particle Level", "p");
        TLegendEntry *sleg_real = test->AddEntry("ptcorr_real", "Corrected", "p");
        test->SetBorderSize(0);
        test->Draw("same");
        
        //pTraw8->GetYaxis()->SetLabelSize(0.);
        TGaxis *axis = new TGaxis(-5, 20, -5, 220, 20, 220, 510, "");
        axis->SetLabelFont(43);
        axis->SetLabelSize(15);
        axis->Draw();
        
        c->cd();
        TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.2);
        pad2->SetGridx();
        pad2->Draw();
        pad2->cd();
        
        TH1D *ratio = (TH1D*)corrected_real->Clone("ratio");
    ratio->GetYaxis()->SetRangeUser(0.8,1.2);
        ratio->SetLineColor(kBlack);
        //ratio->SetMinimum(-5);
        //ratio->SetMaximum(5);
        //ratio->Sumw2();
        ratio->SetStats(0);
        ratio->Divide(partlevel);
        ratio->SetMarkerStyle(21);
        ratio->Draw("ep");
        
        corrected_real->SetLineColor(kBlue+1);
        corrected_real->SetLineWidth(2);
    corrected_real->SetMarkerColor(kBlue+1);
        
        corrected_real->GetYaxis()->SetTitleSize(20);
        corrected_real->GetYaxis()->SetTitleFont(43);
        corrected_real->GetYaxis()->SetTitleOffset(1.55);
        
        partlevel->SetLineColor(kRed);
        partlevel->SetLineWidth(2);
    partlevel->SetMarkerColor(kRed);
        
        ratio->SetTitle("");
        
        ratio->GetYaxis()->SetTitle("Corrected / particle-level");
        ratio->GetYaxis()->SetNdivisions(505);
        ratio->GetYaxis()->SetTitleSize(20);
        ratio->GetYaxis()->SetTitleFont(43);
        ratio->GetYaxis()->SetTitleOffset(1.55);
        ratio->GetYaxis()->SetLabelFont(43);
        ratio->GetYaxis()->SetLabelSize(15);
        
        ratio->GetXaxis()->SetTitle("p_{T} [GeV/c]");
        ratio->GetXaxis()->SetTitleSize(18);
        ratio->GetXaxis()->SetTitleFont(43);
        ratio->GetXaxis()->SetTitleOffset(4.);
        ratio->GetXaxis()->SetLabelFont(43);
        ratio->GetXaxis()->SetLabelSize(15);
        
        //c->SaveAs((histos + type + "_pT.pdf").c_str());
    
    return;

}
