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

#include "efficiency/src/ktTrackEff.hh"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void matchedjetdiffs () {
  gStyle->SetOptStat(0);
  gSystem->Load("efficiency/lib/libMyJetLib.so");
  
    //gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    TFile *tracks2D = new TFile("out/trackloss2D.root", "READ");
    TH2 * bin2D = (TH2D*) tracks2D->Get("tracksvbin");
    
    TFile *input = new TFile("test_please_delete.root");//"out/ddc_matched_skinny_real.root");
    TTree *matched = (TTree*) input->Get("matchedJets");
    TF1 * nbnaive = new TF1("nbnaive","ROOT::Math::negative_binomial_pdf(x,0.8,1)",-0.5, 10.5);
    TF1 * poisnaive = new TF1("poisnaive","ROOT::Math::poisson_pdf(x,0.43)",-0.5,10.5);
    TF1 * binnaive = new TF1("binnaive","ROOT::Math::binomial_pdf(x,0.2,4)",-0.5,10.5);
    TH1 * numdiff = (TH1D*) input->Get("numdiffmatchedperm");
    TH1 * consfromhisto = (TH1D*) input->Get("cons_ptperm");//this is NOT matched!!!
    TCanvas *c0 = new TCanvas("c0", "c0", 600, 800);// c0->SetLogy();
    
    Double_t weight, det_multiplicity;
    TClonesArray *part = new TClonesArray("TLorentzVector");
    TClonesArray *det = new TClonesArray("TLorentzVector");
    TClonesArray *cons = new TClonesArray("TLorentzVector");
    matched->SetBranchAddress("c_matchedpartJets",&part);
    matched->SetBranchAddress("c_matcheddetJets",&det);
    matched->SetBranchAddress("c_matchedCons", &cons);
    matched->SetBranchAddress("weight", &weight);
    matched->SetBranchAddress("det_multiplicity", &det_multiplicity);
    
    TFile *out = new TFile("out/matchedout_realeffic.root", "RECREATE");
    
    TH1 *ptdiff = new TH1D("ptdiff","p_{T} Difference",100,0,100);
    TH1 *ptpart = new TH1D("ptpart","Particle-level charged leading matched jet p_{T}", 15, 15, 30);
    TH1 *ptdet = new TH1D("ptdet","Detector-level charged leading matched jet p_{T}", 15, 15, 30);
    TH1 *ptcorr = new TH1D("ptcorr_real", "Corrected detector-level jet p_{T}", 15, 15, 30);
    TH1 *ptadded = new TH1D("ptadded", "p_{T} added back", 100, 0, 100);
    TH1D *conspt_pminusd = new TH1D("conspt_pminusd","Constituent p_{T} spectrum (detector-level)",30,0,30);//,60,0,60);
    TH2 *cons2D = new TH2D("cons2D","Constituent spectrum (detector-level)",10,-1,1,10,0,10);
    TH1 *numadded = new TH1D("numadded","",20,-0.5,19.5);
    
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
    //mult->Scale(1/mult->Integral());
    mult->SetName("mult");
    mult->Draw();
    //conspt_pminusd->Draw("same");

    //cout << mult->Integral(0,10)<< '\n';

    TCanvas *eff2D = new TCanvas("eff2D","eff2D",800,800);
    eff2D->cd();
    effhist->Draw("colz");
    
    TCanvas *effprofx = new TCanvas("effprofx","effprofx",800,800);
    effprofx->cd();
    effprofX->Draw();

    TCanvas *effprofy = new TCanvas("effprofy","effprofy",800,800);
    effprofy->cd();
    effprofY->Draw();

    TCanvas *effprojx = new TCanvas("effprojx","effprojx",800,800);
    effprojx->cd();
    effprojX->Draw();

    TCanvas *effprojy = new TCanvas("effprojy","effprojy",800,800);
    effprojy->cd();
    effprojY->Draw();

    TCanvas *effinverse = new TCanvas("effinverse","effinverse",800,800);
    effinverse->cd();
    effinv->Draw("colz");

    TCanvas *effinverseminus1 = new TCanvas("effinverseminus1","effinverseminus1",800,800);
    effinverseminus1->cd();
    effinvminus1->Draw("colz");
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    
    
    
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
            //cout << (int) (detjet->Pt() - 1) << '\n';
            //cout << (effprojY->GetBinContent((int)(detjet->Pt() - 1))/(double) 10) << '\n';
            //WANT TO DIVIDE detector-level multiplicity BY (EFFPROJY/10)
            Double_t dividebythis = 0.8; //effprojY->GetBinContent(1/*(int)(detjet->Pt() - 1)*/)/(double) 10;
            Double_t mocked_up_part_mult = det_multiplicity/ (double) dividebythis;
            
            TH1D * project = bin2D->ProjectionX("project",bin2D->GetYaxis()->FindBin(mocked_up_part_mult),bin2D->GetYaxis()->FindBin(mocked_up_part_mult));//(int) mocked_up_part_mult, (int) mocked_up_part_mult);
            //cout << det_multiplicity << " " << mocked_up_part_mult << " " << (int) mocked_up_part_mult << " " << project->GetRandom() << '\n';
            double ktop = project->GetRandom();//nbnaive->GetRandom();
            numadded->Fill((int) ktop);
            for (unsigned k = 0; k < ktop; ++ k) {
	      ptadd += mult->GetRandom();
            }
            ptadded->Fill(ptadd);
            
            ptcorr->Fill(detjet->Pt() + ptadd);
        }
    }

    
    
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
    
    ptpart->Draw();
    ptcorr->Draw("same");
    ptdet->Draw("same");
    
    TLegend * leg2 = new TLegend(0.6,0.75,0.8,0.875);
    leg2->AddEntry("ptpart", "particle-level", "lep");
    leg2->AddEntry("ptdet", "detector-level", "lep");
    leg2->AddEntry("ptcorr_real", "corrected detector-level", "lep");
    leg2->SetBorderSize(0);
    
    leg2->Draw("same");
    
    //one that dips is ptcorr
    
    TCanvas *c5 = new TCanvas("c5", "c5", 800, 800); c5->SetLogy();
    c5->cd();
    
    nbnaive->SetLineColor(kOrange + 1);
    poisnaive->SetLineColor(kBlue + 1);//6); //magenta-ish
    binnaive->SetLineColor(kGreen + 1);//7); //light blue-green?
    numadded->SetLineColor(kRed + 1);
    numdiff->Draw();
    numadded->Draw("same");
    //nbnaive->Draw("same");
    //poisnaive->Draw("same");
    //binnaive->Draw("same");

    TLegend * leg_funcs = new TLegend(0.6,0.75,0.8,0.875);
    //leg_funcs->AddEntry("nbnaive", "negative binomial", "l");
    //leg_funcs->AddEntry("poisnaive", "poisson", "l");
    //leg_funcs->AddEntry("binnaive", "binomial", "l");
    leg_funcs->AddEntry("numdiffmatchedperm", "number lost", "lep");
    leg_funcs->AddEntry("numadded", "number added", "lep");
    leg_funcs->SetBorderSize(0);
    
    leg_funcs->Draw("same");
    
    //ptdiff->Draw();
    //ptadded->Draw("same");
    //ptpart->Draw();
    //ptdet->Draw();
    //ptcorr->Draw("same");
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 800); c1->SetLogy();
    c1->cd();
    conspt_pminusd->Draw();
    
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 800); c2->SetLogy();
    c2->cd();
    ptdiff->Draw(); ptadded->Draw("same");
    
    TLegend * leg3 = new TLegend(0.6,0.75,0.8,0.875);
    leg3->AddEntry("ptdiff", "pT lost", "lep");
    leg3->AddEntry("ptadded", "pT added back", "lep");
    leg3->SetBorderSize(0);
    
    leg3->Draw("same");
    
    TCanvas *c4 = new TCanvas("c4", "c4", 800, 800); c4->SetLogy();
    c4->cd();
    ptdet->Draw();
    
    out->cd();
    effprojY->Write();
    mult->Write();
    out->Write();
    cout << "Writing to out/matchedout.root\n";
    
    return;
}
