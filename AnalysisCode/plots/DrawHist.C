//////////////////////////////////////////////////////////////////////////////////
//	DrawHist.C Description														//
//	root -l -b -q DrawHist.C'("version","opt")									//
//	opt: cpv or data															//
//  	cpv: draw histogram for CPV samples										//
//  	data: draw histogram for data samples									//
//	* histogram *																//
//	1D: dR, pT, eta, phi, invariant mass, O3									//
//	2D: dR, idx, mass															//
//	** 3 cases **																//
//	1) new algorithm v1(use b-tagging jet for matching)							//
//		draw histogram for dR(0.5, 1.0, 1.5, 2.0, 2.5, 3.0)						//
//	2) Gen level																//
//	3) new algorithm v2(use all jet for matching)								//
//	*** also O3 asymmetry parameters are written in text file for each samples	//
//////////////////////////////////////////////////////////////////////////////////
#include "TSystem.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"
#include "toDrawHist.h"
#include "CPVari.C"

/*struct CPVari
{
    double asym_ = 0.;
    double asym_err_ = -999.;
    double num_p_ = 0.;
    double num_p_err_ = -999.;
    double f_p_ = 0.;
    double num_m_ = 0.;
    double num_m_err_ = -999.;
    double f_m_ = 0.;
    double num_total_ = 0.;
};*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
//	iscpv = "cpv" or "data"					   														//
//	data option include MC and data(data_doublemuon, data_singlemuon, DY, ST, TTV, Diboson, Data)	//
//////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawHist(string version, string iscpv, string dR_value, string dR_hist)
{
	void Draw1DHist(TH1D *h_obj, string version, string sample, ostream &fout, string iscpv, string dR_hist);
	void Draw2DHist(TH2D *h_obj2D, string version, string sample, ostream &fout3d, string iscpv);
	CPVari O3Asym(TH1D *hist);
    CPVari tmp;

	/// text file ///
	//gSystem->mkdir(Form("./%s/text",version.c_str()),kTRUE);
	ofstream outtxt;
    if(iscpv == "cpv") {outtxt.open(Form("./Job_Version/%s/Asymmetry_dimu_CPV.dat", version.c_str()));}
	else if(iscpv == "data")
	{
		if(dR_hist.find("v2") != string::npos) {outtxt.open(Form("./Job_Version/%s/Asymmetry_dimu_SystematicStudy_v28_O3v2.dat", version.c_str()));}
		else {outtxt.open(Form("./Job_Version/%s/Asymmetry_dimu_SystematicStudy_v28_O3v1_dR%s.dat", version.c_str(), dR_value.c_str()));}
	}
    ofstream &fout = outtxt;
	fout << "Luminosity:  35867.059983" << endl << endl;
	/// delta R ratio ///
	ofstream outtxt3D(Form("%s/Dist_DeltaR_Ratio.txt",version.c_str()));
    ofstream &fout3d = outtxt3D;

	vector<string> sample;
	if(iscpv == "cpv") {sample = {
		"TTJets_Signal_dtG_m2p60364","TTJets_Signal_dtG_m1p0415","TTJets_Signal_dtG_m0p5207",
		"TTJets_Signal_dtG_0",
		"TTJets_Signal_dtG_0p5207","TTJets_Signal_dtG_1p0415","TTJets_Signal_dtG_2p60364"
		};
	}
	else if(iscpv == "data")
	{
		sample = {
			"Data_DoubleMuon_Run2016B","Data_DoubleMuon_Run2016C","Data_DoubleMuon_Run2016D","Data_DoubleMuon_Run2016E","Data_DoubleMuon_Run2016F","Data_DoubleMuon_Run2016G","Data_DoubleMuon_Run2016HV2","Data_DoubleMuon_Run2016HV3",
			"Data_SingleMuon_Run2016B","Data_SingleMuon_Run2016C","Data_SingleMuon_Run2016D","Data_SingleMuon_Run2016E","Data_SingleMuon_Run2016F","Data_SingleMuon_Run2016G","Data_SingleMuon_Run2016HV2","Data_SingleMuon_Run2016HV3", // ~15
			"DYJetsToLL_M_10To50","DYJetsToLL_M_50", //16~
			"ST_tW_antitop","ST_tW_top",
			"TTJets_Signal",
			"TTJets_others",
			"TTbar_WJetToLNu","TTbar_WQQ","TTbar_ZQQ","TTbar_ZToLLNuNu",
			"WW","WZ","ZZ","WJetsToLNu", //~30
			"Data_DoubleMuon","Data_SingleMuon","DY","ST","TTV","Diboson","Data"
		};
	}

	for(int isn=0; isn<sample.size(); isn++)
	{
		TString ffile;
		/// file read ///
		if(iscpv == "cpv") {ffile = Form("../output/Job_Version/%s/CPV_Sample/%s/%s_All.root",version.c_str(),sample[isn].c_str(),sample[isn].c_str());}
		else if(iscpv == "data")
		{
			if(isn < 30) {ffile = Form("../output/Job_Version/%s/Dataset/%s/%s_All.root",version.c_str(),sample[isn].c_str(),sample[isn].c_str());}
			else {ffile = Form("../output/Job_Version/%s/Dataset/%s/%s.root",version.c_str(),sample[isn].c_str(),sample[isn].c_str());}
		}
		TFile *f = new TFile(ffile,"READ");
		if(f==NULL){cout << "Something Wrong!!" << endl;}
		else {cout << ffile.Data() << endl;}
		/// draw histogram!! ///
		for(int i1d=0; i1d<hist1D.size(); i1d++)
		{
			TH1D *h_obj = (TH1D*)f->Get(Form("%s",hist1D[i1d].Data()));
			if(h_obj == NULL){continue;}
			Draw1DHist(h_obj,version,sample[isn],fout,iscpv,dR_hist);
		}
		for(int i2d=0; i2d<hist2D.size(); i2d++)
		{
			TH2D *h_obj2D = (TH2D*)f->Get(Form("%s",hist2D[i2d].Data()));
			if(h_obj2D == NULL){continue;}
			Draw2DHist(h_obj2D,version,sample[isn],fout3d,iscpv);
		}
	}
}
void Draw1DHist(TH1D *h_obj, string version, string sample, ostream &fout, string iscpv, string dR_hist)
{
	void DrawOverflowBin(TH1D *his, double min, double max);
	CPVari O3Asym(TH1D *hist);
    CPVari tmp;

	TLegend *leg;  

	TString hname = h_obj->GetName();

	vector<string> dR_values = {"0p5", "1p0", "1p5", "2p0", "2p5", "3p0", "10p0"};
	vector<string> dR_idx = {"0","1","2","3","4","5","6"};

	/// define save path ///
	string savepath;
	if(iscpv == "cpv")
	{
		if(hname.Contains("Gen")) {savepath = Form("./Job_Version/%s/CPV_Sample/%s/Gen",version.c_str(),sample.c_str());}
		else if(hname.Contains("v2")) {savepath = Form("./Job_Version/%s/CPV_Sample/%s/Reco/O3_v2",version.c_str(),sample.c_str());}
		else {savepath = Form("./Job_Version/%s/CPV_Sample/%s/Reco",version.c_str(),sample.c_str());}
	}
	else if(iscpv == "data")
	{
		if(hname.Contains("Gen")) {savepath = Form("./Job_Version/%s/Dataset/%s/Gen",version.c_str(),sample.c_str());}
		else if(hname.Contains("v2")) {savepath = Form("./Job_Version/%s/Dataset/%s/Reco/O3_v2",version.c_str(),sample.c_str());}
		else {savepath = Form("./Job_Version/%s/Dataset/%s/Reco",version.c_str(),sample.c_str());}
	}
	gSystem->mkdir(Form("%s/",savepath.c_str()),kTRUE);

	/// canvas ///
	TCanvas *c = new TCanvas("c",hname,800,800);
	c->cd();
	c->SetMargin(0.12,0.12,0.12,0.12);
	h_obj->Draw("hist");
	h_obj->SetTitle("");
	/// DeltaR ///
	if(hname.Contains("dR"))
	{
		h_obj->GetXaxis()->SetRangeUser(0,10);
		//h_obj->Rebin(10);
	}
	/// pT ///
	if(hname.Contains("pt"))
	{
		h_obj->Rebin(5);
		h_obj->GetXaxis()->SetTitle("GeV");
		DrawOverflowBin(h_obj, 0, 200);
	}
	/// eta ///
	if(hname.Contains("eta"))
	{
		h_obj->Rebin(2);
		h_obj->GetXaxis()->SetTitle("#eta");
		DrawOverflowBin(h_obj, -3, 3);
	}
	/// phi ///
	if(hname.Contains("phi"))
	{
		//h_obj->Rebin(4);
		h_obj->GetXaxis()->SetTitle("#phi");
		DrawOverflowBin(h_obj, -3.2, 3.2);
	}
	/// invariant mass ///
	if(hname.Contains("Mass"))
	{
		h_obj->Rebin(5);
		h_obj->GetXaxis()->SetTitle("Invariant Mass (GeV)");
		DrawOverflowBin(h_obj, 0, 300);
	}
	if(hname.Contains("idx"))
	{
		h_obj->GetXaxis()->SetTitle("Index");
	}
	/// O3 ///
	if(hname.Contains("O3"))
	{
		c->SetLogy();
		//h_obj->Rebin(10);

		tmp = O3Asym(h_obj);

		h_obj->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");
		h_obj->SetStats(0);
		leg = new TLegend(.55,.7,.93,.87);
		leg->SetTextSize(0.023);
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(0);
		//leg->AddEntry(h_obj, Form("Entries: %0.f",h_obj->GetEntries()),"");
		if(sample == "Data_DoubleMuon" || sample == "Data_SingleMuon" || sample == "Data")
		{leg->AddEntry(h_obj, Form("Event: %0.f",tmp.num_total_),"");}
		else
		{leg->AddEntry(h_obj, Form("Event: %.3f",tmp.num_total_),"");}
		leg->AddEntry(h_obj, Form("Mean: %.4f",h_obj->GetMean()),"");
		leg->AddEntry(h_obj, Form("Std Dev: %.4f",h_obj->GetRMS()),"");
		if(sample == "DYJetsToLL_M_10To50" || sample == "DYJetsToLL_M_50" || sample == "ST_tW_antitop" || sample == "ST_tW_top" ||
		sample == "TTJets_Signal" || sample == "TTJets_others" ||
		sample == "TTbar_WJetToLNu" || sample == "TTbar_WQQ" || sample == "TTbar_ZQQ" || sample == "TTbar_ZToLLNuNu" ||
		sample == "WW" || sample == "WZ" || sample == "ZZ" || sample == "WJetsToLNu" ||
		sample == "DY" || sample == "ST" || sample == "TTV" || sample == "Diboson")
		{
			leg->AddEntry(h_obj, Form("N (O_{3} < 0): %.4f",tmp.num_m_),"");
			leg->AddEntry(h_obj, Form("N (O_{3} > 0): %.4f",tmp.num_p_),"");
		}
		else
		{
			leg->AddEntry(h_obj, Form("N (O_{3} < 0): %0.f",tmp.num_m_),"");
			leg->AddEntry(h_obj, Form("N (O_{3} > 0): %0.f",tmp.num_p_),"");
		}
		leg->AddEntry(h_obj, Form("A: %.4f",tmp.asym_),"");
		leg->Draw();

		if( ( iscpv == "cpv" && (hname.Contains("_h_GenCPO3_bfReco_") || hname.Contains("_h_CPO3_bfReco_5_") || hname.Contains("_h_v2_CPO3_bfReco_")) ) || 
			( iscpv == "data" && ( hname == dR_hist.c_str() ) && 
		   		(sample == "DYJetsToLL_M_10To50" || sample == "DYJetsToLL_M_50" || sample == "Data" || 
		    	sample == "ST_tW_antitop" || sample == "ST_tW_top" || sample == "TTJets_others" || 
		    	sample == "TTJets_Signal" || sample == "TTbar_WJetToLNu" || sample == "TTbar_WQQ" || 
		    	sample == "TTbar_ZQQ" || sample == "TTbar_ZToLLNuNu" || sample == "WW" || 
		    	sample == "WZ" || sample == "ZZ") )
		)
		{
			if(iscpv =="cpv") {fout << Form("<<<<<< %s >>>>>>",hname.Data()) << endl;}
			fout << Form("dimu_hO3_4%s_asym: %.6f"			,sample.c_str(), tmp.asym_) << endl;
			fout << Form("dimu_hO3_4%s_asym_err: %.6f"		,sample.c_str(), tmp.asym_err_) << endl;
			fout << Form("dimu_hO3_4%s_Nplus: %.6f"			,sample.c_str(), tmp.num_p_) << endl;
			fout << Form("dimu_hO3_4%s_Nplus_err: %.6f"		,sample.c_str(), tmp.num_p_err_) << endl;
			fout << Form("dimu_hO3_4%s_fplus: %.6f"			,sample.c_str(), tmp.f_p_) << endl;
			fout << Form("dimu_hO3_4%s_Nminus: %.6f"		,sample.c_str(), tmp.num_m_) << endl;
			fout << Form("dimu_hO3_4%s_Nminus_err: %.6f"	,sample.c_str(), tmp.num_m_err_) << endl;
			fout << Form("dimu_hO3_4%s_fminus: %.6f"		,sample.c_str(), tmp.f_m_) << endl;
			fout << Form("dimu_hO3_4%s_total: %.6f"			,sample.c_str(), tmp.num_total_) << endl;
			fout << endl;
		}
		DrawOverflowBin(h_obj, -2, 2);
	}

	h_obj->GetYaxis()->SetTitle("Entries");
	h_obj->GetYaxis()->SetTitleOffset(1.5);

	// save canvas
	for(int i=0; i<dR_idx.size(); i++)
	{
		if(hname.Contains("_" + dR_idx[i] + "_"))
		{
			gSystem->mkdir(Form("%s/dR_%s/",savepath.c_str(), dR_values[i].c_str()),kTRUE);
			c->SaveAs(Form("%s/dR_%s/%s.pdf",savepath.c_str(), dR_values[i].c_str(), h_obj->GetName()));
		}
	}
	if(!hname.Contains("_0_") && !hname.Contains("_1_") && !hname.Contains("_2_") && !hname.Contains("_3_") && !hname.Contains("_4_") && !hname.Contains("_5_"))
	{c->SaveAs(Form("%s/%s.pdf",savepath.c_str(),h_obj->GetName()));}

	delete c;
}
void Draw2DHist(TH2D *h_obj2D, string version, string sample, ostream &fout3d, string iscpv)
{
	TString hname = h_obj2D->GetName();

	TLegend *leg2d;

	vector<string> dR_values = {"0p5", "1p0", "1p5", "2p0", "2p5", "3p0", "10p0"};
	vector<string> dR_idx = {"0","1","2","3","4","5","6"};

	TCanvas *c = new TCanvas("c",hname,800,800);

	c->cd();
	c->SetMargin(0.12,0.12,0.12,0.12);
	h_obj2D->Draw("colz");
	h_obj2D->SetTitle("");
	/// invariant mass ///
	if(hname.Contains("comp") || hname.Contains("Mass"))
	{
		h_obj2D->RebinX(10);
		h_obj2D->RebinY(10);
		h_obj2D->GetXaxis()->SetRangeUser(0,1000);
		h_obj2D->GetYaxis()->SetRangeUser(0,1000);
	}
	if(hname.Contains("dR") || hname.Contains("idx"))
	{
		h_obj2D->GetXaxis()->SetRangeUser(0,10);
		h_obj2D->GetYaxis()->SetRangeUser(0,10);
	}
	
	h_obj2D->GetYaxis()->SetTitleOffset(1.5);
	//h_obj2D->GetXaxis()->SetTitle("#DeltaR_{b jet}");
	//h_obj2D->GetYaxis()->SetTitle("#DeltaR_{#bar{b} jet}");

	// check the number of deltaR>3, deltaR<3
	double deltaR_under3 = h_obj2D->Integral(5,30,5,30);
	double deltaR_over3 = h_obj2D->Integral()- deltaR_under3;
	double frac_dR_under3 = deltaR_under3/h_obj2D->Integral();
	double frac_dR_over3 = deltaR_over3/h_obj2D->Integral();
	//fout3d << "Sample name: " << sample.c_str() << endl;
	//fout3d << "entries: " << h_obj2D->GetEntries() << ", Event: " << h_obj2D->Integral() << endl;
	//fout3d << "DeltaR < 3: " << deltaR_under3 << ", DeltaR > 3: " << deltaR_over3 << endl;
	//fout3d << "fraction: DeltaR < 3: " << deltaR_under3/h_obj2D->Integral() << " , DeltaR > 3: " << deltaR_over3/h_obj2D->Integral() << endl;
	//fout3d << endl;

	leg2d = new TLegend(.15,.7,.43,.8);
	leg2d->SetTextSize(0.023);
    leg2d->SetBorderSize(0);
    leg2d->SetFillColor(0);
    leg2d->SetFillStyle(0);
	leg2d->AddEntry(h_obj2D, Form("DeltaR < 3: %.3f",frac_dR_under3),"");
	leg2d->AddEntry(h_obj2D, Form("DeltaR > 3: %.3f",frac_dR_over3),"");
	//leg2d->Draw();

	string savepath;
	if(iscpv == "cpv")
	{
		if(hname.Contains("Gen")){savepath = Form("./Job_Version/%s/CPV_Sample/%s/Gen",version.c_str(),sample.c_str());}
		else if(hname.Contains("v2")) {savepath = Form("./Job_Version/%s/CPV_Sample/%s/Reco/O3_v2",version.c_str(),sample.c_str());}
		else {savepath = Form("./Job_Version/%s/CPV_Sample/%s/Reco",version.c_str(),sample.c_str());}
	}
	else if(iscpv == "data")
	{
		if(hname.Contains("Gen")){savepath = Form("./Job_Version/%s/Dataset/%s/Gen",version.c_str(),sample.c_str());}
		else if(hname.Contains("v2")) {savepath = Form("./Job_Version/%s/Dataset/%s/Reco/O3_v2",version.c_str(),sample.c_str());}
		else {savepath = Form("./Job_Version/%s/Dataset/%s/Reco",version.c_str(),sample.c_str());}
	}
	// save canvas
	for(int i=0; i<dR_idx.size(); i++)
	{
		if(hname.Contains("_" + dR_idx[i] + "_"))
		{
			gSystem->mkdir(Form("%s/dR_%s/",savepath.c_str(), dR_values[i].c_str()),kTRUE);
			c->SaveAs(Form("%s/dR_%s/%s.pdf",savepath.c_str(), dR_values[i].c_str(), h_obj2D->GetName()));
		}
	}
	if(!hname.Contains("_0_") && !hname.Contains("_1_") && !hname.Contains("_2_") && !hname.Contains("_3_") && !hname.Contains("_4_") && !hname.Contains("_5_"))
	{c->SaveAs(Form("%s/%s.pdf",savepath.c_str(),h_obj2D->GetName()));}

	delete c;
}
/*CPVari O3Asym(TH1D *hist)
{
    CPVari tmp;

    double posCount = 0.0;
    double posErr = 0.0;
    double fpos = 0.0;
    double pos_w2 = 0.0;
    double negCount = 0.0;
    double negErr = 0.0;
    double fneg = 0.0;
    double neg_w2 = 0.0;
    double ref_val = 20.496633;
    double norfac = 0.0000;
    double total = 0.0000;
    double Asym = 0.0000;
    double AsymErr = 0.0000;
    double xbin = 0;

    xbin = hist->GetNbinsX();
    //hist->Rebin(10);
    for (int i = 1; i <= hist->GetNbinsX(); i++)
	{
   		double binCenter = hist->GetBinCenter(i);
   		double binContent = hist->GetBinContent(i);
   		if (binCenter > 0)
        {
            posCount += binContent;
            posErr += hist->GetBinError(i);
            pos_w2 += hist->GetSumw2()->At(i); //Sumw2: sum of squares of weights
        }
   		else if (binCenter < 0)
        {
            negCount += binContent;
            negErr += hist->GetBinError(i);
            neg_w2 += hist->GetSumw2()->At(i);
        }
	}
    //norfac = ref_val/xsec;
    total = posCount + negCount;
    fpos = posCount / total;
    fneg = negCount / total;
    Asym = (posCount - negCount) / total;
    AsymErr = sqrt(
                    pow(2*negCount/pow(posCount+negCount,2),2)*pos_w2
                    + pow((-2)*posCount/pow(posCount+negCount,2),2)*neg_w2
                    );

    tmp.asym_ = Asym;
    tmp.asym_err_ = AsymErr;
    tmp.num_p_ = posCount;
    tmp.num_p_err_ = posErr;
    tmp.f_p_ = fpos;
    tmp.num_m_ = negCount;
    tmp.num_m_err_ = negErr;
    tmp.f_m_ = fneg;
    tmp.num_total_ = total;

    return tmp;
}*/
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