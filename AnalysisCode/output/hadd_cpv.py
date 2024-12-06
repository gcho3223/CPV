import shutil
import os

##### get current path #####
print(os.getcwd())
##### define source file path #####
Jobversion = 'v6_O3v2_2'
originPath = f"/u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/output/{Jobversion}/CPV_Sample"

##### hadd list #####
samplelist = [
    "TTJets_Signal_dtG_0","TTJets_Signal_dtG_0p5207","TTJets_Signal_dtG_1p0415","TTJets_Signal_dtG_2p60364","TTJets_Signal_dtG_m0p5207","TTJets_Signal_dtG_m1p0415","TTJets_Signal_dtG_m2p60364"
]

##### hadd #####
for idx in range(len(samplelist)):
    samplename = f"{samplelist[idx]}"
    srcPath = f"{originPath}/{samplename}"
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