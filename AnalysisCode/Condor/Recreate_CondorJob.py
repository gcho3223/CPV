import os
from platform import machine
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
    #["DYJetsToLL_M_10To50",["9","44","51","69","82","101","141","155","168","176"]],
    #["DYJetsToLL_M_50",["1","4","11","22","28","36","42","45","54","57","58","59","82","97","106","112","113","115","116","117","118","119","120","121","135","147","155","159","162","163","164","165","166","167","168","170","185","188","189","191","192","193","194","195","196","197","198","199","214","224","231","239","243","245","246","247","249","250","252","253","270","279","287","294","299","300","301","302","303","304","305","306","319","327","331","333","334","335","336","337","338","339","340","341","357","368","373","376","379","382","385","388","391","393","394","395","414","424","430","435","439","441","443","445","447","449","450","451","467","473","479","482","483","484","485","486","487","488","489","490","506","511","514","515","516","517","518","519","520","521","523","524","542","547","552","555","557","559","560","561","562","563","564","565","584","594","599","603","604","605","606","607","608","609","610","611","625","630","632","633","634","635","636","637","638","639","640","641","659","665","671","678","679","680","681","682","683","684","685","686","706","717","725","731"]],
    #["ST_tW_antitop",["1","2","4","5","6","7","8","9","16","24","51","65"]],
    #["ST_tW_top",["7","19","30","41","53","64"]],
    #["TTbar_WJetToLNu",["3","19"]],
    #["TTbar_WQQ",[]],
    #["TTbar_ZQQ",[]],
    #["TTbar_ZToLLNuNu",["24","35"]],
    ["TTJets_others",["97","99","100","101"]],
    #["TTJets_Signal",[]],
    #["WJetsToLNu",["0","3","4","5","6","7","8","9","10","11","12","13","14","17","19","20","21","22","23","24","25","26","27","28","29","33","34","35","36","37","38","39","40","41","42","43","44","47","48","49","50","51","52","53","55","56","57","58","59","62","63","64","65","66","70","72","75","76","77","78","79","80","81","82","83","89","95","96","97","98","99","100","101","102","103","104","105","110","114","116","117","118","119"]],
    #["WW",["0","1","2","3","4","11","14","15","16","17","18","19","20","21","22","23","30","33","35","37","38","39","40","41","42","43","44","50","51","52","53","54","55","56","57","58","59","60","76"]],
    #["WZ",["5","13","19","23","25","26","29","31","33","35"]],
    #["ZZ",[]],
    #["TTJets_Signal_dtG_0",["4","13","16","22"]],
    #["TTJets_Signal_dtG_0p5207",["6","16","28"]],
    #["TTJets_Signal_dtG_1p0415",["9","17","27"]],
    #["TTJets_Signal_dtG_2p60364",["5","13","18","27"]],
    #["TTJets_Signal_dtG_m0p5207",["9","17"]],
    #["TTJets_Signal_dtG_m1p0415",["14"]],
    #["TTJets_Signal_dtG_m2p60364",["3","8","12","19","20","25"]]
]

version = "v6_O3v2_3"

for sample in range(len(samplelist)):
    ### set up jobversion ###
    if "dtG" in samplelist[sample][0]:
        jobversion = "Job_Version/%s/CPV_Sample" %(version)
    else:
        jobversion = "Job_Version/%s/Dataset" %(version)
    ### set up directory ###
    sample_dir = f"{jobversion}/{samplelist[sample][0]}"
    ### delete log, err, out files for the failed jobs in log_condor directory ###
    for f in os.listdir(f"{sample_dir}/log_condor/err"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/err", f))
    for f in os.listdir(f"{sample_dir}/log_condor/log"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/log", f))
    for f in os.listdir(f"{sample_dir}/log_condor/out"):
        os.remove(os.path.join(f"{sample_dir}/log_condor/out", f))
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
        ## set up memory request: data: 6144, MC: 3072, CPV: 2048
        if "Data" in samplelist[sample][0]:
            file.write("request_memory = 6144 MB \n\n") ## data samples
        elif "dtG" in samplelist[sample][0]:
            file.write("request_memory = 2048 MB \n\n") ## CPV samples
        else:
            file.write("request_memory = 3072 MB \n\n") ## MC samples
        file.write("should_transfer_files = YES \n")
        file.write("when_to_transfer_output = ON_EXIT \n\n")
        file.write('Requirements = (machine =!= "cluster291.knu.ac.kr") && (TARGET.Arch == "X86_64") && (TARGET.OpSys == "LINUX") && (TARGET.HasFileTransfer)\n')
        file.write("JobBatchName = %s \n\n" %(samplelist[sample][0]) )
        for job_number in samplelist[sample][1]:
            file.write(f"arguments = {job_number} \n")
            file.write("output = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/out/out_$(Process).out \n" %(jobversion, samplelist[sample][0]) )
            file.write("error  = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/err/err_$(Process).err \n" %(jobversion, samplelist[sample][0]) )
            file.write("log    = /u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/Condor/%s/%s/log_condor/log/log_$(Process).log \n\n" %(jobversion, samplelist[sample][0]) )
            file.write("queue 1\n\n")
        #file.write("queue %s" %(samplelist[sample][1]) )
    print("%s condor submit file created!!! " %(samplelist[sample][0]))
