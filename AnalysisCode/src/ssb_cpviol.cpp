#define ssb_cpviol_cxx

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "TMath.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"

#include "./../interface/ssb_cpviol.hpp"
#include "./../CommonTools.hpp"

using namespace std;

SSBCPViol::SSBCPViol()                                                                                                                
{
/*   SSBEffReader = new TextReader();
   SSBEffReader->ReadFile("./configs/.txt");
   SSBEffReader->ReadVariables();
   dimu_eff.clear();*/
   getEff();

}                                                                                                                                       

SSBCPViol::~SSBCPViol()
{  
}

void SSBCPViol::getEff()
{
   cout << "test cp violation class " << endl;
   v_q_tilde.clear();
   v_q_tilde.push_back(0);
   v_q_tilde.push_back(0);
   v_q_tilde.push_back(0);
   v_q_tilde.push_back(13000);

   v_p.clear();
   v_p.push_back(13000);
   v_p.push_back(0);
   v_p.push_back(0);
   v_p.push_back(0);

   v_q.clear(); 
   v_q.push_back(0); 
   v_q.push_back(0); 
   v_q.push_back(0); 
   v_q.push_back(13000); 
}
double SSBCPViol::getO1Vari( TLorentzVector* top, TLorentzVector* antitop , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double O1;
   double reO1;

   O1 = -999;
   reO1 = -999; // return value;

   double E_t,     Px_t,    Py_t,    Pz_t;
   double E_tbar,  Px_tbar, Py_tbar, Pz_tbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Get Bjet Info.
   E_t     = top->Energy(); 
   Px_t    = top->Px(); 
   Py_t    = top->Py(); 
   Pz_t    = top->Pz();

   // Get Bbar_jet Info.
   E_tbar  = antitop->Energy(); 
   Px_tbar = antitop->Px(); 
   Py_tbar = antitop->Py(); 
   Pz_tbar = antitop->Pz();
  
   // Get Lepton_tar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   O1  = getDetM( E_t,    Px_t,    Py_t,    Pz_t,
                  E_tbar, Px_tbar, Py_tbar, Pz_tbar,
                  E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                  E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

   reO1 = O1/(172.5*172.5*172.5*172.5);


   return reO1;

}
double SSBCPViol::getO2Vari( TLorentzVector* top, TLorentzVector* antitop , TLorentzVector* bjet_m, TLorentzVector* bjet_p)
{
   double O2;
   double reO2;

   O2 = -999;
   reO2 = -999; // return value;

   double E_t,     Px_t,    Py_t,    Pz_t;
   double E_tbar,  Px_tbar, Py_tbar, Pz_tbar;
   double E_B,  Px_B, Py_B, Pz_B;
   double E_Bbar,  Px_Bbar, Py_Bbar, Pz_Bbar;

   // Get Bjet Info.
   E_t     = top->Energy(); 
   Px_t    = top->Px(); 
   Py_t    = top->Py(); 
   Pz_t    = top->Pz();

   // Get Bbar_jet Info.
   E_tbar  = antitop->Energy(); 
   Px_tbar = antitop->Px(); 
   Py_tbar = antitop->Py(); 
   Pz_tbar = antitop->Pz();
  
   // Get Lepton_tar Info.
   E_B  = bjet_m->Energy();
   Px_B = bjet_m->Px(); 
   Py_B = bjet_m->Py();
   Pz_B = bjet_m->Pz(); 

   // Get Lepton Info.
   E_Bbar  = bjet_p->Energy();
   Px_Bbar = bjet_p->Px(); 
   Py_Bbar = bjet_p->Py();
   Pz_Bbar = bjet_p->Pz(); 

   O2  = getDetM( E_t,    Px_t,    Py_t,    Pz_t,
                  E_tbar, Px_tbar, Py_tbar, Pz_tbar,
                  E_B, Px_B, Py_B, Pz_B,
                  E_Bbar, Px_Bbar, Py_Bbar, Pz_Bbar  );

   reO2 = O2/(172.5*172.5*172.5*172.5);


   return reO2;


}
double SSBCPViol::getO3Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double O3;
   double reO3;

   O3 = -999;
   reO3 = -999; // return value;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Get Bjet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px();
   Py_b    = bjet_m->Py();
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy();
   Px_bbar = bjet_p->Px();
   Py_bbar = bjet_p->Py();
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px();
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px();
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   O3  = getDetM( E_b,    Px_b,    Py_b,    Pz_b,
                  E_bbar, Px_bbar, Py_bbar, Pz_bbar,
                  E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                  E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

//   if ( ddd == O3 ) { cout << "Something Wrong at getDetM " << endl;}
//   cout << "O3 ?? : " << O3 << endl;
//   cout << "O3 ?? : " << O3/(172.5*172.5*172.5*172.5) << endl;
   reO3 = O3/(172.5*172.5*172.5*172.5);
   if (fabs(reO3)<0.1) {
/*   cout << "----------------------------------------------------------" << endl;
   cout << "E_b : " << E_b << " Px_b : " << Px_b << " Py_b : " << Py_b << " Pz_b: " << Pz_b << endl; 
   cout << "E_bbar : " << E_bbar << " Px_bbar : " << Px_bbar << " Py_bbar : " << Py_bbar << " Pz_bbar: " << Pz_bbar << endl; 
   cout << "E_Lepp : " << E_Lepp << " Px_Lepp : " << Px_Lepp << " Py_Lepp : " << Py_Lepp << " Pz_Lepp: " << Pz_Lepp << endl; 
   cout << "E_Lepm : " << E_Lepm << " Px_Lepm : " << Px_Lepm << " Py_Lepm : " << Py_Lepm << " Pz_Lepm: " << Pz_Lepm << endl;
   cout << "O3 : " << reO3 << endl; */

   }

   return reO3;

}

double SSBCPViol::getO3VariJPRUp( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double O3;
   double reO3;

   O3 = -999;
   reO3 = -999; // return value;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Get Bjet Info.
   TLorentzVector* bjet_m_jetUp = new TLorentzVector();
   TLorentzVector* bjet_p_jetUp = new TLorentzVector();

   double jer1_ = 1+getJPR(bjet_m->Eta(),bjet_m->Pt());
   double jer2_ = 1+getJPR(bjet_p->Eta(),bjet_p->Pt());
//   cout << " jer1_ ? " << jer1_ << endl;
//   cout << " jer2_ ? " << jer2_ << endl;
   bjet_m_jetUp->SetPtEtaPhiE( bjet_m->Pt()*jer1_, bjet_m->Eta(), bjet_m->Phi(), bjet_m->Energy()*jer1_ );
   bjet_p_jetUp->SetPtEtaPhiE( bjet_p->Pt()*jer2_, bjet_p->Eta(), bjet_p->Phi(), bjet_p->Energy()*jer2_ );
   // Check for Kinematic Variables...
/*   cout << "jer1_ ? " << jer1_ << endl;
   cout << " bjet_m pt : " << bjet_m->Pt() << " bjet_m eta : "  << bjet_m->Eta() << " bjet_m phi : " << bjet_m->Phi() << " bjet_m energy : "<< bjet_m->Energy() << endl;
   cout << " bjet_m_jetUp pt : " << bjet_m_jetUp->Pt() << " bjet_m_jetUp eta : "  << bjet_m_jetUp->Eta() << " bjet_m_jetUp phi : " << bjet_m_jetUp->Phi() << " bjet_m_jetUp energy : "<< bjet_m_jetUp->Energy() << endl;
   cout << " bjet_m px : " << bjet_m->Px() << " bjet_m py : "  << bjet_m->Py() << " bjet_m pz : " << bjet_m->Pz() << " bjet_m pt : "<< bjet_m->Pt() << endl;
   cout << " bjet_m_jetUp px : " << bjet_m_jetUp->Px() << " bjet_m_jetUp py : "  << bjet_m_jetUp->Py() << " bjet_m_jetUp pz : " << bjet_m_jetUp->Pz() << " bjet_m_jetUp pt : "<< bjet_m_jetUp->Pt() << endl;
   cout << "-------------------------------------------------------------------------------------------" << endl;
   cout << "jer2_ ? " << jer2_ << endl;
   cout << " bjet_p pt : " << bjet_p->Pt() << " bjet_p eta : "  << bjet_p->Eta() << " bjet_p phi : " << bjet_p->Phi() << " bjet_p energy : "<< bjet_p->Energy() << endl;
   cout << " bjet_p_jetUp pt : " << bjet_p_jetUp->Pt() << " bjet_p_jetUp eta : "  << bjet_p_jetUp->Eta() << " bjet_p_jetUp phi : " << bjet_p_jetUp->Phi() << " bjet_p_jetUp energy : "<< bjet_p_jetUp->Energy() << endl;
   cout << " bjet_p px : " << bjet_p->Px() << " bjet_p py : "  << bjet_p->Py() << " bjet_p pz : " << bjet_p->Pz() << " bjet_p pt : "<< bjet_p->Pt() << endl;
   cout << " bjet_p_jetUp px : " << bjet_p_jetUp->Px() << " bjet_p_jetUp py : "  << bjet_p_jetUp->Py() << " bjet_p_jetUp pz : " << bjet_p_jetUp->Pz() << " bjet_p_jetUp pt : "<< bjet_p_jetUp->Pt() << endl;
   cout << "*******************************************************************************************" << endl;*/

   E_b     = bjet_m_jetUp->Energy(); 
   Px_b    = bjet_m_jetUp->Px(); 
   Py_b    = bjet_m_jetUp->Py(); 
   Pz_b    = bjet_m_jetUp->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p_jetUp->Energy(); 
   Px_bbar = bjet_p_jetUp->Px(); 
   Py_bbar = bjet_p_jetUp->Py(); 
   Pz_bbar = bjet_p_jetUp->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   O3  = getDetM( E_b,    Px_b,    Py_b,    Pz_b,
                  E_bbar, Px_bbar, Py_bbar, Pz_bbar,
                  E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                  E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

//   if ( ddd == O3 ) { cout << "Something Wrong at getDetM " << endl;}

//   cout << "O3 JPRUp ?? : " << O3/(172.5*172.5*172.5*172.5) << endl;
   reO3 = O3/(172.5*172.5*172.5*172.5);
   //if (fabs(reO3)<0.1) {
/*   cout << "----JPR---Up------------------------------------------------------" << endl;
   cout << "jer1 : " << jer1_ << " jer2 : " << jer2_ << endl;
   cout << "E_b : " << E_b << " Px_b : " << Px_b << " Py_b : " << Py_b << " Pz_b: " << Pz_b << endl; 
   cout << "E_bbar : " << E_bbar << " Px_bbar : " << Px_bbar << " Py_bbar : " << Py_bbar << " Pz_bbar: " << Pz_bbar << endl; 
   cout << "E_Lepp : " << E_Lepp << " Px_Lepp : " << Px_Lepp << " Py_Lepp : " << Py_Lepp << " Pz_Lepp: " << Pz_Lepp << endl; 
   cout << "E_Lepm : " << E_Lepm << " Px_Lepm : " << Px_Lepm << " Py_Lepm : " << Py_Lepm << " Pz_Lepm: " << Pz_Lepm << endl; 
   cout << "O3JER : " << reO3 << endl; */

   //}
   return reO3;

}

double SSBCPViol::getO3VariJPRDown( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double O3;
   double reO3;

   O3 = -999;
   reO3 = -999; // return value;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Get Bjet Info.
   TLorentzVector* bjet_m_jetDown = new TLorentzVector();
   TLorentzVector* bjet_p_jetDown = new TLorentzVector();

   double jer1_ = 1-getJPR(bjet_m->Eta(),bjet_m->Pt());
   double jer2_ = 1-getJPR(bjet_p->Eta(),bjet_p->Pt());
//   cout << " jer1_ ? " << jer1_ << endl;
//   cout << " jer2_ ? " << jer2_ << endl;
   bjet_m_jetDown->SetPtEtaPhiE( bjet_m->Pt()*jer1_, bjet_m->Eta(), bjet_m->Phi(), bjet_m->Energy()*jer1_ );
   bjet_p_jetDown->SetPtEtaPhiE( bjet_p->Pt()*jer2_, bjet_p->Eta(), bjet_p->Phi(), bjet_p->Energy()*jer2_ );

   E_b     = bjet_m_jetDown->Energy(); 
   Px_b    = bjet_m_jetDown->Px(); 
   Py_b    = bjet_m_jetDown->Py(); 
   Pz_b    = bjet_m_jetDown->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p_jetDown->Energy(); 
   Px_bbar = bjet_p_jetDown->Px(); 
   Py_bbar = bjet_p_jetDown->Py(); 
   Pz_bbar = bjet_p_jetDown->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   O3  = getDetM( E_b,    Px_b,    Py_b,    Pz_b,
                  E_bbar, Px_bbar, Py_bbar, Pz_bbar,
                  E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                  E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

//   if ( ddd == O3 ) { cout << "Something Wrong at getDetM " << endl;}

//   cout << "O3 JPRDown ?? : " << O3/(172.5*172.5*172.5*172.5) << endl;
   reO3 = O3/(172.5*172.5*172.5*172.5);
   return reO3;

}

double SSBCPViol::getDetM( double a1, double a2, double a3, double a4,
                           double b1, double b2, double b3, double b4,
                           double c1, double c2, double c3, double c4,
                           double d1, double d2, double d3, double d4  )
{

   double X1;
   double X2;
   double X3;
   double X4;

   X1 =  a1*(  b2*( c3*d4 - c4*d3 ) 
             - b3*( c2*d4 - c4*d2 )  
             + b4*( c2*d3 - c3*d2 ) );

   X2 =  a2*(  b1*( c3*d4 - c4*d3 )
             - b3*( c1*d4 - c4*d1 )
             + b4*( c1*d3 - c3*d1 ) );
  
   X3 =  a3*(  b1*( c2*d4 - c4*d2 )
             - b2*( c1*d4 - c4*d1 )
             + b4*( c1*d2 - c2*d1 ) );

   X4 =  a4*(  b1*( c2*d3 - c3*d2 )
             - b2*( c1*d3 - c3*d1 )
             + b3*( c1*d2 - c2*d1 ) );

   return X1 - X2 + X3 - X4;
}

double SSBCPViol::getJPR(double j_eta,double j_pt)
{
   double jet_pt;
   double jet_eta;
   double N_;
   double S_;
   double C_;
   double m_;

   jet_pt = j_pt;
   jet_eta  = j_eta;

//   cout << "Jet Pt  : " << jet_pt << endl;
//   cout << "Jet Eta : " << jet_eta << endl;
   if ( fabs(jet_eta) > 0.0 && fabs(jet_eta) < 0.5 )
   {  
//      cout << "Area 1 " << endl;
//      cout << "Jet Eta : " << jet_eta << endl
      N_ = 1.78636; 
      S_ = 0.29067;
      C_ = 0.00000;
      m_ = 0.45928; 
   }
   else if ( fabs(jet_eta) > 0.5 && fabs( jet_eta ) < 1.0 )
   {
//      cout << "Area 2 " << endl;
//      cout << "Jet Eta : " << jet_eta << endl;
      N_ = 1.91181;
      S_ = 0.32630;
      C_ = 0.00000;
      m_ = 0.41743; 
   }
   else if ( fabs(jet_eta) > 1.0 && fabs( jet_eta ) < 1.5 )
   {
//      cout << "Area 3 " << endl;
//      cout << "Jet Eta : " << jet_eta << endl;
      N_ = 3.65215;
      S_ = 0.33353;
      C_ = 0.00000;
      m_ = 0.44271; 

   }
   else if ( fabs(jet_eta) > 1.5 && fabs( jet_eta ) < 2.0 )
   {
//      cout << "Area 4 " << endl;
//      cout << "Jet Eta : " << jet_eta << endl;
      N_ = 3.52673;
      S_ = 0.45978;
      C_ = 0.00000;
      m_ = 0.23355; 
   }
   else if ( fabs(jet_eta) > 2.0 && fabs( jet_eta ) < 2.5 )
   {
//      cout << "Area 5 " << endl;
//      cout << "Jet Eta : " << jet_eta << endl;
      N_ = 2.05452;
      S_ = 0.42609;
      C_ = 0.00000;
      m_ = 0.19965; 
   }
   else if ( fabs(jet_eta) > 2.5 && fabs( jet_eta ) < 3.0 )
   {
//      cout << "Area 6 " << endl;
//      cout << "Jet Eta : " << jet_eta << endl;
      N_ = -3.38936;
      S_ =  0.98723;
      C_ =  0.00000;
      m_ = -0.06752; 
   }
   else if ( fabs(jet_eta) > 3.0 && fabs( jet_eta ) < 5.0 )
   {
      cout << "Area 7 " << endl;
      cout << "Jet Eta : " << jet_eta << endl;
      N_ = -3.90162;
      S_ =  0.56749;
      C_ =  0.00000;
      m_ =  0.29977; 

   }
   else
   {
      cout << "There is something wrong for Jet eta ... " << endl;
      cout << "Jet Eta : " << jet_eta << endl;
      N_ =  0.0;
      S_ =  0.0;
      C_ =  0.0;
      m_ =  0.0; 
      
   }
/*   cout << "N_ : " << N_ << endl;
   cout << "S_ : " << S_ << endl;
   cout << "C_ : " << C_ << endl;
   cout << "m_ : " << m_ << endl;*/
   double J_Res;
/*   cout << "N_/pt " << N_ /jet_pt << endl;
   cout << "First : " <<  pow(N_/jet_pt,2) << endl;
   cout << "Second : " << pow( S_, 2 )*pow( jet_pt, m_-1 ) << endl;
   cout << "Third  : " <<  pow(C_,2) << endl;*/

   if ( N_ > 0 )       { J_Res = sqrt( pow( N_/jet_pt, 2 ) + pow( S_, 2 )*pow( jet_pt, m_-1 ) + pow(C_,2) );}
   else if ( N_ < 0 )  { J_Res = sqrt( -1*pow(N_/jet_pt,2) + pow( S_, 2 )*pow( jet_pt, m_-1 ) + pow(C_,2) );}
   else { J_Res = 0 ;}

//   cout << "J_Res : " << J_Res << endl;
   return  J_Res;
}
std::vector<double> SSBCPViol::Subtract4Vec( std::vector<double> v_a1, std::vector<double> v_b1 ) 
{
   subtract4Vec = 0;
   v_AB.clear();
   
   for ( int i = 0; i < v_a1.size(); ++i)
   {
      subtract4Vec = v_a1.at(i) - v_b1.at(i);
      v_AB.push_back( subtract4Vec ); 
   }
   return v_AB;  
}

std::vector<double> SSBCPViol::Add4Vec( std::vector<double> v_a1, std::vector<double> v_b1 ) 
{
   double add4Vec;
   v_AB.clear();
   for ( int i = 0; i < v_a1.size(); ++i)
   {
      add4Vec = v_a1.at(i) + v_b1.at(i);
      v_AB.push_back( add4Vec ); 
   }
   return v_AB;  
}
double SSBCPViol::getO4Vari( TLorentzVector* bjet_p, TLorentzVector* bjet_m , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double O4;
   double reO4;

   O4 = -999;
   reO4 = -999; // return value;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Get Bjet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   O4  = getDetM( E_bbar, Px_bbar, Py_bbar, Pz_bbar,
                  E_b,    Px_b,    Py_b,    Pz_b,
                  E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                  E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

   reO4 = O4/(172.5*172.5*172.5*172.5);

   return reO4;

}
double SSBCPViol::getO5Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;

   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );


   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_PLep_p   = Add4Vec( v_lep_p, v_lep_m );

   qp_pm = getDot( v_q_tilde , v_PLep_m );
   epsilon =  getDetM( E_b,          Px_b,         Py_b,         Pz_b,
                       E_bbar,       Px_bbar,      Py_bbar,      Pz_bbar,
                       v_PLep_p[0],  v_PLep_p[1],  v_PLep_p[2],  v_PLep_p[3],
                       v_q_tilde[0], v_q_tilde[1], v_q_tilde[2], v_q_tilde[3]    );
//   cout << "test Ob ? " << qp_pm*epsilon << endl;        
   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5);

}
double SSBCPViol::getO6Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double O6;
   double reO6;

   O6 = -999;
   reO6 = -999; // return value;

   double E_p,       Px_p,     Py_p,      Pz_p;
   double E_bmbbar,  Px_bmbbar, Py_bmbbar, Pz_bmbbar;
   double E_Lepp,    Px_Lepp,  Py_Lepp,   Pz_Lepp;
   double E_Lepm,    Px_Lepm,  Py_Lepm,   Pz_Lepm;

   // Get Bjet Info.
   E_p     = v_p[0]; 
   Px_p    = v_p[1]; 
   Py_p    = v_p[2]; 
   Pz_p    = v_p[3];

   // Get Bbar_jet Info.
   E_bmbbar  = bjet_m->Energy() - bjet_p->Energy(); 
   Px_bmbbar = bjet_m->Px()     - bjet_p->Px(); 
   Py_bmbbar = bjet_m->Py()     - bjet_p->Py(); 
   Pz_bmbbar = bjet_m->Pz()     - bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   O6  = getDetM( E_p,    Px_p,    Py_p,    Pz_p,
                  E_bmbbar, Px_bmbbar, Py_bmbbar, Pz_bmbbar,
                  E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                  E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

   reO6 = O6/(172.5*172.5*172.5*172.5*172.5);

   return reO6;

}
double SSBCPViol::getO7Vari( TLorentzVector* top, TLorentzVector* antitop , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double dot_qtt;
   double det_pqll;
   double O7;
   double reO7;

   std::vector<double>v_top;
   std::vector<double>v_antitop;
   std::vector<double>v_topSubantitop;
   v_top.clear();
   v_antitop.clear();
   v_topSubantitop.clear();

   O7 = 0;
   reO7 = 0; // return value;

   double E_p,       Px_p,     Py_p,      Pz_p;
   double E_q,  Px_q, Py_q, Pz_q;
   double E_Lepp,    Px_Lepp,  Py_Lepp,   Pz_Lepp;
   double E_Lepm,    Px_Lepm,  Py_Lepm,   Pz_Lepm;

   v_top.push_back( top->Energy() );
   v_top.push_back( top->Px() );
   v_top.push_back( top->Py() );
   v_top.push_back( top->Pz() );

   v_antitop.push_back( antitop->Energy() );
   v_antitop.push_back( antitop->Px() );
   v_antitop.push_back( antitop->Py() );
   v_antitop.push_back( antitop->Pz() );
   v_topSubantitop = Subtract4Vec(v_top,v_antitop);

   // Get Bjet Info.
   E_p     = v_p[0]; 
   Px_p    = v_p[1]; 
   Py_p    = v_p[2]; 
   Pz_p    = v_p[3];

   // Get Bbar_jet Info.
   E_q  = v_q_tilde[0];
   Px_q = v_q_tilde[1];
   Py_q = v_q_tilde[2];
   Pz_q = v_q_tilde[3];

   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 
   

   dot_qtt = getDot(v_q_tilde,v_topSubantitop);


   det_pqll  = getDetM( E_p,    Px_p,    Py_p,    Pz_p,
                        E_q,    Px_q,    Py_q,    Pz_q,
                        E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp,
                        E_Lepm, Px_Lepm, Py_Lepm, Pz_Lepm  );

   O7= dot_qtt*det_pqll;
   
   reO7 = O7/(172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5);

   return reO7;

}
double SSBCPViol::getO8Vari( TLorentzVector* top, TLorentzVector* antitop ,TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{
   double dot_qtt;
   double det_qbbl_a;
   double det_qbbl_b;
   double dot_plpqbbl_a;
   double dot_plpqbbl_b;
   double O8;
   double reO8;

   std::vector<double>v_top;
   std::vector<double>v_antitop;
   std::vector<double>v_topSubantitop;
   v_top.clear();
   v_antitop.clear();
   v_topSubantitop.clear();
   v_lep_p.clear();
   v_lep_m.clear();

   O8 = -999;
   reO8 = -999; // return value;

   double E_q,     Px_q,    Py_q,    Pz_q;
   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   v_top.push_back( top->Energy() );
   v_top.push_back( top->Px() );
   v_top.push_back( top->Py() );
   v_top.push_back( top->Pz() );

   v_antitop.push_back( antitop->Energy() );
   v_antitop.push_back( antitop->Px() );
   v_antitop.push_back( antitop->Py() );
   v_antitop.push_back( antitop->Pz() );

   v_topSubantitop = Subtract4Vec(v_top,v_antitop);


   // Get Bbar_jet Info.
   E_q  = v_q_tilde[0];
   Px_q = v_q_tilde[1];
   Py_q = v_q_tilde[2];
   Pz_q = v_q_tilde[3];

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );


   dot_qtt = getDot(v_q_tilde,v_topSubantitop); 


   det_qbbl_a  = getDetM( E_q,     Px_q,    Py_q,    Pz_q,
                          E_b,     Px_b,    Py_b,    Pz_b,
                          E_bbar, Px_bbar, Py_bbar, Pz_bbar,
                          E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm  );

   det_qbbl_b  = getDetM( E_q,    Px_q,    Py_q,    Pz_q,
                          E_b,    Px_b,    Py_b,    Pz_b,
                          E_bbar, Px_bbar, Py_bbar, Pz_bbar,
                          E_Lepp, Px_Lepp, Py_Lepp, Pz_Lepp  );
   
   dot_plpqbbl_a = det_qbbl_a*getDot(v_p,v_lep_p);
   dot_plpqbbl_b = det_qbbl_a*getDot(v_p,v_lep_m);

   O8= dot_qtt*(dot_plpqbbl_a+dot_plpqbbl_b);
   reO8 = O8/(172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5);
   return reO8;

}
double SSBCPViol::getO9Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;
   std::vector<double> v_sumBBbar;
   v_sumBBbar.clear();
   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );


   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_sumBBbar.push_back(E_b+E_bbar);  
   v_sumBBbar.push_back(Px_b+Px_bbar);  
   v_sumBBbar.push_back(Py_b+Py_bbar);  
   v_sumBBbar.push_back(Pz_b+Pz_bbar);  

   qp_pm = getDot( v_q_tilde , v_PLep_m );
   epsilon =  getDetM( v_sumBBbar[0],v_sumBBbar[1],v_sumBBbar[2],v_sumBBbar[3],
                       v_q_tilde[0], v_q_tilde[1], v_q_tilde[2], v_q_tilde[3], 
                       E_Lepp,       Px_Lepp,      Py_Lepp,      Pz_Lepp,
                       E_Lepm,       Px_Lepm,      Py_Lepm,      Pz_Lepm);
//   cout << "test Ob ? " << qp_pm*epsilon << endl;        
   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5);

}
double SSBCPViol::getO10Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;
   std::vector<double> v_sumBBbar;
   std::vector<double> v_subBBbar;
   v_sumBBbar.clear();
   v_subBBbar.clear();
   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );

   v_b_p.push_back( E_bbar  );  v_b_m.push_back( E_b );
   v_b_p.push_back( Px_bbar  ); v_b_m.push_back( Px_b );
   v_b_p.push_back( Py_bbar  ); v_b_m.push_back( Py_b );
   v_b_p.push_back( Pz_bbar  ); v_b_m.push_back( Pz_b );
   
   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_PLep_p = Add4Vec( v_lep_p , v_lep_m );
   v_subBBbar = Subtract4Vec(v_b_m,v_b_p);
   qp_pm = getDot( v_q_tilde , v_subBBbar );
   epsilon =  getDetM( v_b_m[0],v_b_m[1],v_b_m[2],v_b_m[3],
                       v_b_p[0],v_b_p[1],v_b_p[2],v_b_p[3],
                       v_q_tilde[0], v_q_tilde[1], v_q_tilde[2], v_q_tilde[3], 
                       v_PLep_p[0], v_PLep_p[1], v_PLep_p[2], v_PLep_p[3] ); 
//   cout << "test Ob ? " << qp_pm*epsilon << endl;        
   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5);

}
double SSBCPViol::getO11Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;
   std::vector<double> v_sumBBbar;
   std::vector<double> v_subBBbar;
   v_sumBBbar.clear();
   v_subBBbar.clear();
   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );

   v_b_p.push_back( E_bbar  );  v_b_m.push_back( E_b );
   v_b_p.push_back( Px_bbar  ); v_b_m.push_back( Px_b );
   v_b_p.push_back( Py_bbar  ); v_b_m.push_back( Py_b );
   v_b_p.push_back( Pz_bbar  ); v_b_m.push_back( Pz_b );
   
   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_PLep_p = Add4Vec( v_lep_p , v_lep_m );
   v_subBBbar = Subtract4Vec(v_b_m,v_b_p);
   v_sumBBbar = Add4Vec(v_b_m,v_b_p);
   qp_pm = getDot( v_q_tilde , v_subBBbar );
   epsilon =  getDetM( v_p[0],v_p[1],v_p[2],v_p[3],
                       v_q_tilde[0], v_q_tilde[1],  v_q_tilde[2],  v_q_tilde[3], 
                       v_sumBBbar[0],v_sumBBbar[1], v_sumBBbar[2], v_sumBBbar[3], 
                       v_PLep_m[0],  v_PLep_m[1],   v_PLep_m[2],   v_PLep_m[3] ); 
   
   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5);

}
double SSBCPViol::getO12Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;
   std::vector<double> v_sumBBbar;
   std::vector<double> v_subBBbar;
   v_sumBBbar.clear();
   v_subBBbar.clear();
   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );

   v_b_p.push_back( E_bbar  );  v_b_m.push_back( E_b );
   v_b_p.push_back( Px_bbar  ); v_b_m.push_back( Px_b );
   v_b_p.push_back( Py_bbar  ); v_b_m.push_back( Py_b );
   v_b_p.push_back( Pz_bbar  ); v_b_m.push_back( Pz_b );
   
   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_PLep_p = Add4Vec( v_lep_p , v_lep_m );
   v_subBBbar = Subtract4Vec(v_b_m,v_b_p);
   v_sumBBbar = Add4Vec(v_b_m,v_b_p);
   qp_pm = getDot( v_q_tilde , v_subBBbar );
   epsilon =  getDetM( v_p[0],       v_p[1],       v_p[2],       v_p[3],
                       v_q_tilde[0], v_q_tilde[1], v_q_tilde[2], v_q_tilde[3],
                       E_b,          Px_b,         Py_b,         Pz_b,
                       E_bbar,       Px_bbar,      Py_bbar,      Pz_bbar ); 

   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5*172.5);

}
double SSBCPViol::getO13Vari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;
   std::vector<double> v_sumBBbar;
   std::vector<double> v_subBBbar;
   v_sumBBbar.clear();
   v_subBBbar.clear();
   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );

   v_b_p.push_back( E_bbar  );  v_b_m.push_back( E_b );
   v_b_p.push_back( Px_bbar  ); v_b_m.push_back( Px_b );
   v_b_p.push_back( Py_bbar  ); v_b_m.push_back( Py_b );
   v_b_p.push_back( Pz_bbar  ); v_b_m.push_back( Pz_b );
   
   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_PLep_p = Add4Vec( v_lep_p , v_lep_m );
   v_subBBbar = Subtract4Vec(v_b_m,v_b_p);
   v_sumBBbar = Add4Vec(v_b_m,v_b_p);
   qp_pm = getDot( v_q_tilde , v_subBBbar );
   epsilon =  getDetM( v_p[0],        v_p[1],        v_p[2],        v_p[3],
                       v_sumBBbar[0], v_sumBBbar[1], v_sumBBbar[2], v_sumBBbar[3],
                       E_Lepp,        Px_Lepp,       Py_Lepp,       Pz_Lepp,
                       E_Lepm,        Px_Lepm,       Py_Lepm,       Pz_Lepm ); 
//   cout << "test Ob ? " << qp_pm*epsilon << endl;        
   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5);

}

double SSBCPViol::getObVari( TLorentzVector* bjet_m, TLorentzVector* bjet_p , TLorentzVector* lep_p, TLorentzVector* lep_m )
{

   double qp_pm;
   double epsilon;

   double E_b,     Px_b,    Py_b,    Pz_b;
   double E_bbar,  Px_bbar, Py_bbar, Pz_bbar;
   double E_Lepp,  Px_Lepp, Py_Lepp, Pz_Lepp;
   double E_Lepm,  Px_Lepm, Py_Lepm, Pz_Lepm;

   // Initailize variable
   qp_pm = -999;  
   epsilon = -999;

   v_PLep_m.clear();
   v_Pb_p.clear();
   v_lep_p.clear();
   v_lep_m.clear();
   v_b_p.clear();
   v_b_m.clear();

   // Get B_jet Info.
   E_b     = bjet_m->Energy(); 
   Px_b    = bjet_m->Px(); 
   Py_b    = bjet_m->Py(); 
   Pz_b    = bjet_m->Pz();

   // Get Bbar_jet Info.
   E_bbar  = bjet_p->Energy(); 
   Px_bbar = bjet_p->Px(); 
   Py_bbar = bjet_p->Py(); 
   Pz_bbar = bjet_p->Pz();
  
   // Get Lepton_bar Info.
   E_Lepp  = lep_p->Energy();
   Px_Lepp = lep_p->Px(); 
   Py_Lepp = lep_p->Py();
   Pz_Lepp = lep_p->Pz(); 

   // Get Lepton Info.
   E_Lepm  = lep_m->Energy();
   Px_Lepm = lep_m->Px(); 
   Py_Lepm = lep_m->Py();
   Pz_Lepm = lep_m->Pz(); 

   v_lep_p.push_back( E_Lepp  );  v_lep_m.push_back( E_Lepm );
   v_lep_p.push_back( Px_Lepp  ); v_lep_m.push_back( Px_Lepm );
   v_lep_p.push_back( Py_Lepp  ); v_lep_m.push_back( Py_Lepm );
   v_lep_p.push_back( Pz_Lepp  ); v_lep_m.push_back( Pz_Lepm );

   v_b_m.push_back( E_b );  v_b_p.push_back( E_bbar );
   v_b_m.push_back( Px_b ); v_b_p.push_back( Px_bbar );
   v_b_m.push_back( Py_b ); v_b_p.push_back( Py_bbar );
   v_b_m.push_back( Pz_b ); v_b_p.push_back( Pz_bbar );

   v_PLep_m = Subtract4Vec( v_lep_p , v_lep_m );
   v_Pb_p   = Add4Vec( v_b_p, v_b_m );

   qp_pm = getDot( v_q_tilde , v_PLep_m );
   epsilon =  getDetM( E_Lepp,       Px_Lepp,      Py_Lepp,      Pz_Lepp,
                       E_Lepm,       Px_Lepm,      Py_Lepm,      Pz_Lepm,
                       v_Pb_p[0],    v_Pb_p[1],    v_Pb_p[2],    v_Pb_p[3],
                       v_q_tilde[0], v_q_tilde[1], v_q_tilde[2], v_q_tilde[3] );
//   cout << "test Ob ? " << qp_pm*epsilon << endl;        
   return qp_pm*epsilon/(172.5*172.5*172.5*172.5*172.5*172.5*172.5);
}

//double SSBCPViol::getDot( double a1, double a2, double a3, double a4,
//                          double b1, double b2, double b3, double b4  )
double SSBCPViol::getDot( std::vector<double> v_a1, std::vector<double> v_a2 )
{
   double getdot = 0.0;
   for (int i = 0; i < v_a1.size(); ++i ) { getdot += v_a1.at(i)*v_a2.at(i); }
   return getdot;
}
TLorentzVector* SSBCPViol::JetAngEta( TLorentzVector* jet, TString op )
{
//   [0]*TMath::Sqrt(1/([2]*x*x) + [3]/x + [4]) +[1]/(x*x)
   double x = jet->Pt();
   double sign = 1.0;
   double par[5] = {6.08014e-03,7.69603e-01,5.11126e-05,1.25687e+02,1.29715e-01};
   
   double sigma_ = par[0]*TMath::Sqrt(1/(par[2]*x*x) + par[3]/x + par[4]) + par[1]/(x*x);
//   double sigma_ = 1.21637e-02*TMath::Sqrt(1/(5.65455e-05*x*x) + -1.23978e+02/x + 5.21222e-01) +(-2.98353e+01/(x*x));

   TLorentzVector* newjet = new TLorentzVector();

   if (TString(op).Contains("Up") || TString(op).Contains("up") ) { sign = 1.0; }
   else if (TString(op).Contains("Down") || TString(op).Contains("down") ) { sign = -1; }
   else  { sign = 0; }// default //
   newjet->SetPtEtaPhiE(jet->Pt(),jet->Eta()+sign*sigma_,jet->Phi(),jet->Energy());
//   cout << "pt : " << x << " sigma_ : " << sigma_ << endl;
//   cout << "Origin Eta smear : " << jet->Eta() << endl;
//   cout << "New Eta smear : " << newjet->Eta() << endl;
   return newjet;
}
TLorentzVector* SSBCPViol::JetAngPhi( TLorentzVector* jet, TString op )
{
   double x = jet->Pt();
   double sign = 1.0;
   double par[5] = {8.40616e-03,-8.39228e+00,4.87489e-05,1.10994e+02,-9.46558e-02};
//   double sigma_ = 1.21637e-02*TMath::Sqrt(1/(5.65455e-05*x*x) + -1.23978e+02/x + 5.21222e-01) +(-2.98353e+01/(x*x));
//   double sigma_ = 9.67486e-03*TMath::Sqrt(1/(4.34086e-05*x*x) + -4.77261e+01/x + 3.91688e-01) +(-2.53293e+01/(x*x));
   double sigma_ = par[0]*TMath::Sqrt(1/(par[2]*x*x) + par[3]/x + par[4]) + par[1]/(x*x);

   TLorentzVector* newjet = new TLorentzVector();

   if (TString(op).Contains("Up") || TString(op).Contains("up") ) { sign = 1.0; }
   else if (TString(op).Contains("Down") || TString(op).Contains("down") ) { sign = -1; }
   else  { sign = 0; }// default //
   newjet->SetPtEtaPhiE(jet->Pt(),jet->Eta(),jet->Phi() +sign*sigma_,jet->Energy());
//   cout << "pt : " << x << " sigma_ : " << sigma_ << endl;
//   cout << "Origin Phi smear : " << jet->Phi() << endl;
//   cout << "New Phi smear : " << newjet->Phi() << endl;
   return newjet;
}



