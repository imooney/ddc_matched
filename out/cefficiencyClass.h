//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May  6 14:58:47 2017 by ROOT version 5.34/36
// from TTree chargedEfficiency/charged efficiency
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef cefficiencyClass_h
#define cefficiencyClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class cefficiencyClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        c_effic_px;
   Double_t        c_effic_py;
   Double_t        c_effic_pz;
   Double_t        c_effic_E;
   Double_t        c_effic_phi;
   Double_t        c_effic_eta;
   Double_t        c_effic_Pt;

   // List of branches
   TBranch        *b_c_effic_px;   //!
   TBranch        *b_c_effic_py;   //!
   TBranch        *b_c_effic_pz;   //!
   TBranch        *b_c_effic_E;   //!
   TBranch        *b_c_effic_phi;   //!
   TBranch        *b_c_effic_eta;   //!
   TBranch        *b_c_effic_Pt;   //!

   cefficiencyClass(TTree *tree=0);
   virtual ~cefficiencyClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef cefficiencyClass_cxx
cefficiencyClass::cefficiencyClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("chargedEfficiency",tree);

   }
   Init(tree);
}

cefficiencyClass::~cefficiencyClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t cefficiencyClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t cefficiencyClass::LoadTree(Long64_t entry)
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

void cefficiencyClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("c_effic_px", &c_effic_px, &b_c_effic_px);
   fChain->SetBranchAddress("c_effic_py", &c_effic_py, &b_c_effic_py);
   fChain->SetBranchAddress("c_effic_pz", &c_effic_pz, &b_c_effic_pz);
   fChain->SetBranchAddress("c_effic_E", &c_effic_E, &b_c_effic_E);
   fChain->SetBranchAddress("c_effic_phi", &c_effic_phi, &b_c_effic_phi);
   fChain->SetBranchAddress("c_effic_eta", &c_effic_eta, &b_c_effic_eta);
   fChain->SetBranchAddress("c_effic_Pt", &c_effic_Pt, &b_c_effic_Pt);
   Notify();
}

Bool_t cefficiencyClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void cefficiencyClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t cefficiencyClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef cefficiencyClass_cxx
