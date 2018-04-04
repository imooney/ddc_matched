//  analysis_funcs.h
//  Created by Isaac Mooney on 5/4/17.

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

#include "ktTrackEff.hh"

#include <iostream>
#include <sstream>
#include <random>
#include <chrono>

#include "../myLorentzVector.h"

#ifndef analysis_funcs_h
#define analysis_funcs_h

namespace analysis {
    class containers;
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PARAMETERS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    //const int NumEvents     = 1e3;
    //CUTS:
    //Particle:
    const double eta_cut    = 1.0;
    const double pt_cut     = 0.2;
    //Jet:
    const double R          = 0.4;
    const double jet_eta    = 1-R;
    const double jet_min    = 15.0;
    const double jet_max    = 30.0;
    
    //EFFICIENCY:
    const double effic = 0.8;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    //initializes Pythia
    void pythia_init(Pythia8::Pythia & pythia, int);
    //checks that particles pass particle cuts
    bool constituent_cuts(const Pythia8::Particle particle);
    //check that pseudojets pass jet cuts (detector-level)
    bool detector_jet_cuts(const fastjet::PseudoJet pseudojet);
    //tracking efficiency selection
    bool efficiency(const double effic_num, const Pythia8::Particle particle, ktTrackEff*, const bool);
    //fills particle vectors after appropriate cuts are passed
    void add_particles(Pythia8::Event event, containers * container, int, ktTrackEff*, const bool, const bool);
    //examines the pT & num difference between efficiency-corrected, and non-efficiency-corrected 'leading' jets
    void geometric_diff(Pythia8::Pythia &, const std::vector<fastjet::PseudoJet> effic_jets, const std::vector<fastjet::PseudoJet> cut2_jets,
                        double & ptdiff, int & num_diff, int & num_before, int & num_after, double & rel_diff, TTree *,
			TClonesArray &, TClonesArray &, TClonesArray &, TH1 *, TH1 *, TH1 *, TH1 *, TH1 *, TH1 *, TH1 *, TH1* , TH1*, TH1 *, double &, double &, double &, ktTrackEff*, double &);
    
    //TLorentzVector MakeTLorentzVector(const fastjet::PseudoJet & pj);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~CONTAINER CLASS~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    class containers {
    private:
        //MOVE THINGS HERE LATER AND WRITE GETTERS? SEEMS LIKE A PAIN
    public:
        //trees
        TTree *tuncut, *tcuncut, *tcut2, *tccut2, *teffic, *tceffic, *tlead, *tdiffs, *tuncutcons, *tcuncutcons, *tcut2cons, *tccut2cons, *tefficcons, *tcefficcons, *tpartmult, *tdetmult, *tmatched;
        
        //branches
        //TClonesArray *uncutJets;
        double weight;
        double det_multiplicity;
        double ccut2mult, cefficmult;
        double uncut_leadPt, cut2_leadPt, effic_leadPt, c_uncut_leadPt, c_cut2_leadPt, c_effic_leadPt;
        double ptdiff, c_ptdiff, rel_diff, c_rel_diff;
        int num_diff, c_num_diff, num_before, num_after, c_num_before, c_num_after;
        
        //particles
        std::vector<fastjet::PseudoJet> uncut_part, c_uncut_part, cut2_part, c_cut2_part, effic_part, c_effic_part;
        
        containers();               //constructor (initializes the trees, reserves space for vectors)
        virtual ~containers() {}    //destructor (shouldn't need to implement?)
        
        //getters & setters
        
        void SetBranches();         //setter
        //SetJets();
        
        void Clear();               //clears the particle vectors for each event
        void write();               //writes the trees to file
    };
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
}

#endif /* analysis_funcs_h */
