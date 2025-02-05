#include "TCanvas.h"
#include "TText.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TH1.h"
#include "TF1.h"
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
void LinearityObs(string version, string opt)
//////////////////////////////////////////////////////////////////////////////////
//  opt description                                                             //
//  1) recov1: reco lv O3 with different dR (0.5~3.0, 0.5 interval, and 10.0)   //
//  2) gen: gen lv O3                                                           //
//  3) recov2: O3 used all jets that is matched to leptons with minimum dR      //
//  4) ref: O3 after top reconstruction for reference                           //
//  ** if you need other options, add savepath and vCPO3 vector array           //
//////////////////////////////////////////////////////////////////////////////////
{
    CPVari O3Asym(TH1D *hist, string version, string savepath, string vdtG, double xsec, ostream &fout);
    //CPVari CalAsymVari(TH1D* h1,string version, string savepath, string vdtG, double xsec, ostream &fout);

    TLatex text;
    TLine *line;
    TLegend *lleg;
    CPVari tmp[7][7];
    CPVari tmp_data;

    ///// canvas save /////
    string savepath;
    if(opt == "recov1") {savepath = Form("./Job_Version/%s/CPV_Sample/Linearity/Reco",version.c_str());}
    else if(opt == "gen") {savepath = Form("./Job_Version/%s/CPV_Sample/Linearity/Gen",version.c_str());}
    else if(opt == "recov2") {savepath = Form("./Job_Version/%s/CPV_Sample/Linearity/Reco/O3_v2",version.c_str());}
    else if(opt == "ref") {savepath = Form("./Job_Version/%s/CPV_Sample/Linearity/Ref",version.c_str());}
    else {cout << "Something Wrong!! check opt!!!: recov1, recov2, gen, ref" << endl;}
    
	gSystem->mkdir(Form("%s/",savepath.c_str()),kTRUE);

    ///// text file /////
    string txtpath = Form("./Job_Version/%s/CPV_Sample/Linearity",version.c_str());
    gSystem->mkdir(Form("%s/",txtpath.c_str()),kTRUE);
    ofstream outtxt(Form("%s/AsymParas_%s_%s.txt",txtpath.c_str(),version.c_str(),opt.c_str()));
    ofstream &fout = outtxt;

    vector<string> vdtG = {"dtG_m2p60364","dtG_m1p0415","dtG_m0p5207","dtG_0","dtG_0p5207","dtG_1p0415","dtG_2p60364"};
    vector<string> vCPO3;
    if(opt == "recov1") {vCPO3 = {"_h_CPO3_bfReco_0_","_h_CPO3_bfReco_1_","_h_CPO3_bfReco_2_","_h_CPO3_bfReco_3_","_h_CPO3_bfReco_4_","_h_CPO3_bfReco_5_","_h_CPO3_bfReco_6_"};}
    else if(opt == "gen") {vCPO3 = {"_h_GenCPO3_bfReco_"};}
    else if(opt == "recov2") {vCPO3 = {"_h_v2_CPO3_bfReco_"};}
    else if(opt == "ref") {vCPO3 = {"h_Reco_CPO3"};}

    vector<string> dR_values = {"0p5", "1p0", "1p5", "2p0", "2p5", "3p0", "10p0"};
	vector<string> dR_idx = {"0","1","2","3","4","5","6"};

    vector<string> vdRO3;
    vector<double> xsec = {27.501577, 21.593304, 20.768613, 20.496633, 20.770416, 21.593084, 27.502804};
    Double_t vdtG_Xval[7] = {-2.60364,-1.0415,-0.5207,0,0.5207,1.0415,2.60364};
    Double_t vdtG_Yval[7][7];
    Double_t vdtG_StatErr[7][7];
    ///////////////////////////////////////////////
    // Data+MC O3 Asymmetry using likelihood fit //
    ///////////////////////////////////////////////
    Double_t vLike_Yval= -99.0;
    Double_t vLike_StatErr_p= -99.0;
    Double_t vLike_StatErr_m= -99.0;
    if(opt == "recov1") {vLike_Yval = -0.00353946; vLike_StatErr_p = 0.00508294; vLike_StatErr_m = 0.00508286;} //dR <10.0
    else if(opt == "recov2") {vLike_Yval = 0.00852509; vLike_StatErr_p = 0.00508235; vLike_StatErr_m = 0.00508338;}

    for(int icp=0; icp<vCPO3.size(); icp++)
    {
        for(int idtg=0; idtg<vdtG.size(); idtg++)
        {
            TString ffile = Form("../output/Job_Version/%s/CPV_Sample/TTJets_Signal_%s/TTJets_Signal_%s_All.root",version.c_str(),vdtG[idtg].c_str(),vdtG[idtg].c_str());
            TFile *f = new TFile(ffile,"READ");
		    if(f==NULL){cout << "Something Wrong!!" << endl;} else {cout << ffile.Data() << endl;}        
            TH1D *h_CPO3;
            if(opt == "recov1")
            {
                h_CPO3 = (TH1D*)f->Get(Form("%s",vCPO3[icp].c_str()));
                if(h_CPO3==NULL){cout << Form("empty %s.......",vCPO3[icp].c_str()) << endl;}
            }
            else if(opt == "gen" || opt == "recov2" || opt == "ref")
            {
                h_CPO3 = (TH1D*)f->Get(Form("%s",vCPO3[0].c_str()));
                if(h_CPO3==NULL){cout << Form("empty %s.......",vCPO3[0].c_str()) << endl;}
            }
            tmp[icp][idtg] = O3Asym(h_CPO3, version, savepath, vdtG[idtg], xsec[idtg], fout);
            vdtG_Yval[icp][idtg] = tmp[icp][idtg].asym_;
            vdtG_StatErr[icp][idtg] = tmp[icp][idtg].asym_err_;
            cout << vdtG_Yval[icp][idtg] << endl;
        }
    }

    ///// draw linearity plot /////
    for(int ilin=0; ilin<vCPO3.size(); ilin++)
    {
        TMultiGraph *multiGraph = new TMultiGraph();
        double xmin = -2.9;
        double xmax = 2.9;
        TCanvas *clin = new TCanvas("clin","clin",800,800);
        clin->cd();
        clin->SetGridx();
        clin->SetGridy();
        clin->SetMargin(0.12,0.02,0.12,0.02);

        TGraph *g_reco = new TGraphAsymmErrors(7,vdtG_Xval,vdtG_Yval[ilin],0,0,vdtG_StatErr[ilin],vdtG_StatErr[ilin]);

        /// fit & fitting band ///
        TF1 *flinear = new TF1("flinear", "pol1", xmin, xmax);
        g_reco->Fit("flinear", "R");
        /// 1σ, 2σ uncertainty band ///
        const int npoint = 100;
        double x[npoint], y[npoint];
        double y1Err[npoint], y2Err[npoint];
        double slope = flinear->GetParameter(1);
        for (int i = 0; i < npoint; i++) {x[i] = -2.60364 + (2.60364 + 2.60364) * i / (npoint - 1);}
        // 1σ 및 2σ 신뢰 구간 계산
        for (int i = 0; i < npoint; i++)
        {
            y[i] = flinear->Eval(x[i]);
            double error1 = sqrt(pow(flinear->GetParError(0), 2) + pow(x[i] * flinear->GetParError(1), 2));
            double error2 = 2 * error1;

            y1Err[i] = error1; // 1σ 상한
            y2Err[i] = error2; // 1σ 하한
            //cout << "y: " << y[i] << ", error1: " << error1 << ", error2: " << error2 << ", y1Err: " << y1Err[i] << ", y2Err: " << y2Err[i] << endl;
        }
        /// uncertainty band setting ///
        TGraph *twoSig = new TGraphErrors(npoint,x,y,0,y2Err);
        twoSig->SetFillColor(kGreen);
        twoSig->SetFillStyle(1001);
        TGraph *oneSig = new TGraphErrors(npoint,x,y,0,y1Err);
        oneSig->SetFillStyle(1001);
        oneSig->SetFillColor(kYellow);
        /// plot setting ///
        g_reco->SetTitle("");
        g_reco->GetXaxis()->SetRangeUser(xmin,xmax);
        if(opt == "recov1" || opt == "recov2" || opt == "ref") {g_reco->GetYaxis()->SetRangeUser(-0.08,0.08);}
        else if(opt == "gen") {g_reco->GetYaxis()->SetRangeUser(-0.12,0.12);}
        g_reco->GetYaxis()->SetTitleOffset(1.5);
        g_reco->SetMarkerStyle(kFullSquare);
        g_reco->SetMarkerColor(kBlack);
        /// plot add to draw in single canvas ///
        multiGraph->Add(twoSig, "3");
        multiGraph->Add(oneSig, "3");
        multiGraph->Add(g_reco,"P");
        multiGraph->Draw("A");
        multiGraph->GetXaxis()->SetTitle("d_{tG}");
        multiGraph->GetYaxis()->SetTitle("Asymmetry in O_{3}");
        multiGraph->GetXaxis()->SetTitleOffset(1.2);
        multiGraph->GetYaxis()->SetTitleOffset(1.5);
        multiGraph->GetXaxis()->SetRangeUser(-2.9,2.9);
        if(opt == "recov1" || opt == "recov2" || opt == "ref") {multiGraph->GetYaxis()->SetRangeUser(-0.08,0.08);}
        else if(opt == "gen") {multiGraph->GetYaxis()->SetRangeUser(-0.12,0.12);}
        /// add O3 asymmetry value using likelihood fit ///
        line = new TLine(xmin, vLike_Yval, xmax, vLike_Yval);
        line->SetLineColor(kRed);
        line->Draw("same");
        TBox *vLike_Box = new TBox(vdtG_Xval[0], vLike_Yval - vLike_StatErr_m, vdtG_Xval[6], vLike_Yval + vLike_StatErr_p); //x1,y1,x2,y2
        vLike_Box->SetFillStyle(1001); // solid로 설정
        vLike_Box->SetFillColorAlpha(kBlue, 0.2); // 20% 반투명하게 설정
        vLike_Box->Draw("same");
        /// add text ///
        text.SetNDC();
        text.SetTextSize(0.03);
        text.SetTextFont(42);
        text.DrawLatex(0.2,0.9,Form("#mu^{+}#mu^{-} channel (%s)",opt.c_str()));
        /// add legend ///
        lleg = new TLegend(0.2, 0.65, 0.5, 0.85);
        lleg->AddEntry(line, "Data O_{3} Asymmetry", "l");
        lleg->AddEntry(vLike_Box, "Data O_{3} Asymmetry Stat. Err.", "f");
        lleg->AddEntry(oneSig, "68% expected", "f");
        lleg->AddEntry(twoSig, "95% expected", "f");
        lleg->AddEntry(flinear, Form("slope: %.3f",slope), "");
        lleg->SetFillStyle(0);
        lleg->SetFillColor(0);
        lleg->SetBorderSize(0);
        lleg->Draw();
        /// save canvas ///
        clin->Update();
        clin->Modified();
        if(opt == "recov1") {{clin->SaveAs(Form("%s/dR_%s/CPV_Linearity.pdf",savepath.c_str(), dR_values[ilin].c_str()));}}
        else if(opt == "gen" || opt == "recov2" || opt == "ref") {clin->SaveAs(Form("%s/CPV_Linearity.pdf",savepath.c_str()));}

        delete clin;
    }
}
CPVari O3Asym(TH1D *hist, string version, string savepath, string vdtG, double xsec, ostream &fout)
{
    void DrawOverflowBin(TH1D *his, double min, double max);
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

    vector<string> dR_values = {"0p5", "1p0", "1p5", "2p0", "2p5", "3p0", "10p0"};
	vector<string> dR_idx = {"0","1","2","3","4","5","6"};

    TLegend *leg;

	TString hname = hist->GetName();
	TCanvas *c = new TCanvas("c",hname,800,800);
    c->cd();
    c->SetLogy();
    c->SetMargin(0.12,0.02,0.12,0.02);
    xbin = hist->GetNbinsX();
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
    norfac = ref_val/xsec;
    total = (posCount + negCount) * norfac;
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

    fout << Form("<<<<<< %s >>>>>>",hname.Data()) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Nega: %.6f", negCount) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Posi: %.6f", posCount) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Asym: %.6f", Asym) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Asym Err: %.6f", AsymErr) << endl;
    fout << Form("%s_",vdtG.c_str()) << Form("Total: %.6f" ,total) << endl;
    fout << endl;

    DrawOverflowBin(hist, -2, 2);
	// save canvas
	for(int i=0; i<dR_idx.size(); i++)
	{
		if(hname.Contains("_" + dR_idx[i] + "_"))
		{
			gSystem->mkdir(Form("%s/dR_%s/",savepath.c_str(), dR_values[i].c_str()),kTRUE);
			c->SaveAs(Form("%s/dR_%s/%s_%s.pdf",savepath.c_str(), dR_values[i].c_str(),vdtG.c_str(), hist->GetName()));
		}
	}
	if(!hname.Contains("_0_") && !hname.Contains("_1_") && !hname.Contains("_2_") && !hname.Contains("_3_") && !hname.Contains("_4_") && !hname.Contains("_5_") && !hname.Contains("_6_"))
	{c->SaveAs(Form("%s/%s_%s.pdf",savepath.c_str(),vdtG.c_str(),hist->GetName()));}

    return tmp;
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
//CPVari Auto_ControlPlot::CalAsymVari(TH1F* h1)
/*CPVari CalAsymVari(TH1D* h1,string version, string savepath, string vdtG, double xsec, ostream &fout)
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
}*/