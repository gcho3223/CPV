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

### version information ###
### should change sample_path when CPV samples are used!!! ###
version="v6_O3v2"
#sample_path="./${version}/Dataset"
sample_path="./${version}/CPV_Sample"

### calc whole of log files from samplelist for progress bar ###
total_sample=0
completed=0
for i in "${samplelist[@]}"
do
    log_path="${sample_path}/${i}/log_condor/out"
    file_count=$(ls "$log_path"/*.out 2>/dev/null | wc -l)
    total_samples=$((total_samples + file_count))  #sum of total log files
done

### start summary !! ###
for i in "${samplelist[@]}"
do
    ### make directory & initiate txt files ###
    mkdir -p ./Run_Summary/${sample_path}/${i}
    > ./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt
    > ./Run_Summary/${sample_path}/${i}/Memory_Summary_${i}.txt
    ## define log, err, out path ##
    log_path="${sample_path}/${i}/log_condor/log"
    out_path="${sample_path}/${i}/log_condor/out"
    err_path="${sample_path}/${i}/log_condor/err"
    ## calc # of log files ##
    file_count=$(ls "$out_path"/*.out 2>/dev/null | wc -l)
    # skip if there is no logfile -> to obtail the number of queue
    if (( file_count == 0 )); then
        echo "No log files found for ${i}" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
        continue
    fi
    failed_evt=()
    ## indicate Sample name ##
    echo "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ ${i} ]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
    echo "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ ${i} ]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]" >> "./Run_Summary/${sample_path}/${i}/Memory_Summary_${i}.txt"
    for ((j=0; j<file_count; j++))
    do 
        ## memory summary ##
        echo ">> $j th-----------------------------------------------------------------------------------" >> "./Run_Summary/${sample_path}/${i}/Memory_Summary_${i}.txt"
        echo "                              	Usage    Requested   Allocated" >> "./Run_Summary/${sample_path}/${i}/Memory_Summary_${i}.txt"
        echo "`grep " Memory (MB)" "$log_path/log_$j.log"`" >> "./Run_Summary/${sample_path}/${i}/Memory_Summary_${i}.txt"""
        ## err, out ,log summary ##
        if ! grep -q "exit-code 0" "$log_path/log_$j.log" || grep -q "killed" "$err_path/err_$j.err" || ! grep -q "End processing" "$out_path/out_$j.out"; then
            echo ">> $j th-----------------------------------------------------------------------------------" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
            # exit-code 확인 : exit-code 0(정상종료)이 아니면 기록
            if ! grep -q "exit-code 0" "$log_path/log_$j.log"; then
                echo "Log: " >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
                echo "`grep "exit-code" "$log_path/log_$j.log"`" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
                echo "`grep " Memory (MB)" "$log_path/log_$j.log"`" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"""
            fi
            # err 파일 확인 : killed가 있으면 기록
            if grep -q "killed" "$err_path/err_$j.err"; then
                echo "Err: " >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
                echo "`grep "killed" "$err_path/err_$j.err"`" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
            fi
            # out 파일 확인 : End processing(정상종료)이 없으면 기록
            if ! grep -q "End processing" "$out_path/out_$j.out"; then
                    echo "Out: " >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
                    echo "`tail -n 7 "$out_path/out_$j.out"`" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
            fi
            failed_evt[$j]=$j 
        fi
        ### progress bar ###
        ((completed++))
        echo -ne "Progress in $i: ["
        progress=$(( (completed * 100) / total_samples )) 
        for ((k=0; k<=100; k+=2))
        do
            if [ $k -le $progress ]
            then
                echo -ne ">"
            else
                echo -ne " "
            fi
        done
        echo -ne "] $progress% \r"
    done
    echo "------------------------------------ Summary -----------------------------------------------" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
    echo "Number of failed inputlist number: ${#failed_evt[@]}" >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
    echo -n "Failed inputlist number: " >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
    for j in "${failed_evt[@]}"
    do
        echo -n "\"$j\"," >> "./Run_Summary/${sample_path}/${i}/Run_Summary_${i}.txt"
    done
done
echo ""