# BELLA RCF Stack Diagnostic Simulation in Geant4

 A flexible RCF Stack simulation to find dose depth and bragg curve measurements for monoenergetic proton beams. 

## Geometry Setup

The RCF stack consists of alternating filter and detector layers. Specify the stack geometry by editing the `geomConfig.txt` file and inputting the layer type (filter or detector), material, and thickness in the beam axis. 

A layer can be made of any material included in the [Geant4 NIST material catalog](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html). You do not need to include the "G4_" prefix, but the material names must be in all caps. A custom gafchromic film substrate material, HDv2, is also available. 

## Simulation Setup 

**For a single run**, use `testrun.mac`. Specify the beam energy using the `/gun/energy` command, and the number of particles with `/run/beamOn`. Some example values are provided.

If you have downloaded [CERN ROOT](https://root.cern/), a root file (`EnergyProfile.root`) is created with a histogram of the Bragg curve that can be visualized with `plotHisto.C`. A text file is created with the normalized dose vs. depth in the detector, which can be visualized instead through python, MATLAB, etc. 


**For a batch run**, use `loop.mac` which iterates over `run.mac`. Specify the energy range in `loop.mac`. 

A similar text file will be created with the normalized dose profile and tabulated for each incident loop energy. To plot the dose depth of the bragg peak for over the energy range, run`dose_depth.py`.

**_Important notes:_**
* There must be a minimum of 1 filter and 1 detector for the simulation to run. 



This product includes software developed by Members of the Geant4 Collaboration (http://cern.ch/geant4) and was developed in the BELLA Center at Lawrence Berkeley National Laboratory.

