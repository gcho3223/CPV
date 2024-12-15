///////////////////////////////////////////////////
//              Histogram list                   //
///////////////////////////////////////////////////


vector<TString> hist1D =
{
    //////////////////////
    ///// reco level /////
    //////////////////////
    // 1) check the fraction of leading(highest pT) b-taggign jet among all b-tagging jets
    "_h_frac_bTagJet_total_","_h_frac_LeadingbTagJet_is_Leading_",
	// 2) kinematics before pairing
	"_h_leading_bTagJetpt_","_h_leading_bTagJeteta_","_h_leading_bTagJetphi_",
	"_h_leading_jetpt_","_h_leading_jeteta_","_h_leading_jetphi_",
	"_h_subleading_jetpt_","_h_subleading_jeteta_","_h_subleading_jetphi_",
	// 3) dR distribution before pairing
	"_h_dR_bTagJet_Lep_","_h_dR_bTagJet_AnLep_",
    // 3-1) dR distribution after pairing
    "_h_dR_pair_bJet_AnLep_","_h_dR_pair_AnbJet_Lep_",
    // 4) Now, kinematics, O3, and invariant mass histograms after pairing based on dR values
    // dR distribution, kinematics, invariant mass, O3
    // 4-1) dR < 0.5
	"_h_dRNum_pair_bJet_AnLep_0_","_h_dRNum_pair_AnbJet_Lep_0_",                                                                        // dR distribution
    "_h_Pair_bJetpt_0_","_h_Pair_bJeteta_0_","_h_Pair_bJetphi_0_","_h_Pair_AnbJetpt_0_","_h_Pair_AnbJeteta_0_","_h_Pair_AnbJetphi_0_",  // kinematics: b, Anb
    "_h_pair_Leppt_0_","_h_pair_Lepeta_0_","_h_pair_Lepphi_0_","_h_pair_AnLeppt_0_","_h_pair_AnLepeta_0_","_h_pair_AnLepphi_0_",        // kinematics: Mu, AnMu
    "_h_pair_b_Anb_Mass_0_","_h_pair_Mu_AnMu_Mass_0_","_h_pair_b_AnMu_Mass_0_","_h_pair_Anb_Mu_Mass_0_",                                // invariant mass
    "_h_CPO3_bfReco_0_","_h_CPO3_Pos_0_","_h_CPO3_Neg_0_",                                                  // O3
    // 4-2) dR < 1.0    
	"_h_dRNum_pair_bJet_AnLep_1_","_h_dRNum_pair_AnbJet_Lep_1_",                                                                        // dR distribution
    "_h_Pair_bJetpt_1_","_h_Pair_bJeteta_1_","_h_Pair_bJetphi_1_","_h_Pair_AnbJetpt_1_","_h_Pair_AnbJeteta_1_","_h_Pair_AnbJetphi_1_", // kinematics: b, Anb
    "_h_pair_Leppt_1_","_h_pair_Lepeta_1_","_h_pair_Lepphi_1_","_h_pair_AnLeppt_1_","_h_pair_AnLepeta_1_","_h_pair_AnLepphi_1_",        // kinematics: Mu, AnMu
    "_h_pair_b_Anb_Mass_1_","_h_pair_Mu_AnMu_Mass_1_","_h_pair_b_AnMu_Mass_1_","_h_pair_Anb_Mu_Mass_1_",                                // invariant mass
    "_h_CPO3_bfReco_1_","_h_CPO3_Pos_1_","_h_CPO3_Neg_1_",                                                  // O3
    // 4-3) dR < 1.5
	"_h_dRNum_pair_bJet_AnLep_2_","_h_dRNum_pair_AnbJet_Lep_2_",                                                                        // dR distribution
    "_h_Pair_bJetpt_2_","_h_Pair_bJeteta_2_","_h_Pair_bJetphi_2_","_h_Pair_AnbJetpt_2_","_h_Pair_AnbJeteta_2_","_h_Pair_AnbJetphi_2_",  // kinematics: b, Anb
    "_h_pair_Leppt_2_","_h_pair_Lepeta_2_","_h_pair_Lepphi_2_","_h_pair_AnLeppt_2_","_h_pair_AnLepeta_2_","_h_pair_AnLepphi_2_",        // kinematics: Mu, AnMu
    "_h_pair_b_Anb_Mass_2_","_h_pair_Mu_AnMu_Mass_2_","_h_pair_b_AnMu_Mass_2_","_h_pair_Anb_Mu_Mass_2_",                                // invariant mass
    "_h_CPO3_bfReco_2_","_h_CPO3_Pos_2_","_h_CPO3_Neg_2_",                                                  // O3
    // 4-4) dR < 2.0
	"_h_dRNum_pair_bJet_AnLep_3_","_h_dRNum_pair_AnbJet_Lep_3_",                                                                        // dR distribution
    "_h_Pair_bJetpt_3_","_h_Pair_bJeteta_3_","_h_Pair_bJetphi_3_","_h_Pair_AnbJetpt_3_","_h_Pair_AnbJeteta_3_","_h_Pair_AnbJetphi_3_",  // kinematics: b, Anb
    "_h_pair_Leppt_3_","_h_pair_Lepeta_3_","_h_pair_Lepphi_3_","_h_pair_AnLeppt_3_","_h_pair_AnLepeta_3_","_h_pair_AnLepphi_3_",        // kinematics: Mu, AnMu
    "_h_pair_b_Anb_Mass_3_","_h_pair_Mu_AnMu_Mass_3_","_h_pair_b_AnMu_Mass_3_","_h_pair_Anb_Mu_Mass_3_",                                // invariant mass
    "_h_CPO3_bfReco_3_","_h_CPO3_Pos_3_","_h_CPO3_Neg_3_",                                                  // O3
    // 4-4) dR < 2.5 
	"_h_dRNum_pair_bJet_AnLep_4_","_h_dRNum_pair_AnbJet_Lep_4_",                                                                        // dR distribution
    "_h_Pair_bJetpt_4_","_h_Pair_bJeteta_4_","_h_Pair_bJetphi_4_","_h_Pair_AnbJetpt_4_","_h_Pair_AnbJeteta_4_","_h_Pair_AnbJetphi_4_",  // kinematics: b, Anb
    "_h_pair_Leppt_4_","_h_pair_Lepeta_4_","_h_pair_Lepphi_4_","_h_pair_AnLeppt_4_","_h_pair_AnLepeta_4_","_h_pair_AnLepphi_4_",        // kinematics: Mu, AnMu
    "_h_pair_b_Anb_Mass_4_","_h_pair_Mu_AnMu_Mass_4_","_h_pair_b_AnMu_Mass_4_","_h_pair_Anb_Mu_Mass_4_",                                // invariant mass
    "_h_CPO3_bfReco_4_","_h_CPO3_Pos_4_","_h_CPO3_Neg_4_",                                                  // O3
    // 4-5) dR < 3.0
	"_h_dRNum_pair_bJet_AnLep_5_","_h_dRNum_pair_AnbJet_Lep_5_",                                                                        // dR distribution
    "_h_Pair_bJetpt_5_","_h_Pair_bJeteta_5_","_h_Pair_bJetphi_5_","_h_Pair_AnbJetpt_5_","_h_Pair_AnbJeteta_5_","_h_Pair_AnbJetphi_5_",  // kinematics: b, Anb
    "_h_pair_Leppt_5_","_h_pair_Lepeta_5_","_h_pair_Lepphi_5_","_h_pair_AnLeppt_5_","_h_pair_AnLepeta_5_","_h_pair_AnLepphi_5_",        // kinematics: Mu, AnMu
    "_h_pair_b_Anb_Mass_5_","_h_pair_Mu_AnMu_Mass_5_","_h_pair_b_AnMu_Mass_5_","_h_pair_Anb_Mu_Mass_5_",                                // invariant mass
    "_h_CPO3_bfReco_5_","_h_CPO3_Pos_5_","_h_CPO3_Neg_5_",                                                  // O3

	///////////////////////////
    ///// generator level /////
    ///////////////////////////
	"_h_GenMuon_pT_","_h_GenMuon_Eta_","_h_GenMuon_Phi_","_h_GenAnMuon_pT_","_h_GenAnMuon_Eta_","_h_GenAnMuon_Phi_",
	"_h_Genb_pT_","_h_Genb_Eta_","_h_Genb_Phi_","_h_GenAnb_pT_","_h_GenAnb_Eta_","_h_GenAnb_Phi_",
	// dR distribution between b and bjet : after, under 3
	"_h_Gen_dR_Genb_GenAnLep_","_h_Gen_dR_GenAnb_GenLep_","_h_Gen_dR3_pair_Genb_GenAnLep_","_h_Gen_dR3_pair_GenAnb_GenLep_",
	// kinematics after pairing
	"_h_Pair_Genbpt_","_h_Pair_Genbeta_","_h_Pair_Genbphi_","_h_Pair_GenAnbpt_","_h_Pair_GenAnbeta_","_h_Pair_GenAnbphi_",          // kinematics: b, Anb
	"_h_pair_GenMupt_","_h_pair_GenMueta_","_h_pair_GenMuphi_","_h_pair_GenAnMupt_","_h_pair_GenAnMueta_","_h_pair_GenAnMuphi_",    // kinematics: Mu, AnMu
    "_h_pair_Gen_b_Anb_Mass_","_h_pair_Gen_Mu_AnMu_Mass_","_h_pair_Gen_b_AnMu_Mass_","_h_pair_Gen_Anb_Mu_Mass_",                    // invariant mass
    "_h_pair_Gen_b_Mass_","_h_pair_Gen_Anb_Mass_","_h_pair_Gen_Mu_Mass_","_h_pair_Gen_AnMu_Mass_",                                  // invariant mass
	"_h_GenCPO3_bfReco_","_h_GenCPO3_Pos_","_h_GenCPO3_Neg_",                                                                       // O3

    ///////////////////////////
    ///// O3 version 2 ////////
    ///////////////////////////
    // before pairing //
    ////////////////////
    // distribution for index and kinematics of all jets
    "_h_AllJet_idx_","_h_AllJet_pt_","_h_AllJet_eta_","_h_AllJet_phi_"
    // all of dR distribution
    "_h_v2_dR_AnLep_","_h_v2_dR_Lep_",
    // find minimum dR
    "_h_v2_dR_Min_AnJet_Lep_","_h_v2_dR_Min_Jet_AnLep_",
    ///////////////////
    // after pairing //
    ///////////////////
    // distribution for index of Jet and AnJet
    "_h_v2_pair_dR_Jet_AnLep_","_h_v2_pair_dR_AnJet_Lep_",
    // index
    "_h_v2_pair_Jet_idx_","_h_v2_pair_AnJet_idx_",
    // kinematics
    "_h_v2_pair_Jetpt_","_h_v2_pair_Jeteta_","_h_v2_pair_Jetphi_","_h_v2_pair_AnJetpt_","_h_v2_pair_AnJeteta_","_h_v2_pair_AnJetphi_",       // kinematics: jet1, jet2
    "_h_v2_pair_AnLeppt_","_h_v2_pair_AnLepeta_","_h_v2_pair_AnLepphi_","_h_v2_pair_Leppt_","_h_v2_pair_Lepeta_","_h_v2_pair_Lepphi_",         // kinematics: Mu, AnMu
    "_h_v2_pair_Jet_AnMu_Mass_","_h_v2_pair_AnJet_Mu_Mass_",                                                                                   // invariant mass
    "_h_v2_CPO3_bfReco_","_h_v2_CPO3_Pos_","_h_v2_CPO3_Neg_",                                                                                  // O3

    /////////////////////////////////
    ///// O3 after top reco /////////
    /////////////////////////////////
    "h_Reco_CPO3",
};

vector<TString> hist2D =
{
	// dR distribution between b and bjet : before, after
	"_h_dR_bJet_AnbJet_","_h_dR_pair_bJet_AnbJet_",
    // dR distribution with different values: [0: 0.5], [1: 1.0], [2: 1.5], [3: 2.0], [4: 2.5], [5: 3.0]
    "_h_dRNum_pair_bJet_AnbJet_0_","_h_dRNum_pair_bJet_AnbJet_1_","_h_dRNum_pair_bJet_AnbJet_2_","_h_dRNum_pair_bJet_AnbJet_3_","_h_dRNum_pair_bJet_AnbJet_4_","_h_dRNum_pair_bJet_AnbJet_5_"
	// gen level : after, under 3
	"_h_Gen_dR_Genb_GenAnb_","_h_Gen_dR3_af_Genb_GenAnb_",
    /////////////////////
    ///// O3 version 2 //
    /////////////////////
    // before pairing  //
    /////////////////////
    // all of dR distribution
    "_h_v2_dR_Lep_AnLep_",
    // find minimum dR
    "_h_v2_dR_Min_Jet_AnJet_",
    ///////////////////
    // after pairing //
    ///////////////////
    // distribution of idx
    "_h_v2_pair_Jet_AnJet_idx_","_h_v2_pair_comp_Jet_pairbJet_","_h_v2_pair_comp_AnJet_pairAnbJet_",
    // invariant mass
    "_h_v2_pair_Jet_AnbJet_Mass_",
};