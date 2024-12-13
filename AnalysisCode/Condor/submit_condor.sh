#!/bin/bash

samplelist=( 
    ### Data & MC Samples ###
    #"Data_DoubleMuon_Run2016B" 
    #"Data_DoubleMuon_Run2016C" 
    #"Data_DoubleMuon_Run2016D" 
    #"Data_DoubleMuon_Run2016E" 
    #"Data_DoubleMuon_Run2016F" 
    #"Data_DoubleMuon_Run2016G" 
    #"Data_DoubleMuon_Run2016HV2" 
    #"Data_DoubleMuon_Run2016HV3" 
    #"Data_SingleMuon_Run2016B" 
    #"Data_SingleMuon_Run2016C" 
    #"Data_SingleMuon_Run2016D" 
    #"Data_SingleMuon_Run2016E" 
    #"Data_SingleMuon_Run2016F" 
    #"Data_SingleMuon_Run2016G" 
    #"Data_SingleMuon_Run2016HV2" 
    #"Data_SingleMuon_Run2016HV3" 
    #"DYJetsToLL_M_10To50" 
    #"DYJetsToLL_M_50" 
    #"ST_tW_antitop" 
    #"ST_tW_top" 
    #"TTbar_WJetToLNu" 
    #"TTbar_WQQ" 
    #"TTbar_ZQQ" 
    #"TTbar_ZToLLNuNu" 
    #"TTJets_others" 
    #"TTJets_Signal" 
    #"WJetsToLNu" 
    #"WW" 
    #"WZ" 
    #"ZZ"
    ### CPV samples ###
    "TTJets_Signal_dtG_0" 
    "TTJets_Signal_dtG_0p5207" 
    "TTJets_Signal_dtG_1p0415" 
    "TTJets_Signal_dtG_2p60364" 
    "TTJets_Signal_dtG_m0p5207" 
    "TTJets_Signal_dtG_m1p0415" 
    "TTJets_Signal_dtG_m2p60364" 
)
### version information ( Dataset / CPV_Sample ) ###
version="Job_Version/v6_O3v2_3/CPV_Sample"
#version="Job_Version/v6_O3v2_3/Dataset"
### submit jobs for each sample ###
for sample in "${samplelist[@]}";
do
    cd $version/$sample
    if [ "$1" == "re" ]; then
        condor_submit "recondor_sub.sub"
    else
        condor_submit "condor_sub.sub"
    fi
    echo "submitted ${sample} jobs!!"
    cd -
done
