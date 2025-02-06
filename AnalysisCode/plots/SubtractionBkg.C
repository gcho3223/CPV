#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "TMath.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TSystem.h"
#include "CPVari.C"

CPVari bgsumAsym, dataAsym, pure_dataAsym;

void SubtractionBkg(string version, TString histname)
{
    string savepath;
    if (histname.Contains("CPO3")) {savepath = Form("Job_Version/%s/Dataset/Data/SubtractionBkg/O3",version.c_str());}
    else if (histname.Contains("pt")) {savepath = Form("Job_Version/%s/Dataset/Data/SubtractionBkg/pT",version.c_str());}
    else if (histname.Contains("eta")) {savepath = Form("Job_Version/%s/Dataset/Data/SubtractionBkg/eta",version.c_str());}
    else if (histname.Contains("phi")) {savepath = Form("Job_Version/%s/Dataset/Data/SubtractionBkg/phi",version.c_str());}
    gSystem->mkdir(Form("%s/",savepath.c_str()),kTRUE);

    TString sSignal      = Form("../output/Job_Version/%s/Dataset/TTJets_Signal/TTJets_Signal_All.root",version.c_str());
    TString sOthers      = Form("../output/Job_Version/%s/Dataset/TTJets_others/TTJets_others_All.root",version.c_str());
    TString sDYM10To50   = Form("../output/Job_Version/%s/Dataset/DYJetsToLL_M_10To50/DYJetsToLL_M_10To50_All.root",version.c_str());
    TString sDYM50       = Form("../output/Job_Version/%s/Dataset/DYJetsToLL_M_50/DYJetsToLL_M_50_All.root",version.c_str());
    TString sSTantitop   = Form("../output/Job_Version/%s/Dataset/ST_tW_antitop/ST_tW_antitop_All.root",version.c_str());
    TString sSTtop       = Form("../output/Job_Version/%s/Dataset/ST_tW_top/ST_tW_top_All.root",version.c_str());
    TString sTTWJetToLNu = Form("../output/Job_Version/%s/Dataset/TTbar_WJetToLNu/TTbar_WJetToLNu_All.root",version.c_str());
    TString sWQQ         = Form("../output/Job_Version/%s/Dataset/TTbar_WQQ/TTbar_WQQ_All.root",version.c_str());
    TString sZQQ         = Form("../output/Job_Version/%s/Dataset/TTbar_ZQQ/TTbar_ZQQ_All.root",version.c_str());
    TString sZToLLNuNu   = Form("../output/Job_Version/%s/Dataset/TTbar_ZToLLNuNu/TTbar_ZToLLNuNu_All.root",version.c_str());
    TString sWJetToLNu   = Form("../output/Job_Version/%s/Dataset/WJetsToLNu/WJetsToLNu_All.root",version.c_str());
    TString sWW          = Form("../output/Job_Version/%s/Dataset/WW/WW_All.root",version.c_str());
    TString sWZ          = Form("../output/Job_Version/%s/Dataset/WZ/WZ_All.root",version.c_str());
    TString sZZ          = Form("../output/Job_Version/%s/Dataset/ZZ/ZZ_All.root",version.c_str());
    TString sdata        = Form("../output/Job_Version/%s/Dataset/Data/Data.root",version.c_str());

    TFile *fSignal       = new TFile(sSignal, "READ");
    TFile *fOthers       = new TFile(sOthers, "READ");
    TFile *fDYM10To50    = new TFile(sDYM10To50, "READ");
    TFile *fDYM50        = new TFile(sDYM50, "READ");
    TFile *fSTantitop    = new TFile(sSTantitop, "READ");
    TFile *fSTtop        = new TFile(sSTtop, "READ");
    TFile *fTTWJetToLNu  = new TFile(sTTWJetToLNu, "READ");
    TFile *fWQQ          = new TFile(sWQQ, "READ");
    TFile *fZQQ          = new TFile(sZQQ, "READ");
    TFile *fZToLLNuNu    = new TFile(sZToLLNuNu, "READ");
    TFile *fWJetToLNu    = new TFile(sWJetToLNu, "READ");
    TFile *fWW           = new TFile(sWW, "READ");
    TFile *fWZ           = new TFile(sWZ, "READ");
    TFile *fZZ           = new TFile(sZZ, "READ");
    TFile *fData         = new TFile(sdata, "READ");

    TFile *filelist[]    = {fSignal, fOthers, //signal: 0, other: 1
                            fDYM10To50, fDYM50, //DY: 2~3
                            fSTtop, fSTantitop, //ST: 4~5
                            fWJetToLNu, fWW, fWZ, fZZ, //diboson: 6~9
                            fTTWJetToLNu, fWQQ, fZToLLNuNu, fZQQ //10~13
                            };
    
    int colorlist[]      = {kBlue+2, kGreen+2, kWhite, kCyan+2, kRed+3, kRed+1};

    TString hist_save_name;
    if (histname.Contains("_0_")) {hist_save_name = "O3v1_dR_0p5";}
    else if (histname.Contains("_1_")) {hist_save_name = "O3v1_dR_1p0";}
    else if (histname.Contains("_2_")) {hist_save_name = "O3v1_dR_1p5";}
    else if (histname.Contains("_3_")) {hist_save_name = "O3v1_dR_2p0";}
    else if (histname.Contains("_4_")) {hist_save_name = "O3v1_dR_2p5";}
    else if (histname.Contains("_5_")) {hist_save_name = "O3v1_dR_3p0";}
    else if (histname.Contains("_6_")) {hist_save_name = "O3v1_dR_10p0";}
    else if (histname.Contains("_v2_")) {hist_save_name = "O3v2";}

    void DrawOverflowBin(TH1D *his, double min, double max);

    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    THStack *sthis = new THStack(Form("%s", histname.Data()), "");
    TH1D *stack_Sig, *stack_Other, *stack_DY, *stack_ST, *stack_DB, *stack_TV, *stack_data;
    //stack_Sig->Sumw2(); stack_Other->Sumw2(); stack_DY->Sumw2(); stack_ST->Sumw2(); stack_DB->Sumw2(); stack_TV->Sumw2(); stack_data->Sumw2(); 
    for (int j = 0; j < sizeof(filelist)/sizeof(filelist[0]); j++) {
        TH1D *his;
        filelist[j]->GetObject(Form("%s", histname.Data()), his);
        if (!his) continue;
        //his->Scale(1/eventnum[j]);
        if (j==0) { //TTJet_Signal
            stack_Sig=his;
            continue;
        }
        if (j==1) { //TTJet_Others
            stack_Other=his;
            continue;
        }
        if (1<j && j<4) { //DY
            if (j==2) {
                stack_DY=his;
            } else {
                stack_DY->Add(his);
            }
            continue;
        }
        if (3<j && j<6) { //ST
            if (j==4) {
                stack_ST=his;
            } else {
                stack_ST->Add(his);
            }
            continue;
        }
        if (5<j && j<10) { //Diboson
            if (j==6) {
                stack_DB=his;
            } else {
                stack_DB->Add(his);
            }
            continue;
        }
        if (9<j && j<14) { //TTV
            if (j==10) {
                stack_TV=his;
            } else {
                stack_TV->Add(his);
            }
            continue;
        }
    }
    // MC stack
    TH1D *stlist[6] = {stack_DY, stack_ST, stack_DB, stack_TV, stack_Other, stack_Sig};
    for (int j=0; j<sizeof(stlist)/sizeof(stlist[0]); j++) {
        stlist[j]->SetFillColor(colorlist[j]);
        //stlist[j]->Rebin(10);
        //stlist[j]->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");
        sthis->Add(stlist[j]);
    }
    // DATA
    fData->GetObject(Form("%s", histname.Data()), stack_data);
    //stack_data->Rebin(10);
    stack_data->SetMarkerStyle(8);

    ///// background sum /////
    TH1D *bgsum;
    for (int j=0; j<5; j++) {
        if (j==0) {
            bgsum = (TH1D*)stlist[j]->Clone();
        } else {
            bgsum->Add(stlist[j]);
        }
    }
    THStack *stack_bgk = new THStack(Form("%s", histname.Data()), "");
    stack_bgk->Add(bgsum);
    bgsum->SetFillColor(kGray);

/////////////////////////////////////
///// 0. subtraction background /////
/////////////////////////////////////
    TH1D *pure_data = (TH1D*)stack_data->Clone();
    pure_data->Add(bgsum,-1);
///// 1. overlay O3 with background and data /////
    TCanvas *c_overlay = new TCanvas("c_overlay", "overlay for bkg & data", 800, 800);
    c_overlay->cd();

    if(histname.Contains("CPO3"))
    {
        ///// O3 asymmetry /////
        // bg
        c_overlay->SetLogy();
        bgsumAsym = O3Asym(bgsum);
        cout << "Bkg: "; 
        cout << "N_+: " << bgsumAsym.num_p_ << ", N_-: " << bgsumAsym.num_m_ 
             << ", total: " << bgsumAsym.num_total_ << ", integ: " << bgsum->Integral() 
             << ", Asym: " << bgsumAsym.asym_ << endl;
        // data
        dataAsym = O3Asym(stack_data);
        cout << "Data: "; 
        cout << "N_+: " << dataAsym.num_p_ << ", N_-: " << dataAsym.num_m_ 
             << ", total: " << dataAsym.num_total_ << ", integ: " << stack_data->Integral() 
             << ", Asym(bf): " << dataAsym.asym_ << endl;
        // pure data
        pure_dataAsym = O3Asym(pure_data);
        cout << "pure data: "; 
        cout << "N_+: " << pure_dataAsym.num_p_ << ", N_-: " << pure_dataAsym.num_m_ 
             << ", total: " << pure_dataAsym.num_total_ << ", integ: " << pure_data->Integral() 
             << ", Asym(bf): " << pure_dataAsym.asym_ << endl;
    }
    stack_data->Draw("P");
    stack_bgk->Draw("same hist");
    if(histname.Contains("pt")) {stack_data->Rebin(5); bgsum->Rebin(5);}
    else if(histname.Contains("eta")) {stack_data->Rebin(2); bgsum->Rebin(2);}
    stack_data->SetTitle("");
    stack_data->GetYaxis()->SetTitle("Entry");
    stack_data->GetYaxis()->SetTitleSize(0.03);
    stack_data->GetYaxis()->SetTitleOffset(1.4);
    stack_data->SetStats(0);
    if(histname.Contains("CPO3")) {stack_data->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");}
    else if(histname.Contains("pt")) {stack_data->GetXaxis()->SetTitle("#p_T");}
    else if(histname.Contains("eta")) {stack_data->GetXaxis()->SetTitle("#eta");}
    else if(histname.Contains("phi")) {stack_data->GetXaxis()->SetTitle("#phi");}

    TLegend *legend_overlay = c_overlay->BuildLegend(0.15, 0.65, 0.4, 0.85);
    legend_overlay->Clear();
    legend_overlay->SetFillStyle(0);
    legend_overlay->SetBorderSize(0);
    legend_overlay->SetTextSize(0.02);
    legend_overlay->AddEntry(stack_data, "Data", "p");
    if(histname.Contains("CPO3"))
    {
        legend_overlay->AddEntry(stack_data, Form("Event: %.4f ", dataAsym.num_total_),"");
        legend_overlay->AddEntry(stack_data, Form("Asym: %.4f #pm %.4f", dataAsym.asym_, dataAsym.asym_err_),"");
    }
    else
    {
        legend_overlay->AddEntry(stack_data, Form("Event: %.4f ", stack_data->Integral()),"");
        legend_overlay->AddEntry(stack_data, Form("Mean: %.4f ", stack_data->GetMean()),"");
        legend_overlay->AddEntry(stack_data, Form("RMS: %.4f ", stack_data->GetRMS()),"");
    }

    legend_overlay->AddEntry(stack_bgk, "Background","f");
    if(histname.Contains("CPO3"))
    {
        legend_overlay->AddEntry(stack_bgk, Form("Event: %.4f ", bgsumAsym.num_total_),"");
        legend_overlay->AddEntry(stack_bgk, Form("Asym(bkg): %.4f #pm %.4f", bgsumAsym.asym_, bgsumAsym.asym_err_),"");
    }
    else
    {
        legend_overlay->AddEntry(stack_bgk, Form("Event: %.4f ", bgsum->Integral()),"");
        legend_overlay->AddEntry(stack_bgk, Form("Mean: %.4f ", bgsum->GetMean()),"");
        legend_overlay->AddEntry(stack_bgk, Form("RMS: %.4f ", bgsum->GetRMS()),"");
    }
    legend_overlay->Draw();

///// 2. pure data O3 distribution /////

    TCanvas *c_pure = new TCanvas("c_pure", "pure data O3 distribution", 800, 800);
    c_pure->cd();
    c_pure->SetLogy();
    
    pure_data->Draw("hist");
    if(histname.Contains("pt")) {pure_data->Rebin(5);}
    else if(histname.Contains("eta")) {pure_data->Rebin(2);}

    pure_data->SetTitle("");
    pure_data->GetYaxis()->SetTitle("Entry");
    pure_data->GetYaxis()->SetTitleSize(0.03);
    pure_data->GetYaxis()->SetTitleOffset(1.4);
    if(histname.Contains("CPO3"))
    {
        pure_data->GetXaxis()->SetRangeUser(-2, 2);
        pure_data->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");
    }
    else if(histname.Contains("pt")) {pure_data->GetXaxis()->SetTitle("#p_T");}
    else if(histname.Contains("eta")) {pure_data->GetXaxis()->SetTitle("#eta");}
    else if(histname.Contains("phi")) {pure_data->GetXaxis()->SetTitle("#phi");}
    pure_data->SetStats(0);

    TLegend *legend_pure = c_pure->BuildLegend(0.15, 0.7, 0.4, 0.85);
    legend_pure->Clear();
    legend_pure->SetFillStyle(0);
    legend_pure->SetBorderSize(0);
    legend_pure->SetTextSize(0.02);
    legend_pure->AddEntry(pure_data, "Data", "f");
    if(histname.Contains("CPO3"))
    {
        legend_pure->AddEntry(pure_data, Form("Event: %.4f ", pure_dataAsym.num_total_),"");
        legend_pure->AddEntry(pure_data, Form("Asym: %.4f #pm %.4f", pure_dataAsym.asym_, pure_dataAsym.asym_err_),"");
    }
    else
    {
        legend_pure->AddEntry(pure_data, Form("Event: %.4f ", pure_data->Integral()),"");
        legend_pure->AddEntry(pure_data, Form("Mean: %.4f ", pure_data->GetMean()),"");
        legend_pure->AddEntry(pure_data, Form("RMS: %.4f ", pure_data->GetRMS()),"");
    }
    legend_pure->Draw();

    if(histname.Contains("CPO3"))
    {
        DrawOverflowBin(stack_data, -2, 2); DrawOverflowBin(bgsum, -2, 2); DrawOverflowBin(pure_data, -2, 2);
        c_overlay->SaveAs(Form("./%s/Overlay_%s.pdf", savepath.c_str(),hist_save_name.Data()));
        c_pure->SaveAs(Form("./%s/PureData_%s.pdf", savepath.c_str(),hist_save_name.Data()));
    }
    else if(histname.Contains("pt"))
    {
        DrawOverflowBin(stack_data, 0, 200); DrawOverflowBin(bgsum, 0, 200); DrawOverflowBin(pure_data, 0, 200);
        c_overlay->SaveAs(Form("./%s/Overlay_%s.pdf", savepath.c_str(),histname.Data()));
        c_pure->SaveAs(Form("./%s/PureData_%s.pdf", savepath.c_str(),histname.Data()));
    }
    else if(histname.Contains("eta"))
    {
        DrawOverflowBin(stack_data, -3, 3); DrawOverflowBin(bgsum, -3, 3); DrawOverflowBin(pure_data, -3, 3);
        c_overlay->SaveAs(Form("./%s/Overlay_%s.pdf", savepath.c_str(),histname.Data()));
        c_pure->SaveAs(Form("./%s/PureData_%s.pdf", savepath.c_str(),histname.Data()));
    }
    else if(histname.Contains("phi"))
    {
        DrawOverflowBin(stack_data, -3.2, 3.2); DrawOverflowBin(bgsum, -3.2, 3.2); DrawOverflowBin(pure_data, -3.2, 3.2);
        c_overlay->SaveAs(Form("./%s/Overlay_%s.pdf", savepath.c_str(),histname.Data()));
        c_pure->SaveAs(Form("./%s/PureData_%s.pdf", savepath.c_str(),histname.Data()));
    }

    // 2. Pure Data ROOT 파일에 저장 (공통 ROOT 파일 사용)
    TString outputRootFile = Form("../output/Job_Version/%s/Dataset/Data/PureData_All.root", version.c_str());
    TFile *fpuredata = TFile::Open(outputRootFile, "UPDATE");
    if (!fpuredata || fpuredata->IsZombie()) {
        fpuredata = new TFile(outputRootFile, "RECREATE");
    }
    pure_data->SetName(Form("PureData%s", histname.Data()));
    pure_data->Write();
    fpuredata->Close();
}

void DrawOverflowBin(TH1D *his, double min, double max) {
    double contentUnder = 0;
    double contentOver  = 0;

    for (int i=0; i<his->GetNbinsX()-1; i++) {
        if (his->GetBinCenter(i+1) < min) {
            contentUnder += his->GetBinContent(i+1);
        }
        if (his->GetBinCenter(i+1) > max) {
            contentOver += his->GetBinContent(i+1);
        }
    }
    for (int i=0; i<his->GetNbinsX()-1; i++) {
        if (his->GetBinCenter(i+1) < min && his->GetBinCenter(i+2) > min) his->SetBinContent(i+2, contentUnder);
        if (his->GetBinCenter(i+1) < max && his->GetBinCenter(i+2) > max) his->SetBinContent(i+1, contentOver);
    }
    his->GetXaxis()->SetRangeUser(min, max);
}
