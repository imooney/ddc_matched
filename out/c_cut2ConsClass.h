//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 12 12:37:59 2017 by ROOT version 5.34/36
// from TTree chargedCut2Constituents/charged cut2 constituents
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef c_cut2ConsClass_h
#define c_cut2ConsClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class c_cut2ConsClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        c_cut2_cons_px;
   Double_t        c_cut2_cons_py;
   Double_t        c_cut2_cons_pz;
   Double_t        c_cut2_cons_E;
   Double_t        c_cut2_cons_phi;
   Double_t        c_cut2_cons_eta;
   Double_t        c_cut2_cons_Pt;

   // List of branches
   TBranch        *b_c_cut2_cons_px;   //!
   TBranch        *b_c_cut2_cons_py;   //!
   TBranch        *b_c_cut2_cons_pz;   //!
   TBranch        *b_c_cut2_cons_E;   //!
   TBranch        *b_c_cut2_cons_phi;   //!
   TBranch        *b_c_cut2_cons_eta;   //!
   TBranch        *b_c_cut2_cons_Pt;   //!

   c_cut2ConsClass(TTree *tree=0);
   virtual ~c_cut2ConsClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef c_cut2ConsClass_cxx
c_cut2ConsClass::c_cut2ConsClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("chargedCut2Constituents",tree);

   }
   Init(tree);
}

c_cut2ConsClass::~c_cut2ConsClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t c_cut2ConsClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t c_cut2ConsClass::LoadTree(Long64_t entry)
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

void c_cut2ConsClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("c_cut2_cons_px", &c_cut2_cons_px, &b_c_cut2_cons_px);
   fChain->SetBranchAddress("c_cut2_cons_py", &c_cut2_cons_py, &b_c_cut2_cons_py);
   fChain->SetBranchAddress("c_cut2_cons_pz", &c_cut2_cons_pz, &b_c_cut2_cons_pz);
   fChain->SetBranchAddress("c_cut2_cons_E", &c_cut2_cons_E, &b_c_cut2_cons_E);
   fChain->SetBranchAddress("c_cut2_cons_phi", &c_cut2_cons_phi, &b_c_cut2_cons_phi);
   fChain->SetBranchAddress("c_cut2_cons_eta", &c_cut2_cons_eta, &b_c_cut2_cons_eta);
   fChain->SetBranchAddress("c_cut2_cons_Pt", &c_cut2_cons_Pt, &b_c_cut2_cons_Pt);
   Notify();
}

Bool_t c_cut2ConsClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void c_cut2ConsClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t c_cut2ConsClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef c_cut2ConsClass_cxx
