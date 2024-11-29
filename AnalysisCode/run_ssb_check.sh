#!/bin/tcsh

#set inputlists = ("Data_DoubleMuon_Run2016G_1")
#set inputlists = ("TTJets_Signal_GluoneMoveCRTune_erdON_1")
#set inputlists = ("TTJets_others_GluoneMoveCRTune_erdON_1")
#set inputlists = ("WJetsToLNu_1")
set inputlists = ("TTJets_Signal_1")
#set inputlists = ("DYJetsToLL_M_10To50_1")
foreach i ( $inputlists )
   mkdir -p output
   ./ssb_analysis ${i}.list ${i}.root 0

end
