#include "TSystem.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TLine.h"

vector<string> samplelist = {
    "Data_DoubleMuon_Run2016B","Data_DoubleMuon_Run2016C","Data_DoubleMuon_Run2016D","Data_DoubleMuon_Run2016E","Data_DoubleMuon_Run2016F","Data_DoubleMuon_Run2016G","Data_DoubleMuon_Run2016HV2","Data_DoubleMuon_Run2016HV3",
    "Data_SingleMuon_Run2016B","Data_SingleMuon_Run2016C","Data_SingleMuon_Run2016D","Data_SingleMuon_Run2016E","Data_SingleMuon_Run2016F","Data_SingleMuon_Run2016G","Data_SingleMuon_Run2016HV2","Data_SingleMuon_Run2016HV3",
    //"DYJetsToLL_M_10To50","DYJetsToLL_M_50",
    //"ST_tW_antitop","ST_tW_top",
    //"TTbar_WJetToLNu","TTbar_WQQ","TTbar_ZQQ","TTbar_ZToLLNuNu",
    //"TTJets_others","TTJets_Signal",
    //"WW","WZ","ZZ", //29
    "Data"
};
vector<string> hlist = {
    "_h_cf_NPV",
    //"_h_cf_Jet1pt","_h_cf_Jet1eta","_h_cf_Jet1phi",
    //"_h_cf_Jet2pt","_h_cf_Jet2eta","_h_cf_Jet2phi",
    //"_h_cf_Lep1pt","_h_cf_Lep1eta","_h_cf_Lep1phi",
    //"_h_cf_Lep2pt","_h_cf_Lep2eta","_h_cf_Lep2phi",
};
void valid_ratio(string version)
{
    void ForData(TH1F *h_Single,TH1F *h_Double,TH1F *h_Data,TH1F *h_CPV, string list, int idx, string dir);

    std::string dir = "./valid_v4_data_duplEvt";
    gSystem->mkdir(Form("%s",dir.c_str()), kTRUE);

    std::string totalEventCountFilename = dir + "/TotalEventCount_v4_data_duplEvt.txt";
    std::ofstream totalEventCountFile(totalEventCountFilename);

    for(int istep = 1; istep <= 8; istep++) // step 1 ~ step 8(top reconstruction)
    {
        gSystem->mkdir(Form("%s/step_%d",dir.c_str(),istep), kTRUE);
        for(int sampleIndex = 0; sampleIndex < samplelist.size(); sampleIndex++)
        {
            for(int ihist=0; ihist<hlist.size(); ihist++)
            {
                if(samplelist[sampleIndex] == "Data")
                {
                    TFile *fileRepro_Single = new TFile(Form("../output/%s/Dataset/Data/Data_SingleMuon.root",version.c_str()), "READ");
                    TFile *fileRepro_Double = new TFile(Form("../output/%s/Dataset/Data/Data_DoubleMuon.root",version.c_str()), "READ");
                    TFile *fileRepro_Data = new TFile(Form("../output/%s/Dataset/Data/Data.root",version.c_str()), "READ");
                    TFile *fileCPV = new TFile(Form("../output/Validation/CPV_MuMu/Data.root"), "READ");

                    TH1F *hRepro_Single = (TH1F*)fileRepro_Single->Get(Form("%s_%d_",hlist[ihist].c_str(), istep));
                    TH1F *hRepro_Double = (TH1F*)fileRepro_Double->Get(Form("%s_%d_",hlist[ihist].c_str(), istep));
                    TH1F *hRepro_Data = (TH1F*)fileRepro_Data->Get(Form("%s_%d_",hlist[ihist].c_str(), istep));
                    TH1F *hCPV = (TH1F*)fileCPV->Get(Form("%s_%d_",hlist[ihist].c_str(), istep));
                    ForData(hRepro_Single,hRepro_Double,hRepro_Data,hCPV,samplelist[sampleIndex],istep,dir);

                    fileRepro_Single->Close();
                    fileRepro_Double->Close();
                    fileRepro_Data->Close();
                    fileCPV->Close();
                    delete fileRepro_Single;
                    delete fileRepro_Double;
                    delete fileRepro_Data;
                    delete fileCPV;
                }
                if(samplelist[sampleIndex] == "Data") continue;
                TFile *fileRepro = new TFile(Form("../output/%s/Dataset/%s/%s_All.root", version.c_str(), samplelist[sampleIndex].c_str(), samplelist[sampleIndex].c_str()), "READ");
                TFile *fileCPV = new TFile(Form("../output/Validation/CPV_MuMu/%s.root", samplelist[sampleIndex].c_str()), "READ");
                TCanvas *c1 = new TCanvas(Form("c1_%s_%d", samplelist[sampleIndex].c_str(), istep), "Validation Ratio Plot", 800, 800);

                TH1F *hRepro = (TH1F*)fileRepro->Get(Form("%s_%d_",hlist[ihist].c_str(), istep));
                TH1F *hCPV = (TH1F*)fileCPV->Get(Form("%s_%d_",hlist[ihist].c_str(), istep));

                if(hRepro && hCPV)
                {
                    TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
                    pad1->SetBottomMargin(0.03);
                    pad1->Draw();

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

                    TLegend *legendRepro = new TLegend(0.67, 0.75, 0.9, 0.9);
                    legendRepro->AddEntry(hRepro, Form("Reproduce"), "l");
                    legendRepro->AddEntry(hRepro, Form("Entries: %.2f", hRepro->Integral()), "");
                    legendRepro->AddEntry(hRepro, Form("Mean: %.2f", hRepro->GetMean()), "");
                    legendRepro->AddEntry(hRepro, Form("Std dev: %.2f", hRepro->GetStdDev()), "");
                    legendRepro->SetLineColor(kRed);
                    legendRepro->SetTextFont(42);
                    legendRepro->SetTextSize(0.03);
                    legendRepro->SetTextColor(kRed);
                    legendRepro->Draw();

                    TLegend *legendCPV = new TLegend(0.67, 0.6, 0.9, 0.75);
                    legendCPV->AddEntry(hCPV, Form("Sk's output"), "p");
                    legendCPV->AddEntry(hCPV, Form("Entries: %.2f", hCPV->Integral()), "");
                    legendCPV->AddEntry(hCPV, Form("Mean: %.2f", hCPV->GetMean()), "");
                    legendCPV->AddEntry(hCPV, Form("Std dev: %.2f", hCPV->GetStdDev()), "");
                    legendCPV->SetLineColor(kBlack);
                    legendCPV->SetTextFont(42);
                    legendCPV->SetTextSize(0.03);
                    legendCPV->SetTextColor(kBlack);
                    legendCPV->Draw();
                
                    pad2->cd();
                    pad2->SetGridy();
                    TH1F *hRatio = (TH1F*)hCPV->Clone("hRatio");
                    hRatio->Divide(hRepro);
                    hRatio->SetTitle("");
                    hRatio->GetXaxis()->SetLabelSize(0.07);
                    hRatio->GetYaxis()->SetLabelSize(0.07);
                    hRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
                    hRatio->GetYaxis()->SetNdivisions(10);
                    hRatio->GetYaxis()->SetTitleOffset(1.5);
                    hRatio->Draw("E");

                    TLine *line = new TLine(hRatio->GetXaxis()->GetXmin(), 1, hRatio->GetXaxis()->GetXmax(), 1);
                    line->SetLineColor(kRed);
                    line->Draw();

                    totalEventCountFile << samplelist[sampleIndex] << " Step " << istep << ": Reproduce Events: " << Form("%.2f",hRepro->Integral()) << ", AN Events: " << Form("%.2f",hCPV->Integral()) << ", Ratio: " << Form("%.2f",hRepro->Integral()/hCPV->Integral()) << std::endl;

                    std::string subdir = dir + "/step_" + std::to_string(istep) + "/" + samplelist[sampleIndex];
                    gSystem->mkdir(subdir.c_str(), kTRUE);
                    c1->Print(Form("%s/Valid%s_%s_step%d.pdf",subdir.c_str(),hlist[ihist].c_str(),samplelist[sampleIndex].c_str(),istep));

                    fileRepro->Close();
                    fileCPV->Close();
                    delete fileRepro;
                    delete fileCPV;
                }
            }//end hist list
        }//end samples
        totalEventCountFile << std::endl;
    }//end steps
    
    totalEventCountFile.close();
}
void ForData(TH1F *h_Single,TH1F *h_Double,TH1F *h_Data,TH1F *h_CPV, string list, int idx, string dir)
{
    TCanvas *c1 = new TCanvas(Form("c1_%s_%s", list.c_str(), std::to_string(idx).c_str()), "Validation Ratio Plot", 800, 800);

    TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
    pad1->SetBottomMargin(0.03);
    pad1->Draw();
    TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    
    pad1->cd();
    h_Data->SetLineColor(kRed);
    h_Data->SetLineWidth(2);
    h_Single->SetLineColor(kMagenta);
    h_Single->SetLineWidth(2);
    h_Double->SetLineColor(kBlue);
    h_Double->SetLineWidth(2);
    h_CPV->SetMarkerColor(kBlack);
    h_CPV->SetMarkerStyle(kFullCircle);
    h_CPV->SetMarkerSize(.75);
    h_CPV->Draw("P");
    h_Data->Draw("hist same");
    h_Single->Draw("hist same");
    h_Double->Draw("hist same");
    
    h_Data->SetStats(kFALSE);
    h_Single->SetStats(kFALSE);
    h_Double->SetStats(kFALSE);
    h_CPV->SetStats(kFALSE);
    h_Data->GetXaxis()->SetLabelSize(0);
    h_Single->GetXaxis()->SetLabelSize(0);
    h_Double->GetXaxis()->SetLabelSize(0);
    h_CPV->GetXaxis()->SetLabelSize(0);
    h_Data->SetTitle("");
    h_Single->SetTitle("");
    h_Double->SetTitle("");
    h_CPV->SetTitle("");

    TLegend *legendCPV = new TLegend(0.67, 0.75, 0.9, 0.9);
    legendCPV->AddEntry(h_CPV, "Sk's output", "p");
    legendCPV->AddEntry(h_CPV, Form("Entries: %.0f", h_CPV->Integral()), "");
    legendCPV->AddEntry(h_CPV, Form("Mean: %.2f", h_CPV->GetMean()), "");
    legendCPV->AddEntry(h_CPV, Form("Std dev: %.2f", h_CPV->GetStdDev()), "");
    legendCPV->SetLineColor(kBlack);
    legendCPV->SetTextFont(42);
    legendCPV->SetTextSize(0.03);
    legendCPV->SetTextColor(kBlack);
    legendCPV->Draw();

    TLegend *legendData = new TLegend(0.67, 0.6, 0.9, 0.75);
    legendData->AddEntry(h_Data, "reproduce", "l");
    legendData->AddEntry(h_Data, Form("Entries: %.0f", h_Data->Integral()), "");
    legendData->AddEntry(h_Data, Form("Mean: %.2f", h_Data->GetMean()), "");
    legendData->AddEntry(h_Data, Form("Std dev: %.2f", h_Data->GetStdDev()), "");
    legendData->SetLineColor(kRed);
    legendData->SetTextFont(42);
    legendData->SetTextSize(0.03);
    legendData->SetTextColor(kRed);
    legendData->Draw();

    TLegend *legendSingle = new TLegend(0.67, 0.45, 0.9, 0.6);
    legendSingle->AddEntry(h_Single, "SingleMuon", "l");
    legendSingle->AddEntry(h_Single, Form("Entries: %.0f", h_Single->Integral()), "");
    legendSingle->AddEntry(h_Single, Form("Mean: %.2f", h_Single->GetMean()), "");
    legendSingle->AddEntry(h_Single, Form("Std dev: %.2f", h_Single->GetStdDev()), "");
    legendSingle->SetLineColor(kMagenta);
    legendSingle->SetTextFont(42);
    legendSingle->SetTextSize(0.03);
    legendSingle->SetTextColor(kMagenta);
    legendSingle->Draw();

    TLegend *legendDouble = new TLegend(0.67, 0.3, 0.9, 0.45);
    legendDouble->AddEntry(h_Double, "DoubleMuon", "l");
    legendDouble->AddEntry(h_Double, Form("Entries: %.0f", h_Double->Integral()), "");
    legendDouble->AddEntry(h_Double, Form("Mean: %.2f", h_Double->GetMean()), "");
    legendDouble->AddEntry(h_Double, Form("Std dev: %.2f", h_Double->GetStdDev()), "");
    legendDouble->SetLineColor(kBlue);
    legendDouble->SetTextFont(42);
    legendDouble->SetTextSize(0.03);
    legendDouble->SetTextColor(kBlue);
    legendDouble->Draw();

    pad2->cd();
    pad2->SetGridy();
    TH1F *hRatio_Data = (TH1F*)h_CPV->Clone("hRatio_Data");
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

    TH1F *hRatio_Single = (TH1F*)h_CPV->Clone("hRatio_Single");
    hRatio_Single->Divide(h_Single);
    hRatio_Single->SetTitle("");
    hRatio_Single->GetXaxis()->SetLabelSize(0.07);
    hRatio_Single->GetYaxis()->SetLabelSize(0.07);
    hRatio_Single->GetYaxis()->SetRangeUser(0.5, 1.5);
    hRatio_Single->GetYaxis()->SetNdivisions(10);
    hRatio_Single->GetYaxis()->SetTitleOffset(1.5);
    hRatio_Single->SetMarkerColor(kMagenta);
    hRatio_Single->SetLineColor(kMagenta);
    hRatio_Single->SetMarkerSize(0.5);
    hRatio_Single->Draw("E same");

    TH1F *hRatio_Double = (TH1F*)h_CPV->Clone("hRatio_Double");
    hRatio_Double->Divide(h_Double);
    hRatio_Double->SetTitle("");
    hRatio_Double->GetXaxis()->SetLabelSize(0.07);
    hRatio_Double->GetYaxis()->SetLabelSize(0.07);
    hRatio_Double->GetYaxis()->SetRangeUser(0.5, 1.5);
    hRatio_Double->GetYaxis()->SetNdivisions(10);
    hRatio_Double->GetYaxis()->SetTitleOffset(1.5);
    hRatio_Double->SetMarkerColor(kBlue);
    hRatio_Double->SetLineColor(kBlue);
    hRatio_Double->SetMarkerSize(0.5);
    hRatio_Double->Draw("E same");
    
    TLine *line = new TLine(hRatio_Single->GetXaxis()->GetXmin(), 1, hRatio_Single->GetXaxis()->GetXmax(), 1);
    line->SetLineColor(kRed);
    line->Draw();

    std::string subdir = dir + "/step_" + std::to_string(idx) + "/" + list;
    gSystem->mkdir(subdir.c_str(), kTRUE);
    c1->Print(Form("%s/Valid_%s_step%s.pdf",subdir.c_str(),list.c_str(),std::to_string(idx).c_str()));
}