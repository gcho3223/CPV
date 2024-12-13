import os
import sys

samplelist = [
    #["Data_DoubleMuon_Run2016B","264"],
    #["Data_DoubleMuon_Run2016C","87"],
    #["Data_DoubleMuon_Run2016D","146"],
    #["Data_DoubleMuon_Run2016E","124"],
    #["Data_DoubleMuon_Run2016F","91"],
    #["Data_DoubleMuon_Run2016G","214"],
    #["Data_DoubleMuon_Run2016HV2","232"],
    #["Data_DoubleMuon_Run2016HV3","7"],
    #["Data_SingleMuon_Run2016B","1054"],
    #["Data_SingleMuon_Run2016C","348"],
    #["Data_SingleMuon_Run2016D","584"],
    #["Data_SingleMuon_Run2016E","496"],
    #["Data_SingleMuon_Run2016F","362"],
    #["Data_SingleMuon_Run2016G","854"],
    #["Data_SingleMuon_Run2016HV2","925"],
    #["Data_SingleMuon_Run2016HV3","25"],
    ["DYJetsToLL_M_10To50","178"],
    ["DYJetsToLL_M_50","732"],
    ["ST_tW_antitop","70"],
    ["ST_tW_top","71"],
    ["TTbar_WJetToLNu","22"],
    ["TTbar_WQQ","4"],
    ["TTbar_ZQQ","4"],
    ["TTbar_ZToLLNuNu","56"],
    ["TTJets_others","621"],
    #["TTJets_Signal","621"],
    ["WJetsToLNu","120"],
    ["WW","81"],
    ["WZ","41"],
    ["ZZ","2"],
    ### CPV Sample ###
    ["TTJets_Signal_dtG_0","30"],
    ["TTJets_Signal_dtG_0p5207","31"],
    ["TTJets_Signal_dtG_1p0415","30"],
    ["TTJets_Signal_dtG_2p60364","30"],
    ["TTJets_Signal_dtG_m0p5207","31"],
    ["TTJets_Signal_dtG_m1p0415","30"],
    ["TTJets_Signal_dtG_m2p60364","30"]
]

version = "v6_O3v2_4"


for sample in range(len(samplelist)):
    ### set up jobversion ###
    if "dtG" in samplelist[sample][0]:
        jobversion = "Job_Version/%s/CPV_Sample" %(version)
    else:
        jobversion = "Job_Version/%s/Dataset" %(version)
    ### set up directory ###
    sample_dir = f"{jobversion}/{samplelist[sample][0]}"
    os.makedirs(sample_dir, exist_ok=True)
    os.makedirs(f"{sample_dir}/log_condor", exist_ok=True)
    ### create log_condor directory ###
    os.makedirs(f"{sample_dir}/log_condor/err", exist_ok=True)
    os.makedirs(f"{sample_dir}/log_condor/log", exist_ok=True) 
    os.makedirs(f"{sample_dir}/log_condor/out", exist_ok=True)
    ### delete log_condor directory ###
    for f in os.listdir(f"{sample_dir}/log_condor/err"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/err", f))
    for f in os.listdir(f"{sample_dir}/log_condor/log"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/log", f))
    for f in os.listdir(f"{sample_dir}/log_condor/out"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/out", f))
    ### created executive condor job: condor_run.sh ###
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
        file.write("./ssb_analysis %s/%s_${fileListNum}.list /%s/%s_${fileListNum}.root 0 %s \n" % (samplelist[sample][0],samplelist[sample][0],sample_dir,samplelist[sample][0],samplelist[sample][0]))
    ### created submit condor job: condor_sub.sub ###
    with open(f"{sample_dir}/condor_sub.sub", "w") as file:
        file.write("universe = vanilla \n")
        file.write("executable = condor_run.sh \n\n")
        file.write("arguments = $(Process) \n")
        file.write("request_memory = 6144 MB \n\n") #data: 6144, MC: 3072
        file.write("should_transfer_files = YES \n")
        file.write("when_to_transfer_output = ON_EXIT \n\n")
        file.write("output = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/out/out_$(Process).out \n" %(jobversion, samplelist[sample][0]) )
        file.write("error  = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/err/err_$(Process).err \n" %(jobversion, samplelist[sample][0]) )
        file.write("log    = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/log/log_$(Process).log \n\n" %(jobversion, samplelist[sample][0]) )
        file.write("JobBatchName = %s \n" %(samplelist[sample][0]) )
        file.write("queue %s" %(samplelist[sample][1]) )
    print("%s condor submit file created!!! " %(samplelist[sample][0]))