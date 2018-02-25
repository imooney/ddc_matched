//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May  6 15:24:44 2017 by ROOT version 5.34/36
// from TTree uncut/uncut
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef uncutClass_h
#define uncutClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class uncutClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        uncut_px;
   Double_t        uncut_py;
   Double_t        uncut_pz;
   Double_t        uncut_E;
   Double_t        uncut_phi;
   Double_t        uncut_eta;
   Double_t        uncut_Pt;

   // List of branches
   TBranch        *b_uncut_px;   //!
   TBranch        *b_uncut_py;   //!
   TBranch        *b_uncut_pz;   //!
   TBranch        *b_uncut_E;   //!
   TBranch        *b_uncut_phi;   //!
   TBranch        *b_uncut_eta;   //!
   TBranch        *b_uncut_Pt;   //!

   uncutClass(TTree *tree=0);
   virtual ~uncutClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef uncutClass_cxx
uncutClass::uncutClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("uncut",tree);

   }
   Init(tree);
}

uncutClass::~uncutClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t uncutClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t uncutClass::LoadTree(Long64_t entry)
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

void uncutClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("uncut_px", &uncut_px, &b_uncut_px);
   fChain->SetBranchAddress("uncut_py", &uncut_py, &b_uncut_py);
   fChain->SetBranchAddress("uncut_pz", &uncut_pz, &b_uncut_pz);
   fChain->SetBranchAddress("uncut_E", &uncut_E, &b_uncut_E);
   fChain->SetBranchAddress("uncut_phi", &uncut_phi, &b_uncut_phi);
   fChain->SetBranchAddress("uncut_eta", &uncut_eta, &b_uncut_eta);
   fChain->SetBranchAddress("uncut_Pt", &uncut_Pt, &b_uncut_Pt);
   Notify();
}

Bool_t uncutClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void uncutClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t uncutClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef uncutClass_cxx
