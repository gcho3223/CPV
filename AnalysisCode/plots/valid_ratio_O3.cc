#include "TSystem.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TLine.h"

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
vector<string> samplelist = {
    "Data_DoubleMuon_Run2016B","Data_DoubleMuon_Run2016C","Data_DoubleMuon_Run2016D","Data_DoubleMuon_Run2016E","Data_DoubleMuon_Run2016F","Data_DoubleMuon_Run2016G","Data_DoubleMuon_Run2016HV2","Data_DoubleMuon_Run2016HV3",
    "Data_SingleMuon_Run2016B","Data_SingleMuon_Run2016C","Data_SingleMuon_Run2016D","Data_SingleMuon_Run2016E","Data_SingleMuon_Run2016F","Data_SingleMuon_Run2016G","Data_SingleMuon_Run2016HV2","Data_SingleMuon_Run2016HV3",
    "DYJetsToLL_M_10To50","DYJetsToLL_M_50",
    "ST_tW_antitop","ST_tW_top",
    "TTbar_WJetToLNu","TTbar_WQQ","TTbar_ZQQ","TTbar_ZToLLNuNu",
    "TTJets_others","TTJets_Signal",
    "WW","WZ","ZZ", //29
    "Data"
};
void valid_ratio_O3(string version)
{
    //void ForData(TH1D *h_Single,TH1D *h_Double,TH1D *h_Data,TH1D *h_CPV, string list, string dir); //single&double included
    void ForData(TH1D *h_Data,TH1D *h_CPV, string list, string dir, string hO3); // combined data
    void DrawOverflowBin(TH1D *his, double min, double max);
    CPVari O3Asym(TH1D *hist);
    CPVari tmp;

    std::string dir = Form("./Job_Version/%s/",version.c_str());
    gSystem->mkdir(Form("%s",dir.c_str()), kTRUE);

    std::string totalEventCountFilename = dir + "/Compare_O3Asym.txt";
    std::ofstream totalEventCountFile(totalEventCountFilename);

    string hO3  = "h_Reco_CPO3";

    gSystem->mkdir(Form("%s",dir.c_str()), kTRUE);
    for(int sampleIndex = 0; sampleIndex < samplelist.size(); sampleIndex++)
    {
        if(samplelist[sampleIndex] == "Data")
        {
            //TFile *fileRepro_Single = new TFile(Form("../output/Job_Version/%s/Dataset/Data_SingleMuon/Data_SingleMuon.root",version.c_str()), "READ");
            //TFile *fileRepro_Double = new TFile(Form("../output/Job_Version/%s/Dataset/Data_DoubleMuon/Data_DoubleMuon.root",version.c_str()), "READ");
            TFile *fileRepro_Data = new TFile(Form("../output/Job_Version/%s/Dataset/Data/Data.root",version.c_str()), "READ");
            TFile *fileCPV = new TFile(Form("../output/Validation/CPV_MuMu/Data.root"), "READ");

            //TH1D *hRepro_Single = (TH1D*)fileRepro_Single->Get(Form("%s",hO3.c_str()));
            //TH1D *hRepro_Double = (TH1D*)fileRepro_Double->Get(Form("%s",hO3.c_str()));
            TH1D *hRepro_Data = (TH1D*)fileRepro_Data->Get(Form("%s",hO3.c_str()));
            TH1D *hCPV = (TH1D*)fileCPV->Get(Form("h_Reco_CPO3"));
            //ForData(hRepro_Single,hRepro_Double,hRepro_Data,hCPV,samplelist[sampleIndex],dir);
            if(hRepro_Data==nullptr || hCPV==nullptr) {cout << "hRepro_Data or hCPV is nullptr" << endl; continue;}
            ForData(hRepro_Data,hCPV,samplelist[sampleIndex],dir,hO3);

            //fileRepro_Single->Close();
            //fileRepro_Double->Close();
            fileRepro_Data->Close();
            fileCPV->Close();
            //delete fileRepro_Single;
            //delete fileRepro_Double;
            delete fileRepro_Data;
            delete fileCPV;
        }
        TFile *fileRepro = new TFile(Form("../output/Job_Version/%s/Dataset/%s/%s_All.root", version.c_str(), samplelist[sampleIndex].c_str(), samplelist[sampleIndex].c_str()), "READ");
        TFile *fileCPV = new TFile(Form("../output/Validation/CPV_MuMu/%s.root", samplelist[sampleIndex].c_str()), "READ");
        TCanvas *c1 = new TCanvas(Form("c1_%s_1", samplelist[sampleIndex].c_str()), "Validation Ratio Plot", 800, 800);

        TH1D *hRepro = (TH1D*)fileRepro->Get(Form("%s",hO3.c_str()));
        TH1D *hCPV = (TH1D*)fileCPV->Get(Form("h_Reco_CPO3"));

        if(hRepro && hCPV)
        {
            TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
            pad1->SetBottomMargin(0.03);
            pad1->Draw();
            pad1->SetLogy();

            TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
            pad2->SetTopMargin(0);
            pad2->SetBottomMargin(0.3);
            pad2->Draw();
            
            pad1->cd();
            if(samplelist[sampleIndex].find("Data") != std::string::npos)
            {
                hRepro->SetLineColor(kRed);
                hRepro->SetLineWidth(2);
                hCPV->SetMarkerColor(kBlack);
                hCPV->SetMarkerStyle(kFullCircle);
                hCPV->SetMarkerSize(.75);
                hCPV->Draw("P");
                hRepro->Draw("hist same");
            }
            else
            {
                hRepro->SetLineColor(kRed);
                hRepro->SetLineWidth(2);
                hCPV->SetMarkerColor(kBlack);
                hCPV->SetMarkerStyle(kFullCircle);
                hCPV->SetMarkerSize(.75);
                hRepro->Draw("hist");
                hCPV->Draw("P same");
            }
            hRepro->SetStats(kFALSE);
            hCPV->SetStats(kFALSE);
            hRepro->GetXaxis()->SetLabelSize(0);
            hCPV->GetXaxis()->SetLabelSize(0);
            hRepro->SetTitle("");
            hCPV->SetTitle("");

            tmp = O3Asym(hRepro);
            TLegend *legendRepro = new TLegend(0.62, 0.75, 0.9, 0.9);
            legendRepro->AddEntry(hRepro, Form("Reproduce"), "l");
            legendRepro->AddEntry(hRepro, Form("Event: %.4f",tmp.num_total_), "");
            legendRepro->AddEntry(hRepro, Form("N (O_{3} < 0): %.4f",tmp.num_m_), "");
            legendRepro->AddEntry(hRepro, Form("N (O_{3} > 0): %.4f",tmp.num_p_), "");
            legendRepro->AddEntry(hRepro, Form("Asym: %.5f",tmp.asym_), "");
            legendRepro->SetLineColor(kRed);
            legendRepro->SetTextFont(42);
            legendRepro->SetTextSize(0.03);
            legendRepro->SetTextColor(kRed);
            legendRepro->Draw();

            tmp = O3Asym(hCPV);
            TLegend *legendCPV = new TLegend(0.62, 0.6, 0.9, 0.75);
            legendCPV->AddEntry(hCPV, Form("Sk's output"), "p");
            legendCPV->AddEntry(hCPV, Form("Event: %.4f",tmp.num_total_), "");
            legendCPV->AddEntry(hCPV, Form("N (O_{3} < 0): %.4f",tmp.num_m_), "");
            legendCPV->AddEntry(hCPV, Form("N (O_{3} > 0): %.4f",tmp.num_p_), "");
            legendCPV->AddEntry(hCPV, Form("Asym: %.5f",tmp.asym_), "");
            legendCPV->SetLineColor(kBlack);
            legendCPV->SetTextFont(42);
            legendCPV->SetTextSize(0.03);
            legendCPV->SetTextColor(kBlack);
            legendCPV->Draw();
            
            pad2->cd();
            pad2->SetGridy();
            TH1D *hRatio = (TH1D*)hCPV->Clone("hRatio");
            hRatio->Divide(hRepro);
            hRatio->SetTitle("");
            hRatio->GetXaxis()->SetLabelSize(0.07);
            hRatio->GetYaxis()->SetLabelSize(0.07);
            hRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
            hRatio->GetYaxis()->SetNdivisions(10);
            hRatio->GetYaxis()->SetTitleOffset(1.5);
            hRatio->Draw("E");

            DrawOverflowBin(hCPV, -2, 2);
            DrawOverflowBin(hRepro, -2, 2);
            DrawOverflowBin(hRatio, -2, 2);

            TLine *line = new TLine(-2, 1, 2, 1);
            line->SetLineColor(kRed);
            line->Draw();

            totalEventCountFile << samplelist[sampleIndex] << " O3: Reproduce Events: " << Form("%.2f",hRepro->Integral()) << ", AN Events: " << Form("%.2f",hCPV->Integral()) << ", Ratio: " << Form("%.2f",hRepro->Integral()/hCPV->Integral()) << std::endl;

            std::string subdir = Form("%s/O3/%s/",dir.c_str(),hO3.c_str());
            gSystem->mkdir(subdir.c_str(), kTRUE);
            c1->Print(Form("%s/Compare_O3Asym_%s.pdf",subdir.c_str(),samplelist[sampleIndex].c_str()));

            fileRepro->Close();
            fileCPV->Close();
            delete fileRepro;
            delete fileCPV;
        }
    }
}
//void ForData(TH1D *h_Single,TH1D *h_Double,TH1D *h_Data,TH1D *h_CPV, string list, string dir)
void ForData(TH1D *h_Data,TH1D *h_CPV, string list, string dir, string hO3)
{
    void DrawOverflowBin(TH1D *his, double min, double max);
    CPVari O3Asym(TH1D *hist);
    CPVari tmp;

    TCanvas *c1 = new TCanvas(Form("c1_%s", list.c_str()), "Validation Ratio Plot", 800, 800);

    TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
    pad1->SetBottomMargin(0.03);
    pad1->Draw();
    pad1->SetLogy();
    TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    
    pad1->cd();
    h_Data->SetLineColor(kRed);
    h_Data->SetLineWidth(2);
    //h_Single->SetLineColor(kMagenta);
    //h_Single->SetLineWidth(2);
    //h_Double->SetLineColor(kBlue);
    //h_Double->SetLineWidth(2);
    h_CPV->SetMarkerColor(kBlack);
    h_CPV->SetMarkerStyle(kFullCircle);
    h_CPV->SetMarkerSize(.75);
    h_Data->Draw("hist");
    //h_Single->Draw("hist same");
    //h_Double->Draw("hist same");
    h_CPV->Draw("P same");
    
    h_Data->SetStats(kFALSE);
    //h_Single->SetStats(kFALSE);
    //h_Double->SetStats(kFALSE);
    h_CPV->SetStats(kFALSE);
    h_Data->GetXaxis()->SetLabelSize(0);
    //h_Single->GetXaxis()->SetLabelSize(0);
    //h_Double->GetXaxis()->SetLabelSize(0);
    h_CPV->GetXaxis()->SetLabelSize(0);
    h_Data->SetTitle("");
    //h_Single->SetTitle("");
    //h_Double->SetTitle("");
    h_CPV->SetTitle("");

    tmp = O3Asym(h_Data);
    TLegend *legendData = new TLegend(0.67, 0.75, 0.9, 0.9);
    legendData->AddEntry(h_Data, "reproduce", "l");
    legendData->AddEntry(h_Data, Form("Event: %.4f",tmp.num_total_), "");
    legendData->AddEntry(h_Data, Form("N (O_{3} < 0): %.4f",tmp.num_m_), "");
    legendData->AddEntry(h_Data, Form("N (O_{3} > 0): %.4f",tmp.num_p_), "");
    legendData->AddEntry(h_Data, Form("Asym: %.5f",tmp.asym_), "");
    legendData->SetLineColor(kRed);
    legendData->SetTextFont(42);
    legendData->SetTextSize(0.03);
    legendData->SetTextColor(kRed);
    legendData->Draw();

    tmp = O3Asym(h_CPV);
    TLegend *legendCPV = new TLegend(0.67, 0.6, 0.9, 0.75);
    legendCPV->AddEntry(h_CPV, "Sk's output", "p");
    legendCPV->AddEntry(h_CPV, Form("Event: %.4f",tmp.num_total_), "");
    legendCPV->AddEntry(h_CPV, Form("N (O_{3} < 0): %.4f",tmp.num_m_), "");
    legendCPV->AddEntry(h_CPV, Form("N (O_{3} > 0): %.4f",tmp.num_p_), "");
    legendCPV->AddEntry(h_CPV, Form("Asym: %.5f",tmp.asym_), "");
    legendCPV->SetLineColor(kBlack);
    legendCPV->SetTextFont(42);
    legendCPV->SetTextSize(0.03);
    legendCPV->SetTextColor(kBlack);
    legendCPV->Draw();

    //tmp = O3Asym(h_Single);
    //TLegend *legendSingle = new TLegend(0.67, 0.45, 0.9, 0.6);
    //legendSingle->AddEntry(h_Single, "SingleMuon", "l");
    //legendSingle->AddEntry(h_Single, Form("Event: %.4f",tmp.num_total_), "");
    //legendSingle->AddEntry(h_Single, Form("N (O_{3} < 0): %.4f",tmp.num_m_), "");
    //legendSingle->AddEntry(h_Single, Form("N (O_{3} > 0): %.4f",tmp.num_p_), "");
    //legendSingle->AddEntry(h_Single, Form("Asym: %.5f",tmp.asym_), "");
    //legendSingle->SetLineColor(kMagenta);
    //legendSingle->SetTextFont(42);
    //legendSingle->SetTextSize(0.03);
    //legendSingle->SetTextColor(kMagenta);
    //legendSingle->Draw();

    //tmp = O3Asym(h_Double);
    //TLegend *legendDouble = new TLegend(0.67, 0.3, 0.9, 0.45);
    //legendDouble->AddEntry(h_Double, "DoubleMuon", "l");
    //legendDouble->AddEntry(h_Double, Form("Event: %.4f",tmp.num_total_), "");
    //legendDouble->AddEntry(h_Double, Form("N (O_{3} < 0): %.4f",tmp.num_m_), "");
    //legendDouble->AddEntry(h_Double, Form("N (O_{3} > 0): %.4f",tmp.num_p_), "");
    //legendDouble->AddEntry(h_Double, Form("Asym: %.5f",tmp.asym_), "");
    //legendDouble->SetLineColor(kBlue);
    //legendDouble->SetTextFont(42);
    //legendDouble->SetTextSize(0.03);
    //legendDouble->SetTextColor(kBlue);
    //legendDouble->Draw();

    pad2->cd();
    pad2->SetGridy();
    //TH1D *hRatio_Single = (TH1D*)h_CPV->Clone("hRatio_Single");
    //hRatio_Single->Divide(h_Single);
    //hRatio_Single->SetTitle("");
    //hRatio_Single->GetXaxis()->SetLabelSize(0.07);
    //hRatio_Single->GetYaxis()->SetLabelSize(0.07);
    //hRatio_Single->GetYaxis()->SetRangeUser(0.5, 1.5);
    //hRatio_Single->GetYaxis()->SetNdivisions(10);
    //hRatio_Single->GetYaxis()->SetTitleOffset(1.5);
    //hRatio_Single->SetMarkerColor(kMagenta);
    //hRatio_Single->SetLineColor(kMagenta);
    //hRatio_Single->SetMarkerSize(0.5);
    //hRatio_Single->Draw("E");

    //TH1D *hRatio_Double = (TH1D*)h_CPV->Clone("hRatio_Double");
    //hRatio_Double->Divide(h_Double);
    //hRatio_Double->SetTitle("");
    //hRatio_Double->GetXaxis()->SetLabelSize(0.07);
    //hRatio_Double->GetYaxis()->SetLabelSize(0.07);
    //hRatio_Double->GetYaxis()->SetRangeUser(0.5, 1.5);
    //hRatio_Double->GetYaxis()->SetNdivisions(10);
    //hRatio_Double->GetYaxis()->SetTitleOffset(1.5);
    //hRatio_Double->SetMarkerColor(kBlue);
    //hRatio_Double->SetLineColor(kBlue);
    //hRatio_Double->SetMarkerSize(0.5);
    //hRatio_Double->Draw("E same");

    TH1D *hRatio_Data = (TH1D*)h_CPV->Clone("hRatio_Data");
    hRatio_Data->Divide(h_Data);
    hRatio_Data->SetTitle("");
    hRatio_Data->GetXaxis()->SetLabelSize(0.07);
    hRatio_Data->GetYaxis()->SetLabelSize(0.07);
    hRatio_Data->GetYaxis()->SetRangeUser(0.5, 1.5);
    hRatio_Data->GetYaxis()->SetNdivisions(10);
    hRatio_Data->GetYaxis()->SetTitleOffset(1.5);
    hRatio_Data->SetLineColor(kRed);
    hRatio_Data->SetMarkerColor(kRed);
    hRatio_Data->SetMarkerSize(0.5);
    hRatio_Data->Draw("E");

    DrawOverflowBin(h_Data, -2, 2);
    //DrawOverflowBin(h_Single, -2, 2);
    //DrawOverflowBin(h_Double, -2, 2);
    //DrawOverflowBin(hRatio_Single, -2, 2);
    //DrawOverflowBin(hRatio_Double, -2, 2);
    DrawOverflowBin(hRatio_Data, -2, 2);
    
    TLine *line = new TLine(-2, 1, 2, 1);
    line->SetLineColor(kRed);
    line->Draw();

    std::string subdir = Form("%s/O3/%s/",dir.c_str(),hO3.c_str());
    gSystem->mkdir(subdir.c_str(), kTRUE);
    c1->Print(Form("%s/Compare_O3Asym_%s.pdf",subdir.c_str(),list.c_str()));
}
CPVari O3Asym(TH1D *hist)
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