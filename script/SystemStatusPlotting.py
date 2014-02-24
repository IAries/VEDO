# Please define what data you want to plot

sSourceFile     = "SystemStatus.csv"
sSplitSymbol    = ", "
iXColume        = 0
lliDataSetIndex = [  (1, 2, 3)
                   , (4, 5, 6)
                   , (6, 7, 8)
                   , (9, 10)
                   , (11, 12)
                   , (13, 14)  ]
lsFigureName    = [  "Number of eiscrete objects"
                   , "Energy"
                   , "Kinetic energy"
                   , "Velocity"
                   , "Angular velocity"
                   , "Norm"                    ]

import sys
iNumOfFigure    = len(lsFigureName)
if len(lliDataSetIndex) != len(lsFigureName):
    print("Length of \"lliDataSetIndex\" doesn't equal to \"lsFigureName\"!!")
    sys.exit()

iMaxIndexInlliDataSetIndex = lliDataSetIndex[0][0]
for i in range(0, iNumOfFigure):
    for j in range(0, len(lliDataSetIndex[i])):
        if iMaxIndexInlliDataSetIndex < max(lliDataSetIndex[i]):
            iMaxIndexInlliDataSetIndex = max(lliDataSetIndex[i])

# Read data from .csv file to 2D array

import csv

sMessage = "Reading data from \"" + sSourceFile + "\"..."
print(sMessage)

import os
if not os.path.exists(sSourceFile):
    _MESSAGE = "File \"" + sSourceFile + "\" doesn't exist!!"
    print(_MESSAGE)
    sys.exit()

fSource       = open(sSourceFile, 'r')

lsRawData     = []
lsTitle       = []
bNotFirstLine = False
for _SingleLineRawData in fSource:
    if bNotFirstLine:
        lsRawData.append(_SingleLineRawData.strip().split(sSplitSymbol))
    else:
        lsTitle.append(_SingleLineRawData.strip().split(sSplitSymbol))
        bNotFirstLine = True

fSource.close()

# Transform the type of elements in "lsRawData" from "string" to "float"

lsRawData = [[float(string) for string in Inner] for Inner in lsRawData]

# Transpose

import itertools

lsRawData = list(itertools.zip_longest(*lsRawData))
lsTitle   = list(itertools.zip_longest(*lsTitle))

lsRawDataColumeNumber = len(lsTitle)
if not lsRawData:
    _MESSAGE = "There is no data in \"" + sSourceFile + "\"!!"
    print(_MESSAGE)
    sys.exit()
else:
    lsRawDataRowNumber = len(lsRawData[0])

if lsRawDataColumeNumber < (iMaxIndexInlliDataSetIndex+1):
    print("Number of cloume if source file \"sSourceFile\" is not enough!!")
    sys.exit()

# Plot

import numpy as np
import matplotlib.pyplot as plt

for iFigure in range(0, iNumOfFigure):
    _MESSAGE = "Plotting figure #" + str(iFigure+1) + ": \"" + lsFigureName[iFigure] + "\"..."
    print(_MESSAGE)
    plt.clf()
#    plt.figure(figsize=(8, 4))
    for iColume in range(0, len(lliDataSetIndex[iFigure])):
        plt.plot(lsRawData[iXColume], lsRawData[lliDataSetIndex[iFigure][iColume]], label=lsTitle[lliDataSetIndex[iFigure][iColume]][0], linewidth=1)
    plt.xlabel(lsTitle[iXColume][0])
    plt.ylabel(lsFigureName[iFigure])
#    plt.title(lsFigureName[iFigure])
#    plt.ylim(-1.2, 1.2)
    plt.legend(loc='best')
    _sFigureName = lsFigureName[iFigure] + ".png"
    plt.savefig(_sFigureName, dpi=180)

print("Finished.")
sys.exit()
