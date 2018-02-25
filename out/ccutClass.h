//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May  6 14:58:13 2017 by ROOT version 5.34/36
// from TTree chargedCut/charged cut
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef ccutClass_h
#define ccutClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class ccutClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        c_cut_px;
   Double_t        c_cut_py;
   Double_t        c_cut_pz;
   Double_t        c_cut_E;
   Double_t        c_cut_phi;
   Double_t        c_cut_eta;
   Double_t        c_cut_Pt;

   // List of branches
   TBranch        *b_c_cut_px;   //!
   TBranch        *b_c_cut_py;   //!
   TBranch        *b_c_cut_pz;   //!
   TBranch        *b_c_cut_E;   //!
   TBranch        *b_c_cut_phi;   //!
   TBranch        *b_c_cut_eta;   //!
   TBranch        *b_c_cut_Pt;   //!

   ccutClass(TTree *tree=0);
   virtual ~ccutClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ccutClass_cxx
ccutClass::ccutClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("chargedCut",tree);

   }
   Init(tree);
}

ccutClass::~ccutClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ccutClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ccutClass::LoadTree(Long64_t entry)
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

void ccutClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("c_cut_px", &c_cut_px, &b_c_cut_px);
   fChain->SetBranchAddress("c_cut_py", &c_cut_py, &b_c_cut_py);
   fChain->SetBranchAddress("c_cut_pz", &c_cut_pz, &b_c_cut_pz);
   fChain->SetBranchAddress("c_cut_E", &c_cut_E, &b_c_cut_E);
   fChain->SetBranchAddress("c_cut_phi", &c_cut_phi, &b_c_cut_phi);
   fChain->SetBranchAddress("c_cut_eta", &c_cut_eta, &b_c_cut_eta);
   fChain->SetBranchAddress("c_cut_Pt", &c_cut_Pt, &b_c_cut_Pt);
   Notify();
}

Bool_t ccutClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ccutClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ccutClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ccutClass_cxx
