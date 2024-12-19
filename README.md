# ⚠️Analysis code for CP violation in top quark pair production in dilepton channel 🚀

The code and scripts included here are for the dilepton channel of the top quark and are intended for **backup** purposes.

## 📂Directory structure
📦SSBAnalysis/   
├── 📂AnalysisCode/   
│ ├── 📂CPVAsymAndPlotters/   
│ ├── 📂Condor/   
│ ├── 📂backup/   
│ ├── 📂interface/   
│ ├── 📂output/   
│ ├── 📂plots/   
│ ├── 📂src/   
│ ├── 📜CommonTools.cpp   
│ ├── 📜main_ssb.cpp   
│ ├── 📜run_ssb_check.sh   
│ └── 📜testrun.sh   
└── 📜README.md   

## 📁Directory description
- **Condor/**: Condor script generator and condor job summary script generator are included.
- **backup/**: Kinematic Reconstruction related code is included.
- **interface/**: header files for the analysis code are included.
- **plots/**: plotting code is included.
- **src/**: source code for the analysis code is included.
- **CommonTools.cpp**: common tools for the analysis code are included.
- **main_ssb.cpp**: main code for the analysis code is included.
- **run_ssb_check.sh**: script for checking the analysis code is included.
- **testrun.sh**: script for testing the analysis code is included.

## 📜Code Description   
### CPVAsymAndPlotters Directory
- calculate O3 asymmetry and ttbar cross section using likelihood function
📦CPVAsymAndPlotters   
├── 📂config   
├── 📂dat   
├── 📂output   
├── 📜main_Asym_Calc.cpp   
└── 📜run_auto_con.sh   
- **config/**: configuration files(signal efficiency at step5 which is before top reconstruction).
- **dat/**: O3 asymmetry parameters at step5 for each samples.
- **output/**: O3 asymmetry and ttbar cross section for new algorithm v1(with different dR cut) & v2.
- **main_Asym_Calc.cpp**: main code
- **run_auto_con.sh**: script for running.
    - to run this code, need an argument for the version of new algorithm(v1 or v2).
    ```
    ./run_auto_con.sh v1(or v2)
    ```

### Condor Directory
📦Condor   
├── 📂Backup   
├── 📜run_summary.sh   
├── 📜submit_condor.sh   
├── 📜Recreate_CondorJob_Sample.py   
└── 📜Create_CondorJob.py   
- **Backup**: condor submit script for the CPV sample is included. (for backup)
- **run_summary.sh**: script for summarizing the condor job
    - before running the script, you need to change the version of the job and sample
    ```
    ./run_summary.sh
    ```
- **submit_condor.sh**: script for submitting the condor job
    - before running the script, you need to change the version of the job and sample
    ```
    ./submit_condor.sh
    ```
    - if you need to submit the job again, command is
    ```
    ./submit_condor.sh re
    ```
- **Create_CondorJob.py**: script for creating the condor job
    - if you need to create the condor job again, you can run this script
    - before running the script, you need to change the version of the job and sample
- **Recreate_CondorJob_Sample.py**: script for recreating the condor job
    - if there are failed jobs or errors, you can recreate the condor job for only failed jobsby running this script

### backup Directory
- all of the code is for backup

### interface Directory
📦interface   
└── 📜ssb_analysis.hpp   
- **ssb_analysis.hpp**: 📈 header file for the analysis code

### output Directory
📦output   
├── 📂Backup   
│ └── 📜hadd_cmb.py   
├── 📜hadd.py   
└── 📜hadd_cmb.py   
- **hadd.py**: hadd code for combining the root files.
    - before running the script, you need to change the version of the job and sample
- **hadd_cmb.py**: This code combines root files by categorizing data with data and similar MC files with each other.
    - Single and Double Muon Data are combined separately and then copied to a newly created Data directory, where they are combined again. DY, TTV, Diboson, and ST are also created in the same way.

### plots Directory
📦plots   
├── 📜DrawHist.C   
├── 📜LinearityObs.C   
├── 📜toDrawHist.h   
├── 📜valid_ratio.cc   
└── 📜valid_ratio_O3.cc   
- **DrawHist.C**: draw the histograms for kinematics, O3, invariant mass, and other variables for Gen. and new algorithm v1 & v2
```
root -l -b -q DrawHist.C'("version","opt")'
```
```opt``` is ```cpv``` for CPV sample and ```data``` for data sample
- **LinearityObs.C**: draw the linearity for the CPV sample
    - before running the script, you need to change the version and add O3 asymmetry parameters measured by likelihood fit
    ```vLike_Yval```, ```vLike_StatErr_p```, ```vLike_StatErr_m```
    ```
    root -l -b -q LinearityObs.C'("version","opt")'
    ```
    - ```opt``` is ```reco``` for new algorithm v1(reco level), ```gen``` for Gen level, ```v2``` for new algorithm v2(reco level), ```ref``` for reference
- **toDrawHist.h**: header file for the DrawHist.C
- **valid_ratio.cc**: code for the validation ratio
- **valid_ratio_O3.cc**: code for the validation ratio for the O3 sample