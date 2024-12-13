import shutil
import os

##### get current path #####
print(os.getcwd())
##### define source file path #####
Jobversion = "v6_O3v2_2"
originPath = f"/u/user/gcho/TopPhysics/CPV/CMSSW_8_0_26_patch1/src/TOP-18-007/SSBAnalysis/AnalysisCode/output/{Jobversion}/Dataset"
##### hadd list #####
samplelist = [
    "Data_DoubleMuon_Run2016B","Data_DoubleMuon_Run2016C","Data_DoubleMuon_Run2016D","Data_DoubleMuon_Run2016E","Data_DoubleMuon_Run2016F","Data_DoubleMuon_Run2016G","Data_DoubleMuon_Run2016HV2","Data_DoubleMuon_Run2016HV3", #0~7
    "Data_SingleMuon_Run2016B","Data_SingleMuon_Run2016C","Data_SingleMuon_Run2016D","Data_SingleMuon_Run2016E","Data_SingleMuon_Run2016F","Data_SingleMuon_Run2016G","Data_SingleMuon_Run2016HV2","Data_SingleMuon_Run2016HV3", #8~15
    "DYJetsToLL_M_10To50","DYJetsToLL_M_50",                        #DY 16~17
    "ST_tW_antitop","ST_tW_top",                                    #ST 18~19
    "TTbar_WJetToLNu","TTbar_WQQ","TTbar_ZQQ","TTbar_ZToLLNuNu",    #tt+V 20~24
    "TTJets_others",                                                #25
    "WJetsToLNu","WW","WZ","ZZ",                                    #Diboson 27~29
]
cmbname = [
    "Data_DoubleMuon","Data_SingleMuon",
    "DY","ST","TTV","Diboson"
]

#################################
##### hadd for each samples #####
#################################
for idx in range(len(samplelist)):
    samplename = f"{samplelist[idx]}"
    srcPath = f"{originPath}/{samplename}"
    print(f"srcPath: {srcPath}")
    print(f"Current path: {os.getcwd()}")
    cpcmd = f'cp -v {srcPath}/{samplename}_All.root ./'
    rmcmd = f'rm {samplename}_All.root'
    ##### make a directories and copy them #####
    if idx < 8: ###----------------------------------------------- DoubleMuon ###
        data_doubleMuon = f"{originPath}/Data_DoubleMuon"
        makedir = f'mkdir {data_doubleMuon}'
        if not os.path.exists(data_doubleMuon):
            os.system(makedir)
        os.chdir(data_doubleMuon)
        print(f'now here: {os.getcwd()}')
        if not os.path.exists(f'{samplename}_All.root'):
            os.system(rmcmd)
        os.system(cpcmd)
    if idx > 7 and idx < 16: ###---------------------------------- SingleMuon ###
        data_singleMuon = f"{originPath}/Data_SingleMuon"
        makedir = f'mkdir {data_singleMuon}'
        if not os.path.exists(data_singleMuon):
            os.system(makedir)
        os.chdir(data_singleMuon)
        print(f'now here: {os.getcwd()}')
        if not os.path.exists(f'{samplename}_All.root'):
            os.system(rmcmd)
        os.system(cpcmd)
    if idx > 15 and idx < 18: ###---------------------------------- DY ##########
        dy = f"{originPath}/DY"
        makedir = f'mkdir {dy}'
        if not os.path.exists(dy):
            os.system(makedir)
        os.chdir(dy)
        print(f'now here: {os.getcwd()}')
        if not os.path.exists(f'{samplename}_All.root'):
            os.system(rmcmd)
        os.system(cpcmd)
    if idx > 17 and idx < 20: ###---------------------------------- ST ##########
        st = f"{originPath}/ST"
        makedir = f'mkdir {st}'
        if not os.path.exists(st):
            os.system(makedir)
        os.chdir(st)
        print(f'now here: {os.getcwd()}')
        if not os.path.exists(f'{samplename}_All.root'):
            os.system(rmcmd)
        os.system(cpcmd)
    if idx > 19 and idx < 25: ###---------------------------------- tt+V ########
        ttv = f"{originPath}/TTV"
        makedir = f'mkdir {ttv}'
        if not os.path.exists(ttv):
            os.system(makedir)
        os.chdir(ttv)
        print(f'now here: {os.getcwd()}')
        if not os.path.exists(f'{samplename}_All.root'):
            os.system(rmcmd)
        os.system(cpcmd)
    if idx > 25 and idx < 30: ###---------------------------------- Diboson #####
        diboson = f"{originPath}/Diboson"
        makedir = f'mkdir {diboson}'
        if not os.path.exists(diboson):
            os.system(makedir)
        os.chdir(diboson)
        print(f'now here: {os.getcwd()}')
        if not os.path.exists(f'{samplename}_All.root'):
            os.system(rmcmd)
        os.system(cpcmd)
print("all copied!!")
###################
##### combine #####
###################
for icmb in range(len(cmbname)):
    cmbPath = f"{originPath}/{cmbname[icmb]}"
    rmcmb = f'rm {cmbname[icmb]}.root'
    cmbhadd = f'hadd {cmbname[icmb]}.root *_All.root'
    os.chdir(cmbPath)
    if not os.path.exists(f'{cmbname[icmb]}.root'):
        os.system(rmcmb)
    os.system(cmbhadd)
##################################
##### data hadd to data.root #####
##################################
dataadd = f"{originPath}/Data"
makedir = f'mkdir {dataadd}'
if not os.path.exists(dataadd):
    os.system(makedir)
os.chdir(dataadd)
for idata in range(2):
    datacp = f'cp -v {originPath}/{cmbname[idata]}/{cmbname[idata]}.root ./'
    os.system(datacp)
    os.system('rm Data.root')
    os.system('hadd Data.root *.root')

print('Done! check the files')