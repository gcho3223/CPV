#!/bin/bash

# Bash script for running DrawHist.C with different parameters

version="v6_O3v2_3"
iscpv="data"
dR_value=("0p5" "1p0" "1p5" "2p0" "2p5" "3p0")
dR_hist=("_h_CPO3_bfReco_0_" "_h_CPO3_bfReco_1_" "_h_CPO3_bfReco_2_" "_h_CPO3_bfReco_3_" "_h_CPO3_bfReco_4_" "_h_CPO3_bfReco_5_")

for ((i=0; i<${#dR_value[@]}; i++)); do
    cmd="root -l -b -q 'DrawHist.C(\"$version\",\"$iscpv\",\"${dR_value[i]}\",\"${dR_hist[i]}\")'"
    echo $cmd
    eval $cmd
done