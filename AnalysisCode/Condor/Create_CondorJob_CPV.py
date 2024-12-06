import os
import sys

samplelist = [
    ["TTJets_Signal_dtG_0","30"],
    ["TTJets_Signal_dtG_0p5207","31"],
    ["TTJets_Signal_dtG_1p0415","30"],
    ["TTJets_Signal_dtG_2p60364","30"],
    ["TTJets_Signal_dtG_m0p5207","31"],
    ["TTJets_Signal_dtG_m1p0415","30"],
    ["TTJets_Signal_dtG_m2p60364","30"]
]
jobversion = "v6_O3v2_2/CPV_Sample"

for sample in range(len(samplelist)):
    sample_dir = f"{jobversion}/{samplelist[sample][0]}"
    os.makedirs(sample_dir, exist_ok=True)
    os.makedirs(f"{sample_dir}/log_condor", exist_ok=True)
    ### create log_condor directory ###
    os.makedirs(f"{sample_dir}/log_condor/err", exist_ok=True)
    os.makedirs(f"{sample_dir}/log_condor/log", exist_ok=True)
    os.makedirs(f"{sample_dir}/log_condor/out", exist_ok=True)
    for f in os.listdir(f"{sample_dir}/log_condor/err"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/err", f))
    for f in os.listdir(f"{sample_dir}/log_condor/log"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/log", f))
    for f in os.listdir(f"{sample_dir}/log_condor/out"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/out", f))
    with open(f"{sample_dir}/condor_run.sh", "w") as file:
        file.write("#!/bin/bash \n\n")
        file.write("export SCRAM_ARCH=slc7_amd64_gcc530\n")
        file.write("source /cvmfs/cms.cern.ch/cmsset_default.sh \n\n")
        file.write("export LD_PRELOAD=""/usr/lib64/libpdcap.so"" \n")
        file.write("export LD_LIBRARY_PATH=""$LD_LIBRARY_PATH:/usr/lib64/dcap"" \n\n")
        file.write("fileListNum=$((${1} + 1)) \n")
        file.write("cd /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/ \n")
        file.write("cmsenv \n\n")
        file.write("mkdir -p ./output/%s/\n" %(sample_dir))
        file.write("./ssb_analysis CEDM_Sample/%s/%s_${fileListNum}.list /%s/%s_${fileListNum}.root 0 %s \n" % (samplelist[sample][0],samplelist[sample][0],sample_dir,samplelist[sample][0],samplelist[sample][0]))
    with open(f"{sample_dir}/condor_sub.sub", "w") as file:
        file.write("universe = vanilla \n")
        file.write("executable = condor_run.sh \n\n")
        file.write("arguments = $(Process) \n")
        file.write("request_memory = 2048 MB \n\n")
        file.write("should_transfer_files = YES \n")
        file.write("when_to_transfer_output = ON_EXIT \n\n")
        file.write("output = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/out/out_$(Process).out \n" %(jobversion, samplelist[sample][0]) )
        file.write("error  = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/err/err_$(Process).err \n" %(jobversion, samplelist[sample][0]) )
        file.write("log    = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/log/log_$(Process).log \n\n" %(jobversion, samplelist[sample][0]) )
        file.write("JobBatchName = %s \n" %(samplelist[sample][0]) )
        file.write("queue %s" %(samplelist[sample][1]) )
    print("%s condor submit file created!!! " %(samplelist[sample][0]))