//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May  8 13:16:43 2017 by ROOT version 5.34/36
// from TTree chargedUncutConstituents/charged uncut constituents
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef c_uncutConsClass_h
#define c_uncutConsClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class c_uncutConsClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        c_uncut_cons_px;
   Double_t        c_uncut_cons_py;
   Double_t        c_uncut_cons_pz;
   Double_t        c_uncut_cons_E;
   Double_t        c_uncut_cons_phi;
   Double_t        c_uncut_cons_eta;
   Double_t        c_uncut_cons_Pt;

   // List of branches
   TBranch        *b_c_uncut_cons_px;   //!
   TBranch        *b_c_uncut_cons_py;   //!
   TBranch        *b_c_uncut_cons_pz;   //!
   TBranch        *b_c_uncut_cons_E;   //!
   TBranch        *b_c_uncut_cons_phi;   //!
   TBranch        *b_c_uncut_cons_eta;   //!
   TBranch        *b_c_uncut_cons_Pt;   //!

   c_uncutConsClass(TTree *tree=0);
   virtual ~c_uncutConsClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef c_uncutConsClass_cxx
c_uncutConsClass::c_uncutConsClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("chargedUncutConstituents",tree);

   }
   Init(tree);
}

c_uncutConsClass::~c_uncutConsClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t c_uncutConsClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t c_uncutConsClass::LoadTree(Long64_t entry)
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

void c_uncutConsClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("c_uncut_cons_px", &c_uncut_cons_px, &b_c_uncut_cons_px);
   fChain->SetBranchAddress("c_uncut_cons_py", &c_uncut_cons_py, &b_c_uncut_cons_py);
   fChain->SetBranchAddress("c_uncut_cons_pz", &c_uncut_cons_pz, &b_c_uncut_cons_pz);
   fChain->SetBranchAddress("c_uncut_cons_E", &c_uncut_cons_E, &b_c_uncut_cons_E);
   fChain->SetBranchAddress("c_uncut_cons_phi", &c_uncut_cons_phi, &b_c_uncut_cons_phi);
   fChain->SetBranchAddress("c_uncut_cons_eta", &c_uncut_cons_eta, &b_c_uncut_cons_eta);
   fChain->SetBranchAddress("c_uncut_cons_Pt", &c_uncut_cons_Pt, &b_c_uncut_cons_Pt);
   Notify();
}

Bool_t c_uncutConsClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void c_uncutConsClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t c_uncutConsClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef c_uncutConsClass_cxx
