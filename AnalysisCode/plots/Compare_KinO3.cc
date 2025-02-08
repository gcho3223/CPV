#include "TSystem.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TLine.h"
#include "./DrawOverflowBin.cc"
#include "./CPVari.C"

string base_path = "u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/output/Job_Version";
vector<string> data_path =
{
    "Validation/CPV_MuMu/Data.root",
    "Dataset/Data/Data.root",
    "Dataset/Data/PureData_All.root",
};
vector< vector<TString> > hlist =
{
    // after top reco for reference
    {"h_Reco_CPO3",
    "_h_cf_Jet1pt_8_",              "_h_cf_Jet1eta_8_",             "_h_cf_Jet1phi_8_",
    "_h_cf_Jet2pt_8_",              "_h_cf_Jet2eta_8_",             "_h_cf_Jet2phi_8_",
    "_h_cf_Lep1pt_8_",              "_h_cf_Lep1eta_8_",             "_h_cf_Lep1phi_8_",
    "_h_cf_Lep2pt_8_",              "_h_cf_Lep2eta_8_",             "_h_cf_Lep2phi_8_",},
    // new algorithm v2
    {"_h_v2_CPO3_bfReco_",
    "_h_v2_pair_Jetpt_",            "_h_v2_pair_Jeteta_",           "_h_v2_pair_Jetphi_",
    "_h_v2_pair_AnJetpt_",          "_h_v2_pair_AnJeteta_",         "_h_v2_pair_AnJetphi_",
    "_h_v2_pair_Leppt_",            "_h_v2_pair_Lepeta_",           "_h_v2_pair_Lepphi_",
    "_h_v2_pair_AnLeppt_",          "_h_v2_pair_AnLepeta_",         "_h_v2_pair_AnLepphi_",},
    // new algorithm v2 for pure data
    {"PureData_h_v2_CPO3_bfReco_",
    "PureData_h_v2_pair_Jetpt_",    "PureData_h_v2_pair_Jeteta_",   "PureData_h_v2_pair_Jetphi_",
    "PureData_h_v2_pair_AnJetpt_",  "PureData_h_v2_pair_AnJeteta_", "PureData_h_v2_pair_AnJetphi_",
    "PureData_h_v2_pair_Leppt_",    "PureData_h_v2_pair_Lepeta_",   "PureData_h_v2_pair_Lepphi_",
    "PureData_h_v2_pair_AnLeppt_",  "PureData_h_v2_pair_AnLepeta_", "PureData_h_v2_pair_AnLepphi_",},
};
CPVari refAsym, v2Asym, v2pureAsym;
void Compare_KinO3(string version)
{
    void DrawOverflowBin(TH1D *his, double min, double max);

    string save_path = Form("./Job_Version/%s/Comparison_Kin-O3",version.c_str());
    gSystem->mkdir(save_path.c_str(), kTRUE);

    for(int ihist=0; ihist<hlist[0].size(); ihist++) 
    {
        TFile *fref     = new TFile(Form("../output/%s", data_path[0].c_str()), "READ");
        TFile *fv2      = new TFile(Form("/%s/%s/%s", base_path.c_str(), version.c_str(), data_path[1].c_str()), "READ");
        TFile *fv2pure  = new TFile(Form("/%s/%s/%s", base_path.c_str(), version.c_str(), data_path[2].c_str()), "READ");

        TH1D *href      = (TH1D*)fref->Get(Form("%s",hlist[0][ihist].Data()));
        TH1D *hv2       = (TH1D*)fv2->Get(Form("%s",hlist[1][ihist].Data()));
        TH1D *hv2pure   = (TH1D*)fv2pure->Get(Form("%s",hlist[2][ihist].Data()));
        if(!href || !hv2 || !hv2pure)
        {
            cout << "one of hist is not the same check hist" << endl;
            cout << "href: " << href->GetName() << ", hv2: " << hv2->GetName() << ", hv2pure: " << hv2pure->GetName() << endl;
        }

        string hname ="";
        if(TString(href->GetName()).Contains("O3")           && TString(hv2->GetName()).Contains("O3")       && TString(hv2pure->GetName()).Contains("O3"))       {hname = "O3";}
        else if(TString(href->GetName()).Contains("Jet1pt")  && TString(hv2->GetName()).Contains("Jetpt")    && TString(hv2pure->GetName()).Contains("Jetpt"))    {hname = "Jetpt";}
        else if(TString(href->GetName()).Contains("Jet1eta") && TString(hv2->GetName()).Contains("Jeteta")   && TString(hv2pure->GetName()).Contains("Jeteta"))   {hname = "Jeteta";}
        else if(TString(href->GetName()).Contains("Jet1phi") && TString(hv2->GetName()).Contains("Jetphi")   && TString(hv2pure->GetName()).Contains("Jetphi"))   {hname = "Jetphi";}
        else if(TString(href->GetName()).Contains("Jet2pt")  && TString(hv2->GetName()).Contains("AnJetpt")  && TString(hv2pure->GetName()).Contains("AnJetpt"))  {hname = "AnJetpt";}
        else if(TString(href->GetName()).Contains("Jet2eta") && TString(hv2->GetName()).Contains("AnJeteta") && TString(hv2pure->GetName()).Contains("AnJeteta")) {hname = "AnJeteta";}
        else if(TString(href->GetName()).Contains("Jet2phi") && TString(hv2->GetName()).Contains("AnJetphi") && TString(hv2pure->GetName()).Contains("AnJetphi")) {hname = "AnJetphi";}
        else if(TString(href->GetName()).Contains("Lep1pt")  && TString(hv2->GetName()).Contains("Leppt")    && TString(hv2pure->GetName()).Contains("Leppt"))    {hname = "Leppt";}
        else if(TString(href->GetName()).Contains("Lep1eta") && TString(hv2->GetName()).Contains("Lepeta")   && TString(hv2pure->GetName()).Contains("Lepeta"))   {hname = "Lepeta";}
        else if(TString(href->GetName()).Contains("Lep1phi") && TString(hv2->GetName()).Contains("Lepphi")   && TString(hv2pure->GetName()).Contains("Lepphi"))   {hname = "Lepphi";}
        else if(TString(href->GetName()).Contains("Lep2pt")  && TString(hv2->GetName()).Contains("AnLeppt")  && TString(hv2pure->GetName()).Contains("AnLeppt"))  {hname = "AnLeppt";}
        else if(TString(href->GetName()).Contains("Lep2eta") && TString(hv2->GetName()).Contains("AnLepeta") && TString(hv2pure->GetName()).Contains("AnLepeta")) {hname = "AnLepeta";}
        else if(TString(href->GetName()).Contains("Lep2phi") && TString(hv2->GetName()).Contains("AnLepphi") && TString(hv2pure->GetName()).Contains("AnLepphi")) {hname = "AnLepphi";}
        else {cout << hname << ": one of hist is not the same check hist" << endl;}

        TCanvas *c1 = new TCanvas(Form("c1_%s", hname.c_str()), "Overlay & ratio plot", 800, 800);

        TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
        pad1->SetBottomMargin(0.03);
        pad1->Draw();

        TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.3);
        pad2->Draw();
        
        pad1->cd();
        hv2->Draw("hist");
        href->Draw("p same");
        hv2pure->Draw("hist same");

        href->SetMarkerColor(kBlack);
        href->SetMarkerStyle(kFullCircle);
        href->SetMarkerSize(.75);
        hv2->SetLineColor(kBlue); hv2pure->SetLineColor(kMagenta);
        hv2->SetLineWidth(2); hv2pure->SetLineWidth(2);

        href->SetStats(kFALSE); hv2->SetStats(kFALSE); hv2pure->SetStats(kFALSE);
        href->GetXaxis()->SetLabelSize(0); hv2->GetXaxis()->SetLabelSize(0); hv2pure->GetXaxis()->SetLabelSize(0);
        href->SetTitle(""); hv2->SetTitle(""); hv2pure->SetTitle("");
        hv2->GetYaxis()->SetTitle("Entries");
        hv2->GetYaxis()->SetTitleOffset(1.3);
        hv2->GetYaxis()->SetTitleFont(42);
        //href->GetXaxis()->SetTitleSize(0.09);

        if(hname.find("O3") != std::string::npos)
        {
            pad1->SetLogy();
            //href->Rebin(2); hv2->Rebin(2); hv2pure->Rebin(2);
            refAsym = O3Asym(href);
            v2Asym = O3Asym(hv2);
            //v2pureAsym = O3Asym(hv2pure);
            DrawOverflowBin(href, -2, 2); DrawOverflowBin(hv2, -2, 2);
        }
        else if(hname.find("pt") != std::string::npos)
        {
            href->Rebin(5); hv2->Rebin(5); //hv2pure->Rebin(5);
            DrawOverflowBin(href, 0, 200); DrawOverflowBin(hv2, 0, 200);
        }
        else if(hname.find("eta") != std::string::npos)
        {
            href->Rebin(2); hv2->Rebin(2); //hv2pure->Rebin(2);
            DrawOverflowBin(href, -3, 3); DrawOverflowBin(hv2, -3, 3);
        }
        else if(hname.find("phi") != std::string::npos)
        {
            //href->Rebin(4);
            DrawOverflowBin(href, -3.2, 3.2); DrawOverflowBin(hv2, -3.2, 3.2);
        }

        TLegend *LegendRef = new TLegend(0.67, 0.75, 0.9, 0.9);
        LegendRef->AddEntry(href, Form("after top reco"), "p");
        if(hname.find("O3") != std::string::npos)
        {
            LegendRef->AddEntry(href, Form("Entries: %.2f", refAsym.num_total_), "");
            LegendRef->AddEntry(href, Form("Asym: %.4f", refAsym.asym_), "");
            LegendRef->AddEntry(href, Form("Asym err: %.4f", refAsym.asym_err_), "");
        }
        else
        {
            LegendRef->AddEntry(href, Form("Entries: %.2f", href->Integral()), "");
            LegendRef->AddEntry(href, Form("Mean: %.2f", href->GetMean()), "");
            LegendRef->AddEntry(href, Form("Std dev: %.2f", href->GetStdDev()), "");
        }
        LegendRef->SetLineColor(kBlack);
        LegendRef->SetTextFont(42);
        LegendRef->SetTextSize(0.03);
        LegendRef->SetTextColor(kBlack);
        LegendRef->SetFillStyle(0);
        LegendRef->SetBorderSize(0);
        LegendRef->Draw();

        TLegend *LegendV2 = new TLegend(0.67, 0.6, 0.9, 0.75);
        LegendV2->AddEntry(hv2, Form("v2"), "l");
        if(hname.find("O3") != std::string::npos)
        {
            LegendV2->AddEntry(hv2, Form("Entries: %.2f", v2Asym.num_total_), "");
            LegendV2->AddEntry(hv2, Form("Asym: %.4f", v2Asym.asym_), "");
            LegendV2->AddEntry(hv2, Form("Asym err: %.4f", v2Asym.asym_err_), "");
        }
        else
        {
            LegendV2->AddEntry(hv2, Form("Entries: %.2f", hv2->Integral()), "");
            LegendV2->AddEntry(hv2, Form("Mean: %.2f", hv2->GetMean()), "");
            LegendV2->AddEntry(hv2, Form("Std dev: %.2f", hv2->GetStdDev()), "");
        }
        LegendV2->SetTextFont(42);
        LegendV2->SetTextSize(0.03);
        LegendV2->SetTextColor(kBlue);
        LegendV2->SetFillStyle(0);
        LegendV2->SetBorderSize(0);
        LegendV2->Draw();

        TLegend *LegendV2pure = new TLegend(0.67, 0.45, 0.9, 0.6);
        LegendV2pure->AddEntry(hv2pure, Form("v2 pure"), "l");
        if(hname.find("O3") != std::string::npos)
        {
            LegendV2pure->AddEntry(hv2pure, Form("Entries: 49078.4"), "");
            LegendV2pure->AddEntry(hv2pure, Form("Asym: 0.0087"), "");
            LegendV2pure->AddEntry(hv2pure, Form("Asym err: 0.0057"), "");
        }
        else
        {
            LegendV2pure->AddEntry(hv2pure, Form("Entries: %.2f", hv2pure->Integral()), "");
            LegendV2pure->AddEntry(hv2pure, Form("Mean: %.2f", hv2pure->GetMean()), "");
            LegendV2pure->AddEntry(hv2pure, Form("Std dev: %.2f", hv2pure->GetStdDev()), "");
        }
        LegendV2pure->SetLineColor(kMagenta);
        LegendV2pure->SetTextFont(42);
        LegendV2pure->SetTextSize(0.03);
        LegendV2pure->SetTextColor(kMagenta);
        LegendV2pure->SetFillStyle(0);
        LegendV2pure->SetBorderSize(0);
        LegendV2pure->Draw();
                
        pad2->cd();
        pad2->SetGridy();
        TH1F *hRatio_v2 = (TH1F*)hv2->Clone("hRatio_v2");
        hRatio_v2->Divide(href);
        hRatio_v2->SetTitle("");
        hRatio_v2->GetXaxis()->SetLabelSize(0.07);
        hRatio_v2->GetYaxis()->SetLabelSize(0.07);
        hRatio_v2->GetYaxis()->SetNdivisions(10);
        hRatio_v2->GetYaxis()->SetTitleOffset(0.5);
        hRatio_v2->GetYaxis()->SetTitleSize(0.08);
        hRatio_v2->GetYaxis()->SetTitleFont(42);
        hRatio_v2->GetYaxis()->SetTitle("v2/ref");
        hRatio_v2->SetMarkerColor(kBlue);
        hRatio_v2->SetMarkerStyle(kFullCircle);
        hRatio_v2->SetMarkerSize(.55);
        hRatio_v2->GetYaxis()->SetRangeUser(0.5, 1.5);
        hRatio_v2->Draw("E");

        TH1F *hRatio_v2pure = (TH1F*)hv2pure->Clone("hRatio_v2pure");
        hRatio_v2pure->Divide(href);
        hRatio_v2pure->SetTitle("");
        hRatio_v2pure->GetXaxis()->SetLabelSize(0.07);
        hRatio_v2pure->GetYaxis()->SetLabelSize(0.07);
        hRatio_v2pure->GetYaxis()->SetNdivisions(10);
        hRatio_v2pure->GetYaxis()->SetTitleOffset(1.5);
        hRatio_v2pure->SetMarkerColor(kMagenta);
        hRatio_v2pure->SetMarkerStyle(kFullCircle);
        hRatio_v2pure->SetMarkerSize(.55);
        hRatio_v2pure->GetYaxis()->SetRangeUser(0.5, 1.5);
        hRatio_v2pure->Draw("E same");

        if(hname.find("O3") != std::string::npos)       {hRatio_v2->GetXaxis()->SetTitle("O_{3}/m_{t}^{4}");}
        else if(hname.find("pt") != std::string::npos)  {hRatio_v2->GetXaxis()->SetTitle("p_{T} (GeV)");}
        else if(hname.find("eta") != std::string::npos) {hRatio_v2->GetXaxis()->SetTitle("#eta");}
        else if(hname.find("phi") != std::string::npos) {hRatio_v2->GetXaxis()->SetTitle("#phi");}
        hRatio_v2->GetXaxis()->SetTitleSize(0.09);

        pad2->Update();
        Double_t xmin = gPad->GetUxmin();
        Double_t xmax = gPad->GetUxmax();
        TLine *line = new TLine(xmin, 1, xmax, 1);
        line->SetLineColor(kRed);
        line->SetLineWidth(1);
        line->Draw("same");

        gSystem->mkdir(save_path.c_str(), kTRUE);
        c1->SaveAs(Form("%s/Compare_%s.pdf",save_path.c_str(),hname.c_str()));

        fref->Close();
        fv2->Close();
        fv2pure->Close();
        delete fref;
        delete fv2;
        delete fv2pure;
    }
}