#!/bin/bash

# Bash script for running DrawHist.C with different parameters

version="v6_O3v2_3_dR"
iscpv="data"
dR_value=("0p5" "1p0" "1p5" "2p0" "2p5" "3p0" "10p0")
dR_hist=("_h_CPO3_bfReco_0_" "_h_CPO3_bfReco_1_" "_h_CPO3_bfReco_2_" "_h_CPO3_bfReco_3_" "_h_CPO3_bfReco_4_" "_h_CPO3_bfReco_5_" "_h_CPO3_bfReco_6_")
v2_hist="_h_v2_CPO3_bfReco_"

# 옵션 확인
if [ "$1" == "v1" ]; then
    # new algorithm v1
    for ((i=0; i<${#dR_value[@]}; i++)); do
        cmd="root -l -b -q 'DrawHist.C(\"$version\",\"$iscpv\",\"${dR_value[i]}\",\"${dR_hist[i]}\")'"
        echo $cmd
        eval $cmd
    done
elif [ "$1" == "v2" ]; then
    # new algorithm v2
    cmd="root -l -b -q 'DrawHist.C(\"$version\",\"$iscpv\",\"\",\"$v2_hist\")'"
    echo $cmd
    eval $cmd
else
    echo "사용법: $0 [v1|v2]"
    echo "v1: new algorithm v1"
    echo "v2: new algorithm v2"
fi