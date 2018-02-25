//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun  6 16:31:14 2017 by ROOT version 5.34/36
// from TTree numberAndpTDifferences/number and pT differences
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef DiffClass_h
#define DiffClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class DiffClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           num_diff;
   Int_t           c_num_diff;
   Double_t        ptdiff;
   Double_t        c_ptdiff;
   Double_t        rel_diff;
   Double_t        c_rel_diff;
   Int_t           num_before;
   Int_t           c_num_before;
   Int_t           num_after;
   Int_t           c_num_after;

   // List of branches
   TBranch        *b_num_diff;   //!
   TBranch        *b_c_num_diff;   //!
   TBranch        *b_ptdiff;   //!
   TBranch        *b_c_ptdiff;   //!
   TBranch        *b_rel_diff;   //!
   TBranch        *b_c_rel_diff;   //!
   TBranch        *b_num_before;   //!
   TBranch        *b_c_num_before;   //!
   TBranch        *b_num_after;   //!
   TBranch        *b_c_num_after;   //!

   DiffClass(TTree *tree=0);
   virtual ~DiffClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef DiffClass_cxx
DiffClass::DiffClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("numberAndpTDifferences",tree);

   }
   Init(tree);
}

DiffClass::~DiffClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DiffClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DiffClass::LoadTree(Long64_t entry)
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

void DiffClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("num_diff", &num_diff, &b_num_diff);
   fChain->SetBranchAddress("c_num_diff", &c_num_diff, &b_c_num_diff);
   fChain->SetBranchAddress("ptdiff", &ptdiff, &b_ptdiff);
   fChain->SetBranchAddress("c_ptdiff", &c_ptdiff, &b_c_ptdiff);
   fChain->SetBranchAddress("rel_diff", &rel_diff, &b_rel_diff);
   fChain->SetBranchAddress("c_rel_diff", &c_rel_diff, &b_c_rel_diff);
   fChain->SetBranchAddress("num_before", &num_before, &b_num_before);
   fChain->SetBranchAddress("c_num_before", &c_num_before, &b_c_num_before);
   fChain->SetBranchAddress("num_after", &num_after, &b_num_after);
   fChain->SetBranchAddress("c_num_after", &c_num_after, &b_c_num_after);
   Notify();
}

Bool_t DiffClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DiffClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DiffClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DiffClass_cxx
