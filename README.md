# BELLA RCF Stack Diagnostic Simulation in Geant4

 A flexible RCF Stack simulation to find dose depth and bragg curve measurements for monoenergetic proton beams. 

## Geometry Setup

The RCF stack consists of alternating filter and detector layers. Specify the stack geometry by editing the `geomConfig.txt` file and inputting the layer type (filter or detector), material, and thickness in the beam axis [Note 1]

A layer can be made of any material included in the [Geant4 NIST material catalog](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html) [Note 2]. You do not need to include the "G4_" prefix, but the material names must be in all caps. Custom materials for HDv2 and EBT3 active layers are available, as well as their substrates with names `HDv2`, `EBT3`,and `GS`, respectively.  

## Simulation Setup 

**For a single run**, use `testrun.mac`. Specify the beam energy using the `/gun/energy` command, and the number of particles with `/run/beamOn`. Some example values are provided.

If you have downloaded [CERN ROOT](https://root.cern/), a root file (`EnergyProfile.root`) is created with a histogram of the Bragg curve that can be visualized with `plotHisto.C`. A text file, `bragg_curve.txt`, is created with the normalized dose vs. depth in the detector, which can be visualized instead through python, MATLAB, etc. 

**For a batch run**, use `loop.mac` which iterates over `run.mac`. Specify the energy range in `loop.mac`. 

`bragg_curve.txt` will again be created with the normalized dose profile and tabulated for each incident loop energy. `dose_depth.py` plots the depth of the bragg peak as a function of incident beam energy, with red boxes indicating detector locations along the stack as set in `geomConfig.txt`. 

### Important notes:
1. HDv2 and EBT3 detectors must be constructed by building separate geometries for their active layers and substrate. For example, and EBT3 layer can be constructed in `geomConfig.txt` as 
```
Detector    GS      0.125 <-- Substrate Layer
Detector    EBT3    0.028 <--    Active Layer
Detector    GS      0.125 <-- Substrate Layer
```

2. Element or material names taken from the NIST material catalog must be written in ALL CAPS

------
This product includes software developed by Members of the Geant4 Collaboration (http://cern.ch/geant4) and was created in the BELLA Center at Lawrence Berkeley National Laboratory.

