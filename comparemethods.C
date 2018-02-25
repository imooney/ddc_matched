//  comparemethods.C
//  Created by Isaac Mooney on 7/12/17.

#include "TROOT.h"
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

const string path = "out/";
const string histos = "compare/";

void comparemethods () {
    
    gStyle->SetOptStat(0);
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    TFile *flat = TFile::Open((path + "matchedout_flateffic.root").c_str());
    TFile *real = TFile::Open((path + "matchedout_realeffic.root").c_str());
    
    TCanvas * c = new TCanvas("c", "canvas", 800, 800);
    c->cd();
    TH1D *corrected = (TH1D*)flat->Get("ptcorr");
    TH1D *partlevel = (TH1D*)flat->Get("ptpart");
    c->cd();
    TH1D *corrected_real = (TH1D*)real->Get("ptcorr_real");
    
    TPad * pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0);
    pad1->SetGridx();
    pad1->Draw();
    pad1->cd();
    pad1->SetLogy();
    corrected->SetStats(0);
    
    corrected->Draw();
    partlevel->Draw("same");
    corrected_real->Draw("same");
    
    TLegend * test = new TLegend(0.6,0.75,0.8,0.875);
    TLegendEntry *sleg = test->AddEntry("ptcorr", "Corrected", "lep");
    TLegendEntry *eleg = test->AddEntry("ptpart", "Particle Level", "lep");
    TLegendEntry *sleg_real = test->AddEntry("ptcorr_real", "Corrected (real efficiency)", "lep");
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
    
    TH1D *ratio = (TH1D*)corrected->Clone("ratio");
    ratio->SetLineColor(kBlack);
    //ratio->SetMinimum(-5);
    //ratio->SetMaximum(5);
    //ratio->Sumw2();
    ratio->SetStats(0);
    ratio->Divide(corrected_real);
    ratio->SetMarkerStyle(21);
    ratio->Draw("ep");
    
    corrected->SetLineColor(kBlue+1);
    corrected->SetLineWidth(2);
    
    corrected->GetYaxis()->SetTitleSize(20);
    corrected->GetYaxis()->SetTitleFont(43);
    corrected->GetYaxis()->SetTitleOffset(1.55);
    
    partlevel->SetLineColor(kRed);
    partlevel->SetLineWidth(2);
    
    ratio->SetTitle("");
    
    ratio->GetYaxis()->SetTitle("Flat / Real");
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
