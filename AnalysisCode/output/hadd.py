import shutil
import os

##### get current path #####
print(os.getcwd())
##### define source file path #####
version = "v6_O3v2_3"
originPath = f"/u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/output"
##### hadd list #####
samplelist = [
    "Data_DoubleMuon_Run2016B","Data_DoubleMuon_Run2016C","Data_DoubleMuon_Run2016D","Data_DoubleMuon_Run2016E","Data_DoubleMuon_Run2016F","Data_DoubleMuon_Run2016G","Data_DoubleMuon_Run2016HV2","Data_DoubleMuon_Run2016HV3",
    "Data_SingleMuon_Run2016B","Data_SingleMuon_Run2016C","Data_SingleMuon_Run2016D","Data_SingleMuon_Run2016E","Data_SingleMuon_Run2016F","Data_SingleMuon_Run2016G","Data_SingleMuon_Run2016HV2","Data_SingleMuon_Run2016HV3",
    "DYJetsToLL_M_10To50","DYJetsToLL_M_50",
    "ST_tW_antitop","ST_tW_top",
    "TTbar_WJetToLNu","TTbar_WQQ","TTbar_ZQQ","TTbar_ZToLLNuNu",
    "TTJets_others",
    "TTJets_Signal",
    "WJetsToLNu","WW","WZ","ZZ", #29
    ### CPV sample ###
    "TTJets_Signal_dtG_0",
    "TTJets_Signal_dtG_0p5207",
    "TTJets_Signal_dtG_1p0415",
    "TTJets_Signal_dtG_2p60364",
    "TTJets_Signal_dtG_m0p5207",
    "TTJets_Signal_dtG_m1p0415",
    "TTJets_Signal_dtG_m2p60364"
]

##### hadd #####
for idx in range(len(samplelist)):
    samplename = f"{samplelist[idx]}"
    if "dtG" in samplename:
        srcPath = f"{originPath}/Job_Version/{version}/CPV_Sample/{samplename}"
    else:
        srcPath = f"{originPath}/Job_Version/{version}/Dataset/{samplename}"
    print(f"srcPath: {srcPath}")
    # hadd
    os.chdir(srcPath) # go to directory
    # if *_All.root exists, remove it
    if os.path.exists(f"{samplelist[idx]}_All.root"):
        rmall = f"rm {samplelist[idx]}_All.root"
        os.system(rmall)
    cmdhadd = f"hadd {samplelist[idx]}_All.root {samplelist[idx]}_*.root" 
    os.system(cmdhadd)
    print(f"Current path: {os.getcwd()}")

print("Done! check the files")