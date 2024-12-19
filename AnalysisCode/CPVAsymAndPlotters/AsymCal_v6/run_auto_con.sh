#!/bin/bash

#./Asym_Calc -isComb true -inputFile MuMu SystematicStudy_v12/MuMu/SystematicStudy_v12_MuMu.list -inputFile ElEl SystematicStudy_v12/ElEl/SystematicStudy_v12_ElEl.list -inputFile MuEl SystematicStudy_v12/MuEl/SystematicStudy_v12_MuEl.list  -outputDir SystematicStudy_v12/DiLep/ -scaleOpt MuMu SetScaleMuMu.cfg ElEl SetSetSaleElEl.cfg MuEl SetScaleMuEl.cfg
#./Asym_Calc -channel DiLep -studyName SystematicStudy_v12 -obs O1 -outputDir SystematicStudy_v12/DiLep/ -inputDAT SystematicStudy_v12/DiLep/Asymmetry_dilep_SystematicStudy_v12.dat -config SystematicStudy_v12/DiLep/CP_Inputs_dilep_SystematicStudy_v12.config 
#./Asym_Calc -channel MuMu -studyName SystematicStudy_v13 -obs O1 -outputDir SystematicStudy_v13/MuMu/ -inputDAT SystematicStudy_v13/MuMu/Asymmetry_dimu_SystematicStudy_v13.dat -config SystematicStudy_v13/MuMu/CP_Inputs_dimu_SystematicStudy_v13.config
### version 22 is the result from SK ###
#./Asym_Calc -channel MuMu -studyName SystematicStudy_v22 -obs O3 -outputDir SystematicStudy_v22/MuMu/ -inputDAT SystematicStudy_v22/MuMu/Asymmetry_dimu_SystematicStudy_v22.dat -config SystematicStudy_v22/MuMu/CP_Inputs_dimu_SystematicStudy_v22.config -systName none  
#mkdir -p output/SystematicStudy_v22/MuMu/BGStat 
#./Asym_Calc -channel MuMu -studyName SystematicStudy_v22 -obs O1 -outputDir SystematicStudy_v22/MuMu/BGStat -inputDAT SystematicStudy_v22/MuMu/Asymmetry_dimu_SystematicStudy_v22.dat -config SystematicStudy_v22/MuMu/CP_Inputs_dimu_SystematicStudy_v13.config -systName BGStat 

version="SystematicStudy_v28"
dr_value=("0p5" "1p0" "1p5" "2p0" "2p5" "3p0" "10p0")

mkdir -p output/${version}/MuMu/
if [ "$1" == "v1" ]; then
    ### display code message and record them as log file ###
    ### new algorithm v1 ###
    O3version="O3v1_dR"
    for ((i=0; i<${#dr_value[@]}; i++)); do
        log_path=output/${version}/MuMu/log_${version}_${O3version}_dR${dr_value[i]}.log
        ./Asym_Calc -channel MuMu -studyName ${version} -obs O3 -outputDir ${version}/MuMu/ -outputO3version ${O3version}${dr_value[i]} -inputDAT ${version}/MuMu/Asymmetry_dimu_${version}_${O3version}${dr_value[i]}.dat -config ${version}/MuMu/CP_Inputs_dimu_${version}.config -systName none 2>&1 | tee "${log_path}"
    done
elif [ "$1" == "v2" ]; then
    ### new algorithm v2 ###
    O3version="O3v2"
    log_path=output/${version}/MuMu/log_${version}_${O3version}.log
    ./Asym_Calc -channel MuMu -studyName ${version} -obs O3 -outputDir ${version}/MuMu/ -outputO3version ${O3version} -inputDAT ${version}/MuMu/Asymmetry_dimu_${version}_${O3version}.dat -config ${version}/MuMu/CP_Inputs_dimu_${version}.config -systName none 2>&1 | tee "${log_path}"
fi
