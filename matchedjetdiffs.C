//  matchedjetdiffs.C
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
#include <sstream>
#include <vector>
#include <string>

using namespace std;

void matchedjetdiffs (){//(string filesize, string effic_type) {
  //gRandom->Uniform(0.0, 2pi) for later (check if it should be -pi to pi or 0 to 2pi).
  
    gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();

    string filesize = "skinny", effic_type = "real";
    cout << "Please enter 'skinny' / 'fat' then 'real' / 'flat' \n"; 

    cin >> filesize >> effic_type;

    TFile *input = new TFile(("out/ddc_matched_" + filesize + "_" + effic_type + ".root").c_str(), "READ");
    TTree *matched = (TTree*) input->Get("matchedJets");
    TF1 * nbnaive = new TF1("nbnaive","ROOT::Math::negative_binomial_pdf(x,0.8,1)",-0.5, 10.5);
    TF1 * poisnaive = new TF1("poisnaive","ROOT::Math::poisson_pdf(x,0.4764)",-0.5,10.5);
    TF1 * binnaive = new TF1("binnaive","ROOT::Math::binomial_pdf(x,0.2,5)",-0.5,10.5);
    TH1 * numdiff = (TH1D*) input->Get("numdiffmatched");
    TH1 * radial_dist = (TH1D*) input->Get("radial_distmatched");
    TFile *effic = new TFile("efficiency/out/effic_plots.root", "READ");
    TH1 * mult = (TH1D*) effic->Get("mult");
    
    Double_t weight;
    TClonesArray *part = new TClonesArray("TLorentzVector");
    TClonesArray *det = new TClonesArray("TLorentzVector");
    TClonesArray *cons = new TClonesArray("TLorentzVector");
    matched->SetBranchAddress("c_matchedpartJets",&part);
    matched->SetBranchAddress("c_matcheddetJets",&det);
    matched->SetBranchAddress("c_matchedCons", &cons);
    matched->SetBranchAddress("weight", &weight);
    
    TFile *out = new TFile(("out/matchedout_" + filesize + "_" + effic_type + ".root").c_str(), "RECREATE");
    
    TH1 *ptdiff = new TH1D("ptdiff",""/*"p_{T} Difference"*/,100,0,100);
    TH1 *ptpart = new TH1D("ptpart",""/*"Particle-level charged leading matched jet p_{T}"*/, 15, 15, 30);
    TH1 *ptdet = new TH1D("ptdet",""/*"Detector-level charged leading matched jet p_{T}"*/, 15, 15, 30);
    TH1 *ptcorr = new TH1D("ptcorr_real",""/*"Corrected detector-level jet p_{T}"*/, 15, 15, 30);
    TH1 *ptadded = new TH1D("ptadded",""/*"p_{T} added back"*/, 100, 0, 100);
    TH1 *numadded = new TH1D("numadded","",20,-0.5,19.5);
    
    unsigned nEntries = matched->GetEntries(); //number of charged jets

    radial_dist->GetYaxis()->SetTitle("1/N_{jets} dN_{tracks}/d#DeltaR");
    radial_dist->GetXaxis()->SetTitle("#DeltaR");

    TCanvas *c0 = new TCanvas("c0","c0",800,800); c0->SetLogy(); c0->cd();
    radial_dist->Draw();
    
    for (unsigned i = 0; i < nEntries; ++i) {
        part->Clear(); det->Clear();
        matched->GetEntry(i);
        
        if (part->GetEntriesFast() != det->GetEntriesFast()) {cerr << "HEY WTF" << endl; exit(1);}
        for (unsigned j = 0; j < part->GetEntriesFast(); ++ j) {
            TLorentzVector *partjet = (TLorentzVector*) part->At(j);
            TLorentzVector *detjet = (TLorentzVector*) det->At(j);
            ptdiff->Fill(partjet->Pt() - detjet->Pt());
            ptpart->Fill(partjet->Pt());
            ptdet->Fill(detjet->Pt());
            
            Double_t ptadd = 0;
            
            double ktop = poisnaive->GetRandom();
            numadded->Fill((int) ktop);
            for (unsigned k = 0; k < ktop; ++ k) {
	      ptadd += mult->GetRandom();
            }
            ptadded->Fill(ptadd);
            
            ptcorr->Fill(detjet->Pt() + ptadd);
        }
    }
    
    out->cd();
    numdiff->Write();
    radial_dist->Write();
    out->Write();
    cout << "Writing to out/matchedout_" << filesize << "_" << effic_type << ".root\n";
    
    return;
}
