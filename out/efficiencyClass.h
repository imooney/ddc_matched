//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May  6 14:52:51 2017 by ROOT version 5.34/36
// from TTree efficiency/efficiency
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef efficiencyClass_h
#define efficiencyClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class efficiencyClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        effic_px;
   Double_t        effic_py;
   Double_t        effic_pz;
   Double_t        effic_E;
   Double_t        effic_phi;
   Double_t        effic_eta;
   Double_t        effic_Pt;

   // List of branches
   TBranch        *b_effic_px;   //!
   TBranch        *b_effic_py;   //!
   TBranch        *b_effic_pz;   //!
   TBranch        *b_effic_E;   //!
   TBranch        *b_effic_phi;   //!
   TBranch        *b_effic_eta;   //!
   TBranch        *b_effic_Pt;   //!

   efficiencyClass(TTree *tree=0);
   virtual ~efficiencyClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef efficiencyClass_cxx
efficiencyClass::efficiencyClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("efficiency",tree);

   }
   Init(tree);
}

efficiencyClass::~efficiencyClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t efficiencyClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t efficiencyClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void efficiencyClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("effic_px", &effic_px, &b_effic_px);
   fChain->SetBranchAddress("effic_py", &effic_py, &b_effic_py);
   fChain->SetBranchAddress("effic_pz", &effic_pz, &b_effic_pz);
   fChain->SetBranchAddress("effic_E", &effic_E, &b_effic_E);
   fChain->SetBranchAddress("effic_phi", &effic_phi, &b_effic_phi);
   fChain->SetBranchAddress("effic_eta", &effic_eta, &b_effic_eta);
   fChain->SetBranchAddress("effic_Pt", &effic_Pt, &b_effic_Pt);
   Notify();
}

Bool_t efficiencyClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void efficiencyClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t efficiencyClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef efficiencyClass_cxx
