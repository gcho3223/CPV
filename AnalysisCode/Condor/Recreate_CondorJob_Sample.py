import os
import sys

samplelist = [
    #["Data_DoubleMuon_Run2016B",[]],
    #["Data_DoubleMuon_Run2016C",[]],
    #["Data_DoubleMuon_Run2016D",["137"]],
    #["Data_DoubleMuon_Run2016E",["1", "9", "18", "26", "41", "45", "51", "57", "62", "65"]],
    #["Data_DoubleMuon_Run2016F",[]],
    #["Data_DoubleMuon_Run2016G",[]],
    #["Data_DoubleMuon_Run2016HV2",["8","18"]],
    #["Data_DoubleMuon_Run2016HV3",[]],
    #["Data_SingleMuon_Run2016B","[]],
    #["Data_SingleMuon_Run2016C",[]],
    #["Data_SingleMuon_Run2016D",[]],
    #["Data_SingleMuon_Run2016E",[]],
    #["Data_SingleMuon_Run2016F",[]],
    #["Data_SingleMuon_Run2016G",[]],
    #["Data_SingleMuon_Run2016HV2",["225","231","235","238"]],
    #["Data_SingleMuon_Run2016HV3",["24"]],
    #["DYJetsToLL_M_10To50",[]],
    #["DYJetsToLL_M_50",[]],
    #["ST_tW_antitop",[]],
    #["ST_tW_top",[]],
    #["TTbar_WJetToLNu",[]],
    #["TTbar_WQQ",[]],
    #["TTbar_ZQQ",[]],
    #["TTbar_ZToLLNuNu",[]],
    #["TTJets_others",[]],
    #["TTJets_Signal",[]],
    #["WJetsToLNu",[]],
    #["WW",[]],
    #["WZ",[]],
    #["ZZ",[]],
    #["TTJets_Signal_dtG_0",[]],
    #["TTJets_Signal_dtG_0p5207",[]],
    ["TTJets_Signal_dtG_1p0415",["13"]],
    #["TTJets_Signal_dtG_2p60364",[]],
    #["TTJets_Signal_dtG_m0p5207",[]],
    #["TTJets_Signal_dtG_m1p0415",[]],
    #["TTJets_Signal_dtG_m2p60364",[]]
]

jobversion = "v6_O3v2/CPV_Sample"

for sample in range(len(samplelist)):
    sample_dir = f"{jobversion}/{samplelist[sample][0]}"
    ### delete log, err, out files for the failed jobs in log_condor directory ###
    for job_number in samplelist[sample][1]:
        os.remove(os.path.join(f"{sample_dir}/log_condor/err/err_{job_number}.err"))
        os.remove(os.path.join(f"{sample_dir}/log_condor/log/log_{job_number}.log"))
        os.remove(os.path.join(f"{sample_dir}/log_condor/out/out_{job_number}.out"))
    ### created executive condor job: condor_run.sh ###
    with open(f"{sample_dir}/recondor_run.sh", "w") as file:
        file.write("#!/bin/bash \n\n")
        file.write("export SCRAM_ARCH=slc7_amd64_gcc530\n")
        file.write("source /cvmfs/cms.cern.ch/cmsset_default.sh \n\n")
        file.write("export LD_PRELOAD=""/usr/lib64/libpdcap.so"" \n")
        file.write("export LD_LIBRARY_PATH=""$LD_LIBRARY_PATH:/usr/lib64/dcap"" \n\n")
        file.write("fileListNum=$((${1}+1)) \n")
        file.write("cd /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/ \n")
        file.write("cmsenv \n\n")
        file.write("mkdir -p ./output/%s/\n" %(sample_dir))
        file.write("./ssb_analysis %s/%s_${fileListNum}.list /%s/%s_${fileListNum}.root 0 %s \n" % (samplelist[sample][0],samplelist[sample][0],sample_dir,samplelist[sample][0],samplelist[sample][0]))
    ### created submit condor job: condor_sub.sub ###
    with open(f"{sample_dir}/recondor_sub.sub", "w") as file:
        file.write("universe = vanilla \n")
        file.write("executable = condor_run.sh \n\n")
        file.write("request_memory = 6050 MB \n")
        file.write("should_transfer_files = YES \n")
        file.write("when_to_transfer_output = ON_EXIT \n\n")
        file.write("JobBatchName = %s \n\n" %(samplelist[sample][0]) )
        for job_number in samplelist[sample][1]:
            file.write(f"arguments = {job_number} \n")
            file.write("output = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/out/out_$(Process).out \n" %(jobversion, samplelist[sample][0]) )
            file.write("error  = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/err/err_$(Process).err \n" %(jobversion, samplelist[sample][0]) )
            file.write("log    = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/log/log_$(Process).log \n\n" %(jobversion, samplelist[sample][0]) )
            file.write("queue 1\n\n")
        #file.write("queue %s" %(samplelist[sample][1]) )
    print("%s condor submit file created!!! " %(samplelist[sample][0]))
