#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Proton.hh"


SteppingAction::SteppingAction(DetectorConstruction* det): G4UserSteppingAction(),fDetector(det) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    
    calorVolume = fDetector->GetCalorVolume();
    G4int ParentID = step->GetTrack()->GetParentID();
    G4double edep = step->GetTotalEnergyDeposit();
    G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
    G4double stepLength = step->GetStepLength();

    if (!calorVolume) {return;}
    if (edep <=0) {return;}

    if (ParentID == 0 && step->GetTrack()->GetDefinition() == G4Proton::ProtonDefinition()) {
        G4double z1 = step->GetPreStepPoint()->GetPosition().z();
        G4double z2 = step->GetPostStepPoint()->GetPosition().z();
        G4double z = z1 + G4UniformRand()*(z2-z1);
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(0,z,edep);
    }

}

