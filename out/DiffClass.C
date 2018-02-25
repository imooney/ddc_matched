#define DiffClass_cxx
#include "DiffClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void DiffClass::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L DiffClass.C
//      Root > DiffClass t
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
    
    string path = "../histos/diffs/";
    string type = "num_diff";
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    
    //gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);
    
    TH1 * numdiff   = new TH1D((type).c_str(), "Number difference before - after efficiency", 100, 0, 30);
    TH1 * cnumdiff  = new TH1D(("c_" + type).c_str(), "Number difference before - after efficiency (charged)", 10, 0, 10);
    TH1 * reldiff   = new TH1D("rel_diff", "ratio of track difference to number of tracks",100, -0.1, 1.1);
    TH1 * creldiff  = new TH1D("crel_diff", "ratio of track difference to number of tracks (charged)",100, -0.1, 1.1);
    
    TH2 * reldiff2D = new TH2D("reldiff2D","relative difference v. number of tracks",100,-0.1,1.1,100,0,25);
    TH2 * creldiff2D= new TH2D("creldiff2D","relative difference v. number of tracks (charged)",100,-0.1,1.1,100,0,25);
    
    TH3 * cdifftotpt = new TH3D("cdifftotpt", "num diff, total num, pt diff (charged)",100, -5, 10, 100, 0, 25, 100, -20, 5);
    TH3 * difftotpt = new TH3D("difftotpt", "num diff, total num, pt diff", 100, -5, 10, 100, 0, 25, 100, -20, 5);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        numdiff->Fill(num_diff);
        cnumdiff->Fill(c_num_diff);
        reldiff->Fill(rel_diff);
        creldiff->Fill(c_rel_diff);
        reldiff2D->Fill(rel_diff, num_before);
        creldiff2D->Fill(c_rel_diff, c_num_before);
        cdifftotpt->Fill(c_num_diff, c_num_before, c_ptdiff);
        difftotpt->Fill(num_diff, num_before, ptdiff);
    }
    /*
    TH1D * reldiffprofX = reldiff2D->ProfileX("reldiffprofX", 0, 5);
    TH1D * creldiffprofX = creldiff2D->ProfileX("creldiffprofX", 0, 5);
    
    TH1D * reldiffX = reldiff2D->ProjectionX("reldiffX", 8, 16);
    TH1D * creldiffX = creldiff2D->ProjectionX("creldiffX", 8, 16);
    */

    
    
    Int_t nEntries = cnumdiff->GetEntries();
    /*
    const char *xaxis = "Number difference";
    const char *yaxis = "Total number";
    const char *zaxis = "p_{T} difference [GeV/c]";
    
    c1->SetLogz();
    
    reldiff2D->GetXaxis()->SetTitle("Relative difference"); reldiff2D->GetYaxis()->SetTitle("Total Number");
    reldiff2D->DrawCopy("colz"); c1->SaveAs((path + "reldiff2D.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    creldiff2D->GetXaxis()->SetTitle("Relative difference"); creldiff2D->GetYaxis()->SetTitle("Total Number");
    creldiff2D->DrawCopy("colz"); c1->SaveAs((path + "c_reldiff2D.pdf").c_str()); gPad->Modified(); gPad->Update(); c1->SetLogz(0);
    
    
    
    reldiffprofX->GetXaxis()->SetTitle("Total Number"); reldiffprofX->GetYaxis()->SetTitle("Relative difference");
    reldiffprofX->DrawCopy(); c1->SaveAs((path + "reldiffprofileX.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    creldiffprofX->GetXaxis()->SetTitle("Total Number"); creldiffprofX->GetYaxis()->SetTitle("Relative difference");
    creldiffprofX->DrawCopy(); c1->SaveAs((path + "c_reldiffprofileX.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    c1->SetLogy();
    
    reldiffX->GetXaxis()->SetTitle("Relative difference");
    reldiffX->DrawCopy(); c1->SaveAs((path + "reldiffX.pdf").c_str()); gPad->Modified(); gPad->Update();
    creldiffX->GetXaxis()->SetTitle("Relative difference");
    creldiffX->DrawCopy(); c1->SaveAs((path + "c_reldiffX.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    reldiff->Scale(1/(double) nEntries);
    creldiff->Scale(1/(double) nEntries);
    
    reldiff->GetXaxis()->SetTitle("Number difference/Total number");
    reldiff->DrawCopy(); c1->SaveAs((path + "reldiff.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    creldiff->GetXaxis()->SetTitle("Number difference/Total number");
    creldiff->DrawCopy(); c1->SaveAs((path + "creldiff.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    c1->SetLogy(0);
    
    //charged
    
    cdifftotpt->Project3D("yx"); //first vertical, second horizontal
    cdifftotpt_yx->GetXaxis()->SetTitle(xaxis); cdifftotpt_yx->GetYaxis()->SetTitle(yaxis);
    c1->SetLogz(); cdifftotpt_yx->DrawCopy("colz"); c1->SaveAs((path + "cdifftotpt_yx.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    cdifftotpt->Project3D("zy");
    cdifftotpt_zy->GetXaxis()->SetTitle(yaxis); cdifftotpt_zy->GetYaxis()->SetTitle(zaxis);
    cdifftotpt_zy->DrawCopy("colz"); c1->SaveAs((path + "cdifftotpt_zy.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    cdifftotpt->Project3D("zx");
    cdifftotpt_zx->GetXaxis()->SetTitle(xaxis); cdifftotpt_zx->GetYaxis()->SetTitle(zaxis);
    cdifftotpt_zx->DrawCopy("colz"); c1->SaveAs((path + "cdifftotpt_zx.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    
    cdifftotpt->Project3D("x");
    cdifftotpt_x->GetXaxis()->SetTitle(xaxis);
    cdifftotpt_x->Scale(1/(double) nEntries); c1->SetLogy(); cdifftotpt_x->DrawCopy(); c1->SaveAs((path + "cdifftotpt_x.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    cdifftotpt->Project3D("y");
    cdifftotpt_y->GetXaxis()->SetTitle(yaxis);
    cdifftotpt_y->Scale(1/(double) nEntries); cdifftotpt_y->DrawCopy(); c1->SaveAs((path + "cdifftotpt_y.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    cdifftotpt->Project3D("z");
    cdifftotpt_z->GetXaxis()->SetTitle(zaxis);
    cdifftotpt_z->Scale(1/(double) nEntries); cdifftotpt_z->DrawCopy(); c1->SaveAs((path + "cdifftotpt_z.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    
    //neutral + charged
    
    difftotpt->Project3D("yx"); //first vertical, second horizontal
    difftotpt_yx->GetXaxis()->SetTitle(xaxis); difftotpt_yx->GetYaxis()->SetTitle(yaxis);
    c1->SetLogz(); difftotpt_yx->DrawCopy("colz"); c1->SaveAs((path + "difftotpt_yx.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    difftotpt->Project3D("zy");
    difftotpt_zy->GetXaxis()->SetTitle(yaxis); difftotpt_zy->GetYaxis()->SetTitle(zaxis);
    difftotpt_zy->DrawCopy("colz"); c1->SaveAs((path + "difftotpt_zy.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    difftotpt->Project3D("zx");
    difftotpt_zx->GetXaxis()->SetTitle(xaxis); difftotpt_zx->GetYaxis()->SetTitle(zaxis);
    difftotpt_zx->DrawCopy("colz"); c1->SaveAs((path + "difftotpt_zx.pdf").c_str()); gPad->Modified(); gPad->Update();
    
    
    difftotpt->Project3D("x");
    difftotpt_x->GetXaxis()->SetTitle(xaxis);
    difftotpt_x->Scale(1/(double) nEntries); c1->SetLogy(); difftotpt_x->DrawCopy(); c1->SaveAs((path + "difftotpt_x.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    difftotpt->Project3D("y");
    difftotpt_y->GetXaxis()->SetTitle(yaxis);
    difftotpt_y->Scale(1/(double) nEntries); difftotpt_y->DrawCopy(); c1->SaveAs((path + "difftotpt_y.pdf").c_str());
    gPad->Modified(); gPad->Update();
    
    difftotpt->Project3D("z");
    difftotpt_z->GetXaxis()->SetTitle(zaxis);
    difftotpt_z->Scale(1/(double) nEntries); difftotpt_z->DrawCopy(); c1->SaveAs((path + "difftotpt_z.pdf").c_str());
    gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    */
    //gStyle->SetOptStat(1);
    cnumdiff->GetXaxis()->SetTitle("Number of tracks");
    /**/numdiff->Scale(1/(double) nEntries);
    cnumdiff->Scale(1/(double) nEntries); c1->SetLogy();
    /*
    TF1 * f = new TF1("f","[0]*ROOT::Math::negative_binomial_pdf(x,[1],[2])",0, 10);
    //TF1 * f = new TF1("f","[0]*ROOT::Math::negative_binomial_pdf(x,[1],[2])",0, 10);
    f->SetParameter(0, 0.1);
    f->SetParameter(1, 0.6);
    f->SetParameter(2, 20);
    numdiff->Fit("f");*/
    //cout << "NEUTRAL + CHARGED" << endl;
    TF1 * fc = new TF1("fc","[0]*ROOT::Math::negative_binomial_pdf(x,[1],[2])",0, 10);
    TF1 * fcbin = new TF1("fcbin","[0]*ROOT::Math::binomial_pdf(x,[1],[2])",0, 10);
    TF1 * fcpois = new TF1("fcpois","[0]*ROOT::Math::poisson_pdf(x,[1])", 0, 10);
    
    fc->SetParameter(0, 1);
    fc->SetParameter(1, 0.8);
    fc->SetParameter(2, 3.717);
    cnumdiff->Fit("fc", "MEI");
    
    fcbin->SetParameter(0, 1);
    fcbin->SetParameter(1, 0.2);
    fcbin->SetParameter(2, 5.043);
    cnumdiff->Fit("fcbin", "ME");
    
    fcpois->SetParameter(0, 1);
    fcpois->SetParameter(1, 1.826);
    cnumdiff->Fit("fcpois", "MEI");
    
    //cout << "CHARGED" << endl;
    cnumdiff->SetLineColor(1); fc->SetLineColor(2); fcbin->SetLineColor(1); fcpois->SetLineColor(3);/*f->SetLineColor(1);*/
    /*numdiff->DrawCopy(); f->DrawCopy("same");*/ cnumdiff->DrawCopy();fc->DrawCopy("same"); fcbin->DrawCopy("same"); fcpois->DrawCopy("same");
    //
    TLegend * l = new TLegend(0.5, 0.75, 0.75, 0.875);
    //l->AddEntry(numdiff, "charged + neutral", "lep");
    l->AddEntry(cnumdiff, "number difference", "lep");
    l->AddEntry(fc, "nbd", "l");
    l->AddEntry(fcbin, "binomial", "l");
    l->AddEntry(fcpois, "poisson", "l");
    l->SetBorderSize(0);
    l->Draw();
    //
    //c1->SaveAs((path /*+ type*/ + "comparefits.pdf").c_str());
    gPad->Modified(); gPad->Update(); /*c1->SetLogy(0);*/
    
    //Double_t chi2 = fc->GetChisquare();
    //cout << "Chi squared: " << chi2 << "!" << '\n';
    //Double_t p1 = f->GetParameter(0);
    //Double_t e1 = f->GetParError(0);
    
    //cnumdiff->GetXaxis()->SetTitle("Number of jets");
    /*cnumdiff->Scale(1/(double) nEntries);*/ /*c1->SetLogy();*/ //cnumdiff->DrawCopy(); c1->SaveAs((path + "c_" + type + ".pdf").c_str());
    //gPad->Modified(); gPad->Update(); c1->SetLogy(0);
    //gStyle->SetOptStat(0);
    
    //delete c1; c1 = NULL;
}
