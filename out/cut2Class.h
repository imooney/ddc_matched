//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May  6 14:52:36 2017 by ROOT version 5.34/36
// from TTree cut2/cut2
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef cut2Class_h
#define cut2Class_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class cut2Class {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        cut2_px;
   Double_t        cut2_py;
   Double_t        cut2_pz;
   Double_t        cut2_E;
   Double_t        cut2_phi;
   Double_t        cut2_eta;
   Double_t        cut2_Pt;

   // List of branches
   TBranch        *b_cut2_px;   //!
   TBranch        *b_cut2_py;   //!
   TBranch        *b_cut2_pz;   //!
   TBranch        *b_cut2_E;   //!
   TBranch        *b_cut2_phi;   //!
   TBranch        *b_cut2_eta;   //!
   TBranch        *b_cut2_Pt;   //!

   cut2Class(TTree *tree=0);
   virtual ~cut2Class();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef cut2Class_cxx
cut2Class::cut2Class(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("cut2",tree);

   }
   Init(tree);
}

cut2Class::~cut2Class()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t cut2Class::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t cut2Class::LoadTree(Long64_t entry)
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

void cut2Class::Init(TTree *tree)
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

   fChain->SetBranchAddress("cut2_px", &cut2_px, &b_cut2_px);
   fChain->SetBranchAddress("cut2_py", &cut2_py, &b_cut2_py);
   fChain->SetBranchAddress("cut2_pz", &cut2_pz, &b_cut2_pz);
   fChain->SetBranchAddress("cut2_E", &cut2_E, &b_cut2_E);
   fChain->SetBranchAddress("cut2_phi", &cut2_phi, &b_cut2_phi);
   fChain->SetBranchAddress("cut2_eta", &cut2_eta, &b_cut2_eta);
   fChain->SetBranchAddress("cut2_Pt", &cut2_Pt, &b_cut2_Pt);
   Notify();
}

Bool_t cut2Class::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void cut2Class::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t cut2Class::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef cut2Class_cxx
