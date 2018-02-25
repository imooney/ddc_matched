//  numloss.c
//  represents a ~failed~ attempt at a 2D correction: an appropriate binomial distribution
//  for each multiplicity, to be sampled in matchedjetdiffs.C.
//  Created by Isaac Mooney on 2/4/18.

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include <iostream>
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"

#include <vector>

using namespace std;

const unsigned num_events = 1e5;
const unsigned num_tracks = 20;

void numloss() {
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    TFile *trackloss2D = new TFile("out/trackloss2D.root","RECREATE");
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    //gStyle->SetOptStat(0);
    TH2 *binloss = new TH2D("tracksvbin","", 20, -0.5, 19.5, num_tracks, -0.5, 19.5);
    
    for (int j = 1; j < num_tracks; ++ j) {
        TF1 * b = new TF1("b", "ROOT::Math::binomial_pdf(x,[0],[1])", -0.5, 19.5);
        //TF1 * p = new TF1("p", "ROOT::Math::poisson_pdf(x,[0])", -0.5, 19.5);
        //p->SetParameter(0, j*0.2);
        b->SetParameter(0, 0.227);
        b->SetParameter(1, j);
        for (unsigned i = 0; i < num_events; ++ i) {
            binloss->Fill(b->GetRandom(), j);
            //binloss->Fill(p->GetRandom(), j); //not renaming histo "poisloss" because lazy
        }
    }
    binloss->GetXaxis()->SetTitle("number loss");
    binloss->GetYaxis()->SetTitle("particle-level multiplicity");
    
    binloss->Draw("colz");
    TH1D * projtest = binloss->ProjectionX("proj",5,5);
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    c2->cd();
    projtest->Draw();
    
    trackloss2D->Write();
    
    return;
}
