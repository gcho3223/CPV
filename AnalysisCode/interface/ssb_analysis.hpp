#ifndef _ssb_analysis_

#define _ssb_analysis_
  
#include <set>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <TH1.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TGraphErrors.h>
#include "TLorentzVector.h"
#include "TEnv.h"
  
#include "./../analysis/SSBTree.h"

// Textreader
#include "./../TextReader/TextReader.hpp"

#include "./../interface/ssb_eff.hpp"
#include "./../interface/ssb_cpviol.hpp"
#include "./../kinsol/TtFullLepKinSolver.hpp"
#include "./../interface/BTagCalibrationStandalone.h"
#include "./../interface/LumiReWeighting.h"

#include "./../roccor.2016.v3/RoccoR.h"

#include "./../KinSolv/analysisUtils.h"
#include "./../KinSolv/KinematicReconstruction.h"
#include "./../KinSolv/KinematicReconstructionSolution.h"

using namespace std;

class ssb_analysis : public SSBTree 
{
   public:
      //declare functions
      ssb_analysis(TTree *tree=0);
      virtual ~ssb_analysis();

      //basic frame
      virtual void Loop( char *logfile );
      void GetNtupleTotalEvent( unsigned int totevent );
      void Start( int genLoopon );
      void End();

      //user define functions
      void SetInputFileName( char *inname );
      void SetOutputFileName(char *outname);
      void DeclareHistos();
      void DeclareHistosSyst(int index_);
      void SetConfig();

      // TLorentVector Initailize function
      void TLVInitial();
   
      void ClearVectors();

      // Scale Factor function
      void GetTotalEvent();
      void MCSF();
      void MCSFApply();
      void GenWeightApply();
      void TriggerSFApply();
      void PileUpReWeightApply();
      void L1PreFireApply();
      void PDFWeightApply();
      void FactRenoApply();
      void FragmentApply();
      void DecayTableApply();
      void LeptonSFApply();
      void BTaggigSFApply();
//      void BTaggigSFApplyJESR(TString sys_);
      void BTaggigSFApplyJESR(std::vector<int>v_jidx, std::vector<TLorentzVector*>v_jtl, int sysidx_);
      void BTaggigSFApplyDilu();
      ///////////////////////
      /// Top pT Reweight ///
      ///////////////////////
      double CalTopPtRewight(TString Opt_);
      void TopPtReweightApply();
      TLorentzVector* FindGenPar(int pdgid_);

      void Weight();

      // Read config files function
      void ReadConfigs();
      void GetVariables();

      // Make TL for object //
      void MakeVecforTL();
      // EventRun Selection
      bool EveRun();
      // Remove Duplicate Event
      void ReadDupleList();
      bool RMDuplEvt(int run_,int lumi_, int evt_num_);

      // METFilter Function  
      bool METFilterAPP();

      // Num.Primary Vertex Function
      void NumPVCount();

      // Trigger Requirement Function
      bool Trigger();

      // Lepton Selection Funtion
      TLorentzVector* ApplyRocCor(TLorentzVector* tmp,int index_);
      TLorentzVector* ApplyElecSCSM(TLorentzVector* tmp,int index_);
      void CorrectedMuonCollection();
      void MakeElecCollection();
      void LeptonSelector();
      bool NumIsoLeptons();
      bool LeptonsPtAddtional();
      void LeptonOrder();
      void LeptonGetSF();
    
      // ElectronID Function
      bool ElecId( TString eid , int Bx , int idx );

      // Lepton Veto Function 
      bool MuVeto();
      bool ElVeto();
      bool ThirdLeptonVeto();

      // Jet Seletion and JetCleanning Function
      void JetSelector();
      void JetCleaning(std::vector<int> v_jidx_cand, std::vector<TLorentzVector*>v_jtl_cand, std::vector<int> &v_jidx, std::vector<TLorentzVector*>&v_jtl );
      bool JetCleaning(TLorentzVector* jet_);
      void JetDefiner();
      void JetDefinerTL( std::vector<TLorentzVector*> v_jets, std::vector<TLorentzVector*> v_jetsup, std::vector<TLorentzVector*> v_jetsdn );
      void BDsicApply();
      void BJetDefiner();
      void BJetDefiner( std::vector<int> v_jets, std::vector<int> v_bjets );
      /////////////////////////////////////////////////////
      /// Dilution For b & b Bar Jet Different response ///
      /////////////////////////////////////////////////////
      void BandBbarJetDiff();  

      // MET Define ...
      void METDefiner();
      // MET Smear ...
      TLorentzVector* METSmear(std::vector<double> v_dpt, std::vector<double> v_dpx, std::vector<double> v_dpy, TLorentzVector* met_);

      // Cut step Function 
      bool ChannelIndex();
      bool DiLeptonMassCut();
      bool ZVetoCut();
      bool NumJetCut(std::vector<int> v_jets);
      bool METCut(TLorentzVector* met);
      bool BJetCut( std::vector<int>v_bjets );
      bool DoubleBtag( std::vector<int>v_bjets );

      ///////////////////////////////////
      // Calculation of b-TaggingSF... //
      ///////////////////////////////////
      void CalBTagWeight( std::vector<int> v_jets );
      double GetBTagWeight( int nbjet );
      void ApplyBTagWeight( int nbjet );

      /////////////////////////////
      // Top Reconstruction .... //
      /////////////////////////////
      void KinSol();
      void KinSolSys( TLorentzVector* lep, TLorentzVector* alep, TLorentzVector*bj1 , TLorentzVector *bj2 , TLorentzVector* met,TString Sys_);
      void Bjorken( TLorentzVector *lep, TLorentzVector *alep,TLorentzVector *bj,TLorentzVector *abj,TLorentzVector *nu,TLorentzVector *anu );

      ////////////////////////////
      /// New Kinematic Solver ///
      ////////////////////////////
      void SetUpKINObs();
      void SetUpKINObsSyst(vector<int> v_jetsys_idx, vector<TLorentzVector*> v_jetsys_TL, TLorentzVector* metsys);
      bool isKinSol;
      VLV v_leptons_VLV; 
      VLV v_jets_VLV; 
      VLV v_bjets_VLV; 
      vector<int> v_lepidx_KIN; 
      vector<int> v_anlepidx_KIN; 
      vector<int> v_jetidx_KIN; 
      vector<int> v_bjetidx_KIN; 
      vector<double> v_btagging_KIN; 


      /////////////////////////////
      /// For Charge Mis Id ... ///
      /////////////////////////////
      void LepAnLepMisCharge();
      void NewLepAnLepMisCharge();
      void SetGenLepAnLep();
      bool Matching(TLorentzVector* par1, TLorentzVector* par2 );


      ///////////////////////////////
      /// For JetPtResolution ... ///
      ///////////////////////////////
      bool FindSameObj(TLorentzVector* ref, TLorentzVector* tar);
      void ApplyJetPtPhiDilution();
      double JetPtResol(TLorentzVector *jet);
      TLorentzVector* ApplyJetPtRes(TLorentzVector *jet, int idx_, TString op_);
      TLorentzVector* ApplyJetPhiRes(TLorentzVector *jet, int idx_, TString op_);
      //void ApplyJetPtRes(TString op_);
      //void ApplyJetPhiRes(TString op_);

      ////////////////////
      /// JER Smearing /// 
      ////////////////////
      TLorentzVector* JERSmearing(TLorentzVector *jet, int idx_, TString op_);


   private:
      //put variables that you want
      char *outfile;
      TFile *fout;
      TFile *a_fout[60]; // for systematics 
      std::vector<TString>v_outName;
      unsigned int NtupletotalEvent;
      unsigned int totalEvent;

      std::map<int, map<int, map<int, int> > > dilepevt;
      //////////////////////////////
      /// Scale Factor variables ///
      //////////////////////////////
      double mc_sf_;
      double evt_weight_;
      double evt_weight_beforemcsf_;
      double evt_weight_beforegenweight_;
      double evt_weight_beforefactrenoweight_;
      double evt_weight_beforedectabweight_;
      double evt_weight_beforefragmentweight_;
      double evt_weight_beforepdfweight_;
      double evt_weight_beforePileup_;
      double evt_weight_beforeL1PreFire_;
      double evt_weight_beforeTrigger_;
      double evt_weight_beforeLepsf_;
      double evt_weight_beforeBtag_;
      double lep_eff;
      double btag_weight[3];

      /////////////////////////////
      /// Btagging Scale Factor ///
      /////////////////////////////
//      BTagCalibration *calib; 
//      BTagCalibrationReader *reader;
      edm::LumiReWeighting *puweight;
      edm::LumiReWeighting *puweightup;
      edm::LumiReWeighting *puweightdn;
 
      //TextReader from Jaehoon.
      TextReader *SSBConfReader;
      // Muon RocCorrection //
      RoccoR *ssbmucor;
      // Object Eff. 
      SSBEffCal *SSBEffcal;

      // KinSolver 
      TtFullLepKinSolver *ssbflsolver; 

      // CPViolation 
      SSBCPViol *ssbcpviol; 

      // Center Of Energy
      TString CenOfE;
  
      // Luminosity
      double Lumi;
      
      //Decay Channel
      TString Decaymode;

      //Input File Name
      TString FileName_; 

      //Using Total Event Number//
      TString UsingTotEnv;

      // Type of Systematics
      std::vector<TString> v_SystType;
      std::vector<TString> v_SystFullName;// For Histogram
      std::vector<double> v_SystEvt;// For Histogram
      std::map<TString,double> m_Syst_EvtW;// For Histogram
      
      //cut flow 
      TString cutflowName[10];
      //IsSignal or MC
      bool isSignal; 

      //IsSignal or MC
      bool isAllSyst;
      int idx_jecup;
      int idx_jecdn;
      int idx_jerup;
      int idx_jerdn;

      //Variable for Primary Vertex
      int num_pv;

      //to get trigger information from config.
      int num_metfilt;
      //to get trigger information from config.
      int num_trig;
      TString TrigSFSys;
      /// Muon Energy (Momentum) Sys. ///
      TString MuEnSys;
      /// Electron Scale Smear Sys. ///
      TString EleScSmSys;

      /// Dummy Variable for JetSelector
      int l_jet;

      /// Variable for DiLeptonMassCut
      bool dimu_masscut;

      /// Variable for ZVetoCut
      bool zvetocut;

      /// Variable for NumJetCut
      bool numjetcut;

      /// Variable for METCut
      bool metcut;

      /// to generate smeared jet 
      bool dojer;
      TString JetResSys;

      /// to define bjet at One-bjet tagged cut 
      int bcandi;
   
      /// Variable for BJetCut and DoubleBatag 
      bool bjetcut;
      bool doublebtag;

      /// Variable for KinSol Function 
      double xconstraint  , yconstraint ;

      //lepton iso type
      TString lepisotype;
      TString lepId;

      //Muon Type for MuEl - channel.
      TString Muonisotype;
      TString Muonid;

      //Jet type
      TString jetId;
      TString jetbtag;
      // Jet Systematic ...
      TString JetEnSys;
      TString BTagSFSys;
      TString BTagEffSys;
      // MET Systematic ...
      TString MetSys;

      // Lepton Systematic
      TString LepIdSFSys;
      TString LepIsoSFSys;
      TString LepRecoSFSys;
      TString LepTrackSFSys;

      // PileUp Systematic ...
      string PileUpMCFile;
      string PileUpDATAFile;
      string PileUpDATAFileUp;
      string PileUpDATAFileDown;
      TString PileUpSys;
      TString L1PreFireSys;

      // PDF Systematic ... Getting the Num of PDF Set
      int PDFSys;
      // FactReno Systematic ... Getting the Num of FactReno Set
      int FactRenoSys;
      // Fragment Systematic ... Getting the Num of Fragment Set
      TString FragmentSys;
      // DecayTable Systematic ... Getting the Num of DecayTable Set
      TString DecayTableSys;
      // TopPtReweight Sys
      TString TopPtSys;
   
      /// Dilution Study for JetPtResolution ///
      TString JetPtResDil; 
      TString JetPhiResDil;
      bool    JetPtPhiDil; 
      bool    bAndbBarDil; 
      //Electron Type for MuEl - channel.
      TString Elecisotype;
      TString Elecid;

      //JetCleanning information from config.
      TString muiso_type;
      TString muId;
      TString eliso_type;
      TString elId;

      //Kinetic Variables 
      double lep_pt;
      double lep_eta;
      double lepisocut;
      int    n_elep_Id;
      double el_id_1;
      double el_id_2;

      double muon_pt;
      double muon_eta;
      double muonisocut;
      double elec_pt;
      double elec_eta;
      double elecisocut;
      int    n_elec_Id;


      double jet_pt;
      double jet_eta;
      int    jet_id;
      double met_cut;

      //Jetcleannig Variables
      double mu_pt_jetcl;
      double mu_eta_jetcl;
      double mu_iso_jetcl;

      double el_pt_jetcl;
      double el_eta_jetcl;
      double el_iso_jetcl;
      int    n_el_id_jetcl;
      double el_id_jetcl_1;
      double el_id_jetcl_2;
      
      //b-tagging Variables
      float bdisccut;
      int   nbtagged;
     
      // KinSolver Variables.
      double topmass_begin;
      double topmass_end;
      double topmass_step_;

      // Weighting Factor of KinSolver
      double ksolweight_;
    
      // Bjorken variable
      double x1_bj;
      double x2_bj;
      double x3_bj;

      ///////////////////
      //  CP Violation.//
      ///////////////////
      double Np_eventw_2;
      double Nm_eventw_2;
      int    Num_cpo3_p; 
      int    Num_cpo3_m;
      std::vector<double> v_recocp_O;
      ///////////////////////
      //Mis - Charge Study //
      ///////////////////////

      double laecO3;
      double laecOb;
      double laecO5;
      std::vector<int> v_MisCharge;

      /// pT Range for JetAngular ///
//      static double ptRange[] = {30,37,44,52,60,69,80,94,114,156,1000};
//      double ptRange[11];
//      double pTRange[22];
      double pi;
      /////////////////////////////
      /// vector for variables. ///
      /////////////////////////////
      std::vector<std::string> v_METFilterName; // METFilterName 
      std::vector<std::string> trigName; // trigger 
      std::vector<int> v_lepton_idx; // Indecies of lepton
      std::vector<int> v_muon_idx; // Indecies of muon
      std::vector<int> v_electron_idx; // Indecies of electron
      std::vector<int> v_jet_idx;  // Indecies of jet

      // JES Variation //
      std::vector<int> v_jetup_idx;  // Indecies of jet for All-in-One syst.
      std::vector<int> v_jetdn_idx;  // Indecies of jet for All-in-One syst.

      // JER Variation //
      std::vector<int> v_jetresup_idx;  // Indecies of jetresup for All-in-One syst.
      std::vector<int> v_jetresdn_idx;  // Indecies of jetresdn for All-in-One syst.
      // b-Jet //
      std::vector<int> v_bjet_idx; // Indecies of bjet JES Up
      // b-Jet w JES //
      std::vector<int> v_bjetup_idx; // Indecies of bjet JES Up
      std::vector<int> v_bjetdn_idx; // Indecies of bjet JES Down
      // b-Jet w JER //
      std::vector<int> v_bjetresup_idx; // Indecies of bjetresup JES Up
      std::vector<int> v_bjetresdn_idx; // Indecies of bjetresup JES Down

      std::vector<TLorentzVector*> v_jet_TL;  // Indecies of jet
      // JES Variation TLV //
      std::vector<TLorentzVector*> v_jetup_TL;  // TLV of jet
      std::vector<TLorentzVector*> v_jetdn_TL;  // TLV of jet
      // JER Variation TLV //
      std::vector<TLorentzVector*> v_jetresup_TL;  // TLV of jetres
      std::vector<TLorentzVector*> v_jetresdn_TL;  // TLV of jetres

      std::vector<TLorentzVector*> v_bjet_TL;  // TLV of jet

      //b-Jets JES Variation TLV //
      std::vector<TLorentzVector*> v_bjetup_TL;  // TLV of jet
      std::vector<TLorentzVector*> v_bjetdn_TL;  // TLV of jet

      //b-Jets JER Variation TLV //
      std::vector<TLorentzVector*> v_bjetresup_TL;  // TLV of jetres
      std::vector<TLorentzVector*> v_bjetresdn_TL;  // TLV of jetres
 
      // Delta pT JER Variation //
      std::vector<double> v_jetdpt_res;    // dpt of JER for All-in-One syst.
      std::vector<double> v_jetdpt_jesup;  // dpt of JesUp for All-in-One syst.
      std::vector<double> v_jetdpt_jesdn;  // dpt of JesDn for All-in-One syst.
      std::vector<double> v_jetdpt_resup;  // dpt of JERUp for All-in-One syst.
      std::vector<double> v_jetdpt_resdn;  // dpt of JERDn for All-in-One syst.

      // Delta px JER Variation // 
      std::vector<double> v_jetdpx_res;    // dpx of JER for All-in-One syst.
      std::vector<double> v_jetdpx_jesup;  // dpx of JESUp for All-in-One syst.
      std::vector<double> v_jetdpx_jesdn;  // dpx of JESDn for All-in-One syst.
      std::vector<double> v_jetdpx_resup;  // dpx of JERUp for All-in-One syst.
      std::vector<double> v_jetdpx_resdn;  // dpx of JERDn for All-in-One syst.

      // Delta py JER Variation // 
      std::vector<double> v_jetdpy_res;    // dpy of JER for All-in-One syst.
      std::vector<double> v_jetdpy_jesup;  // dpy of JERUp for All-in-One syst.
      std::vector<double> v_jetdpy_jesdn;  // dpy of JERDn for All-in-One syst.
      std::vector<double> v_jetdpy_resup;  // dpy of JERUp for All-in-One syst.
      std::vector<double> v_jetdpy_resdn;  // dpy of JERDn for All-in-One syst.

      ////////////////////
      /// dummy vector ///
      ////////////////////
      std::vector<TLorentzVector*> v_CorMuons; // for corrected Muon 
      std::vector<TLorentzVector*> v_Elec; // for Electron 
      std::vector<int> v_lep_idx_temp; // for lepton selection
      std::vector<int> v_muon_idx_temp;
      std::vector<int> v_electron_idx_temp;

      std::vector<int> v_jet_idx_cand; // for JetSelector
      std::vector<int> v_jetup_idx_cand; 
      std::vector<int> v_jetdn_idx_cand; 
      std::vector<int> v_jetresup_idx_cand; 
      std::vector<int> v_jetresdn_idx_cand; 

      std::vector<int> v_ele_idx_cand;
      std::vector<int> v_mu_idx_cand;

      std::vector<TLorentzVector*> v_jet_TL_cand;  // Indecies of jet
      std::vector<TLorentzVector*> v_jetup_TL_cand;  // Indecies of jet
      std::vector<TLorentzVector*> v_jetdn_TL_cand;  // Indecies of jet
      std::vector<TLorentzVector*> v_jetresup_TL_cand;  // Indecies of jetres
      std::vector<TLorentzVector*> v_jetresdn_TL_cand;  // Indecies of jetres

      std::vector<TLorentzVector*> v_bjet_TL_cand;  // Indecies of jet
      std::vector<TLorentzVector*> v_bjetup_TL_cand;  // Indecies of jet
      std::vector<TLorentzVector*> v_bjetdn_TL_cand;  // Indecies of jet
      std::vector<TLorentzVector*> v_bjetresup_TL_cand;  // Indecies of jetres
      std::vector<TLorentzVector*> v_bjetresdn_TL_cand;  // Indecies of jetres

      // TLorentzVector for object selection
      TLorentzVector *Mu_lep_sel;
      TLorentzVector *Ele_lep_sel;

      // TLorentzVector for object selection
      TLorentzVector *Mu_jet_clean;
      TLorentzVector *Ele_jet_clean;
      TLorentzVector *selected_Ele_jet_clean;
      TLorentzVector *selected_Mu_jet_clean; 
      TLorentzVector *TJet; // Jet For Selection

      // TLorentzVector : Lepton
      TLorentzVector *Lep1; // Leading Lepton 
      TLorentzVector *Lep2; // Second Leading Lepton
      TLorentzVector *TMuon; // Leading Lepton 
      TLorentzVector *TElectron; // Second Leading Lepton

      // Veto muon and electrons
      TLorentzVector *vetoMu;
      TLorentzVector *vetoEle;

      // TLorentzVector : Jet
      TLorentzVector *Jet1; // Leading Jet 
      TLorentzVector *Jet2; // Second Leading Jet
      TLorentzVector *Jet1Up; // Leading Jet 
      TLorentzVector *Jet2Up; // Second Leading Jet
      TLorentzVector *Jet1Dn; // Leading Jet 
      TLorentzVector *Jet2Dn; // Second Leading Jet
      TLorentzVector *Jet1JERUp; // Leading Jet 
      TLorentzVector *Jet2JERUp; // Second Leading Jet
      TLorentzVector *Jet1JERDn; // Leading Jet 
      TLorentzVector *Jet2JERDn; // Second Leading Jet

      TLorentzVector *bJet1; // Leading Jet 
      TLorentzVector *bJet2; // Second Leading Jet
      TLorentzVector *bJet1Up; // Leading Jet 
      TLorentzVector *bJet2Up; // Second Leading Jet
      TLorentzVector *bJet1Dn; // Leading Jet 
      TLorentzVector *bJet2Dn; // Second Leading Jet
      TLorentzVector *bJet1JERUp; // Leading Jet 
      TLorentzVector *bJet2JERUp; // Second Leading Jet
      TLorentzVector *bJet1JERDn; // Leading Jet 
      TLorentzVector *bJet2JERDn; // Second Leading Jet


      ///// new algorithm /////
      TString cutdRName[6] = {"0.5","1.0","1.5","2.0","2.5","3.0"};

      std::vector<double> dR_jet_Lep;
      std::vector<double> dR_jet_AnLep;

      TLorentzVector *highPtJet; // highetst pT of jet
      TLorentzVector *leading_bTagJet; // highetst pT b tagging jet
      TLorentzVector *pair_bJet;       // b jet after pairing
      TLorentzVector *pair_AnbJet;     // anti b jet after pairing
      TLorentzVector *pair_Lep;        // lepton after pairing
      TLorentzVector *pair_AnLep;      // anti lepton after pairing
      
      TLorentzVector *v2_pair_Jet;     // paired with AnLep with minimum dR
      TLorentzVector *v2_pair_AnJet;   // paired with Lep with minimum dR
      TLorentzVector *v2_pair_Lep;     // paired with Jet with minimum dR
      TLorentzVector *v2_pair_AnLep;   // paired with AnJet with minimum dR

      TLorentzVector *Genb;            // b quark at generator level
      TLorentzVector *GenAnb;          // anti-b quark at generator level
      TLorentzVector *GenMu;           // muon at generator level
      TLorentzVector *GenAnMu;         // anti-muon at generator level


      // TLorentzVector : MET
      TLorentzVector *Met; // MET
      TLorentzVector *MetJESUp; // MET
      TLorentzVector *MetJESDn; // MET
      TLorentzVector *MetJERUp; // MET
      TLorentzVector *MetJERDn; // MET
  
      // TLorentzVector : Nutrino
      TLorentzVector *Nu1; // Nutrino 1 ( For Lep1 )
      TLorentzVector *Nu2; // Nutrino 2 ( For Lep2 )

      // TLorentzVector : WBoson
      TLorentzVector *W1; // W boson 1 ( For Lep1 )
      TLorentzVector *W2; // W boson 2 ( For Lep2 )

      // TLorentzVector : Top
      TLorentzVector *Top;   // Top
      TLorentzVector *AnTop; // Anti-top

      TLorentzVector *Top1; // Top ( For Lep 1 )
      TLorentzVector *Top2; // Top ( For Lep 2 )

      TLorentzVector *bJet; // bJet
      TLorentzVector *AnbJet; // Anti-bJet
  
      TLorentzVector *Lep; // Lepton
      TLorentzVector *AnLep; // Anti-Lepton

      TLorentzVector *Nu; // Neutrino
      TLorentzVector *AnNu; // Anti-Neutrino

      TLorentzVector *GenNu; // Neutrino at Generator Level
      TLorentzVector *GenAnNu; // Anti-Neutrino at Generator Level

      TLorentzVector *GenLep; // Neutrino at Generator Level
      TLorentzVector *GenAnLep; // Anti-Neutrino at Generator Level

      TLorentzVector *Mu; // Muon
      TLorentzVector *Ele; // Electron For Selection

      /// User variable for Kinematic cut 
      std::vector<double> *v_lepton_iso;
      std::vector<bool>   *v_lepton_Id;
      std::vector<int>    *v_jet_Id;
      std::vector<double> *v_mlep_iso;
      std::vector<double> *v_elec_iso;
      std::vector<bool>   *v_muon_Id;  // for MuEl channel
      //std::vector<float>  *v_electron_Id; // for ElEl and MuEl channels
      std::vector<bool>  *v_electron_Id; // for ElEl and MuEl channels
    
      /// User variables for JetCleanning cut
      std::vector<double> *v_mu_iso_jcl;
      std::vector<bool>   *v_mu_Id_jcl;
      std::vector<double> *v_el_iso_jcl;
      //std::vector<float>  *v_el_Id_jcl;
      std::vector<bool>  *v_el_Id_jcl;

      // User variable for PileUp Reweight 
      std::vector<double> *v_pileup_weight;
      // vector for KinSolver
      std::vector<double> nupars_;

      // vector for TGraph //
      std::vector<double> v_eta_bjet;
      std::vector<double> v_pt_bjet;
      // vector for ChargeMisId

   public:

      //declare histograms


      TH1D *h_PileUp;
      TH1D *h_PileUp_Up;
      TH1D *h_PileUp_Down;
      TH1D *h_NumEl;
      TH1D *h_NumMu;
      TH1D *h_Num_PV_BeforePreSel;
      TH1D *h_Num_PV_AfterMetFilter;
      TH1D *h_Num_PV_AfterTrigger;
      // Cut Flow
      TH1D *h_cf_NLeptons[10];
      TH1D *h_cf_Lep1pt[10];
      TH1D *h_cf_Lep1eta[10];
      TH1D *h_cf_Lep1phi[10];
      TH1D *h_cf_Lep2pt[10];
      TH1D *h_cf_Lep2eta[10];
      TH1D *h_cf_Lep2phi[10];
      TH1D *h_cf_dilep_inv_mass[10];
      TH1D *h_cf_Jet1pt[10];
      TH1D *h_cf_Jet1eta[10];
      TH1D *h_cf_Jet1phi[10];
      TH1D *h_cf_Jet2pt[10];
      TH1D *h_cf_Jet2eta[10];
      TH1D *h_cf_Jet2phi[10];
      TH1D *h_cf_NJets[10];
      TH1D *h_cf_metpt[10];
      TH1D *h_cf_metphi[10];
      TH1D *h_cf_Nbjets[10];
      TH1D *h_cf_NPV[10];

      TH1D *h_Lep1pt[10];
      TH1D *h_Lep2pt[10];
      TH1D *h_Lep1eta[10];
      TH1D *h_Lep2eta[10];
      TH1D *h_Lep1phi[10];
      TH1D *h_Lep2phi[10];
      TH1D *h_Jet1pt[10];
      TH1D *h_Jet2pt[10];
      TH1D *h_Jet1eta[10];
      TH1D *h_Jet2eta[10];
      TH1D *h_Jet1phi[10];
      TH1D *h_Jet2phi[10];
      TH1D *h_HT[10]; 
      TH1D *h_METpt[10]; 
      TH1D *h_METphi[10]; 
      TH1D *h_DiLepMass[10]; 
      TH1D *h_Num_PV[10];
      TH1D *h_Num_Jets[10];
      TH1D *h_Num_bJets[10];
      TH1D *h_Muonpt[10];
      TH1D *h_Elecpt[10];
      TH1D *h_Muoneta[10];
      TH1D *h_Eleceta[10];
      TH1D *h_Muonphi[10];
      TH1D *h_Elecphi[10];

      ////////// new algorithm //////////
      /// Reco ///
      // find highest pT b tagging jet & (sub)leading jet
      TH1D* h_frac_bTagJet_total;
      TH1D* h_frac_LeadingbTagJet_is_Leading;

      TH1D *h_leading_bTagJetpt;
      TH1D *h_leading_bTagJeteta;
      TH1D *h_leading_bTagJetphi;
      TH1D *h_leading_jetpt;
      TH1D *h_leading_jeteta;
      TH1D *h_leading_jetphi;
      TH1D *h_subleading_jetpt;
      TH1D *h_subleading_jeteta;
      TH1D *h_subleading_jetphi;
      // pairing
      // before
      TH1D *h_dR_bTagJet_Lep;
      TH1D *h_dR_bTagJet_AnLep;
      TH2D *h_dR_bJet_AnbJet;
      // after
      TH1D *h_dR_pair_bJet_AnLep;
      TH1D *h_dR_pair_AnbJet_Lep;
      TH2D *h_dR_pair_bJet_AnbJet;
      // if dR is under 3
      TH1D *h_dRNum_pair_bJet_AnLep[7];
      TH1D *h_dRNum_pair_AnbJet_Lep[7];
      TH2D *h_dRNum_pair_bJet_AnbJet[7];
      // kinematics after pairing
      TH1D *h_Pair_bJetpt[7];
      TH1D *h_Pair_bJeteta[7];
      TH1D *h_Pair_bJetphi[7];
      TH1D *h_Pair_AnbJetpt[7];
      TH1D *h_Pair_AnbJeteta[7];
      TH1D *h_Pair_AnbJetphi[7];
      TH1D *h_pair_Leppt[7];
      TH1D *h_pair_Lepeta[7];
      TH1D *h_pair_Lepphi[7];
      TH1D *h_pair_AnLeppt[7];
      TH1D *h_pair_AnLepeta[7];
      TH1D *h_pair_AnLepphi[7];
      // invariant mass after pairing
      TH1D *h_pair_b_Anb_Mass[7];
      TH1D *h_pair_Mu_AnMu_Mass[7];
      TH1D *h_pair_b_AnMu_Mass[7];
      TH1D *h_pair_Anb_Mu_Mass[7];
      // calcuate O3
      TH1D *h_CPO3_bfReco[7];
      TH1D *h_CPO3_bfReco_ReRange[7];
      TH1D *h_CPO3_Pos[7];
      TH1D *h_CPO3_Neg[7];

      ///// generator level /////
      TH1D *h_GenMuon_pt;
      TH1D *h_GenMuon_eta;
      TH1D *h_GenMuon_phi;
      TH1D *h_GenAnMuon_pt;
      TH1D *h_GenAnMuon_eta;
      TH1D *h_GenAnMuon_phi;
      TH1D *h_Genb_pt;
      TH1D *h_Genb_eta;
      TH1D *h_Genb_phi;
      TH1D *h_GenAnb_pt;
      TH1D *h_GenAnb_eta;
      TH1D *h_GenAnb_phi;
      // pairing and if dR is under 3
      TH1D *h_Gen_dR_Genb_GenAnLep;
      TH1D *h_Gen_dR_GenAnb_GenLep;
      TH2D *h_Gen_dR_Genb_GenAnb;
      TH1D *h_Gen_dR3_pair_Genb_GenAnLep;
      TH1D *h_Gen_dR3_pair_GenAnb_GenLep;
      TH2D *h_Gen_dR3_af_Genb_GenAnb;
      // kinematics after pairing
      TH1D *h_Pair_Genbpt;
      TH1D *h_Pair_Genbeta;
      TH1D *h_Pair_Genbphi;
      TH1D *h_Pair_GenAnbpt;
      TH1D *h_Pair_GenAnbeta;
      TH1D *h_Pair_GenAnbphi;
      TH1D *h_pair_GenMupt;
      TH1D *h_pair_GenMueta;
      TH1D *h_pair_GenMuphi;
      TH1D *h_pair_GenAnMupt;
      TH1D *h_pair_GenAnMueta;
      TH1D *h_pair_GenAnMuphi;
      // invariant mass after pairing
      TH1D *h_pair_Gen_b_Anb_Mass;
      TH1D *h_pair_Gen_Mu_AnMu_Mass;
      TH1D *h_pair_Gen_b_AnMu_Mass;
      TH1D *h_pair_Gen_Anb_Mu_Mass;
      TH1D *h_pair_Gen_b_Mass;
      TH1D *h_pair_Gen_Anb_Mass;
      TH1D *h_pair_Gen_Mu_Mass;
      TH1D *h_pair_Gen_AnMu_Mass;
      // calcuate O3
      TH1D *h_GenCPO3_bfReco;
      TH1D *h_GenCPO3_bfReco_ReRange;
      TH1D *h_GenCPO3_Pos;
      TH1D *h_GenCPO3_Neg;

      ///// O3 version 2 /////
      //////////////////////
      /// before pairing ///
      //////////////////////
      TH1D *h_AllJet_idx;
      TH1D *h_AllJet_pt;
      TH1D *h_AllJet_eta;
      TH1D *h_AllJet_phi;
      // all of dR distribution
      TH1D *h_v2_dR_AnLep;
      TH1D *h_v2_dR_Lep;
      TH2D *h_v2_dR_Lep_AnLep;
      // find minimum dR
      TH1D *h_v2_dR_Min_Lep;
      TH1D *h_v2_dR_Min_AnLep;
      TH2D *h_v2_dR_Min_Lep_AnLep;
      /////////////////////
      /// after pairing ///
      /////////////////////
      /// Jet & AnLep ///
      ///////////////////
      // check dR values final_dR_jet1_AnLep is the same as min_dR_jet1_AnLep and index is the same as idx_jet1
      TH1D *h_v2_pair_Jet_dR_AnLep;
      TH1D *h_v2_pair_Jet_idx;
      // kinematics
      TH1D *h_v2_pair_Jetpt;
      TH1D *h_v2_pair_Jeteta;
      TH1D *h_v2_pair_Jetphi;
      TH1D *h_v2_pair_AnLeppt;
      TH1D *h_v2_pair_AnLepeta;
      TH1D *h_v2_pair_AnLepphi;
      ///////////////////
      /// AnJet & Lep ///
      ///////////////////
      // check dR values final_dR_jet1_AnLep is the same as min_dR_jet1_AnLep and index is the same as idx_jet1
      TH1D *h_v2_pair_AnJet_dR_Lep;
      TH1D *h_v2_pair_AnJet_idx;
      // kinematics
      TH1D *h_v2_pair_AnJetpt;
      TH1D *h_v2_pair_AnJeteta;
      TH1D *h_v2_pair_AnJetphi;
      TH1D *h_v2_pair_Leppt;
      TH1D *h_v2_pair_Lepeta;
      TH1D *h_v2_pair_Lepphi;
      // distribution of idx
      TH2D *h_v2_pair_Jet_AnJet_idx;
      TH2D *h_v2_pair_comp_Jet_pairbJet;
      TH2D *h_v2_pair_comp_AnJet_pairAnbJet;
      // invariant mass
      TH1D *h_v2_pair_Jet_AnMu_Mass;
      TH1D *h_v2_pair_AnJet_Mu_Mass;
      TH2D *h_v2_pair_Jet_AnbJet_Mass;
      // O3
      TH1D *h_v2_CPO3_bfReco;
      TH1D *h_v2_CPO3_bfReco_ReRange;
      TH1D *h_v2_CPO3_Pos;
      TH1D *h_v2_CPO3_Neg;
      ///////////////////////////////////

      TH1D *h_Topo_Apla[5];
      TH1D *h_Topo_Sphe[5];
      TH1D *h_Topo_Plan[5];
      TH1D *h_bTagEff[5];

      TH1D *h_Top1Mass; 
      TH1D *h_Top1pt; 
      TH1D *h_Top1Rapidity; 
      TH1D *h_Top1phi; 
      TH1D *h_Top1Energy; 

      TH1D *h_Top2Mass; 
      TH1D *h_Top2pt; 
      TH1D *h_Top2Rapidity; 
      TH1D *h_Top2phi; 
      TH1D *h_Top2Energy; 

      TH1D *h_TopMass; 
      TH1D *h_Toppt; 
      TH1D *h_TopRapidity; 
      TH1D *h_Topphi; 
      TH1D *h_TopEnergy; 

      TH1D *h_AnTopMass;
      TH1D *h_AnToppt; 
      TH1D *h_AnTopRapidity; 
      TH1D *h_AnTopphi; 
      TH1D *h_AnTopEnergy; 

      TH1D *h_Top1Mass_Reweight; 
      TH1D *h_Top1pt_Reweight;
      TH1D *h_Top1Rapidity_Reweight; 
      TH1D *h_Top1phi_Reweight; 
      TH1D *h_Top1Energy_Reweight; 

      TH1D *h_Top2Mass_Reweight;
      TH1D *h_Top2pt_Reweight; 
      TH1D *h_Top2Rapidity_Reweight; 
      TH1D *h_Top2phi_Reweight; 
      TH1D *h_Top2Energy_Reweight; 

      TH1D *h_TopMass_Reweight; 
      TH1D *h_Toppt_Reweight; 
      TH1D *h_TopRapidity_Reweight; 
      TH1D *h_Topphi_Reweight; 
      TH1D *h_TopEnergy_Reweight; 

      TH1D *h_AnTopMass_Reweight;
      TH1D *h_AnToppt_Reweight; 
      TH1D *h_AnTopRapidity_Reweight; 
      TH1D *h_AnTopphi_Reweight; 
      TH1D *h_AnTopEnergy_Reweight; 



      TH1D *h_GenTopMass; 
      TH1D *h_GenToppt; 
      TH1D *h_GenTopRapidity; 
      TH1D *h_GenTopphi; 
      TH1D *h_GenTopEnergy; 

      TH1D *h_GenAnTopMass;
      TH1D *h_GenAnToppt; 
      TH1D *h_GenAnTopRapidity; 
      TH1D *h_GenAnTopphi; 
      TH1D *h_GenAnTopEnergy; 

      TH1D *h_W1Mass; 
      TH1D *h_W2Mass;
      TH1D *h_W3Mass;
      TH1D *h_W4Mass;

      TH1D *h_W1Mt; 
      TH1D *h_W2Mt;

      TH1D *h_bJet1Energy;
      TH1D *h_bJet2Energy;

      TH1D *h_bJetEnergy;
      TH1D *h_AnbJetEnergy;
      TH1D *h_bJetPt;
      TH1D *h_AnbJetPt;

      TH1D *h_Lep1Energy;
      TH1D *h_Lep2Energy;

      TH1D *h_LepEnergy;
      TH1D *h_AnLepEnergy;

      TH1D *h_Nu1Energy;
      TH1D *h_Nu2Energy;

      TH1D *h_NuEnergy;
      TH1D *h_AnNuEnergy;

      TH1D *h_Reco_CPO_[13];
      TH1D *h_Reco_CPO_ReRange_[13];
      TH1D *h_LepAnLepEngCheck_[13];
      TH1D *h_CPO3_reco;
      TH1D *h_CPO5_reco;
      TH1D *h_CPO3_reco_JPRUp;
      TH1D *h_CPO3_reco_JPRDown;
      TH1D *h_CPO3_reco_TopRes;
      TH1D *h_CPOb_reco;

      TH1D *h_BjorkenX1;
      TH1D *h_BjorkenX2;
      TH1D *h_BjorkenX3;

      TH1D *h_GenNuEnergy;
      TH1D *h_GenAnNuEnergy;

      TH1D *h_EventWeight[10];

      TH1D *h_LepbJetMass;
      TH1D *h_AnLepbJetMass;

      TH2F *h2_AnTopMassVsLepBMass;
      TH2F *h2_TopMassVsLepBMass;
      TH2F *h2_AnLepBMassVsLepBMass;
   
      TH1D *h_CPO3_evtweight;
      TH1D *h_CPOb_evtweight;
      TH1D *h_CPO5_evtweight;

      TH1D *h_Diff_BBar_Pt;
      TH1D *h_Diff_BBar_Energy;
      TH1D *h_Diff_BBar_P;

      TH1D *h_Diff_LepAnLep_Pt;
      TH1D *h_Diff_LepAnLep_Energy;
      TH1D *h_Diff_LepAnLep_P;

      TH1D *h_LepAnLepEngCheckO3;
      TH1D *h_LepAnLepEngCheckOb;
      TH1D *h_LepAnLepEngCheckO5;

      TH1D *h_CPO3_Plus;
      TH1D *h_CPO3_Minus;

      TH1D *h_CPO3_JPRUp_Plus_Plus;
      TH1D *h_CPO3_JPRUp_Plus_Minus;
      TH1D *h_CPO3_JPRUp_Minus_Minus;
      TH1D *h_CPO3_JPRUp_Minus_Plus;

      TH1D *h_CPO3_JPRDown_Plus_Plus;
      TH1D *h_CPO3_JPRDown_Plus_Minus;
      TH1D *h_CPO3_JPRDown_Minus_Minus;
      TH1D *h_CPO3_JPRDown_Minus_Plus;

      ////////////////////////////////////////////////
      /// CPO3 Variables for JetAngular Resolution ///
      ////////////////////////////////////////////////

      TH1D *h_CPO3reco_evtweight;
      TH1D *h_CPO3reco_evtweight_EtaVariUp;
      TH1D *h_CPO3reco_evtweight_EtaVariDown;
      TH1D *h_CPO3reco_evtweight_PhiVariUp; 
      TH1D *h_CPO3reco_evtweight_PhiVariDown;

      TH1D *h_CPO3_EtaVariUp;
      TH1D *h_CPO3_EtaVariUp_Plus_Plus;
      TH1D *h_CPO3_EtaVariUp_Plus_Minus;
      TH1D *h_CPO3_EtaVariUp_Minus_Minus;
      TH1D *h_CPO3_EtaVariUp_Minus_Plus;

      TH1D *h_CPO3_EtaVariDown;
      TH1D *h_CPO3_EtaVariDown_Plus_Plus;
      TH1D *h_CPO3_EtaVariDown_Plus_Minus;
      TH1D *h_CPO3_EtaVariDown_Minus_Minus;
      TH1D *h_CPO3_EtaVariDown_Minus_Plus;

      TH1D *h_CPO3_PhiVariUp;
      TH1D *h_CPO3_PhiVariUp_Plus_Plus;
      TH1D *h_CPO3_PhiVariUp_Plus_Minus;
      TH1D *h_CPO3_PhiVariUp_Minus_Minus;
      TH1D *h_CPO3_PhiVariUp_Minus_Plus;

      TH1D *h_CPO3_PhiVariDown;
      TH1D *h_CPO3_PhiVariDown_Plus_Plus;
      TH1D *h_CPO3_PhiVariDown_Plus_Minus;
      TH1D *h_CPO3_PhiVariDown_Minus_Minus;
      TH1D *h_CPO3_PhiVariDown_Minus_Plus;
      TH1D *h_PileUpCheck;

      /// Systematic study with 
      // Cut Flow
      TH1D *h_cf_sys_NLeptons[60][10];
      TH1D *h_cf_sys_Lep1pt[60][10];
      TH1D *h_cf_sys_Lep1eta[60][10];
      TH1D *h_cf_sys_Lep1phi[60][10];
      TH1D *h_cf_sys_Lep2pt[60][10];
      TH1D *h_cf_sys_Lep2eta[60][10];
      TH1D *h_cf_sys_Lep2phi[60][10];
      TH1D *h_cf_sys_dilep_inv_mass[60][10];
      TH1D *h_cf_sys_Jet1pt[60][10];
      TH1D *h_cf_sys_Jet1eta[60][10];
      TH1D *h_cf_sys_Jet1phi[60][10];
      TH1D *h_cf_sys_Jet2pt[60][10];
      TH1D *h_cf_sys_Jet2eta[60][10];
      TH1D *h_cf_sys_Jet2phi[60][10];
      TH1D *h_cf_sys_NJets[60][10];
      TH1D *h_cf_sys_metpt[60][10];
      TH1D *h_cf_sys_metphi[60][10];
      TH1D *h_cf_sys_Nbjets[60][10];
      TH1D *h_cf_sys_NPV[60][10];

      TH1D *h_sys_Lep1pt[60][10];
      TH1D *h_sys_Lep2pt[60][10];
      TH1D *h_sys_Lep1eta[60][10];
      TH1D *h_sys_Lep2eta[60][10];
      TH1D *h_sys_Lep1phi[60][10];
      TH1D *h_sys_Lep2phi[60][10];
      TH1D *h_sys_Jet1pt[60][10];
      TH1D *h_sys_Jet2pt[60][10];
      TH1D *h_sys_Jet1eta[60][10];
      TH1D *h_sys_Jet2eta[60][10];
      TH1D *h_sys_Jet1phi[60][10];
      TH1D *h_sys_Jet2phi[60][10];
      TH1D *h_sys_HT[60][10]; 
      TH1D *h_sys_METpt[60][10]; 
      TH1D *h_sys_METphi[60][10]; 
      TH1D *h_sys_DiLepMass[60][10]; 
      TH1D *h_sys_Num_PV[60][10];
      TH1D *h_sys_Num_Jets[60][10];
      TH1D *h_sys_Num_bJets[60][10];
      TH1D *h_sys_Muonpt[60][10];
      TH1D *h_sys_Elecpt[60][10];
      TH1D *h_sys_Muoneta[60][10];
      TH1D *h_sys_Eleceta[60][10];
      TH1D *h_sys_Muonphi[60][10];
      TH1D *h_sys_Elecphi[60][10];

      TH1D *h_sys_Top1Mass_[60]; 
      TH1D *h_sys_Top1pt_[60]; 
      TH1D *h_sys_Top1Rapidity_[60]; 
      TH1D *h_sys_Top1phi_[60]; 
      TH1D *h_sys_Top1Energy_[60]; 

      TH1D *h_sys_Top2Mass_[60]; 
      TH1D *h_sys_Top2pt_[60]; 
      TH1D *h_sys_Top2Rapidity_[60]; 
      TH1D *h_sys_Top2phi_[60]; 
      TH1D *h_sys_Top2Energy_[60]; 

      TH1D *h_sys_TopMass_[60]; 
      TH1D *h_sys_Toppt_[60]; 
      TH1D *h_sys_TopRapidity_[60]; 
      TH1D *h_sys_Topphi_[60]; 
      TH1D *h_sys_TopEnergy_[60]; 

      TH1D *h_sys_AnTopMass_[60];
      TH1D *h_sys_AnToppt_[60]; 
      TH1D *h_sys_AnTopRapidity_[60]; 
      TH1D *h_sys_AnTopphi_[60]; 
      TH1D *h_sys_AnTopEnergy_[60]; 

      TH1D *h_sys_Reco_CPO_[60][13];
      TH1D *h_sys_Reco_CPO_ReRange_[60][13];

      TH1D *h_bTagWeight; 
};
#endif

#ifdef ssb_analysis_cxx

ssb_analysis::ssb_analysis(TTree *tree)
{
   if (tree == 0)
   {
      printf("ERROR: Can't find any input tree.\n");
   }
   Init(tree);
   
   // Text Reader from Jaehoon.
   SSBConfReader = new TextReader();
   SSBConfReader->ReadFile("./configs/13TeV_25ns/dimuon_config13TeV.config");
   //SSBConfReader->ReadFile("./configs/dimuon13TeV_config_2.config");
   SSBConfReader->ReadVariables();
   num_metfilt = SSBConfReader->Size( "METFilters" );
   num_trig = SSBConfReader->Size( "trigger" );

   SSBConfReader->PrintoutVariables();

   CenOfE       = SSBConfReader->GetText( "CenOfEn" ); // 8TeV or 13TeV
   Lumi         = SSBConfReader->GetNumber( "Luminosity" ); // 8TeV or 13TeV
   Decaymode    = SSBConfReader->GetText( "Channel" ); // Channel
   UsingTotEnv  = SSBConfReader->GetText( "UsingTotalEvent" ); // TotalEvent option
   TString RunPeriod = SSBConfReader->GetText( "RunRange" ); 

   /// Luminosity for BCDEF or GH or All ...
   double total_lumi = 0.0;
   std::vector<double> v_lumis;
   v_lumis.clear();
   for (int i = 0; i < SSBConfReader->Size( "Luminosities" ); ++i)
   {
      v_lumis.push_back( SSBConfReader->GetNumber( "Luminosities",i+1 ) );
      total_lumi += SSBConfReader->GetNumber( "Luminosities",i+1 );
   }
   /// Set Luminosity /// 
   if ( RunPeriod.Contains("All") || RunPeriod.Contains("ALL") || RunPeriod.Contains("all") )
   {
      Lumi = total_lumi; // To cover all Run period //
   }
   else if ( RunPeriod.Contains("BCDEF") || RunPeriod.Contains("bcedf") )
   {
      Lumi = SSBConfReader->GetNumber( "Luminosities",1 ) ; // To cover all Run period //
   }  
   else if ( RunPeriod.Contains("GH") || RunPeriod.Contains("gh") )
   {
      Lumi = SSBConfReader->GetNumber( "Luminosities",2 ) ; // To cover all Run period //
   }
   else {
   // To cover all Run period // No Select Lumi, So default is Luminosity vale in the config //
   }

   /////////////////////////////////
   /// METFilter Information !!! ///
   /////////////////////////////////
   for(int i =0; i < num_metfilt; ++i)
   {
      cout << SSBConfReader->GetText("METFilters",i+1) << endl;
      v_METFilterName.push_back( SSBConfReader->GetText("METFilters",i+1) );
   }

   //////////////////////////////
   // Trigger Information !!! ///
   //////////////////////////////

   for(int i =0; i < num_trig; ++i)
   {
      cout << SSBConfReader->GetText("trigger",i+1) << endl;
      trigName.push_back( SSBConfReader->GetText("trigger",i+1) );
   }

   //cout << " JER ??? "<< SSBConfReader->GetBool("DoJER") << endl;
   /// JERSys_
   dojer = SSBConfReader->GetBool("DoJER"); // This is to do jer 

   /////////////////////////
   /// For Systematic !! ///
   /////////////////////////
   TrigSFSys   = SSBConfReader->GetText("Trigsys"); // Getting Jet Energy Systematic Type ...
   MuEnSys     = SSBConfReader->GetText("MuEnsys"); // Getting Muon Energy Systematic Type ...
   EleScSmSys  = SSBConfReader->GetText("EleScSmsys"); // Getting Electron Energy Systematic Type ...
   JetEnSys    = SSBConfReader->GetText("JESsys"); // Getting Jet Energy Systematic Type ...
   JetResSys   = SSBConfReader->GetText("JERsys"); // Getting Jet Smearing Systematic Type ... 

   BTagSFSys    = SSBConfReader->GetText( "BtaggingSFSys"        );
   BTagEffSys   = SSBConfReader->GetText( "BtaggingEffSys"       );

   MetSys    = SSBConfReader->GetText("METsys"); // Getting Jet Energy Systematic Type ...
   LepIdSFSys   = SSBConfReader->GetText( "LepIDSFSys"           );
   LepIsoSFSys  = SSBConfReader->GetText( "LepIsoSFSys"          );
   LepRecoSFSys  = SSBConfReader->GetText( "LepRecoSFSys"        );
   LepTrackSFSys  = SSBConfReader->GetText( "LepTrackSFSys"      );
   PDFSys         = SSBConfReader->GetNumber( "PDFSys"           );
   FactRenoSys    = SSBConfReader->GetNumber( "FactRenoSys"      );
   FragmentSys  = SSBConfReader->GetText( "FragmentSys"          );
   DecayTableSys  = SSBConfReader->GetText( "DecayTableSys"      ); 
   PileUpMCFile = SSBConfReader->GetText( "PileUpMCFileName"     );
   PileUpDATAFile = SSBConfReader->GetText( "PileUpDATAFileName" );
   PileUpDATAFileUp = SSBConfReader->GetText( "PileUpUpFileName" );
   PileUpDATAFileDown = SSBConfReader->GetText( "PileUpDownFileName" );
   PileUpSys    = SSBConfReader->GetText( "PileUpSys"            );
   L1PreFireSys    = SSBConfReader->GetText( "L1PreFireSys"            );
   TopPtSys    = SSBConfReader->GetText(  "TopPtSys"             );

   ///////////////////////////////////////////
   /// Dilution Study of Jet pT Resolution ///
   ///////////////////////////////////////////
   JetPtResDil  = (TString)SSBConfReader->GetText( "JPRDilu" );

   ////////////////////////////////////////////
   /// Dilution Study of Jet Phi Resolution ///
   ////////////////////////////////////////////
   JetPhiResDil = SSBConfReader->GetText( "JPhiRDilu" );
   JetPtPhiDil = true;
   if (( JetPtResDil == "None" || JetPtResDil == "none")
    && ( JetPhiResDil == "None" || JetPhiResDil == "none" ) ) {
   JetPtPhiDil = false;
   }
   ////////////////////////////////////////////
   /// Dilution Study of Jet Phi Resolution ///
   ////////////////////////////////////////////
   bAndbBarDil = SSBConfReader->GetBool( "bAndBDilu" );

   SetConfig();
   SSBEffcal   = new SSBEffCal();
   ssbflsolver = new TtFullLepKinSolver( topmass_begin, topmass_end, topmass_step_, nupars_);
   ssbcpviol   = new SSBCPViol();
   puweight      = new edm::LumiReWeighting("./pileuInfo/" + PileUpMCFile,
                                            "./pileuInfo/" + PileUpDATAFile,
                                            "pileup",                       
                                            "pileup" );

   puweightup      = new edm::LumiReWeighting("./pileuInfo/" + PileUpMCFile,
                                            "./pileuInfo/" + PileUpDATAFileUp,
                                            "pileup",                       
                                            "pileup" );

   puweightdn      = new edm::LumiReWeighting("./pileuInfo/" + PileUpMCFile,
                                            "./pileuInfo/" + PileUpDATAFileDown,
                                            "pileup",                       
                                            "pileup" );

   cutflowName[0] = "Step_0";
   cutflowName[1] = "Step_1" ;
   cutflowName[2] = "Step_2";
   cutflowName[3] = "Step_3";
   cutflowName[4] = "Step_4";
   cutflowName[5] = "bTagged Jet >= 1";
   cutflowName[6] = "bTagged Jet >= 2";
   cutflowName[7] = "bTagged Jet == 2";
   cutflowName[8] = "Top-Recon.";
   cutflowName[9] = "Top-Pt-Rewight";

   cout << "FactRenoSys : " << FactRenoSys << endl;

   pi = TMath::Pi();
   //////////////////////////
   /// For All Systematic ///
   //////////////////////////

   if (SSBConfReader->GetText( "isAllSys" ) == "False" || SSBConfReader->GetText( "isAllSys" ) == "false" ) {isAllSyst = false;}
   else {isAllSyst = true;}

   if (isAllSyst == true){
      for (int i = 0; i < SSBConfReader->Size("Allsys"); ++i)
      {
         cout << SSBConfReader->GetText("Allsys",i+1) << endl;
         TString sys_ = SSBConfReader->GetText("Allsys",i+1);
         TString sysname_ = "";
         v_SystType.push_back(sys_);
         if ( TString(sys_).Contains( "Central" ) ) { sysname_ = "Central"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1; }
         else if ( TString(sys_).Contains( "TrigSF" ) ) 
         {
            sysname_ = "TrigSFUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "TrigSFDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "LepID" ) ) 
         {
            sysname_ = "LepIDUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "LepIDDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "LepIso" ) )
         {
            sysname_ = "LepIsoUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1; 
            sysname_ = "LepIsoDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "PileUp" ) )
         {
            sysname_ = "PileUpUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1; 
            sysname_ = "PileUpDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "L1PreFire" ) )
         {
            sysname_ = "L1PreFireUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1; 
            sysname_ = "L1PreFireDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "JetEn" ) ) 
         {
            sysname_ = "JetEnUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "JetEnDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "BTagSF" ) ) 
         {
            sysname_ = "BTagSFBHadUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagSFBHadDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagSFCHadUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagSFCHadDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagSFLFUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagSFLFDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "BTagEff" ) ) 
         {
            sysname_ = "BTagEffBHadUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagEffBHadDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagEffCHadUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagEffCHadDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagEffLFUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "BTagEffLFDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "LepReco" ) ) 
         {
            sysname_ = "LepRecoUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "LepRecoDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "LepTrack" ) ) 
         {
            sysname_ = "LepTrackUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "LepTrackDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "FactReno" ) ) 
         {
            sysname_ = "FactReno_1"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_2"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_3"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_4"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_5"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_6"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_7"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FactReno_8"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "JetRes" ) ) 
         {
            /// If you didn't select doJer option, we will skip this systematic study. ///
            if ( dojer == true ) {
               sysname_ = "JetResUp";   v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
               sysname_ = "JetResDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            }
         }
         else if ( TString(sys_).Contains( "PDF" ) ) 
         {
            /// If you didn't select doJer option, we will skip this systematic study. ///
            int ipdf = 1;
      
            int ipdfmax = 0; 
            int ipdfmaxset = 20; 
            if ( TString(sys_).Contains( "Set1" ) )      {ipdf = 1; }
            else if ( TString(sys_).Contains( "Set2" ) ) {ipdf = 21;}
            else if ( TString(sys_).Contains( "Set3" ) ) {ipdf = 41;}
            else if ( TString(sys_).Contains( "Set4" ) ) {ipdf = 61;}
            else if ( TString(sys_).Contains( "Set5" ) ) {ipdf = 81;}
            else if ( TString(sys_).Contains( "AlphaS" ) ) {ipdf = 101;ipdfmaxset =2;}
            else {ipdf = 1; ipdfmaxset =1;} 
            ipdfmax = ipdf+ipdfmaxset; 
            for (; ipdf < ipdfmax; ++ipdf ){
               sysname_ = Form("PDF_%d",ipdf); v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            }
         }
         else if ( TString(sys_).Contains( "Fragment" ) ) 
         {
            sysname_ = "FragmentCentral"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FragmentUp"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FragmentDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "FragmentPeterson"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "DecayTable" ) ) 
         {
            sysname_ = "DecayTableUp"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
            sysname_ = "DecayTableDown"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else if ( TString(sys_).Contains( "TopPt" ) ) 
         {
            sysname_ = "TopPtSys"; v_SystFullName.push_back(sysname_); m_Syst_EvtW[sysname_] = 1;
         }
         else {
            v_SystFullName.push_back(sys_); m_Syst_EvtW[sys_] = 1;
         }
      
      }
   }
   idx_jecup = -2; 
   idx_jecdn = -2; 
   idx_jerup = -2; 
   idx_jerdn = -2; 
   for (int i = 0; i < v_SystFullName.size(); ++i) 
   {
     v_SystEvt.push_back(1);// Event weight for systematics //
     if ( v_SystFullName[i] == "JetEnUp" ) {idx_jecup=i;} 
     if ( v_SystFullName[i] == "JetEnDown" ) {idx_jecdn=i;} 
     if ( v_SystFullName[i] == "JetResUp" ) {idx_jerup=i;} 
     if ( v_SystFullName[i] == "JetResDown" ) {idx_jerdn=i;} 
   }
   for (int i =0 ; i< v_SystFullName.size();++i){
      cout << "v_SystFullName ["<<i<<"]: "<< v_SystFullName[i] << endl;
   }
   cout << "JetPtPhiDil : " << JetPtPhiDil << endl;
   //ReadDupleList();
   /// Get Info of Muon Correction ///
   ssbmucor = new RoccoR("./roccor.2016.v3/rcdata.2016.v3");
}

ssb_analysis::~ssb_analysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   delete ssbcpviol;
   delete SSBEffcal ;
   delete fout;
   delete SSBConfReader;
   delete ssbflsolver;
   delete puweight;
   delete puweightup;
   delete puweightdn;
}

#endif