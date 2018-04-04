//  analysis_funcs.cxx
//  Created by Isaac Mooney on 5/4/17; cleaned on 8/7/17

#include "analysis_funcs.h"

namespace analysis {
    
    //Initializes Pythia
    void pythia_init(Pythia8::Pythia & pythia, int iBin) {
        //Setting parameters for Pythia
        pythia.readString("HardQCD:all = on");
        //pythia.readString("SoftQCD:nonDiffractive = off");
        
        pythia.readFile("bins.cmnd", iBin);
        
        pythia.readString("Beams:eCM = 200.");
        
        pythia.init();
        
    }
    
    //checks that constituent cuts are satisfied
    bool constituent_cuts(const Pythia8::Particle particle) {
        if (fabs(particle.eta()) < eta_cut && particle.pT() >= pt_cut) {
            return true;
        }
        return false;
    }
    
    //checks that pseudojet cuts are satisfied (detector-level jets)
    bool detector_jet_cuts(const fastjet::PseudoJet pseudojet) {
        if (fabs(pseudojet.eta()) < jet_eta && pseudojet.pt() >= jet_min && pseudojet.pt() <= jet_max) {
            return true;
        }
        return false;
    }
    
    //checks that the tracking efficiency criteria is passed
  bool efficiency(const double effic_num, const Pythia8::Particle particle, ktTrackEff* te, const bool real_effic) {
    if (!real_effic) {
      if (effic_num > effic) {
	//means it won't be considered
	return false;
      }
      return true;
    }
    else {
      if (effic_num > te->EffPPY06(particle.eta(), particle.pT())) {
	//means it won't be considered
	return false;
      }
      return true;
    }    
  }
    
  void add_particles(Pythia8::Event event, containers * container, int iBin, ktTrackEff* te, const bool skinny_file, const bool real_effic){
        for (unsigned i = 0; i < event.size(); ++i) {
            //only select final state particles within particle-level eta cut
            if (event[i].isFinal() && event[i].isVisible()) {
                //save particle charge info for later in fastjet
                fastjet::PseudoJet current = fastjet::PseudoJet(event[i].px(),event[i].py(),
                                                                event[i].pz(),event[i].e());
                //gives the PseudoJet a charge if applicable
                current.set_user_index(event[i].isCharged());
                //fill vector of raw particles, regardless of cuts
                //if pedantic mode is on, fill everything
                if (!skinny_file) { // argnum == 2) {
                    container->uncut_part.push_back(current);
                    if (event[i].isCharged()) {
                        container->c_uncut_part.push_back(current);
                    }
                }

		if (constituent_cuts (event[i])) {
		  double effic_num = gRandom->Uniform(0.0, 1.0);
		  //immediately fill charged+neutral cut_2, effic if pedantic
		  if (!skinny_file) {
		    container->cut2_part.push_back(current);
		    //NOTE: since there is no efficiency being applied to neutrals, I just fill the detector-level charged+neutral vector with everything that passes constituent selection
		    container->effic_part.push_back(current); 
		  }
		  
		  if (event[i].isCharged()) {
		    if(!efficiency(effic_num, event[i], te, real_effic)) {
		      //this is a charged track that didn't quite make it to detector level so fill c_cut2 but not c_effic
		      container->c_cut2_part.push_back(current);
		    }
		    else {
		      container->c_cut2_part.push_back(current);
		      container->c_effic_part.push_back(current);
		    }
		  } 
		}
	    }
	}
	return;
  }
    
    //Pt & num difference between leading jet before efficiency correction and geometrically
    //closest (eta-phi) jet after correction
  void geometric_diff(Pythia8::Pythia & pythia, const std::vector<fastjet::PseudoJet> effic_jets, const std::vector<fastjet::PseudoJet> cut2_jets,
                        double & ptdiff, int & num_diff, int & num_before, int & num_after, double & rel_diff, TTree * matched,
		      TClonesArray & part, TClonesArray & det, TClonesArray & cons,
		      TH1 * hist, TH1 * hist1, TH1 * hist2, TH1 * cons_part_matched, TH1 * cons_det_matched, TH1 * part_matched, TH1* det_matched, TH1 * diff_matched, TH1 *radial, TH1 *pthat,
		      double & det_multiplicity, double & nJets_matched, double & nTracks_matched, ktTrackEff* te, double & matched_weightsum) {
    //TEMPORARY PT HAT CUT!
    //if (pythia.info.pTHat() < 15) {return;} //PUT THIS BACK LATER???
        //initializing relevant quantities
        double mindist  =   99999.0;
        double pt_diff  =  -99999.0;
        int numdiff     =  -99999;
        int num_b       =  -99999;
        double maxR     =   R;
        //int whichone    =   0;
        unsigned which = 0;
        
        num_before  = -99;
        num_after   = -99;
        rel_diff    = -99.0;
        num_diff    = -99;
        
        num_b   = cut2_jets[0].constituents().size();
        //starting with jet passing cuts, loop over jets before efficiency to find best match
	double pseudo_particle_level_multiplicity = 0;
	
        for (unsigned i = 0; i < cut2_jets.size(); ++ i) {
            double dist = effic_jets[0].delta_R(cut2_jets[i]);
            //implies found a match with highest pT (since it's ordered by pT)
            if (dist < maxR) {
                pthat->Fill(pythia.info.pTHat());//, pythia.info.sigmaGen());
                //will be used for weighting later -- just realized, this should actually be the same as matched_weightsum down below since there's one matched jet per matched jet event (duh)
                nJets_matched ++;//+= pythia.info.sigmaGen(); //FIX THIS LATER PLEASEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE!
                det_multiplicity = effic_jets[0].constituents().size();
		
                //fill TLorentzVectors with matched jets (particle & detector level)
                new ( part[which ++] ) myLorentzVector(cut2_jets[i].px(),cut2_jets[i].py(),
                                                          cut2_jets[i].pz(),cut2_jets[i].E(), cut2_jets[i].constituents().size());
                new ( det[which - 1] ) myLorentzVector(effic_jets[0].px(),effic_jets[0].py(),
                                                          effic_jets[0].pz(),effic_jets[0].E(), effic_jets[0].constituents().size());
                cons.Clear();
		pseudo_particle_level_multiplicity = 0;
                for (unsigned j = 0; j < effic_jets[0].constituents().size(); ++ j) {
                    nTracks_matched ++;//+= pythia.info.sigmaGen();
                    new ( cons[j] ) myLorentzVector(effic_jets[0].constituents()[j].px(),effic_jets[0].constituents()[j].py(),
                                                       effic_jets[0].constituents()[j].pz(),effic_jets[0].constituents()[j].E());
                    radial->Fill(effic_jets[0].delta_R(effic_jets[0].constituents()[j]));//,pythia.info.sigmaGen());//put weight back later
                }

		part_matched->Fill(cut2_jets[i].pt());
		det_matched->Fill(effic_jets[0].pt());
		diff_matched->Fill(cut2_jets[i].pt() - effic_jets[0].pt());

		//these two loops fill the constituent pt spectra for matched jets (particle & detector level)
                for (unsigned j = 0; j < cut2_jets[i].constituents().size(); ++ j) {
                    cons_part_matched->Fill(cut2_jets[i].constituents()[j].pt());//, pythia.info.sigmaGen());
                }
                for (unsigned j = 0; j < effic_jets[0].constituents().size(); ++ j) {
                    cons_det_matched->Fill(effic_jets[0].constituents()[j].pt());//, pythia.info.sigmaGen());
                }

		//filling multiplicity & number loss histograms
                hist->Fill(cut2_jets[i].constituents().size() - effic_jets[0].constituents().size());//, pythia.info.sigmaGen());
                hist1->Fill(cut2_jets[i].constituents().size());//, pythia.info.sigmaGen());
                hist2->Fill(effic_jets[0].constituents().size());//, pythia.info.sigmaGen());
		
                matched->Fill();

		//will use as an overall weight later
                matched_weightsum ++;//+= pythia.info.sigmaGen();
                return;
                //break;
            }
        }
        return;
    }

  /*void poisson_parameter_determination (jets,ktTrackEff* te) {
    //need - for each matched detector-level jet, loop over number of tracks and add 1/efficiency of that track's pt/eta to a running "particle-level" multiplicity counter, reset for each jet. The mean or most probable value of that histogram should be the poisson parameter.

    
    return;
  }
  */
  
    ////////////////////////////////////////////////////////////////////////////
    //IMPLEMENTATION FOR CLASS 'ANALYSIS::CONTAINERS'!
    
    containers::containers() {
        //trees
        tuncut   = new TTree("uncut","uncut");
        tcuncut  = new TTree("chargedUncut","charged uncut");
        tcut2    = new TTree("cut2","cut2");
        tccut2   = new TTree("chargedCut2","charged cut2");
        teffic   = new TTree("efficiency","efficiency");
        tceffic  = new TTree("chargedEfficiency","charged efficiency");
        tlead    = new TTree("leadingJetpTs","leading jet pTs");
        tdiffs   = new TTree("numberAndpTDifferences","number and pT differences");
        tpartmult= new TTree("particleLevelMultiplicity", "particle-level track multiplicity");
        tdetmult = new TTree("detectorLevelMultiplicity", "detector-level track multiplicity");
        tmatched = new TTree("matchedJets", "matched jets");
        TTree::SetMaxTreeSize ( 2000000000LL ); // in bytes I think
        
        
        tuncutcons  = new TTree("uncutConstituents","uncut constituents");
        tcuncutcons = new TTree("chargedUncutConstituents","charged uncut constituents");
        tcut2cons   = new TTree("cut2Constituents","cut2 constituents");
        tccut2cons  = new TTree("chargedCut2Constituents","charged cut2 constituents");
        tefficcons  = new TTree("efficConstituents","effic constituents");
        tcefficcons = new TTree("chargedEfficConstituents","charged effic constituents");
        
        //book space for the vectors of particles
        uncut_part.reserve(50); c_uncut_part.reserve(50); cut2_part.reserve(50);
        c_cut2_part.reserve(50); effic_part.reserve(50); c_effic_part.reserve(50);
        
        //initialize the TClonesArrays?
        //uncutJets = new TClonesArray("TLorentzVector");
    }
    
    //getters & setters
    
    void containers::SetBranches() {
        //branches

        //uncutJs = new TClonesArray("TLorentzVector");
        //TClonesArray &uncutJs = *uncutJets;
        //TClonesArray uncutJets ("TLorentzVector");
        //tuncut->Branch("uncutJs", &uncutJs);
        
        tuncut->Branch("weight", &weight);
        tcut2->Branch("weight", &weight);
        teffic->Branch("weight", &weight);
        tcuncut->Branch("weight", &weight);
        tccut2->Branch("weight", &weight);
        tceffic->Branch("weight", &weight);
        tuncutcons->Branch("weight", &weight);
        tcut2cons->Branch("weight", &weight);
        tefficcons->Branch("weight", &weight);
        tcuncutcons->Branch("weight", &weight);
        tccut2cons->Branch("weight", &weight);
        tcefficcons->Branch("weight", &weight);
        tdiffs->Branch("weight", &weight);
        tmatched->Branch("weight", &weight);
        tmatched->Branch("det_multiplicity", &det_multiplicity);
        
        tpartmult->Branch("ccut2mult", &ccut2mult);
        tdetmult->Branch("cefficmult", &cefficmult);
        
        tdiffs->Branch("num_diff", &num_diff);                      tdiffs->Branch("c_num_diff", &c_num_diff);
        tdiffs->Branch("ptdiff", &ptdiff);                          tdiffs->Branch("c_ptdiff", &c_ptdiff);
        tdiffs->Branch("rel_diff", &rel_diff);                      tdiffs->Branch("c_rel_diff", &c_rel_diff);
        tdiffs->Branch("num_before", &num_before);                  tdiffs->Branch("c_num_before", &c_num_before);
        tdiffs->Branch("num_after", &num_after);                    tdiffs->Branch("c_num_after", &c_num_after);
        
        tlead->Branch("uncut_leadPt", &uncut_leadPt);               tlead->Branch("c_uncut_leadPt", &c_uncut_leadPt);
        tlead->Branch("cut2_leadPt", &cut2_leadPt);                 tlead->Branch("c_cut2_leadPt", &c_cut2_leadPt);
        tlead->Branch("effic_leadPt", &effic_leadPt);               tlead->Branch("c_effic_leadPt", &c_effic_leadPt);
        
    }
    
    void containers::Clear() {
        uncut_part.clear(); cut2_part.clear(); effic_part.clear();
        c_uncut_part.clear(); c_cut2_part.clear(); c_effic_part.clear();
        //uncutJets.Clear();
    }
    
    void containers::write() {
        tuncut->Write(); tcuncut->Write(); tcut2->Write(); tccut2->Write();
        teffic->Write(); tceffic->Write(); tlead->Write(); tdiffs->Write();
        tpartmult->Write(); tdetmult->Write();
        tmatched->Write();
        
        tuncutcons->Write(); tcuncutcons->Write();
        tcut2cons->Write();  tccut2cons->Write();
        tefficcons->Write(); tcefficcons->Write();
    }
    
}
