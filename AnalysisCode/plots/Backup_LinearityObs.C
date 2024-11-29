#include "TCanvas.h"
#include "TText.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TH1.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TLine.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

struct CPVari
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
};
void LinearityObs(string version)
{
    double O3Asym(TH1D *hist, string version, string savepath, string vdtG, double xsec, ostream &fout);
    double HistStatErr(TH1D *hist, string vdtG, double xsec, ostream &fout);
    CPVari CalAsymVari(TH1D* h1,string version, string savepath, string vdtG, double xsec, ostream &fout);
    //void CalAsymVari(TH1F* h1);

    TLatex text;
    TLine line;
    TLegend *lleg;

    ///// canvas save /////
    string savepath;
    savepath = Form("./CPV_Sample/%s/linearity_newStatErr",version.c_str());
	gSystem->mkdir(Form("%s/",savepath.c_str()),kTRUE);

    ///// text file /////
    string txtpath = Form("./text/CPV_Sample/%s",version.c_str());
    gSystem->mkdir(Form("%s/",txtpath.c_str()),kTRUE);
    ofstream outtxt(Form("%s/AsymParas_%s_newStatErr.txt",txtpath.c_str(),version.c_str()));
    ofstream &fout = outtxt;

    vector<string> vdtG = {"dtG_m2p60364","dtG_m1p0415","dtG_m0p5207","dtG_0","dtG_0p5207","dtG_1p0415","dtG_2p60364"};
    vector<string> vCPO3 = {"h_Reco_CPO3"};
    vector<string> vdRO3;
    vector<double> xsec = {27.501577, 21.593304, 20.768613, 20.496633, 20.770416, 21.593084, 27.502804};
    Double_t vdtG_Xval[7] = {-2.60364,-1.0415,-0.5207,0,0.5207,1.0415,2.60364};
    Double_t vdtG_Yval[7];
    Double_t vdtG_StatErr[7];

    for(int idtg=0; idtg<vdtG.size(); idtg++)
    {
        TString ffile = Form("../output/CPV_Sample/%s/TTJets_Signal_%s/TTJets_Signal_%s/TTJets_Signal_%s_All.root",version.c_str(),vdtG[idtg].c_str(),vdtG[idtg].c_str(),vdtG[idtg].c_str());
        TFile *f = new TFile(ffile,"READ");
		if(f==NULL){cout << "Something Wrong!!" << endl;} else {cout << ffile.Data() << endl;}
        TH1D *h_CPO3 = (TH1D*)f->Get(Form("%s",vCPO3[0].c_str()));
        if(h_CPO3==NULL){cout << Form("empty %s.......",vCPO3[0].c_str()) << endl;}
        //vdtG_Yval[idtg] = O3Asym(h_CPO3, version, savepath, vdtG[idtg], xsec[idtg], fout);
        vdtG_Yval[idtg] = O3Asym(h_CPO3, version, savepath, vdtG[idtg], xsec[idtg], fout);
        //vdtG_StatErr[idtg] = HistStatErr(h_CPO3, vdtG[idtg], xsec[idtg], fout);
        CPVari tmp = CalAsymVari(h_CPO3,version, savepath, vdtG[idtg], xsec[idtg], fout);
        vdtG_Yval[idtg] = tmp.asym_;
        vdtG_StatErr[idtg] = tmp.asym_err_;
    }

    ///// draw linearity plot /////
    TCanvas *clin = new TCanvas("clin","clin",800,800);
    TGraph *g_reco = new TGraphAsymmErrors(7,vdtG_Xval,vdtG_Yval,0,0,vdtG_StatErr,vdtG_StatErr);
    clin->cd();
    clin->SetGridx();
    clin->SetGridy();
    clin->SetMargin(0.12,0.02,0.12,0.02);
    g_reco->Draw("AP");
    g_reco->SetTitle("");
    g_reco->GetXaxis()->SetRangeUser(-2.9,2.9);
    g_reco->GetYaxis()->SetRangeUser(-0.08,0.08);
    g_reco->GetXaxis()->SetTitle("Im(d_{tG})");
    g_reco->GetYaxis()->SetTitle("Asymmetry in O_{3}");
    g_reco->GetYaxis()->SetTitleOffset(1.5);
    g_reco->SetMarkerStyle(kFullCircle);
    g_reco->SetMarkerColor(kBlack);

    text.SetNDC();
    text.SetTextSize(0.03);
    text.SetTextFont(42);
    text.DrawLatex(0.2,0.9,"#mu^{+}#mu^{-} channel");

    clin->SaveAs(Form("%s/CPV_Linearity.pdf",savepath.c_str()));
}
/*double O3Asym(TH1D *hist, string version, string savepath, string vdtG, double xsec, ostream &fout)
{
    void DrawOverflowBin(TH1D *his, double min, double max);

    double posCount = 0.0;
    double negCount = 0.0;
    double ref_val = 20.496633;
    double norfac = 0.0000;
    double total = 0.0000;
    double Asym = 0.0000;
    double xbin = 0;

    TLegend *leg;

	TString hname = hist->GetName();
	TCanvas *c = new TCanvas("c",hname,800,800);
    c->cd();
    c->SetLogy();
    c->SetMargin(0.12,0.02,0.12,0.02);
    hist->Draw("hist");
	hist->SetTitle("");
    //hist->Rebin(10);
	hist->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");
    hist->GetYaxis()->SetTitle("Entries");
	hist->GetYaxis()->SetTitleOffset(1.5);

    for (int i = 1; i <= hist->GetNbinsX(); i++)
	{
   		double binCenter = hist->GetBinCenter(i);
   		double binContent = hist->GetBinContent(i);
   		if (binCenter > 0) {posCount += binContent;}
   		else if (binCenter < 0) {negCount += binContent;}
	}
    norfac = ref_val/xsec;
    //if(hname == "_h_bf_TopReco_O3") {total = (posCount + negCount) * norfac;}
    //else {total = (posCount + negCount);}
    total = (posCount + negCount) * norfac;
    Asym = (posCount - negCount) / total;
    xbin = hist->GetNbinsX();
    
    hist->SetStats(0);
	leg = new TLegend(.55,.7,.93,.87);
	leg->SetTextSize(0.023);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
	leg->AddEntry(hist, Form("Event: %.4f",hist->Integral(0,xbin+1)),"");
	leg->AddEntry(hist, Form("Mean: %.4f",hist->GetMean()),"");
	leg->AddEntry(hist, Form("Std Dev: %.4f",hist->GetRMS()),"");
	leg->AddEntry(hist, Form("N (O_{3} < 0): %.4f",negCount),"");
	leg->AddEntry(hist, Form("N (O_{3} > 0): %.4f",posCount),"");
	leg->AddEntry(hist, Form("A: %.4f",Asym),"");
	leg->Draw();

    //fout << From()
    fout << Form("%s_",vdtG.c_str()) << Form("Nega: %.6f", negCount) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Posi: %.6f", posCount) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Asym: %.6f", Asym) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Total: %.6f" ,total) << endl;
    //fout << endl;

    //c->SaveAs(Form("%s/%s%s_1.pdf",savepath.c_str(),vdtG.c_str(),hist->GetName()));
    DrawOverflowBin(hist, -2, 2);
	c->SaveAs(Form("%s/%s_%s.pdf",savepath.c_str(),vdtG.c_str(),hist->GetName()));

    return Asym;
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
}*/
//CPVari Auto_ControlPlot::CalAsymVari(TH1F* h1)
CPVari CalAsymVari(TH1D* h1,string version, string savepath, string vdtG, double xsec, ostream &fout)
{
    // vector clear and declare variables
    CPVari tmp;
    if (h1 ==NULL) {return tmp;}
    int nbin = h1->GetNbinsX();
    double asym     = 0.0;
    double asym_err = 0.0;
    double posi     = 0.0;
    double nega     = 0.0;
    double posi_err = 0.0;
    double nega_err = 0.0;
    double fposi    = 0.0;
    double fnega    = 0.0;
    double total    = 0.0;
    double posi_w2  = 0.0;
    double nega_w2  = 0.0;
    double ref_val  = 20.496633;
    double norfac   = 0.0000;

    norfac = ref_val/xsec;
    total = h1->Integral() * norfac; // Total 
    TH1F *hnew = (TH1F*)h1->Clone("hnew");
    hnew->Rebin(nbin/2);
    posi = hnew->GetBinContent(2);
    posi_err = hnew->GetBinError(2);
    nega = hnew->GetBinContent(1);
    nega_err = hnew->GetBinError(1);
    if (total != 0) // nan check !!
    {
        fnega = nega/total; // Negative fraction 
        fposi = posi/total; // Positive fraction
        asym  = (posi - nega)/total;
        nega_w2 = hnew->GetSumw2()->At(1); // Negative ,Sumw2: sum of squares of weights
        posi_w2 = hnew->GetSumw2()->At(2); // Positive 
        //asym_err = sqrt(  (1-asym*asym) / total ); // Asym. Error // Not Use //
        asym_err = sqrt(
                        pow(2*nega/pow(posi+nega,2),2)*posi_w2
                        + pow((-2)*posi/pow(posi+nega,2),2)*nega_w2
                        ); // Asym. Error // Not Use //
    }

    tmp.asym_=asym;
    tmp.asym_err_=asym_err;

    tmp.num_p_=posi;
    tmp.num_p_err_=posi_err;
    tmp.f_p_=fposi;

    tmp.num_m_=nega;
    tmp.num_m_err_=nega_err;
    tmp.f_m_ = fnega;
    tmp.num_total_=total;

    TLegend *leg;

    TString hname = h1->GetName();
    TCanvas *c = new TCanvas("c",hname,800,800);
    c->cd();
    c->SetLogy();
    c->SetMargin(0.12,0.02,0.12,0.02);
    h1->Draw("h1");
    h1->SetTitle("");
    //h1->Rebin(10);
    h1->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");
    h1->GetYaxis()->SetTitle("Entries");
    h1->GetYaxis()->SetTitleOffset(1.5);

    h1->SetStats(0);
	leg = new TLegend(.55,.7,.93,.87);
	leg->SetTextSize(0.023);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
	leg->AddEntry(h1, Form("Event: %.4f",h1->Integral()),"");
	leg->AddEntry(h1, Form("Mean: %.4f",h1->GetMean()),"");
	leg->AddEntry(h1, Form("Std Dev: %.4f",h1->GetRMS()),"");
	leg->AddEntry(h1, Form("N (O_{3} < 0): %.4f",nega),"");
	leg->AddEntry(h1, Form("N (O_{3} > 0): %.4f",posi),"");
	leg->AddEntry(h1, Form("A: %.4f",asym),"");
	leg->Draw();

    //fout << From()
    fout << Form("%s_",vdtG.c_str()) << Form("Nega: %.6f", nega) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Posi: %.6f", posi) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Asym: %.6f", asym) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Total: %.6f" ,total) << endl;
    //fout << endl;

	c->SaveAs(Form("%s/%s_%s.pdf",savepath.c_str(),vdtG.c_str(),h1->GetName()));

    return tmp;
}