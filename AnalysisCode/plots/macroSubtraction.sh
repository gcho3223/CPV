#!/bin/bash

# Bash script for running DrawHist.C with different parameters
# This script is for subtraction background from data for each dR values and v2
# the plots which are overaly and subtracted are saved in the folder ./plots/Job_Version/Dataset/Data/SubtractionBkg as Overaly_*.pdf and PureData_*.pdf
# A root file is saved in the folder ./output/Job_Version/Dataset/Data/ as PureData_O3v*_*.root

version="v6_O3v2_3_dR"
dR_hist=(
    "_h_CPO3_bfReco_0_" "_h_CPO3_bfReco_1_" "_h_CPO3_bfReco_2_" "_h_CPO3_bfReco_3_" "_h_CPO3_bfReco_4_" "_h_CPO3_bfReco_5_" "_h_CPO3_bfReco_6_" "_h_v2_CPO3_bfReco_"
    "_h_v2_pair_Jetpt_" "_h_v2_pair_Jeteta_" "_h_v2_pair_Jetphi_" 
    "_h_v2_pair_AnJetpt_" "_h_v2_pair_AnJeteta_" "_h_v2_pair_AnJetphi_" 
    "_h_v2_pair_Leppt_" "_h_v2_pair_Lepeta_" "_h_v2_pair_Lepphi_"
    "_h_v2_pair_AnLeppt_" "_h_v2_pair_AnLepeta_" "_h_v2_pair_AnLepphi_" 
)

# 옵션 확인
for ((i=0; i<${#dR_hist[@]}; i++)); do
    cmd="root -l -b -q 'SubtractionBkg.C(\"$version\",\"${dR_hist[i]}\")'"
    echo $cmd
    eval $cmd
done