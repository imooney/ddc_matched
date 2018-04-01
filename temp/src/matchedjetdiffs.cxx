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

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <math.h>

#include "../../myLorentzVector.h"

using namespace std;
using namespace fastjet;

//hardcoding mass of tracks to be m_pi0:
double m_pi0 = 0.1349766;

int main (){//(string filesize, string effic_type) {
  
    gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();

    string filesize = "skinny", effic_type = "real";
    //cout << "Please enter 'skinny' / 'fat' then 'real' / 'flat' \n";

    //HARD-CODING THIS WHILE IM COMPILING cin >> filesize >> effic_type;

    TFile *input = new TFile(("../out/ddc_matched_" + filesize + "_" + effic_type + ".root").c_str(), "READ");
    TTree *matched = (TTree*) input->Get("matchedJets");
    TF1 * nbnaive = new TF1("nbnaive","ROOT::Math::negative_binomial_pdf(x,0.8,1)",-0.5, 10.5);
    TF1 * poisnaive = new TF1("poisnaive","ROOT::Math::poisson_pdf(x,1.22)"/*0.63)*/ /*0.4764)"*/,-0.5,10.5);
    TF1 * binnaive = new TF1("binnaive","ROOT::Math::binomial_pdf(x,0.2,5)",-0.5,10.5);
    TH1 * numdiff = (TH1D*) input->Get("numdiffmatched");
    TH1 * radial_dist = (TH1D*) input->Get("radial_distmatched");
    TFile *effic = new TFile("../efficiency/out/effic_plots.root", "READ");
    TH1 * mult = (TH1D*) effic->Get("mult");
    
    Double_t weight;
    TClonesArray *part = new TClonesArray("myLorentzVector");
    TClonesArray *det = new TClonesArray("myLorentzVector");
    TClonesArray *cons = new TClonesArray("myLorentzVector");
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
    TH1 *mass = new TH1D("mass", "", 100, -0.5,99.5);
    TH1 *test_rad = new TH1D("test_rad","",40,0,0.4);
    TH1 *test_phi = new TH1D("test_phi","",40,-7,7);
    TH1 *test_y = new TH1D("test_y","",40,-2,2);
    TH1 *test_delta_r = new TH1D("test_delta_r","",80,-4,4);
    TH1 *test_track_masses = new TH1D("test_track_masses","",80,0,80);
    
    unsigned nEntries = matched->GetEntries(); //number of charged jets

    radial_dist->GetYaxis()->SetTitle("1/N_{jets} dN_{tracks}/d#DeltaR");
    radial_dist->GetXaxis()->SetTitle("#DeltaR");
    
    for (unsigned i = 0; i < nEntries; ++i) {
        part->Clear(); det->Clear();
        matched->GetEntry(i);
        
        if (part->GetEntriesFast() != det->GetEntriesFast()) {cerr << "HEY WTF" << endl; exit(1);}
        for (unsigned j = 0; j < part->GetEntriesFast(); ++ j) {
            myLorentzVector *partjet = (myLorentzVector*) part->At(j);
            myLorentzVector *detjet = (myLorentzVector*) det->At(j);
            ptdiff->Fill(partjet->Pt() - detjet->Pt());
            ptpart->Fill(partjet->Pt());
            ptdet->Fill(detjet->Pt());
            
            Double_t ptadd = 0;
            
            double ktop = poisnaive->GetRandom();
            numadded->Fill((int) ktop);

            //SIMPLE METHOD!!!
            for (unsigned k = 0; k < ktop; ++ k) {
                double pt_single = mult->GetRandom();
                ptadd += pt_single;
            }
            
            //COMPLICATED METHOD!!! - come back to this when you output pseudojets
            fastjet::PseudoJet jet_add = fastjet::PseudoJet(0,0,0,0);
            std::vector<fastjet::PseudoJet> track_collection;
            track_collection.reserve(50);
            
            for (unsigned k = 0; k < ktop; ++ k) {
                double pt_single = mult->GetRandom();
                double phi_dist = gRandom->Uniform(0.0, 2*M_PI);
                double rad_dist = radial_dist->GetRandom();
                double y_dist = rad_dist*cos(phi_dist);
                double phi_abs = /*detjet->Phi() + */phi_dist;
                double rap_abs = /*detjet->Rapidity() +*/ y_dist;
                double m_track = m_pi0;
                
                fastjet::PseudoJet track_add = fastjet::PseudoJet(0,0,0,0);
                track_add.reset_PtYPhiM(pt_single, rap_abs, phi_abs, m_track);
                track_collection.push_back(track_add);
                //test_track_masses->Fill(track_add.m());
                //cout << pt_single << " " << rap_abs << " " << phi_abs << " " << m_track <<'\n';
                //jet_add += track_add;
                //ptadd += pt_single;
            }
            
            //cout << jet_add.m() << '\n';
            mass->Fill(jet_add.m());
            ptadded->Fill(ptadd);
            
            ptcorr->Fill(detjet->Pt() + ptadd);
            
            
        }
    }
    
    out->cd();
    numdiff->Write();
    radial_dist->Write();
    out->Write();
    cout << "Writing to out/matchedout_" << filesize << "_" << effic_type << ".root\n";
    
    return 0;
}
