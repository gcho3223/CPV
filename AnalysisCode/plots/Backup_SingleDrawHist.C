#include "TSystem.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"

void SingleDrawHist(string version)
{
	void Draw1DHist(TH1D *h_obj, string ttbar_v, string version, string vdtG);
	void Draw2DHist(TH2D *h_obj2D);

	string ttbar_v = "ttbar_v7";

	vector<string> vdtG = {
		"dtG_m2p60364","dtG_m1p0415","dtG_m0p5207",
		"dtG_0",
		"dtG_0p5207","dtG_1p0415","dtG_2p60364"
		};

	vector<TString> hist1D = {
	//gen. lv.
	"_h_deltaR_Genb_Recobjet","_h_deltaR_GenAnb_Recobbarjet",
	"_h_Genb_pT","_h_Genb_Eta","_h_Genb_Phi","_h_GenAnb_pT","_h_GenAnb_Eta","_h_GenAnb_Phi",
	"_h_GenMuon_pT","_h_GenMuon_Eta","_h_GenMuon_Phi","_h_GenAnMuon_pT","_h_GenAnMuon_Eta","_h_GenAnMuon_Phi",
	"_h_pair_Genb_pT_0p1","_h_pair_Genb_Eta_0p1","_h_pair_Genb_Phi_0p1","_h_pair_GenAnb_pT_0p1","_h_pair_GenAnb_Eta_0p1","_h_pair_GenAnb_Phi_0p1",
	"_h_pair_Genb_pT_0p2","_h_pair_Genb_Eta_0p2","_h_pair_Genb_Phi_0p2","_h_pair_GenAnb_pT_0p2","_h_pair_GenAnb_Eta_0p2","_h_pair_GenAnb_Phi_0p2",
	"_h_pair_Genb_pT_0p3","_h_pair_Genb_Eta_0p3","_h_pair_Genb_Phi_0p3","_h_pair_GenAnb_pT_0p3","_h_pair_GenAnb_Eta_0p3","_h_pair_GenAnb_Phi_0p3",
	"_h_pair_Genb_pT_over0p3","_h_pair_Genb_Eta_over0p3","_h_pair_Genb_Phi_over0p3","_h_pair_GenAnb_pT_over0p3","_h_pair_GenAnb_Eta_over0p3","_h_pair_GenAnb_Phi_over0p3",
	"_h_Gen_O3_dR0p1","_h_Gen_O3_dR0p2","_h_Gen_O3_dR0p3","_h_Gen_O3_over0p3",
	// reco lv.
	"_h_Paired_Bjetpt","_h_Paired_Bjeteta","_h_Paired_Bjetphi","_h_Paired_AnBjetpt","_h_Paired_AnBjeteta","_h_Paired_AnBjetphi",
	"_h_pair_bjet_pT_0p1","_h_pair_bjet_Eta_0p1","_h_pair_bjet_Phi_0p1","_h_pair_bbarjet_pT_0p1","_h_pair_bbarjet_Eta_0p1","_h_pair_bbarjet_Phi_0p1",
	"_h_pair_bjet_pT_0p2","_h_pair_bjet_Eta_0p2","_h_pair_bjet_Phi_0p2","_h_pair_bbarjet_pT_0p2","_h_pair_bbarjet_Eta_0p2","_h_pair_bbarjet_Phi_0p2",
	"_h_pair_bjet_pT_0p3","_h_pair_bjet_Eta_0p3","_h_pair_bjet_Phi_0p3","_h_pair_bbarjet_pT_0p3","_h_pair_bbarjet_Eta_0p3","_h_pair_bbarjet_Phi_0p3",
	"_h_pair_bjet_pT_over0p3","_h_pair_bjet_Eta_over0p3","_h_pair_bjet_Phi_over0p3","_h_pair_bbarjet_pT_over0p3","_h_pair_bbarjet_Eta_over0p3","_h_pair_bbarjet_Phi_over0p3",
	"_h_bf_TopReco_O3","_h_Reco_O3_dR0p1","_h_Reco_O3_dR0p2","_h_Reco_O3_dR0p3","_h_Reco_O3_over0p3"
	};
	//vector<TString> hist2D = {"_h_deltaR_b_bbar","_h_deltaR_b_bbar3"};

	cout << hist1D.size() << endl;
	for(int idtg=0; idtg<vdtG.size(); idtg++)
	{
		/// file read ///
		TString ffile = Form("../output/%s/CEDM/%s/TTJets_Signal_%s/TTJets_Signal_%s/TTJets_Signal_%s_All.root",ttbar_v.c_str(),version.c_str(),vdtG[idtg].c_str(),vdtG[idtg].c_str(),vdtG[idtg].c_str());

		TFile *f = new TFile(ffile,"READ");
		if(f==NULL){cout << "Something Wrong!!" << endl;}
		else {cout << ffile.Data() << endl;}
		for(int i1d=0; i1d<hist1D.size(); i1d++)
		{
			TH1D *h_obj = (TH1D*)f->Get(Form("%s",hist1D[i1d].Data()));
			if(h_obj == NULL){cout << "empty histo......." << endl;}
			Draw1DHist(h_obj,ttbar_v,version,vdtG[idtg]);
		}
		//for(int i2d=0; i2d<hist2D.size(); i2d++)
		//{
		//	TH2D *h_obj2D = (TH2D*)f->Get(Form("%s",hist2D[i2d].Data()));
		//	if(h_obj2D == NULL){continue;}
		//	Draw2DHist(h_obj2D);
		//}
	}
}
void Draw1DHist(TH1D *h_obj, string ttbar_v, string version, string vdtG)
{
	void DrawOverflowBin(TH1D *his, double min, double max);
	
	TLegend *leg;
	TLegend *leg_o3;
	double xbin = 0;
	double nevt = 0;

	double dR_frac0p1 = 0;
	double dR_frac0p2 = 0;
	double dR_frac0p3 = 0;
	double dR_fracover3 = 0;

	double posCount = 0.0;
	double negCount = 0.0;
	double Asym = 0.0;
	double statErr = -999;
	double total = 0.0;
	double xbino3 = 0.0;

	TString hname = h_obj->GetName();

	TCanvas *c = new TCanvas("c",hname,800,800);
	c->cd();
	c->SetMargin(0.12,0.12,0.12,0.12);
	h_obj->Draw("hist");
	h_obj->SetTitle("");
	/// pT ///
	//if(hname == "_h_Genbjet_pT" || hname == "_h_GenAnbjet_pT" || hname == "_h_dr_Genbjet_pT" || hname == "_h_dr_GenAnbjet_pT" || hname == "_h_Paired_Bjetpt" || hname == "_h_Paired_AnBjetpt")
	if(hname.Contains("pT") || hname.Contains("pt"))
	{
		xbin = h_obj->GetNbinsX();
		nevt = h_obj->Integral(); // !!!!!!!before DrawOverflowBin function!!!!!!
		h_obj->Rebin(5);
		h_obj->GetXaxis()->SetTitle("GeV");
		DrawOverflowBin(h_obj, 0, 200);
	}
	/// eta ///
	//if(hname == "_h_Genbjet_Eta" || hname == "_h_GenAnbjet_Eta" || hname == "_h_dr_Genbjet_Eta" || hname == "_h_dr_GenAnbjet_Eta" || hname == "_h_Paired_Bjeteta" || hname == "_h_Paired_AnBjeteta")
	if(hname.Contains("Eta") || hname.Contains("eta"))
	{
		nevt = h_obj->Integral();

		h_obj->Rebin(4);
		h_obj->GetXaxis()->SetTitle("#eta");
		DrawOverflowBin(h_obj, -3, 3);
	}
	/// phi ///
	//if(hname == "_h_Genbjet_Phi" || hname == "_h_GenAnbjet_Phi" || hname == "_h_dr_Genbjet_Phi" || hname == "_h_dr_GenAnbjet_Phi" || hname == "_h_Paired_Bjetphi" || hname == "_h_Paired_AnBjetphi")
	if(hname.Contains("Phi") || hname.Contains("phi"))
	{
		nevt = h_obj->Integral();
		
		h_obj->Rebin(4);
		h_obj->GetXaxis()->SetTitle("#phi");
		DrawOverflowBin(h_obj, -3.2, 3.2);
	}
	/// DeltaR ///
	if(hname == "_h_deltaR_Genb_Recobjet" || hname == "_h_deltaR_GenAnb_Recobbarjet")
	{
		c->SetLogy();
		nevt = h_obj->Integral();
		for(int idr=1; idr<h_obj->GetNbinsX(); idr++)
		{
			double binCenter = h_obj->GetBinCenter(idr);
			double binContent = h_obj->GetBinContent(idr);
			//cout << binCenter << ", " << binContent << endl;
			if(binCenter < 0.1) {dR_frac0p1 += binContent;}
			else if(binCenter < 0.2) {dR_frac0p2 += binContent;}
			else if(binCenter < 0.3) {dR_frac0p3 += binContent;}
			else {dR_fracover3 += binContent;}
		}
		h_obj->Rebin(2);
		TLine *line0p1 = new TLine(0.1,0,0.1,10e4);
		TLine *line0p2 = new TLine(0.2,0,0.2,10e4);
		TLine *line0p3 = new TLine(0.3,0,0.3,10e4);
		line0p1->Draw();
		line0p2->Draw();
		line0p3->Draw();
		line0p1->SetLineColor(kGreen+2);
		line0p2->SetLineColor(kBlue);
		line0p3->SetLineColor(kRed);
		line0p1->SetLineWidth(1);
		line0p2->SetLineWidth(1);
		line0p3->SetLineWidth(1);
		cout << "dR fraction" << endl;
		cout << "# of event: " << nevt << endl;
		cout << "dR < 0.1 : " << dR_frac0p1 << ", frac: " << dR_frac0p1/nevt << endl;
		cout << "dR < 0.2 : " << dR_frac0p1+dR_frac0p2 << ", frac: " << (dR_frac0p1+dR_frac0p2)/nevt << endl;
		cout << "dR < 0.3 : " << dR_frac0p1+dR_frac0p2+dR_frac0p3 << ", frac: " << (dR_frac0p1+dR_frac0p2+dR_frac0p3)/nevt << endl;
		cout << "dR >= 0.3 : " << dR_fracover3 << ", frac: " << dR_fracover3/nevt << endl;
	}
	/// O3 ///
	//if(hname == "_h_bf_TopReco_O3" || hname == "_h_GenInfo_O3")
	if(hname.Contains("O3"))
	{
		c->SetLogy();
		h_obj->Rebin(10);
		h_obj->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");

		for (int i = 1; i <= h_obj->GetNbinsX(); i++)
		{
    		double binCenter = h_obj->GetBinCenter(i);
    		double binContent = h_obj->GetBinContent(i);

    		if (binCenter > 0) {posCount += binContent;}
    		else if (binCenter < 0) {negCount += binContent;}
		}
		total = posCount + negCount;
		Asym = (posCount - negCount) / total;
		statErr = h_obj->GetRMS() / TMath::Sqrt(total);
		xbin = h_obj->GetNbinsX();
		cout << "N_+: " << posCount << ", N_-: " << negCount << ", Total(sum): " << total << ", integral: " << h_obj->Integral(0,xbin+1) << endl;
		cout << "Asym1: " << Asym << endl;

		DrawOverflowBin(h_obj, -2, 2);
	}
	h_obj->SetStats(0);
	leg = new TLegend(.55,.7,.93,.87);
	leg->SetTextSize(0.023);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
	if(hname.Contains("O3"))
	{leg->AddEntry(h_obj, Form("Event: %0.f",total),"");}
	else
	{leg->AddEntry(h_obj, Form("Event: %0.f",nevt),"");}
	leg->AddEntry(h_obj, Form("Mean: %.4f",h_obj->GetMean()),"");
	leg->AddEntry(h_obj, Form("Std Dev: %.4f",h_obj->GetRMS()),"");
	if(hname.Contains("O3"))
	{
		leg->AddEntry(h_obj, Form("N (O_{3} < 0): %.4f",negCount),"");
		leg->AddEntry(h_obj, Form("N (O_{3} > 0): %.4f",posCount),"");
		leg->AddEntry(h_obj, Form("A: %.4f#pm%.4f",Asym,statErr),"");
	}
	leg->Draw();

	/// canvas save ///
	string savepath = "";
	if(hname.Contains("Genb") || hname.Contains("GenAnb") || hname.Contains("GenMuon") || hname.Contains("GenAnMuon")) {savepath = Form("./%s/CEDM/%s/TTJets_Signal_%s/kinematics/Gen",ttbar_v.c_str(),version.c_str(),vdtG.c_str());}
	if(hname.Contains("pair_bjet") || hname.Contains("pair_bbarjet") || hname.Contains("Paired")) {savepath = Form("./%s/CEDM/%s/TTJets_Signal_%s/kinematics/Reco",ttbar_v.c_str(),version.c_str(),vdtG.c_str());}
	if(hname.Contains("O3")) {savepath = Form("./%s/CEDM/%s/TTJets_Signal_%s/kinematics/O3",ttbar_v.c_str(),version.c_str(),vdtG.c_str());}
	gSystem->mkdir(Form("%s/",savepath.c_str()),kTRUE);

	c->SaveAs(Form("%s/%s.pdf",savepath.c_str(),h_obj->GetName()));

	h_obj->GetYaxis()->SetTitle("Entries");
	h_obj->GetYaxis()->SetTitleOffset(1.5);
}
void Draw2DHist(TH2D *h_obj2D)
{
	TString hname = h_obj2D->GetName();

	TLegend *leg2d;

	TCanvas *c = new TCanvas("c",hname,800,800);

	c->cd();
	c->SetMargin(0.12,0.12,0.12,0.12);
	h_obj2D->Draw("colz");
	h_obj2D->SetTitle("");
	h_obj2D->GetXaxis()->SetRangeUser(0,10);
	h_obj2D->GetYaxis()->SetRangeUser(0,10);
	h_obj2D->GetYaxis()->SetTitleOffset(1.5);
	h_obj2D->GetXaxis()->SetTitle("#DeltaR_{b jet}");
	h_obj2D->GetYaxis()->SetTitle("#DeltaR_{#bar{b} jet}");

	// check the number of deltaR>3, deltaR<3
	double deltaR_under3 = h_obj2D->Integral(5,30,5,30);
	double deltaR_over3 = h_obj2D->Integral()- deltaR_under3;
	double frac_dR_under3 = deltaR_under3/h_obj2D->Integral();
	double frac_dR_over3 = deltaR_over3/h_obj2D->Integral();

	leg2d = new TLegend(.15,.7,.43,.8);
	leg2d->SetTextSize(0.023);
    leg2d->SetBorderSize(0);
    leg2d->SetFillColor(0);
    leg2d->SetFillStyle(0);
	leg2d->AddEntry(h_obj2D, Form("DeltaR < 3: %.3f",frac_dR_under3),"");
	leg2d->AddEntry(h_obj2D, Form("DeltaR > 3: %.3f",frac_dR_over3),"");
	leg2d->Draw();

	//string savepath = Form("./ttbar_%s/v2/%s",version.c_str());
	//gSystem->mkdir(Form("%s/",savepath.c_str()),kTRUE);
	//c->SaveAs(Form("%s/%s.pdf",savepath.c_str(),h_obj2D->GetName()));
}
void DrawOverflowBin(TH1D *his, double min, double max)
{
    double contentUnder = 0;
    double contentOver  = 0;
    
    for (int i=0; i<his->GetNbinsX()-1; i++)
	{
        if (his->GetBinCenter(i+1) < min)
		{contentUnder += his->GetBinContent(i+1);}
        if (his->GetBinCenter(i+1) > max)
		{contentOver += his->GetBinContent(i+1);}
    }
    for (int i=0; i<his->GetNbinsX()-1; i++)
	{
        if (his->GetBinCenter(i+1) < min && his->GetBinCenter(i+2) > min) his->SetBinContent(i+2, contentUnder);
        if (his->GetBinCenter(i+1) < max && his->GetBinCenter(i+2) > max) his->SetBinContent(i+1, contentOver);
    }
    his->GetXaxis()->SetRangeUser(min, max);
}