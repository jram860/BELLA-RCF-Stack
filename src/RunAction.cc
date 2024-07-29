#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Threading.hh"

#include <fstream>

RunAction::RunAction(DetectorConstruction*det, PrimaryGeneratorAction* prim): G4UserRunAction(),fDetector(det),fPrimary(prim) {

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Reset();
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetVerboseLevel(0);
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("EnergyProfile.root");
    G4double length =fDetector->GetCalorSizeZ();
    analysisManager->CreateH1("EnergyProfile","Proton Energy Deposition Profile",200,0,length/mm);
    analysisManager->SetH1XAxisTitle(0,"Depth into Detector [mm]");
    analysisManager->SetH1YAxisTitle(0,"Dose [MeV]");


    auto h1 = analysisManager->GetH1(0);
    //Create the outputfile
    if (aRun->GetRunID()==0){
        std::ofstream outFile("bragg_curve.txt");
        outFile << std::string(80, '#') << G4endl;
        outFile << "# Normalized dose profile over range of monoenergetic particle beams.\n"
                << "# First Row = length along detector; First Column = incident beam energy.\n"
                << "# Dose is normalized to number of primary particles (no. events)\n"
                << std::string(80, '#') << G4endl;
        outFile << std::setw(6) << 0 << std::setw(10);
        for (G4int i = 0; i < h1->axis().bins(); ++i) {
            G4double z = h1->axis().bin_center(i);
            outFile << z/mm << std::setw(10);
        }
        outFile << G4endl;
        outFile.close();
    }
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();

    auto h1 = analysisManager->GetH1(0);
    std::ofstream outFile("bragg_curve.txt", std::ios::app);
    G4int numberOfEvents = aRun->GetNumberOfEvent();

    if (G4Threading::IsMasterThread()) {
        G4double Ekin = fPrimary->GetIncidentEnergy();
        outFile << std::setw(6) <<  Ekin/MeV << std::setw(10);
        for (G4int i = 0; i < h1->axis().bins(); ++i) {
            G4double dose = h1->bin_height(i);
            G4double normalizedDose = dose/numberOfEvents;
            outFile << std::setw(10) << std::setprecision(4) << normalizedDose/MeV;
        }
        outFile << G4endl;
    }

    analysisManager->CloseFile();
}