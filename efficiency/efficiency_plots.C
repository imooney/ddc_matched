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
  gStyle->SetOptStat(0);
  gSystem->Load("lib/libMyJetLib.so"); //doesn't work here for some reason - have to load it in a root session
  
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    TFile *input = new TFile("../out/ddc_matched_skinny_real.root");
    
    TFile *out = new TFile("out/effic_plots.root", "RECREATE");

    TH1D *conspt_pminusd = (TH1D*) input->Get("ptconsdetmatched");//("cons_pt");
    
    //~~~~~~~~~~~~(1/eff - 1)~~~~~~~~~~~~//
    ktTrackEff* eff = new ktTrackEff();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    TF2* funcpp = new TF2("funcpp"/*"ppEfficiency"*/,"[0]-0.06-[1]*exp([2]/x)+[3]*exp(-0.5*((x-[4])/[5])**2)/sqrt(2*pi*[5]*[5])-[6]*exp(-0.5*((x-[7])/[8])**2)/sqrt(2*pi*[8]*[8])+([9]-[10]*(y-[11])^2-[12]*(y-[11])^4-[13]*(y-[11])^6-[14]*(y-[11])^8)*exp(-[15]*x)",0.2,30.2,-1.,1.);

  Double_t parset[] = {0.869233,0.0223402,0.44061,0.558762,0.145162,0.508033,110.008,-4.63659,1.73765,0.0452674,-0.101279,0.0081551,0.945287,-2.00949,1.61746,1.39352};

  ((TF2*)funcpp)->SetParameters(parset);

  TF2* eff_inv_minus_one = new TF2 ( "eff_inv_minus_one", "1./funcpp - 1", 0.2,30.2,-1.,1.);
  TF12 *eff_inv_minus_one_proj = new TF12("eff_inv_minus_one_proj",eff_inv_minus_one,0,"x");
  TF12 *eff_proj = new TF12("eff_proj",funcpp,0,"x");
  
  
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    TH1D * mult = (TH1D*) conspt_pminusd->Clone();
    mult->SetDirectory(0);
    mult->SetName("mult");

    //TCanvas *c1 = new TCanvas("c1","c1",800,800);c1->cd(); //c1->SetLogy();
    eff_inv_minus_one_proj->GetXaxis()->SetRangeUser(0.5,60);
    eff_inv_minus_one_proj->SetLineColor(kBlue);
    //eff_proj->Draw("same");
    mult->Multiply(eff_inv_minus_one);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    out->cd();
    conspt_pminusd->Write();
    mult->Write();
    out->Write();
    eff_inv_minus_one->Write();
    eff_inv_minus_one_proj->Write();

    cout << "Writing to out/effic_plots.root\n";
    
    return;
}
