//Created by Isaac Mooney on 2/20/18

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

#include "src/ktTrackEff.hh"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void efficiency_plots () {
  //gRandom->Uniform(0.0, 2pi) for later (check if it's -pi to pi or 0 to 2pi).
  gStyle->SetOptStat(0);
  gSystem->Load("lib/libMyJetLib.so"); //doesn't work here for some reason - have to load it in a root session
  
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    TFile *input = new TFile("../out/ddc_matched_skinny_real_fixed_weighting.root");
    TTree *matched = (TTree*) input->Get("matchedJets");
    TH1 * radial_dist = (TH1D*) input->Get("radial_distmatched"); //IF YOU SWITCH BACK TO "ddc_matched_skinny_real.root" MAKE SURE TO APPEND "perm" TO radial_distmatched
    
    Double_t weight;
    TClonesArray *part = new TClonesArray("TLorentzVector");
    TClonesArray *det = new TClonesArray("TLorentzVector");
    TClonesArray *cons = new TClonesArray("TLorentzVector");
    matched->SetBranchAddress("c_matchedpartJets",&part);
    matched->SetBranchAddress("c_matcheddetJets",&det);
    matched->SetBranchAddress("c_matchedCons", &cons);
    matched->SetBranchAddress("weight", &weight);
    
    TFile *out = new TFile("out/effic_plots.root", "RECREATE");
   
    TH1D *conspt_pminusd = new TH1D("conspt_pminusd","Constituent p_{T} spectrum (detector-level)",30,0,30);//,60,0,60);
    TH2 *cons2D = new TH2D("cons2D","Constituent spectrum (detector-level)",10,-1,1,10,0,10);
    
    unsigned nEntries = matched->GetEntries();
    
    for (unsigned i = 0; i < nEntries; ++ i) {
        cons->Clear();
        matched->GetEntry(i);
        for (unsigned j = 0; j < cons->GetEntriesFast(); ++ j) {
            TLorentzVector *detcons = (TLorentzVector*) cons->At(j);
            conspt_pminusd->Fill(detcons->Pt());
	    cons2D->Fill(detcons->Eta(), detcons->Pt());
        }
    }
    conspt_pminusd->Scale(1/conspt_pminusd->Integral());

    //~~~~~~~~~~~~(1/eff - 1)~~~~~~~~~~~~//
    ktTrackEff* eff = new ktTrackEff();

    unsigned eta_bins = 10, pt_bins = 30;
    TH2 *effhist = new TH2D("effhist","",eta_bins,-1,1,pt_bins,0,30);
    TH2 *effinv = new TH2D("effinv","",eta_bins,-1,1,pt_bins,0,30);
    TH2 *effinvminus1 = new TH2F("effinvminus1","",eta_bins,-1,1,pt_bins,0,30);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    /*
    TF2* funcpp = new TF2("ppEfficiency","[0]-0.06-[1]*exp([2]/x)+[3]*exp(-0.5*((x-[4])/[5])**2)/sqrt(2*pi*[5]*[5])-[6]*exp(-0.5*((x-[7])/[8])**2)/sqrt(2*pi*[8]*[8])+([9]-[10]*(y-[11])^2-[12]*(y-[11])^4-[13]*(y-[11])^6-[14]*(y-[11])^8)*exp(-[15]*x)",0.,10.,-1.,1.);

  Double_t parset[] = {0.869233,0.0223402,0.44061,0.558762,0.145162,0.508033,110.008,-4.63659,1.73765,0.0452674,-0.101279,0.0081551,0.945287,-2.00949,1.61746,1.39352};

  ((TF2*)funcpp)->SetParameters(parset);
    */
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    for (double i = 0; i < eta_bins; ++ i) {
      for (double j = 0; j < pt_bins; ++ j) {
	//effhist->Fill(-0.9 + i/ (double) 5, j + 0.5, eff->EffPPY06(-0.9 + i/(double) 5, j + 0.5));
	double inew = -1 + (1/(double) eta_bins) + 2*i/ (double) eta_bins; double jnew = j + 0.5;
	Double_t bin_num = effhist->FindBin(inew, jnew);
	effhist->SetBinContent(bin_num, eff->EffPPY06(inew, jnew));
	effhist->SetBinError(bin_num, 0.0005*(eff->EffPPY06(inew, jnew)));
	effinv->Fill(inew, jnew, 1/(double)eff->EffPPY06(inew, jnew));
	effinvminus1->Fill(inew, jnew, ((1/(double)eff->EffPPY06(inew, jnew)) - 1));
      }
    }
    effinvminus1->Scale(1/effinvminus1->Integral());

    TH1D *effprojY = effhist->ProjectionY("projy",0,10);
    TProfile *effprofX = effhist->ProfileX("profx",0,30);
    TH1D *effprojX = effhist->ProjectionX("projx",0,30);
    TProfile *effprofY = effhist->ProfileY("profy",0,10);

    TH1D *projfinal = effinvminus1->ProjectionY("projfinal",0,10);
    TH1D *mult = new TH1D("mult","",30,0,30);
      *mult = (*conspt_pminusd)*(*projfinal);
    mult->SetName("mult");
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    out->cd();
    effprojY->Write();
    mult->Write();
    out->Write();
    cout << "Writing to out/effic_plots.root\n";
    
    return;
}
