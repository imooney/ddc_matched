//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug 28 12:58:20 2017 by ROOT version 5.34/36
// from TTree leadingJetpTs/leading jet pTs
// found on file: trackeffic.root
//////////////////////////////////////////////////////////

#ifndef leadPtClass_h
#define leadPtClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class leadPtClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        uncut_leadPt;
   Double_t        c_uncut_leadPt;
   Double_t        cut_leadPt;
   Double_t        c_cut_leadPt;
   Double_t        cut2_leadPt;
   Double_t        c_cut2_leadPt;
   Double_t        effic_leadPt;
   Double_t        c_effic_leadPt;

   // List of branches
   TBranch        *b_uncut_leadPt;   //!
   TBranch        *b_c_uncut_leadPt;   //!
   TBranch        *b_cut_leadPt;   //!
   TBranch        *b_c_cut_leadPt;   //!
   TBranch        *b_cut2_leadPt;   //!
   TBranch        *b_c_cut2_leadPt;   //!
   TBranch        *b_effic_leadPt;   //!
   TBranch        *b_c_effic_leadPt;   //!

   leadPtClass(TTree *tree=0);
   virtual ~leadPtClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef leadPtClass_cxx
leadPtClass::leadPtClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trackeffic.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trackeffic.root");
      }
      f->GetObject("leadingJetpTs",tree);

   }
   Init(tree);
}

leadPtClass::~leadPtClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t leadPtClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t leadPtClass::LoadTree(Long64_t entry)
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

void leadPtClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("uncut_leadPt", &uncut_leadPt, &b_uncut_leadPt);
   fChain->SetBranchAddress("c_uncut_leadPt", &c_uncut_leadPt, &b_c_uncut_leadPt);
   fChain->SetBranchAddress("cut_leadPt", &cut_leadPt, &b_cut_leadPt);
   fChain->SetBranchAddress("c_cut_leadPt", &c_cut_leadPt, &b_c_cut_leadPt);
   fChain->SetBranchAddress("cut2_leadPt", &cut2_leadPt, &b_cut2_leadPt);
   fChain->SetBranchAddress("c_cut2_leadPt", &c_cut2_leadPt, &b_c_cut2_leadPt);
   fChain->SetBranchAddress("effic_leadPt", &effic_leadPt, &b_effic_leadPt);
   fChain->SetBranchAddress("c_effic_leadPt", &c_effic_leadPt, &b_c_effic_leadPt);
   Notify();
}

Bool_t leadPtClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void leadPtClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t leadPtClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef leadPtClass_cxx
