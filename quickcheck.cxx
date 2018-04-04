{
  TFile * f0 = new TFile("lost.root");
  TH1D* lost = (TH1D*) f0->Get("numdiffmatched");
  TFile * f1 = new TFile("pttoadd.root");
  TH1D* pttoadd = (TH1D*) f1->Get("mult");

  
  // new TCanvas; gPad->SetLogy();
  // pttoadd->Draw();

  int binl = lost->FindBin(0+0.001);
  int binr = lost->FindBin(9-0.001);
  // lost->GetXaxis()->SetRange(binl, binr);
    
  TH1D* losttosample = new TH1D ( "losttosample","", binr-binl+1, 0, 10 );
  for ( int i=0; i<losttosample->GetNbinsX(); ++i ){
    int bin = lost->FindBin ( losttosample->GetBinLowEdge(i) );
    losttosample->SetBinContent ( i, lost->GetBinContent ( bin ) );
  }

  new TCanvas;
  lost->Draw();
  losttosample->SetLineColor(kGreen +1 );
  losttosample->Draw("same");

  
  TH1D* AddedBack = new TH1D("AddedBack", "", 200, -10, 100);
  TH1D* nlostused = (TH1D*) lost->Clone("nlostused");

    
    
  nlostused->Reset();
  
  for ( int i=0; i<100000; ++i ){
    // int nlost = lost->GetRandom();
    int nlost = losttosample->GetRandom();
    float pt=0;
    nlostused->Fill(nlost);
    for ( int j = 0; j<nlost; ++j ){
      pt += pttoadd->GetRandom();
    }
    AddedBack->Fill(pt);
  }

  new TCanvas;
  AddedBack->Draw();


  new TCanvas;
  lost->SetLineColor(kBlack);
  nlostused->SetLineColor(kRed);
  nlostused->DrawNormalized();
  lost->DrawNormalized("same");

}
