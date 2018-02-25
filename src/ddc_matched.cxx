//  ddc_matched.cxx
//Runs Pythia events in multiple pT hard-bins, clusters the final state particles with
//FastJet's anti-kT, applies cuts and tracking efficiency, matches detector- and particle-level jets,
//outputs the results to a root file for later application of the data-driven correction.
//Isaac Mooney - WSU, 2017
// Spruced up in Feb. 2018

#include "Pythia8/Pythia.h"
#include "Pythia8/PhaseSpace.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

#include "TROOT.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TH3.h"
#include "TClonesArray.h"
#include "TLatex.h"
#include "TMathText.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TBranch.h"

#include "ktTrackEff.hh"

#include <iostream>
#include <sstream>
#include <random>
#include <chrono>
#include <string>

#include <getopt.h>

#include "analysis_funcs.h"

using namespace fastjet;
using namespace Pythia8;
using namespace std;
using namespace analysis;

//command line arguments
bool skinny_file = true;
bool real_effic = true;
string size_string = "skinny";
string effic_string = "real";
float num_events = 1e4;
//bool verbose = true;

void PrintHelp()
{
    std::cout <<
            "--num_events <n>:      Set number of events per pT hard-bin\n"
            "--flat / real:         Set real or flat efficiency\n"
            "--skinny / grande:    Set size of output root file (skinny file only fills matched jet trees)\n"
      //"--writeFile <fname>:   File to write to\n"
            "--help:                Show help\n";
    exit(1);
}

void arguments (int argc, char ** argv) {
  const char* const short_opts = "n:frsgh";
    const option long_opts[] = {
            {"num_events", 1, nullptr, 'n'},
            {"flat", 0, nullptr, 'f'},
	    {"real", 0, nullptr, 'r'},
            {"skinny", 0, nullptr, 's'},
	    {"grande", 0, nullptr, 'g'},
            //{"writeFile", 1, nullptr, 'w'},
            {"help", 0, nullptr, 'h'},
            {nullptr, 0, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'n':
	  num_events = std::stof(optarg);
	  std::cout << "Number of events set to: " << num_events << std::endl;
	  break;

        case 'f':
	  real_effic = false;
	  std::cout << "Efficiency is set to flat, 80%\n";
	  break;

	case 'r':
	  real_effic = true;
	  std::cout << "Efficiency is set to real STAR efficiency\n";
	  break;

        case 's':
	  skinny_file = true;
	  std::cout << "Size of output is skinny" << std::endl;
	  break;

	case 'g':
	  skinny_file = false;
	  std::cout << "Size of output is grande" << std::endl;
	  break;

	  /* case 'w':
	     write_file = std::string(optarg);
	     std::cout << "Write file set to: " << write_file << std::endl;
	     break;*/

        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp();
            break;
        }
    }
}

int main(int argc, char ** argv) {
    // Generator. Process selection. STAR initialization. Trees.
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();

    //processes command line arguments
    arguments(argc, argv);
    
    //-------------------------File(s) and containers-------------------------//
    TFile *fout;
    fout = new TFile(("out/ddc_matched_" + size_string + "_" + effic_string + ".root").c_str(),"RECREATE");
    //if (skinny_file) { fout = new TFile("out/ddc_matched_skinny_real.root","RECREATE");} // Create output file
    //else { fout = new TFile("out/ddc_matched_fat.root","RECREATE");}
    analysis::containers * holder = new containers();   // Create trees and pseudojet vectors
    holder->SetBranches();                              // Create branches
    
    //------------------------------------------------------------------------//
    
    //Define jet for FastJet clustering
    JetDefinition jet_def(antikt_algorithm, R);
    
    //number of bins in histograms and pTrange
    int nRange = 60;//120;
    double range = 60;
    
    //Histograms (temporary - one for each Pythia bin, all added up later)
    TH1 * c_cons_pt = new TH1D("cons_pt","constituent pt", nRange,0,range);
    TH1 * ptbefore = new TH1D("ptbefore", "", nRange,0,range);
    TH1 * ptprecorrection = new TH1D("ptprecorrection", "", nRange,0,range);
    TH1 * numbefore = new TH1D("numbefore","",nRange,0,range);
    TH1 * numprecorrection = new TH1D("numprecorrection","",nRange,0,range);
    TH1 * testpt = new TH1D("testpt","",nRange,0,range);
    TH1 * ctestpt = new TH1D("ctestpt", "", nRange, 0, range);
    TH1 * numdiffmatched = new TH1D("numdiffmatched", "", 17, -2.5, 14.5);
    TH1 * numpartmatched = new TH1D("numpartmatched", "", 20, -0.5, 19.5);
    TH1 * numdetmatched = new TH1D("numdetmatched","",20,-0.5,19.5);
    TH1 * ptconspartmatched = new TH1D("ptconspartmatched", "", nRange,0,range);
    TH1 * radial_distmatched = new TH1D("radial_distmatched","",nRange,-0.05,0.55);
    
    //---------------------------Initialize Pythia----------------------------//
    Pythia pythia;
    //shorthand
    Pythia8::Info& info = pythia.info;
    unsigned nBin = 11;
    //Prepare to run over multiple pT hardbins with ranges from file
    pythia.readFile("bins.cmnd");
    nBin = pythia.mode("Main:numberOfSubruns");

    //to be incremented after each event by sigmaGen for that event. Used for normalization at the end.
    double my_own_weightsum = 0;
    
    //------------------------------------------------------------------------//
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~TClonesArrays for jets~~~~~~~~~~~~~~~~~~~~~~~~~//
    TClonesArray *uJets = new TClonesArray("TLorentzVector");
    TClonesArray &uncutJets = *uJets;
    holder->tuncut->Branch("uncutJets", &uncutJets, 32000, 0);
    TClonesArray cut2Jets ("TLorentzVector");
    holder->tcut2->Branch("cut2Jets", &cut2Jets, 32000, 0);
    TClonesArray efficJets ("TLorentzVector");
    holder->teffic->Branch("efficJets", &efficJets, 32000, 0);
    TClonesArray c_uncutJets ("TLorentzVector");
    holder->tcuncut->Branch("c_uncutJets", &c_uncutJets, 32000, 0);
    TClonesArray c_cut2Jets ("TLorentzVector");
    holder->tccut2->Branch("c_cut2Jets", &c_cut2Jets, 32000, 0);
    TClonesArray *ceJets = new TClonesArray("TLorentzVector");
    TClonesArray &c_efficJets = *ceJets;
    holder->tceffic->Branch("c_efficJets", &c_efficJets, 32000, 0);
    TClonesArray matcheddetJets ("TLorentzVector");
    holder->tmatched->Branch("matcheddetJets", &matcheddetJets, 32000, 0);
    TClonesArray matchedpartJets("TLorentzVector");
    holder->tmatched->Branch("matchedpartJets", &matchedpartJets, 32000, 0);
    TClonesArray c_matcheddetJets ("TLorentzVector");
    holder->tmatched->Branch("c_matcheddetJets", &c_matcheddetJets, 32000, 0);
    TClonesArray c_matchedpartJets("TLorentzVector");
    holder->tmatched->Branch("c_matchedpartJets", &c_matchedpartJets, 32000, 0);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~TClonesArrays for cons~~~~~~~~~~~~~~~~~~~~~~~~~//
    TClonesArray uncutCons ("TLorentzVector");
    holder->tuncutcons->Branch("uncutCons", &uncutCons, 32000, 0);
    TClonesArray cut2Cons ("TLorentzVector");
    holder->tcut2cons->Branch("cut2Cons", &cut2Cons, 32000, 0);
    TClonesArray efficCons ("TLorentzVector");
    holder->tefficcons->Branch("efficCons", &efficCons, 32000, 0);
    TClonesArray matchedCons ("TLorentzVector");
    holder->tmatched->Branch("matchedCons", &matchedCons, 32000, 0);
    TClonesArray c_uncutCons ("TLorentzVector");
    holder->tcuncutcons->Branch("c_uncutCons", &c_uncutCons, 32000, 0);
    TClonesArray c_cut2Cons ("TLorentzVector");
    holder->tccut2cons->Branch("c_cut2Cons", &c_cut2Cons, 32000, 0);
    TClonesArray c_efficCons ("TLorentzVector");
    holder->tcefficcons->Branch("c_efficCons", &c_efficCons, 32000, 0);
    TClonesArray c_matchedCons ("TLorentzVector");
    holder->tmatched->Branch("c_matchedCons", &c_matchedCons, 32000, 0);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    //tracking efficiency
    ktTrackEff* tEff = new ktTrackEff();

    double nJets = 0.0;
    
    for (int iBin = 0; iBin < nBin; ++ iBin) {//bin loop
        //initialize pythia in this pT hardbin
        analysis::pythia_init(pythia, iBin);

        for (unsigned iEvent = 0; iEvent < num_events; ++iEvent) {//event loop
            //clear containers and TClonesArrays
            holder->Clear();
            uncutJets.Clear(); cut2Jets.Clear(); efficJets.Clear(); c_uncutJets.Clear();
            c_cut2Jets.Clear(); c_efficJets.Clear(); c_matcheddetJets.Clear(); c_matchedpartJets.Clear();
            
            if (!pythia.next()) continue;
            
            analysis::add_particles(pythia.event, holder, iBin, tEff, skinny_file, real_effic);
            //fill particle pT spectrum histograms
            if (!skinny_file) {
                for (unsigned i = 0; i < holder->uncut_part.size(); ++ i) {
		  testpt->Fill(holder->uncut_part[i].pt(), info.sigmaGen());
                }
                for (unsigned i = 0; i < holder->c_uncut_part.size(); ++ i) {
		  ctestpt->Fill(holder->c_uncut_part[i].pt(), info.sigmaGen());
                }
            }
            
            //~~~~~~~~~~~~~~~~~~~~~~CLUSTER AND GET JETS~~~~~~~~~~~~~~~~~~~~~~//
            
            ClusterSequence uncut(holder->uncut_part, jet_def);
            ClusterSequence cut2(holder->cut2_part, jet_def);
            ClusterSequence effic(holder->effic_part, jet_def);
            vector<PseudoJet> uncut_jets = sorted_by_pt(uncut.inclusive_jets());
            vector<PseudoJet> cut2_jets = sorted_by_pt(cut2.inclusive_jets());
            vector<PseudoJet> effic_jets = sorted_by_pt(effic.inclusive_jets());
            
            ClusterSequence c_uncut(holder->c_uncut_part, jet_def);
            ClusterSequence c_cut2(holder->c_cut2_part, jet_def);
            ClusterSequence c_effic(holder->c_effic_part, jet_def);
            vector<PseudoJet> c_uncut_jets = sorted_by_pt(c_uncut.inclusive_jets());
            vector<PseudoJet> c_cut2_jets = sorted_by_pt(c_cut2.inclusive_jets());
            vector<PseudoJet> c_effic_jets = sorted_by_pt(c_effic.inclusive_jets());
            
            //cout << "Clustering with " << jet_def.description() << '\n';
            if (!skinny_file) {//this is pedantic mode, fill all containers
                //cout << "PEDANTIC MODE!\n";
                //~~~~~~~~~~~~~~~~~~FILL TREES WITH LEAD PTS~~~~~~~~~~~~~~~~~~//
                
                //leading jet pt spectra (total and only charged)
                //(a) without cuts, (c) with jet cuts, and (d) with cuts + efficiency
                
                //(a):
                if (uncut_jets.size() != 0) {
                    holder->uncut_leadPt = uncut_jets[0].pt();
                }
                
                if (c_uncut_jets.size() != 0) {
                    holder->c_uncut_leadPt = c_uncut_jets[0].pt();
                }
                
                //(c):
                if(cut2_jets.size() != 0) {
                    //leading jet pt spectrum
                    holder->cut2_leadPt = cut2_jets[0].pt();
                }
                
                if (c_cut2_jets.size() != 0) {
                    holder->c_cut2_leadPt = c_cut2_jets[0].pt();
                }
                
                //(d):
                if (effic_jets.size() != 0) {
                    if (detector_jet_cuts(effic_jets[0])) {
                        holder->effic_leadPt = effic_jets[0].pt();
                    }
                }
                
                if (c_effic_jets.size() != 0 && detector_jet_cuts(c_effic_jets[0])) {
                    holder->c_effic_leadPt = c_effic_jets[0].pt();
                }
                
                //branches in tree "tlead" have been populated, so fill the tree with these branches
                //NOTE: this fills ALL branches in the tree - even ones without a value on this pass-thru
                holder->tlead->Fill();
            
            //~~~~~~~~~~~~~~~FILL TREES WITH JETS & CONSTITUENTS~~~~~~~~~~~~~~~~//
            
            unsigned jetmarker = 0;
            unsigned consmarker = 0;
            
            //pseudojets without cuts
            jetmarker = 0;
            for (unsigned i = 0; i < uncut_jets.size(); ++ i) {
                uncutCons.Clear();
                new ( uncutJets[jetmarker ++] ) TLorentzVector(uncut_jets[i].px(),uncut_jets[i].py(),
                                                               uncut_jets[i].pz(),uncut_jets[i].E());
                vector<PseudoJet> uncut_cons = uncut_jets[i].constituents();
                consmarker = 0;
                for (unsigned j = 0; j < uncut_cons.size(); ++ j) {
                    new ( uncutCons[consmarker ++] ) TLorentzVector(uncut_cons[j].px(),uncut_cons[j].py(),
                                                                    uncut_cons[j].pz(),uncut_cons[j].E());
                }
                holder->tuncutcons->Fill();
            }
            
            //pseudojets without cuts (charged)
            jetmarker = 0;
            for (unsigned i = 0; i < c_uncut_jets.size(); ++ i) {
                c_uncutCons.Clear();
                new ( c_uncutJets[jetmarker ++] ) TLorentzVector(c_uncut_jets[i].px(),c_uncut_jets[i].py(),
                                                                 c_uncut_jets[i].pz(),c_uncut_jets[i].E());
                vector<PseudoJet> c_uncut_cons = c_uncut_jets[i].constituents();
                consmarker = 0;
                for (unsigned j = 0; j < c_uncut_cons.size(); ++ j) {
                    new ( c_uncutCons[consmarker ++] ) TLorentzVector(c_uncut_cons[j].px(),c_uncut_cons[j].py(),
                                                                      c_uncut_cons[j].pz(),c_uncut_cons[j].E());
                }
                holder->tcuncutcons->Fill();
            }
            
            //pseudojets with both sets of cuts
            jetmarker = 0;
            for (unsigned i = 0; i < cut2_jets.size(); ++ i) {
                cut2Cons.Clear();
                new ( cut2Jets[jetmarker ++] ) TLorentzVector(cut2_jets[i].px(),cut2_jets[i].py(),
                                                                cut2_jets[i].pz(),cut2_jets[i].E());
                vector<PseudoJet> cut2_cons = cut2_jets[i].constituents();
                consmarker = 0;
                for (unsigned j = 0; j < cut2_cons.size(); ++ j) {
                    new ( cut2Cons[consmarker ++] ) TLorentzVector(cut2_cons[j].px(),cut2_cons[j].py(),
                                                                    cut2_cons[j].pz(),cut2_cons[j].E());
                }
                holder->tcut2cons->Fill();
            }
            
            jetmarker = 0;
            for (unsigned i = 0; i < c_cut2_jets.size(); ++ i) {
                c_cut2Cons.Clear();
                new ( c_cut2Jets[jetmarker ++] ) TLorentzVector(c_cut2_jets[i].px(),c_cut2_jets[i].py(),
                                                                c_cut2_jets[i].pz(),c_cut2_jets[i].E());
                vector<PseudoJet> c_cut2_cons = c_cut2_jets[i].constituents();
                holder->ccut2mult = c_cut2_cons.size();
                holder->tpartmult->Fill();
                consmarker = 0;
                for (unsigned j = 0; j < c_cut2_cons.size(); ++ j) {
                    new ( c_cut2Cons[consmarker ++] ) TLorentzVector(c_cut2_cons[j].px(),c_cut2_cons[j].py(),
                                                                        c_cut2_cons[j].pz(),c_cut2_cons[j].E());
                }
                holder->tccut2cons->Fill();
            }
            
            //pseudojets with cuts + charged particle tracking efficiency
            jetmarker = 0;
            for (unsigned i = 0; i < effic_jets.size(); ++ i) {
                if (detector_jet_cuts(effic_jets[i])) {
                    efficCons.Clear();
                    new ( efficJets[jetmarker ++] ) TLorentzVector(effic_jets[i].px(),effic_jets[i].py(),
                                                                   effic_jets[i].pz(),effic_jets[i].E());
                    vector<PseudoJet> effic_cons = effic_jets[i].constituents();
                    consmarker = 0;
                    for (unsigned j = 0; j < effic_cons.size(); ++ j) {
                        new ( efficCons[consmarker ++] ) TLorentzVector(effic_cons[j].px(),effic_cons[j].py(),
                                                                        effic_cons[j].pz(),effic_cons[j].E());
                    }
                    holder->tefficcons->Fill();
                }
            }
            
            jetmarker = 0;
            for (unsigned i = 0; i < c_effic_jets.size(); ++ i) {
                if (detector_jet_cuts(c_effic_jets[i])) {
                    c_efficCons.Clear();
                    new ( c_efficJets[jetmarker ++] ) TLorentzVector(c_effic_jets[i].px(),c_effic_jets[i].py(),
                                                                     c_effic_jets[i].pz(),c_effic_jets[i].E());
                    vector<PseudoJet> c_effic_cons = c_effic_jets[i].constituents();
                    holder->cefficmult = c_effic_cons.size();
                    holder->tdetmult->Fill();
                    consmarker = 0;
                    for (unsigned j = 0; j < c_effic_cons.size(); ++ j) {
                        new ( c_efficCons[consmarker ++] ) TLorentzVector(c_effic_cons[j].px(),c_effic_cons[j].py(),
                                                                          c_effic_cons[j].pz(),c_effic_cons[j].E());
                    }
                    holder->tcefficcons->Fill();
                }
            }
            
        } //Exiting pedantic conditional. Rest will fill no matter what.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            
        //~~~~~~~~~~~~~~~~~~~~~~~~~~FILL HISTOGRAMS!~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            
        //Filling particle-level histograms with charged leading jet info
        if (c_cut2_jets.size() != 0) {
	  ptbefore->Fill(c_cut2_jets[0].pt(), info.sigmaGen());
	  numbefore->Fill(c_cut2_jets[0].constituents().size(), info.sigmaGen());
        }
            
        //Filling detector-level histograms with charged leading jet info
        if (c_effic_jets.size() != 0) {
            if (detector_jet_cuts(c_effic_jets[0])) {
	      ptprecorrection->Fill(c_effic_jets[0].pt(), info.sigmaGen());
	      numprecorrection->Fill(c_effic_jets[0].constituents().size(), info.sigmaGen());
            }
        }
            
        //Filling detector-level charged leading jet constituent pT spectrum
        if (c_effic_jets.size() != 0) {
            if (detector_jet_cuts(c_effic_jets[0])) {
                vector<PseudoJet> c_effic_cons = c_effic_jets[0].constituents();
                for (unsigned j = 0; j < c_effic_jets[0].constituents().size(); ++j) {
		  c_cons_pt->Fill(c_effic_cons[j].pt(), info.sigmaGen());
                }
            }
        }
            
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            
        //~~~~~~~~~~~~~~~~~~~MATCHING JETS FOR ANALYSIS!~~~~~~~~~~~~~~~~~~~//

        if (c_effic_jets.size() != 0 && detector_jet_cuts(c_effic_jets[0])){
	  analysis::geometric_diff(pythia, c_effic_jets, c_cut2_jets, holder->c_ptdiff,
                                        holder->c_num_diff, holder->c_num_before,
                                        holder->c_num_after, holder->c_rel_diff,
                                        holder->tmatched, c_matchedpartJets, c_matcheddetJets,
                                     c_matchedCons, numdiffmatched, numpartmatched, numdetmatched, ptconspartmatched, radial_distmatched,
				   holder->det_multiplicity, nJets);
            holder->tdiffs->Fill();
        }
            
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            
        //~~~~~~~~~~~~~~~~~~~~~FILLING REMAINING TREES~~~~~~~~~~~~~~~~~~~~~//
            
        if (uncutJets.GetLast() > 0)      {holder->tuncut->Fill();}
        if (c_uncutJets.GetLast() > 0)    {holder->tcuncut->Fill();}
        if (cut2Jets.GetLast() > 0)       {holder->tcut2->Fill();}
        if (c_cut2Jets.GetLast() > 0)     {holder->tccut2->Fill();}
        if (efficJets.GetLast() > 0)      {holder->teffic->Fill();}
        if (c_efficJets.GetLast() > 0)    {holder->tceffic->Fill();}
            
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	holder->weight = info.sigmaGen();
	
	//incrementing my own weightSum since pythia's isn't what I want
	my_own_weightsum += info.sigmaGen();
        }//end of event loop!
    
    }//end of bin loop!
    
    //Defining a weight for each pt hard bin
    double binwidth_inverse = (double) (nRange / (double) range); // = 1 for now
    double sigmaNorm = (double) (binwidth_inverse / (double) my_own_weightsum); //scaling by sigmaNorm should make the plots = 1/sigma dsigma/d(quantity)
    nJets *= (double) (1/(double) my_own_weightsum);
        
    //scaling histograms by the bin width and the cross section
    testpt->Scale(sigmaNorm); ctestpt->Scale(sigmaNorm);
    c_cons_pt->Scale(sigmaNorm); ptbefore->Scale(sigmaNorm);
    ptprecorrection->Scale(sigmaNorm); numbefore->Scale(sigmaNorm);
    numprecorrection->Scale(sigmaNorm); numdiffmatched->Scale(sigmaNorm);
    numpartmatched->Scale(sigmaNorm); numdetmatched->Scale(sigmaNorm);
    ptconspartmatched->Scale(sigmaNorm);

    double binwidth_inverse_for_radial = 1.0; //100.0;  // 1/bin_width is 100 here, but not dividing by it, so the integral is average track multiplicity.
    double sigmaNorm_for_radial = (double) (binwidth_inverse_for_radial / (double) my_own_weightsum);
    radial_distmatched->Scale((double) sigmaNorm_for_radial/ (double) nJets);
    
    pythia.stat();
    
    cout << "Writing to " << fout->GetName() << '\n';
    
    //tout->Print(); if status of a particular tree is desired.
    //save trees to output root file
    fout->cd();
    holder->write();
    
    fout->Write();
    
    return 0;
}
