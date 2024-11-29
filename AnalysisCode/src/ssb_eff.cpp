#define ssbgen_analysis_cxx

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "TMath.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"

#include "./../interface/ssb_eff.hpp"
#include "./../CommonTools.hpp"

using namespace std;

SSBEffCal::SSBEffCal()                                                                                                                
{
   SSBEffReader = new TextReader();
   SSBEffReader->ReadFile("./configs/DimuonCorr.txt");
   SSBEffReader->ReadVariables();

   SSBConfigReader = new TextReader();
   SSBConfigReader->ReadFile("./configs/analysis_config.config");
   SSBConfigReader->ReadVariables();
   dimu_eff.clear();
   getEff();

   RunPeriod     = SSBConfigReader->GetText( "RunRange" );

   MuonID_File   = "./lepEff/" + SSBConfigReader->GetText( "MuonIDFile" );
   MuonIso_File  = "./lepEff/" + SSBConfigReader->GetText( "MuonIsoFile" );
   Track_File    = "./lepEff/" + SSBConfigReader->GetText( "TrackEffFile" );  
   ElecID_File   = "./lepEff/" + SSBConfigReader->GetText( "ElectronIDFile" );
   ElecReco_File = "./lepEff/" + SSBConfigReader->GetText( "ElectronRecoFile" );
   Trig_File     = "./lepEff/" + SSBConfigReader->GetText( "TrigSFFile" );
   cout << "Trig_File : " << Trig_File << endl;

   mydummycanvas=new TCanvas();
   /// Get Lepton SF Information ///
   v_h_muid.clear();
   v_h_muiso.clear();
   /// To get Luminosites for each run-period ///
   std::vector<double> v_lumis;
   v_lumis.clear();
   double total_lumi = 0.0;
   for (int i = 0; i < SSBConfigReader->Size( "Luminosities" ); ++i)
   {
      v_lumis.push_back( SSBConfigReader->GetNumber( "Luminosities",i+1 ) );
      total_lumi += SSBConfigReader->GetNumber( "Luminosities",i+1 );
   }
   v_lumi_ratio.clear();
   /// Set Lumi ratio for each Run ///
   for (int i =0; i < v_lumis.size(); ++i) 
   {
      v_lumi_ratio.push_back(v_lumis[i]/total_lumi);
   }
   /// Muon ID  ///
   for (int i =0; i < SSBConfigReader->Size( "MuonIDFiles" ); ++i )
   {
      TString fmuidpath = "./lepEff/" + SSBConfigReader->GetText( "MuonIDFiles",i+1 );
      TFile* fmuid = new TFile(fmuidpath);
      TH2F* h_mu = (TH2F*) fmuid->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
      v_h_muid.push_back(h_mu);
   }
   for (int i =0; i < SSBConfigReader->Size( "MuonIsoFiles" ); ++i )
   {
      TString fmuidpath = "./lepEff/" + SSBConfigReader->GetText( "MuonIsoFiles",i+1 );
      TFile* fmuiso = new TFile(fmuidpath);
      TH2F* h_mu = (TH2F*) fmuiso->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");
      v_h_muiso.push_back(h_mu);
   }
   // Load the file for Eff(Lepton Id Iso, btagging) //
   f_muid    = new TFile(MuonID_File.Data());
   f_muiso   = new TFile(MuonIso_File.Data());
   f_track   = new TFile(Track_File.Data());
   f_eleid   = new TFile(ElecID_File.Data());
   f_eleiso  = new TFile(MuonIso_File.Data());
   f_elereco = new TFile(ElecReco_File.Data());
   f_trg     = new TFile(Trig_File.Data());

   H_muid  = (TH2F*) f_muid->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
   H_muiso = (TH2F*) f_muiso->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");
   H_track = (TGraphAsymmErrors*) f_track->Get("ratio_eff_eta3_dr030e030_corr");
   //H_track = (TH1D*) f_track->Get("ratio_eff_eta3_dr030e030_corr");
   //cout << "H_track -> " << H_track->GetName() << endl;
   H_eleid    = (TH2F*) f_eleid->Get("EGamma_SF2D");
   H_elereco  = (TH2F*) f_elereco->Get("EGamma_SF2D");
   /// Trigger SF /// 
   //H_trig     = (TH2D*) f_trg->Get("h_eff");
   H_trig     = (TH2D*) f_trg->Get("scalefactor_eta2d_with_syst");

   f_btag_loose[0]  = new TFile("./btagInfo/BTagEff_Loose_b.root" );
   f_btag_loose[1]  = new TFile("./btagInfo/BTagEff_Loose_c.root" );
   f_btag_loose[2]  = new TFile("./btagInfo/BTagEff_Loose_udsg.root" );
   f_btag_medium[0] = new TFile("./btagInfo/BTagEff_Medium_b.root");
   f_btag_medium[1] = new TFile("./btagInfo/BTagEff_Medium_c.root");
   f_btag_medium[2] = new TFile("./btagInfo/BTagEff_Medium_udsg.root");
   f_btag_tight[0]  = new TFile("./btagInfo/BTagEff_Tight_b.root" );
   f_btag_tight[1]  = new TFile("./btagInfo/BTagEff_Tight_c.root" );
   f_btag_tight[2]  = new TFile("./btagInfo/BTagEff_Tight_udsg.root" );
   /// Get BTagging SF from scv
   jetbtag      = SSBConfigReader->GetText( "Jet_btag" );
   BTagSFSys    = SSBConfigReader->GetText( "BtaggingSFSys" );
   BTagEffSys   = SSBConfigReader->GetText( "BtaggingEffSys" );
   BTagCSV_File = "./btagInfo/" + SSBConfigReader->GetText( "BTaggingCSVFile" );
   for (int i = 0; i < SSBConfigReader->Size("BTaggingCSVFiles"); ++i)
   {
      BTagCSV_File = "./btagInfo/" + SSBConfigReader->GetText( "BTaggingCSVFiles",i+1 );
      cout << "BTagCSV_File : " << BTagCSV_File << endl;
      calib       = new BTagCalibration("csvv2", BTagCSV_File);
      if      ( TString(jetbtag).Contains( "pfCSVV2L" ) )
      { 
         reader       = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "mujets", BTagSFSys.Data()); 
         readerup     = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "mujets", "up" );
         readerdown   = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "mujets", "down");
         readerlf     = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "incl", BTagSFSys.Data()); 
         readerlfup   = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "incl", "up"); 
         readerlfdown = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "incl", "down"); 
      }
      else if ( TString(jetbtag).Contains( "pfCSVV2M" ) )
      {
         reader       = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", BTagSFSys.Data()); 
         readerup     = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "up" );
         readerdown   = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "down");
         readerlf     = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl", BTagSFSys.Data()); 
         readerlfup   = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl", "up"); 
         readerlfdown = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl", "down"); 
      }
      else if ( TString(jetbtag).Contains( "pfCSVV2T" ) )
      {
         reader       = new BTagCalibrationReader(calib, BTagEntry::OP_TIGHT , "mujets", BTagSFSys.Data()); 
         readerup     = new BTagCalibrationReader(calib, BTagEntry::OP_TIGHT, "mujets", "up" );
         readerdown   = new BTagCalibrationReader(calib, BTagEntry::OP_TIGHT, "mujets", "down");
         readerlf     = new BTagCalibrationReader(calib, BTagEntry::OP_TIGHT, "incl", BTagSFSys.Data()); 
         readerlfup   = new BTagCalibrationReader(calib, BTagEntry::OP_TIGHT, "incl", "up"); 
         readerlfdown = new BTagCalibrationReader(calib, BTagEntry::OP_TIGHT, "incl", "down"); 
      }
      else {
         cout << "Couldn't Apply BTagging SF . Defalut is Loose" << endl;
         reader       = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "mujets", BTagSFSys.Data() );
         readerup     = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "mujets", "up" );
         readerdown   = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "mujets", "down");
         readerlf     = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "incl", BTagSFSys.Data()); 
         readerlfup   = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "incl", "up"); 
         readerlfdown = new BTagCalibrationReader(calib, BTagEntry::OP_LOOSE, "incl", "down"); 
      }
      v_reader.push_back(reader);
      v_readerup.push_back(readerup);
      v_readerdown.push_back(readerdown);
      v_readerlf.push_back(readerlf);
      v_readerlfup.push_back(readerlfup);
      v_readerlfdown.push_back(readerlfdown);
   }
   if      ( TString(jetbtag).Contains( "pfCSVV2L" )  ){ 
      H_btageff_fl[0]  = (TH2D*) f_btag_loose[0]->Get("BTagEff_Loose_b"); 
      H_btageff_fl[1]  = (TH2D*) f_btag_loose[1]->Get("BTagEff_Loose_c"); 
      H_btageff_fl[2]  = (TH2D*) f_btag_loose[2]->Get("BTagEff_Loose_udsg");
   }
   else if ( TString(jetbtag).Contains( "pfCSVV2M" )  ){
      H_btageff_fl[0]  = (TH2D*) f_btag_medium[0]->Get("BTagEff_Medium_b"); 
      H_btageff_fl[1]  = (TH2D*) f_btag_medium[1]->Get("BTagEff_Medium_c"); 
      H_btageff_fl[2]  = (TH2D*) f_btag_medium[2]->Get("BTagEff_Medium_udsg"); 
      cout << "H_btageff_fl ? " << H_btageff_fl[0]->GetName() << endl; 
   }
   else if ( TString(jetbtag).Contains( "pfCSVV2T" )  ){ 
      H_btageff_fl[0]  = (TH2D*) f_btag_tight[0]->Get("BTagEff_Tight_b"); 
      H_btageff_fl[1]  = (TH2D*) f_btag_tight[1]->Get("BTagEff_Tight_c"); 
      H_btageff_fl[2]  = (TH2D*) f_btag_tight[2]->Get("BTagEff_Tight_udsg"); 
   }
   else {cout << "Check the btag !!!! " << endl;}

}                                                                                                                                       

SSBEffCal::~SSBEffCal()
{
   delete f_muid;
   delete f_muiso; 
   delete f_track;
   delete f_eleid;
   delete f_eleiso;
   delete f_elereco;
   delete f_trg;
   delete reader; 
   delete readerup;
   delete readerdown;
   delete readerlf; 
   delete calib;
}   

void SSBEffCal::getEff()
{
   dimu_eff.clear();

   for (int i =0; i < 10; ++i)
   {
      dimu_eff.push_back(SSBEffReader->GetNumber(Form("DoubleMuonCorr_%d",i)) );
   }
}
//double SSBEffCal::DoubleMuon_Eff( double mu1eta, double mu2eta ) 
double SSBEffCal::DoubleMuon_Eff( TLorentzVector* lep1, TLorentzVector* lep2 ) 
{ 
   
   eff   = 1.0;
   m1pt  = -999;
   m2pt  = -999;
   m1eta = -999;
   m2eta = -999;


   m1pt  = lep1->Pt();
   m2pt  = lep2->Pt();
   m1eta = lep1->Eta();
   m2eta = lep2->Eta();

   if ( m1pt < 500.0 && m2pt < 500.0 )
   {
      if ( ( fabs(m1eta) > 0.0 && fabs( m1eta ) < 0.9 && (fabs( m2eta ) > 2.1 && fabs( m2eta ) < 2.4 ) ) ||
           ( fabs(m2eta) > 0.0 && fabs( m2eta ) < 0.9 && (fabs( m1eta ) > 2.1 && fabs( m1eta ) < 2.4 ) )    )// case 1 
      {
         eff = dimu_eff.at(0);
      }
      else if ( ( fabs(m1eta) > 2.1 && fabs( m1eta ) < 2.4 && (fabs( m2eta ) > 1.2 && fabs( m2eta ) < 2.1 ) ) ||
                ( fabs(m2eta) > 2.1 && fabs( m2eta ) < 2.4 && (fabs( m1eta ) > 1.2 && fabs( m1eta ) < 2.1 ) )    ) // case 2
      {
         eff = dimu_eff.at(1);
      }
      else if ( ( fabs(m1eta) > 0.0 && fabs( m1eta ) < 0.9 && (fabs( m2eta ) > 0.0 && fabs( m2eta ) < 0.9 ) ) ||
                ( fabs(m2eta) > 0.0 && fabs( m2eta ) < 0.9 && (fabs( m1eta ) > 0.0 && fabs( m1eta ) < 0.9 ) )    ) //case 3
      {
         eff = dimu_eff.at(2);
      }
      else if ( ( fabs(m1eta) > 1.2 && fabs( m1eta ) < 2.1 && (fabs( m2eta ) < 1.2 && fabs( m2eta ) < 2.1 ) ) ||
                ( fabs(m2eta) > 1.2 && fabs( m2eta ) < 2.1 && (fabs( m1eta ) < 1.2 && fabs( m1eta ) < 2.1 ) )    ) //case 4
      {
         eff = dimu_eff.at(3);
      }
      else if ( ( fabs(m1eta) > 0.9 && fabs( m1eta ) < 1.2 && (fabs( m2eta ) < 0.0 && fabs( m2eta ) < 0.9 ) ) ||
                ( fabs(m2eta) > 0.9 && fabs( m2eta ) < 1.2 && (fabs( m1eta ) < 0.0 && fabs( m1eta ) < 0.9 ) )    ) //case 5
      {
         eff = dimu_eff.at(4);
      }
      else if ( ( fabs(m1eta) > 1.2 && fabs( m1eta ) < 2.1 && (fabs( m2eta ) < 0.9 && fabs( m2eta ) < 1.2 ) ) ||
                ( fabs(m2eta) > 1.2 && fabs( m2eta ) < 2.1 && (fabs( m1eta ) < 0.9 && fabs( m1eta ) < 1.2 ) )    ) //case 6
      {
         eff = dimu_eff.at(5);
      }
      else if ( ( fabs(m1eta) > 2.1 && fabs( m1eta ) < 2.4 && (fabs( m2eta ) < 0.9 && fabs( m2eta ) < 1.2 ) ) ||
                ( fabs(m2eta) > 2.1 && fabs( m2eta ) < 2.4 && (fabs( m1eta ) < 0.9 && fabs( m1eta ) < 1.2 ) )    ) //case 7
      {
         eff = dimu_eff.at(6);
      }
      else if ( ( fabs(m1eta) > 0.9 && fabs( m1eta ) < 1.2 && (fabs( m2eta ) < 0.9 && fabs( m2eta ) < 1.2 ) ) ||
                ( fabs(m2eta) > 0.9 && fabs( m2eta ) < 1.2 && (fabs( m1eta ) < 0.9 && fabs( m1eta ) < 1.2 ) )    ) //case 8
      {
         eff = dimu_eff.at(7);
      }
      else if ( ( fabs(m1eta) > 1.2 && fabs( m1eta ) < 2.1 && (fabs( m2eta ) < 0.0 && fabs( m2eta ) < 0.9 ) ) ||
                ( fabs(m2eta) > 1.2 && fabs( m2eta ) < 2.1 && (fabs( m1eta ) < 0.0 && fabs( m1eta ) < 0.9 ) )    ) //case 9
      {
         eff = dimu_eff.at(8);
      }
      else if ( ( fabs(m1eta) > 2.1 && fabs( m1eta ) < 2.4 && (fabs( m2eta ) < 2.1 && fabs( m2eta ) < 2.4 ) ) ||
                ( fabs(m2eta) > 2.1 && fabs( m2eta ) < 2.4 && (fabs( m1eta ) < 2.1 && fabs( m1eta ) < 2.4 ) )    ) //case 10
      {
         eff = dimu_eff.at(9);
      }
      else { eff = 1.0; }
   }
   else {eff = 1.0;}
   return eff;
}

double SSBEffCal::DoubleMuon_EffROOT( TLorentzVector* lep1, TLorentzVector* lep2, TString muidsys, TString muisosys, TString tracksys )
{
   double mu1id;
   double mu2id;
   double mu1iso;
   double mu2iso;
   double mu1trk;
   double mu2trk;
   double douleMueff;
   double mu1pt = 0;
   double mu2pt = 0;
   double mu1idsys = 0.0;
   double mu2idsys = 0.0;
   double mu1isosys = 0.0;
   double mu2isosys = 0.0;
   double mu1trksys = 0.0;
   double mu2trksys = 0.0;

   double a_mu1id[v_h_muid.size()];
   double a_mu2id[v_h_muid.size()];
   double a_mu1iso[v_h_muiso.size()];
   double a_mu2iso[v_h_muiso.size()];

   double a_mu1idsys[v_h_muid.size()];
   double a_mu2idsys[v_h_muid.size()];
   double a_mu1isosys[v_h_muiso.size()];
   double a_mu2isosys[v_h_muiso.size()];
   ///////////////////////////////////////
   /// Initialize array for muonId sf. ///
   ///////////////////////////////////////
   for (int i = 0; i < v_h_muid.size(); ++i)
   {
      a_mu1id[i] =0;
      a_mu2id[i] =0;
      a_mu1idsys[i] =0;
      a_mu2idsys[i] =0;
   }
   ////////////////////////////////////////
   /// Initialize array for muonIso sf. ///
   ////////////////////////////////////////
   for (int i = 0; i < v_h_muiso.size(); ++i)
   {
      a_mu1iso[i] =0;
      a_mu2iso[i] =0;
      a_mu1isosys[i] =0;
      a_mu2isosys[i] =0;
   }


   mu1pt = lep1->Pt();
   mu2pt = lep2->Pt();
   if ( lep1->Pt() < 120  ) {  mu1pt = lep1->Pt();  } else { mu1pt = 119.999;}
   if ( lep2->Pt() < 120  ) {  mu2pt = lep2->Pt();  } else { mu2pt = 119.999;}

   /// Muon Id for All Run Range ///
   for (int i = 0; i < v_h_muid.size(); ++i)
   {
      a_mu1id[i] = v_h_muid[i]->GetBinContent( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) );// + a_mu1idsys[i]; 
      a_mu2id[i] = v_h_muid[i]->GetBinContent( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) );// + a_mu2idsys[i];
   }

   /// Muon Iso for All Run Range ///
   for (int i = 0; i < v_h_muiso.size(); ++i)
   {
      a_mu1iso[i] = v_h_muiso[i]->GetBinContent( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) );// + a_mu1isosys[i]; 
      a_mu2iso[i] = v_h_muiso[i]->GetBinContent( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) );// + a_mu2isosys[i];
   }


   // Getting Systematic Parameter... about MuonID
   if ( TString(muidsys).Contains("cen") || TString(muidsys).Contains("Cen") ) { mu1idsys = 0.0; mu1idsys = 0.0;
      for (int i = 0; i < v_h_muid.size(); ++i)
      {
         a_mu1idsys[i] = 0.0;
         a_mu2idsys[i] = 0.0;
      }
   }
   else if ( TString(muidsys).Contains("up") || TString(muidsys).Contains("Up") || TString(muidsys).Contains("UP") ) 
   {  
//      mu1idsys = H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) );
//      mu2idsys = H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) );
      // For B to G 
      for (int i = 0; i < v_h_muid.size(); ++i)
      {
//         a_mu1idsys[i] = v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) );
//         a_mu2idsys[i] = v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) );
         a_mu1idsys[i] = sqrt(
                               pow( 0.01*a_mu1id[i],2) + 
                               pow(v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) ),2)
                             );
         a_mu2idsys[i] = sqrt(
                               pow( 0.01*a_mu2id[i],2) +
                               pow(v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) ),2)
                             );
      }
   }
   else if ( TString(muidsys).Contains("down") || TString(muidsys).Contains("Down") || TString(muidsys).Contains("DOWN") )
   {
//      mu1idsys = -1*(H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) ) );
//      mu2idsys = -1*(H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) ) );
      // For B to G 
      for (int i = 0; i < v_h_muid.size(); ++i)
      {
         a_mu1idsys[i] = -1*sqrt( 
                                  pow(0.01*a_mu1id[i],2) + 
                                  pow((v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) )),2) 
                                );
         a_mu2idsys[i] = -1*sqrt(
                                  pow(0.01*a_mu2id[i],2) + 
                                  pow(v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) ),2) 
                                );
      }
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
//      mu1idsys = 0.0;
//      mu2idsys = 0.0;
      // For B to G 
      for (int i = 0; i < v_h_muid.size(); ++i)
      {
         a_mu1idsys[i] = 0.0;
         a_mu2idsys[i] = 0.0;
      }
   } 
 
   // Getting Systematic Parameter... about MuonIso
   if ( TString(muisosys).Contains("cen") || TString(muisosys).Contains("Cen") ) { mu1isosys = 0.0; mu1isosys = 0.0; }
   else if ( TString(muisosys).Contains("up") || TString(muisosys).Contains("Up") || TString(muisosys).Contains("UP") ) 
   {  
//      mu1isosys = H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) );
//      mu2isosys = H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) );
      // For B to G 
      for (int i = 0; i < v_h_muiso.size(); ++i)
      {
//         a_mu1isosys[i] = v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) );
//         a_mu2isosys[i] = v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) );
         a_mu1isosys[i] = sqrt(
                              pow(0.005*a_mu1iso[i],2) + 
                              pow( v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) ),2)
                              );
         a_mu2isosys[i] = sqrt( 
                              pow(0.005*a_mu2iso[i],2) +
                              pow(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) ),2)
                              );
      }
   }
   else if ( TString(muisosys).Contains("down") || TString(muisosys).Contains("Down") || TString(muisosys).Contains("DOWN") )
   {

//      mu1isosys = -1*(H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) ) );
//      mu2isosys = -1*(H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) ) );
      // For B to G 
      for (int i = 0; i < v_h_muiso.size(); ++i)
      {
//         a_mu1isosys[i] = -1*(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) ));
//         a_mu2isosys[i] = -1*(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) ));
         a_mu1isosys[i] = -1*sqrt(
                                  pow(0.005*a_mu1iso[i],2)
                                + pow(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) ),2));
         a_mu2isosys[i] = -1*sqrt(
                                  pow(0.005*a_mu2iso[i],2) 
                                + pow(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) ),2));
      }
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
//      mu1isosys = 0.0;
//      mu2isosys = 0.0;
      // For B to G 
      for (int i = 0; i < v_h_muiso.size(); ++i)
      {
         a_mu1isosys[i] = 0.0;
         a_mu2isosys[i] = 0.0;
      }
   } 
   // Getting Systematic Parameter... about Track
   if ( TString(tracksys).Contains("cen") || TString(tracksys).Contains("Cen") ) { mu1isosys = 0.0; mu1isosys = 0.0; }
   else if ( TString(tracksys).Contains("up") || TString(tracksys).Contains("Up") || TString(tracksys).Contains("UP") ) 
   {  
      mu1trksys = TrackSFErr(lep1->Eta(),tracksys); 
      mu2trksys = TrackSFErr(lep2->Eta(),tracksys); 
   }
   else if ( TString(tracksys).Contains("down") || TString(tracksys).Contains("Down") || TString(tracksys).Contains("DOWN") )
   {
      mu1trksys = -1*TrackSFErr(lep1->Eta(),tracksys); 
      mu2trksys = -1*TrackSFErr(lep2->Eta(),tracksys); 
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      mu1trksys = 0.0;
      mu2trksys = 0.0;
   } 
   /// For Normal method ///
/*   mu1id = H_muid->GetBinContent( H_muid->GetXaxis()->FindBin( mu1pt ),H_muid->GetYaxis()->FindBin( abs(lep1->Eta() )) ) + mu1idsys;
   mu1iso = H_muiso->GetBinContent( H_muiso->GetXaxis()->FindBin( mu1pt ),H_muiso->GetYaxis()->FindBin( abs(lep1->Eta() )) ) + mu1isosys;
   mu2id = H_muid->GetBinContent( H_muid->GetXaxis()->FindBin( mu2pt ),H_muid->GetYaxis()->FindBin( abs(lep2->Eta() )) ) + mu2idsys;
   mu2iso = H_muiso->GetBinContent( H_muiso->GetXaxis()->FindBin( mu2pt ),H_muiso->GetYaxis()->FindBin( abs(lep2->Eta() )) ) + mu2isosys;*/
   mu1trk = TrackSF(lep1->Eta())+mu1trksys;
   mu2trk = TrackSF(lep2->Eta())+mu2trksys;

//   cout << "Before all run-range mu1id : " << mu1id << " mu1iso : " << mu1iso << " mu2id : " << mu2id << " mu2iso : " << mu2iso << endl;

   /// Muon Id for All Run Range ///
   for (int i = 0; i < v_h_muid.size(); ++i)
   {
      a_mu1id[i] = a_mu1id[i] + a_mu1idsys[i]; 
      a_mu2id[i] = a_mu2id[i] + a_mu2idsys[i];
   }

   /// Muon Iso for All Run Range ///
   for (int i = 0; i < v_h_muiso.size(); ++i)
   {
      a_mu1iso[i] = a_mu1iso[i] + a_mu1isosys[i]; 
      a_mu2iso[i] = a_mu2iso[i] + a_mu2isosys[i];
   }

   /// For All Range ///
   if (RunPeriod.Contains("all") || RunPeriod.Contains("All") ||RunPeriod.Contains("ALL") )
   {
      mu1id = v_lumi_ratio[0]*a_mu1id[0] + v_lumi_ratio[1]*a_mu1id[1];
      mu2id = v_lumi_ratio[0]*a_mu2id[0] + v_lumi_ratio[1]*a_mu2id[1];
      mu1iso = v_lumi_ratio[0]*a_mu1iso[0] + v_lumi_ratio[1]*a_mu1iso[1];
      mu2iso = v_lumi_ratio[0]*a_mu2iso[0] + v_lumi_ratio[1]*a_mu2iso[1];
//        cout << "v_lumi_ratio[0] : " << v_lumi_ratio[0] << " a_mu1id[0]" << a_mu1id[0] 
//             << " v_lumi_ratio[1] : " << v_lumi_ratio[1] << " a_mu1id[1]" << a_mu1id[1] << " mu1id: " << mu1id << endl;
  //    cout << "v_lumi_ratio[0] : " << v_lumi_ratio[0] << " a_mu2id[0]" << a_mu2id[0] 
  //         << " v_lumi_ratio[1] : " << v_lumi_ratio[1] << " a_mu2id[1]" << a_mu2id[1] << endl;
   }
   else if ( RunPeriod.Contains("bcdef") || RunPeriod.Contains("BCDEF") )
   {
      mu1id = a_mu1id[0];
      mu2id = a_mu2id[0];
      mu1iso = a_mu1iso[0];
      mu2iso = a_mu2iso[0];
   }
   else if ( RunPeriod.Contains("gh") || RunPeriod.Contains("GH") )
   {
      mu1id = a_mu1id[1];
      mu2id = a_mu2id[1];
      mu1iso = a_mu1iso[1];
      mu2iso = a_mu2iso[1];
   }
   else {
   }
   douleMueff = mu1id*mu2id*mu1iso*mu2iso*mu1trk*mu2trk;
//   cout << "After all run-range mu1id : " << mu1id << " mu1iso : " << mu1iso << " mu1trk : " << mu1trk << " mu2id : " << mu2id << " mu2iso : " << mu2iso << " mu2trk : " << mu2trk << " doubleMueff : " << douleMueff << endl;
//   cout << "mu1pt : " << mu1pt << " mu1eta : " <<lep1->Eta() << " mu2eta : " << lep2->Eta()  << " mu1id  " << mu1id << " mu1trk : " << mu1trk << " mu2trk : " << mu2trk << endl;
   return douleMueff;

}

double SSBEffCal::DoubleElec_EffROOT( TLorentzVector* lep1, TLorentzVector* lep2 , double ele1sueta, double ele2sueta, TString elecidsys, TString elecrecosys )
{
   double ele1id;
   double ele2id;
   double ele1iso;
   double ele2iso;
   double ele1reco;
   double ele2reco;
   double douleEleff = 1.0;
   double ele1idsys  = 0.0;
   double ele2idsys  = 0.0;
   double ele1isosys = 0.0;
   double ele2isosys = 0.0;
   double ele1recosys  = 0.0;
   double ele2recosys  = 0.0;

   double lep1pt    = 0.0;
   double lep1sueta = 0.0;
   double lep2pt    = 0.0;
   double lep2sueta = 0.0;

   if ( lep1->Pt() < 500 ) { lep1pt = lep1->Pt(); }
   else  { lep1pt = 499.99; }

   if ( lep2->Pt() < 500 ) { lep2pt = lep2->Pt(); }
   else  { lep2pt = 499.99; }

   if (ele1sueta >= 2.5) { lep1sueta = 2.499; }
   else if (ele1sueta <= -2.5){ lep1sueta = -2.499; }
   else { lep1sueta=ele1sueta; }

   if (ele2sueta >= 2.5) {lep2sueta = 2.499;}
   else if (ele2sueta <= -2.5){lep2sueta = -2.499;}
   else {lep2sueta=ele2sueta;}

   // Getting Systematic Parameter...
   if ( TString(elecidsys).Contains("cen") || TString(elecidsys).Contains("Cen") ) { ele1idsys = 0.0; ele2idsys = 0.0; }
   else if ( TString(elecidsys).Contains("up") || TString(elecidsys).Contains("Up") || TString(elecidsys).Contains("UP") ) 
   { 
      ele1idsys = H_eleid->GetBinError( H_eleid->GetXaxis()->FindBin( lep1sueta ),H_eleid->GetYaxis()->FindBin( lep1pt) );
      ele2idsys = H_eleid->GetBinError( H_eleid->GetXaxis()->FindBin( lep2sueta ),H_eleid->GetYaxis()->FindBin( lep2pt) );
   }
   else if ( TString(elecidsys).Contains("down") || TString(elecidsys).Contains("Down") || TString(elecidsys).Contains("DOWN") )
   {
      ele1idsys = -1*H_eleid->GetBinError( H_eleid->GetXaxis()->FindBin( lep1sueta ),H_eleid->GetYaxis()->FindBin( lep1pt) );
      ele2idsys = -1*H_eleid->GetBinError( H_eleid->GetXaxis()->FindBin( lep2sueta ),H_eleid->GetYaxis()->FindBin( lep2pt) );
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      ele1idsys = 0.0;
      ele2idsys = 0.0;
   } 
 
   if ( TString(elecrecosys).Contains("cen") || TString(elecrecosys).Contains("Cen") ) { ele1recosys = 0.0; ele2recosys = 0.0; }
   else if ( TString(elecrecosys).Contains("up") || TString(elecrecosys).Contains("Up") || TString(elecrecosys).Contains("UP") ) 
   { 
      ele1recosys = H_elereco->GetBinError( H_elereco->GetXaxis()->FindBin( lep1sueta ),H_elereco->GetYaxis()->FindBin( 50 ) );
      ele2recosys = H_elereco->GetBinError( H_elereco->GetXaxis()->FindBin( lep2sueta ),H_elereco->GetYaxis()->FindBin( 50 ) );
   }
   else if ( TString(elecrecosys).Contains("down") || TString(elecrecosys).Contains("Down") || TString(elecrecosys).Contains("DOWN") )
   {
      ele1recosys = -1*H_elereco->GetBinError( H_elereco->GetXaxis()->FindBin( lep1sueta ),H_elereco->GetYaxis()->FindBin( 50 ) );
      ele2recosys = -1*H_elereco->GetBinError( H_elereco->GetXaxis()->FindBin( lep2sueta ),H_elereco->GetYaxis()->FindBin( 50 ) );
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      ele1recosys = 0.0;
      ele2recosys = 0.0;
   } 

   ele1id = H_eleid->GetBinContent( H_eleid->GetXaxis()->FindBin( lep1sueta ),H_eleid->GetYaxis()->FindBin( lep1pt ) ) + ele1idsys;
   ele2id = H_eleid->GetBinContent( H_eleid->GetXaxis()->FindBin( lep2sueta ),H_eleid->GetYaxis()->FindBin( lep2pt ) ) + ele2idsys;
   ele1iso = 1; 
   ele2iso = 1;
   ele1reco = H_elereco->GetBinContent( H_elereco->GetXaxis()->FindBin( lep1sueta ),H_elereco->GetYaxis()->FindBin( 50 ) ) + ele1recosys;
   ele2reco = H_elereco->GetBinContent( H_elereco->GetXaxis()->FindBin( lep2sueta ),H_elereco->GetYaxis()->FindBin( 50 ) ) + ele2recosys;
   douleEleff = ele1id*ele2id*ele1iso*ele2iso*ele1reco*ele2reco;
   //cout << "douleEleff : " << douleEleff << " douleEleff wo recosf : " << ele1id*ele2id*ele1iso*ele2iso << endl;

   //cout << "Ele1 pt : " << lep1pt << " Ele1 superEta : " << lep1sueta << " ele1id : " << ele1id << " ele1reco: " << ele1reco << " ele1idsys : " << ele1idsys << endl;
   //cout << "Ele2 pt : " << lep2pt << " Ele2 superEta : " << lep2sueta << " ele2id : " << ele2id << " ele2reco: " << ele2reco << " ele2idsys : " << ele2idsys << endl;
   //cout << "douleEleff : " << douleEleff << endl;

   return douleEleff;
}
double SSBEffCal::MuonElec_EffROOT( TLorentzVector* muon, TLorentzVector* elec, double elecsueta, TString lepidsys, TString lepisosys, TString muontrksys, TString elecrecosys )
{
   double MuEleff;

   double muid;
   double muiso;
   double mutrk;
   double muidsys = 0.0;
   double muisosys = 0.0;
   double mutrksys = 0.0;

   double eleid;
   double eleiso;
   double elereco;
   double eleidsys  = 0.0;
   double eleisosys = 0.0;
   double elerecosys = 0.0;

   double elept    = 0.0;
   double elesueta = 0.0;

   double mupt    = 0.0;

   /// To execute all run-range
   double a_muid[v_h_muid.size()];
   double a_muiso[v_h_muiso.size()];
   double a_muidsys[v_h_muid.size()];
   double a_muisosys[v_h_muiso.size()];

   ///////////////////////////////////////
   /// Initialize array for muonId sf. ///
   ///////////////////////////////////////
   for (int i = 0; i < v_h_muid.size(); ++i)
   {
      a_muid[i] =0;
      a_muidsys[i] =0;
   }
   ////////////////////////////////////////
   /// Initialize array for muonIso sf. ///
   ////////////////////////////////////////
   for (int i = 0; i < v_h_muiso.size(); ++i)
   {
      a_muiso[i] =0;
      a_muisosys[i] =0;
   }


   if (elec->Pt()<500){elept = elec->Pt();}
   else  { elept = 499.99; }

   if ( elecsueta >= 2.5 ) { elesueta = 2.499; }
   else if (elecsueta <= -2.5 ){ elesueta = -2.499;}   
   else {elesueta=elecsueta;}
   if ( fabs(elecsueta) >= 2.5)cout << elecsueta << endl;
   if ( muon->Pt() < 120 ) { mupt = muon->Pt();} 
   else { mupt = 119.99;}


   /// Getting Central Value ... ///
   muid = H_muid->GetBinContent( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) );// + muidsys;
   muiso = H_muiso->GetBinContent( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) );// + muisosys;
   mutrk = TrackSF(muon->Eta());//+mutrksys;
   eleid = H_eleid->GetBinContent( H_eleid->GetXaxis()->FindBin( elesueta ),H_eleid->GetYaxis()->FindBin( elept ) );// + eleidsys;
   elereco = H_elereco->GetBinContent( H_elereco->GetXaxis()->FindBin( elesueta ),H_elereco->GetYaxis()->FindBin( 50 ) );// + elerecosys;
   eleiso = 1;

   /// Muon Id for All Run Range ///
   for (int i = 0; i < v_h_muid.size(); ++i)
   {
      a_muid[i] = v_h_muid[i]->GetBinContent( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) );// + a_muidsys[i]; 
   }
   /// Muon Iso for All Run Range ///
   for (int i = 0; i < v_h_muiso.size(); ++i)
   {
      a_muiso[i] = v_h_muiso[i]->GetBinContent( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) );// + a_muisosys[i]; 
   }

   // Getting Systematic Parameter...
   if ( TString(lepidsys).Contains("cen") || TString(lepidsys).Contains("Cen") ) { muidsys = 0.0; eleidsys = 0.0; }
   else if ( TString(lepidsys).Contains("up") || TString(lepidsys).Contains("Up") || TString(lepidsys).Contains("UP") ) 
   { 
      muidsys = sqrt(
                      pow(0.01*muid,2) + 
                      pow( H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) ) ,2) 
                    );
      eleidsys = H_eleid->GetBinError( H_eleid->GetXaxis()->FindBin( elesueta ),H_eleid->GetYaxis()->FindBin( elept ) );
      for (int i = 0; i < v_h_muid.size(); ++i)
      {
         a_muidsys[i] = sqrt( 
                              pow(0.01*a_muid[i],2) + 
                              pow( v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) ) ,2) 
                            );
      }

   }
   else if ( TString(lepidsys).Contains("down") || TString(lepidsys).Contains("Down") || TString(lepidsys).Contains("DOWN") )
   {
      //muidsys = -1*(H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) ) );
      muidsys = -1*sqrt(pow(0.01*muid,2) 
                      + pow( H_muid->GetBinError( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) ) ,2) );
      eleidsys = -1*H_eleid->GetBinError( H_eleid->GetXaxis()->FindBin( elesueta ),H_eleid->GetYaxis()->FindBin( elept ) );
      for (int i = 0; i < v_h_muid.size(); ++i)
      {
         //a_muidsys[i] = -1*(v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) ));
         a_muidsys[i] = -1*sqrt(
                                pow(0.01*a_muid[i],2) + 
                                pow(v_h_muid[i]->GetBinError( H_muid->GetXaxis()->FindBin( mupt ),H_muid->GetYaxis()->FindBin( abs(muon->Eta() )) ) ,2) 
                               );
      }

   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      muidsys  = 0.0;
      eleidsys = 0.0;
   } 
 
   if ( TString(lepisosys).Contains("cen") || TString(lepisosys).Contains("Cen") ) { muisosys = 0.0; eleisosys = 0.0; }
   else if ( TString(lepisosys).Contains("up") || TString(lepisosys).Contains("Up") || TString(lepisosys).Contains("UP") ) 
   {  
      //muisosys = H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) );
      muisosys = sqrt(pow(0.005*muiso,2) + pow(H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) ),2 ));
      eleisosys = 0.0;
      for (int i = 0; i < v_h_muiso.size(); ++i)
      {
         a_muisosys[i] = sqrt (
                              pow(0.005*a_muiso[i],2) +
                              pow(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) ),2)
                              );
      }
   }
   else if ( TString(lepisosys).Contains("down") || TString(lepisosys).Contains("Down") || TString(lepisosys).Contains("DOWN") )
   {
      //muisosys = -1*(H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) ) );
      muisosys = -1*sqrt(pow(0.005*muiso,2) + pow(H_muiso->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) ),2 ));
      eleisosys = 0.0;
      for (int i = 0; i < v_h_muiso.size(); ++i)
      {
//         a_muisosys[i] = -1*(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) ));
         a_muisosys[i] = -1*sqrt(
                              pow(0.005*a_muiso[i],2) +
                              pow(v_h_muiso[i]->GetBinError( H_muiso->GetXaxis()->FindBin( mupt ),H_muiso->GetYaxis()->FindBin( abs(muon->Eta() )) ),2)
                              );

      }
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      muisosys  = 0.0;
      eleisosys = 0.0;
   } 

   if ( TString(elecrecosys).Contains("cen") || TString(elecrecosys).Contains("Cen") ) { elerecosys = 0.0; }
   else if ( TString(elecrecosys).Contains("up") || TString(elecrecosys).Contains("Up") || TString(elecrecosys).Contains("UP") ) 
   { 
      elerecosys = H_elereco->GetBinError( H_elereco->GetXaxis()->FindBin( elesueta ),H_elereco->GetYaxis()->FindBin( 50) );
      mutrksys = TrackSFErr(muon->Eta(),elecrecosys); 
   }
   else if ( TString(elecrecosys).Contains("down") || TString(elecrecosys).Contains("Down") || TString(elecrecosys).Contains("DOWN") )
   {
      elerecosys = -1*H_elereco->GetBinError( H_elereco->GetXaxis()->FindBin( elesueta ),H_elereco->GetYaxis()->FindBin( 50) );
      mutrksys = -1*TrackSFErr(muon->Eta(),elecrecosys); 
   }
   else 
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      elerecosys = 0.0;
   }
   /// Muon Track systematic ///
   if ( TString(muontrksys).Contains("cen") || TString(muontrksys).Contains("Cen") ) { mutrksys = 0.0; }
   else if ( TString(muontrksys).Contains("up") || TString(muontrksys).Contains("Up") || TString(muontrksys).Contains("UP") )
   {
      mutrksys = TrackSFErr(muon->Eta(),muontrksys);
   }
   else if ( TString(muontrksys).Contains("down") || TString(muontrksys).Contains("Down") || TString(muontrksys).Contains("DOWN") )
   {
      mutrksys = -1*TrackSFErr(muon->Eta(),muontrksys);
   }
   else
   {
      cout << "CheckUp Lepton Systematic Option ... Default is Central ..." << endl;
      mutrksys = 0.0;
   } 

   /// Muon Id for All Run Range ///
   for (int i = 0; i < v_h_muid.size(); ++i)
   {
      a_muid[i] = a_muid[i] + a_muidsys[i]; 
   }
   /// Muon Iso for All Run Range ///
   for (int i = 0; i < v_h_muiso.size(); ++i)
   {
      a_muiso[i] = a_muiso[i] + a_muisosys[i]; 
   }
   /// For All Range ///
   if (RunPeriod.Contains("all") || RunPeriod.Contains("All") ||RunPeriod.Contains("ALL") )
   {
      muid = v_lumi_ratio[0]*a_muid[0] + v_lumi_ratio[1]*a_muid[1];
      muiso = v_lumi_ratio[0]*a_muiso[0] + v_lumi_ratio[1]*a_muiso[1];
   }
   else if ( RunPeriod.Contains("bcdef") || RunPeriod.Contains("BCDEF") )
   {
      muid = a_muid[0];
      muiso = a_muiso[0];
   }
   else if ( RunPeriod.Contains("gh") || RunPeriod.Contains("GH") )
   {
      muid = a_muid[1];
      muiso = a_muiso[1];
   }
   else {
   }

   eleid = eleid + eleidsys;
   elereco = elereco + elerecosys;
   eleiso = 1;

   MuEleff = muid*muiso*eleid*eleiso*elereco*mutrk;

/*   cout << "muid : "<< muid << endl;
   cout << "muiso : "<< muiso << endl;
   cout << "eleid : "<< eleid << endl;
   cout << "eleiso : "<< eleiso << endl;
   cout << "elereco : "<< elereco << endl;
   cout << "muon pt  : " << muon->Pt() << endl;
   cout << "elept : " << elept << endl;
   cout << "MuEleff : " << MuEleff << endl;*/

   return MuEleff;
   
}
double SSBEffCal::Btagging_Eff( TLorentzVector* jet, TString btag, TString btageffsys )
{
   double btag_eff;
   double btag_effsys = 0.0;
   if ( TString(btageffsys).Contains("cen") || TString(btageffsys).Contains("Cen") ) { btageffsys = 0.0;}
   else if ( TString(btageffsys).Contains("up") || TString(btageffsys).Contains("Up") || TString(btageffsys).Contains("UP") ) 
   {  
      btag_effsys = H_btageff->GetBinError( H_btageff->GetXaxis()->FindBin( jet->Pt() ),H_btageff->GetYaxis()->FindBin( jet->Eta() ) );
   }
   else if ( TString(btageffsys).Contains("down") || TString(btageffsys).Contains("Down") || TString(btageffsys).Contains("DOWN") )
   {
      btag_effsys = -1*(H_btageff->GetBinError( H_btageff->GetXaxis()->FindBin( jet->Pt() ),H_btageff->GetYaxis()->FindBin( jet->Eta() ) ) );
   }
   else 
   {
      cout << "CheckUp BJet Tagging Eff. Systematic Option ... Default is Central ..." << endl;
      btag_effsys = 0.0;
   } 

   btag_eff = H_btageff->GetBinContent( H_btageff->GetXaxis()->FindBin( jet->Pt() ), H_btageff->GetYaxis()->FindBin( jet->Eta() ) ) + btag_effsys;
   return btag_eff;
}
double SSBEffCal::Btagging_Eff( double jpt_, double jeta_, TString btag, TString btageffsys , int jet_fl)
{
   double btag_eff;
   double btag_effsys = 0.0;
   if (jet_fl == 5){
   H_btageff = H_btageff_fl[0];
   }
   else if (jet_fl == 4){
   H_btageff = H_btageff_fl[1];
   }
   else{
   H_btageff = H_btageff_fl[2];
   }

   if (jpt_ >= 1000){ jpt_ = 999.9;}
   if ( TString(btageffsys).Contains("cen") || TString(btageffsys).Contains("Cen") ) { btageffsys = 0.0;}
   else if ( TString(btageffsys).Contains("up") || TString(btageffsys).Contains("Up") || TString(btageffsys).Contains("UP") ) 
   {  
      btag_effsys = H_btageff->GetBinError( H_btageff->GetXaxis()->FindBin( jpt_ ),H_btageff->GetYaxis()->FindBin( jeta_ ) );
   }
   else if ( TString(btageffsys).Contains("down") || TString(btageffsys).Contains("Down") || TString(btageffsys).Contains("DOWN") )
   {
      btag_effsys = -1*(H_btageff->GetBinError( H_btageff->GetXaxis()->FindBin( jpt_ ),H_btageff->GetYaxis()->FindBin( jeta_ ) ) );
   }
   else 
   {
      cout << "CheckUp BJet Tagging Eff. Systematic Option ... Default is Central ..." << endl;
      btag_effsys = 0.0;
   } 

   btag_eff = H_btageff->GetBinContent( H_btageff->GetXaxis()->FindBin( jpt_ ), H_btageff->GetYaxis()->FindBin( jeta_ ) ) + btag_effsys;
   //cout << "jpt_ : " << jpt_ << " jeta_ : " << jeta_ << " btag_eff : " << H_btageff->GetBinContent( H_btageff->GetXaxis()->FindBin( jpt_ ), H_btageff->GetYaxis()->FindBin( jeta_ ) ) <<endl;
   return btag_eff;
}
double SSBEffCal::TrigDiMuon_Eff( TLorentzVector* lep1, TLorentzVector* lep2, TString Sys_ )
{
   double TrgSF =1.0;
   double l1 = abs(lep1->Eta());
   double l2 = abs(lep2->Eta());
   if ( l1> 2.4) {l1=2.4;}
   if ( l2> 2.4) {l2=2.4;}

   TrgSF = GetTrgEff(l1,l2,Sys_);
   return TrgSF;
}

double SSBEffCal::TrigDiElec_Eff( TLorentzVector* lep1, TLorentzVector* lep2, TString Sys_ )
{
   double TrgSF =1.0;
   double l1 = abs(lep1->Eta());
   double l2 = abs(lep2->Eta());
   if ( l1> 2.4) {l1=2.4;}
   if ( l2> 2.4) {l2=2.4;}

   TrgSF = GetTrgEff(l1,l2,Sys_);
   return TrgSF;
}
double SSBEffCal::TrigMuElec_Eff( TLorentzVector* lep1, TLorentzVector* lep2, TString Sys_ )
{
   double TrgSF =1.0;
   double l1 = abs(lep1->Eta());
   double l2 = abs(lep2->Eta());
   if ( l1> 2.4) {l1=2.4;}
   if ( l2> 2.4) {l2=2.4;}


   TrgSF = GetTrgEff(l1,l2,Sys_);
   return TrgSF;
}
double SSBEffCal::Btagging_EvenWeight( TLorentzVector* jet, double btagdisc, double btagcut)
{
   double btag_evt_weight_ = 1;
   double bjetpt_ = jet->Pt();
   double JetMaxPt = 999;
   double btagdisc_ = btagdisc;
   double bcsv_sf_;
   double btag_eff_;
   double p_mc = 1;
   double p_data = 1;
   if ( bjetpt_ > JetMaxPt ){ bjetpt_ = JetMaxPt; }
   bcsv_sf_  = reader->eval(BTagEntry::FLAV_B,jet->Eta(),jet->Pt());
   btag_eff_ = Btagging_Eff(jet,jetbtag,BTagEffSys);
   
   if ( btagdisc_ > btagcut ){
      p_mc *= btag_eff_;
      p_data *= (bcsv_sf_*btag_eff_);
   }
   else {
      p_mc *= ( 1-btag_eff_ );
      p_data *= ( 1-(bcsv_sf_*btag_eff_) );
   } 
//   cout << "bcsv_sf_ : " << bcsv_sf_ << " btag_eff_ : " << btag_eff_ << endl;
   btag_evt_weight_ = p_data/p_mc;

   return btag_evt_weight_;
//   return bcsv_sf_;
}
double SSBEffCal::Btagging_EvenWeight( std::vector<double>v_jetpt, std::vector<double> v_jeteta, std::vector<double> v_btagdisc, double btagcut, std::vector<int> v_jetf)
{
   double btag_evt_weight_ = 1;
   double JetMaxPt = 999;
   double bcsv_sf_[2];
   double btag_eff_;
   double p_mc = 1;
   double p_data[2] = {1.,1.};
   BTagEntry::JetFlavor jflavor;
   if ( v_jetpt.size() != v_jeteta.size() || 
        v_jetpt.size() != v_btagdisc.size()  ){ cout << "v_jeteta, v_jetpt and v_btagdisc have different size for each other" << endl; return 1.0;}
   else {


      for ( int i =0; i< v_jetpt.size(); ++i )
      {
         v_btagCal.clear();
         double bjetpt_ = v_jetpt[i];
         // Set Max Pt 
         if ( bjetpt_ > JetMaxPt ){ bjetpt_ = JetMaxPt; }

         if (v_jetf[i] == 5){jflavor = BTagEntry::FLAV_B; reader_ = reader; v_btagCal = v_reader; }
         else if (v_jetf[i] == 4){jflavor = BTagEntry::FLAV_C; reader_ = reader; v_btagCal = v_reader; }
         else { jflavor = BTagEntry::FLAV_UDSG; reader_ = readerlf; v_btagCal = v_readerlf; }

         btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, BTagEffSys,v_jetf[i]);
         
         if ( v_btagdisc[i] > btagcut ){
            p_mc *= btag_eff_;
            p_data[0] *= (btag_eff_*v_btagCal[0]->eval(jflavor,v_jeteta[i],bjetpt_));
            p_data[1] *= (btag_eff_*v_btagCal[1]->eval(jflavor,v_jeteta[i],bjetpt_));
         }
         else {
            p_mc *= ( 1-btag_eff_ );
            p_data[0] *= (1 - (btag_eff_*v_btagCal[0]->eval(jflavor,v_jeteta[i],bjetpt_)));
            p_data[1] *= (1 - (btag_eff_*v_btagCal[1]->eval(jflavor,v_jeteta[i],bjetpt_)));
         } 
      }
      if (RunPeriod.Contains("all") || RunPeriod.Contains("All") ||RunPeriod.Contains("ALL") )
      {
         btag_evt_weight_  = (v_lumi_ratio[0]*p_data[0] + v_lumi_ratio[1]*p_data[1])/p_mc;
      }
      else if ( RunPeriod.Contains("bcdef") || RunPeriod.Contains("BCDEF") )
      {
         btag_evt_weight_  = p_data[0]/p_mc;
      }
      else if ( RunPeriod.Contains("gh") || RunPeriod.Contains("GH") )
      {
         btag_evt_weight_  = p_data[1]/p_mc;
      }
      else {
      }
//      btag_evt_weight_ = p_data/p_mc;
//      cout << " btag_evt_weight_ : " << btag_evt_weight_ << endl;
      return btag_evt_weight_;
   }
//   return bcsv_sf_;
}
double SSBEffCal::Btagging_EvenWeightSys( std::vector<double>v_jetpt, std::vector<double> v_jeteta, std::vector<double> v_btagdisc, double btagcut, std::vector<int> v_jetf, TString sys_)
{
   double btag_evt_weight_ = 1;
   double JetMaxPt = 999;
   double bcsv_sf_[2];
   double btag_eff_;
   double p_mc = 1;
   double p_data[2] = {1.,1.};
   BTagEntry::JetFlavor jflavor;
   if ( v_jetpt.size() != v_jeteta.size() || 
        v_jetpt.size() != v_btagdisc.size()  ){ cout << "v_jeteta, v_jetpt and v_btagdisc have different size for each other" << endl; return 1.0;}
   else {
      for ( int i =0; i< v_jetpt.size(); ++i )
      {
         v_btagCal.clear();
         double bjetpt_ = v_jetpt[i];
         // Set Max Pt 
         if ( bjetpt_ > JetMaxPt ){ bjetpt_ = JetMaxPt; }

         if (TString(sys_).Contains( "BTagSFBHadUp") )
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_readerup; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "BTagSFBHadDown" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_readerdown; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }

         }
         else if (TString(sys_).Contains( "BTagSFCHadUp") )
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_readerup; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "BTagSFCHadDown" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_readerdown; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }

         }
         else if (TString(sys_).Contains( "BTagSFLFUp") )
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlfup; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "BTagSFLFDown" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlfdown; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "BTagEffBHadUp" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "up",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains( "BTagEffBHadDown" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "down",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "BTagEffCHadUp" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "up",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains( "BTagEffCHadDown" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "down",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "BTagEffLFUp" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "up",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains( "BTagEffLFDown" ))
         {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "down",v_jetf[i]); 
            }
         }
         else if (TString(sys_).Contains(  "Central") ) {
            if (v_jetf[i] == 5){
               jflavor = BTagEntry::FLAV_B;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else if (v_jetf[i] == 4){
               jflavor = BTagEntry::FLAV_C;  v_btagCal = v_reader; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
            else { 
               jflavor = BTagEntry::FLAV_UDSG;  v_btagCal = v_readerlf; 
               btag_eff_ = Btagging_Eff(v_jetpt[i], v_jeteta[i], jetbtag, "central",v_jetf[i]); 
            }
         }
         else {cout << "Check you systematic for BTagging SF !! : " << sys_ << endl;}

         
         if ( v_btagdisc[i] > btagcut ){
            p_mc *= btag_eff_;
            p_data[0] *= (btag_eff_*v_btagCal[0]->eval(jflavor,v_jeteta[i],bjetpt_));
            p_data[1] *= (btag_eff_*v_btagCal[1]->eval(jflavor,v_jeteta[i],bjetpt_));
         }
         else {
            p_mc *= ( 1-btag_eff_ );
            p_data[0] *= (1 - (btag_eff_*v_btagCal[0]->eval(jflavor,v_jeteta[i],bjetpt_)));
            p_data[1] *= (1 - (btag_eff_*v_btagCal[1]->eval(jflavor,v_jeteta[i],bjetpt_)));
         } 
      }
      if (RunPeriod.Contains("all") || RunPeriod.Contains("All") ||RunPeriod.Contains("ALL") )
      {
         btag_evt_weight_  = (v_lumi_ratio[0]*p_data[0] + v_lumi_ratio[1]*p_data[1])/p_mc;
      }
      else if ( RunPeriod.Contains("bcdef") || RunPeriod.Contains("BCDEF") )
      {
         btag_evt_weight_  = p_data[0]/p_mc;
      }
      else if ( RunPeriod.Contains("gh") || RunPeriod.Contains("GH") )
      {
         btag_evt_weight_  = p_data[1]/p_mc;
      }
      else {
      }
      //cout << "btag_evt_weight_ : " << btag_evt_weight_ << endl;
      return btag_evt_weight_;
   }

//   return bcsv_sf_;
}

double SSBEffCal::TrackSF(double eta)
{
   double trksf = 0; double xlow = 0; double xhigh = 0;
   for(Int_t i = 0; i < H_track->GetN(); i++)
   {
      xlow  = H_track->GetX()[i] - H_track->GetErrorXlow(i);
      xhigh = H_track->GetX()[i] + H_track->GetErrorXhigh(i);
      if(xlow <= eta && xhigh > eta) trksf = H_track->GetY()[i];
   }
   return trksf;
}
double SSBEffCal::TrackSFErr(double eta,TString trksys)
{
   double trkerrup = 0;double trkerrdn = 0; double xlow = 0; double xhigh = 0;
   for(Int_t i = 0; i < H_track->GetN(); i++)
   {
      xlow  = H_track->GetX()[i] - H_track->GetErrorXlow(i);
      xhigh = H_track->GetX()[i] + H_track->GetErrorXhigh(i);
      if(xlow <= eta && xhigh > eta) { 
         trkerrup = H_track->GetErrorYhigh(i+1); 
         trkerrdn = H_track->GetErrorYlow(i+1);
         cout << " trkerrup : " << trkerrup << " trkerrdn : " << trkerrdn << endl; 
      }
   }
   if ( trksys == "Central" || trksys == "central" ) { return 0; }
   else if ( trksys == "Up" || trksys == "up" ) { return trkerrup; }
   else if ( trksys == "Down"|| trksys == "down" ) { return trkerrdn; }
   else {cout << "Check Out TrackSystematic !!!!" << endl; return 0; }
}
double SSBEffCal::GetTrgEff(double eta1, double eta2, TString Sys_)
{
   double trgsf = 0;
   double trgsferr = 0;
   int xbin = 0; 
   int ybin = 0;
   xbin     = H_trig->GetXaxis()->FindBin(eta1); 
   ybin     = H_trig->GetYaxis()->FindBin(eta2);
   trgsf    = H_trig->GetBinContent(xbin,ybin);
   if (Sys_== "Central" || Sys_== "central" ) trgsferr = 0;
   else if ( Sys_==  "Up" || Sys_==  "up" ) trgsferr = H_trig->GetBinError(xbin,ybin); 
   else if ( Sys_==  "Down" || Sys_==  "down" ) trgsferr = (-1)*(  H_trig->GetBinError(xbin,ybin) ); 
   //cout << "trgsf : " << trgsf << " trgsferr : " << trgsferr<< endl;
   trgsf = trgsf+trgsferr;
   //cout << "trgsf : " << trgsf << endl;
   return trgsf;
}
