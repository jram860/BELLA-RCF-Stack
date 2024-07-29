# BELLA RCF Stack Diagnostic Simulation in Geant4

This program is a flexible RCF Stack "calibration" simulation: the dose depth and Bragg Curve are calculated (1) for a single monoenergetic proton beam or (2) over a range of monoenergetic proton beams. 

## Geometry Setup

The calorimeter is a stack of alternating filter and detector layers. The `geomConfig.txt` file specifies the layer type (filter or detector), material, and thickness in the beam axis. 

You can use any material included in the Geant4 NIST material catalog (https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html). You do not need to include the "G4_" prefix, but the material names must be in all caps.

## Simulation Setup

Simulations of a single run or looped run are available. 

**For a single run**, use `testrun.mac`. Specify the beam energy in the `/gun/energy` command, and how many particles you'd like to simulate with the `/run/beamOn`. Some example values are provided.

If you have ROOT (https://root.cern/), a root file (`EnergyProfile.root`) is created with a histogram of the Bragg curve that can be visualized with `plotHisto.C`. A text file is created with the particle-number-normalized dose over depth in the detector, which can be visualized in python, MATLAB, etc. 


**For a batch run**, use `loop.mac` which iterates over `run.mac`. Specify the energy range in `loop.mac`, and specify the number of particles in the same way in `run.mac`. 

A similar text file will be created with the normalized dose profile and tabulated for each incident loop energy. To plot the dose depth of the bragg peak for over the energy range, run`dose_depth.py`.

**_Important notes:_**
* There must be a minimum of 1 filter and 1 detector for the simulation to run. 



This product includes software developed by Members of the Geant4 Collaboration (http://cern.ch/geant4) and was developed in the BELLA Center at Lawrence Berkeley National Laboratory.

