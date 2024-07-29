#ifndef SteppingAction_hh
#define SteppingAction_hh

#include "G4UserSteppingAction.hh"
#include "G4LogicalVolume.hh"
#include "DetectorConstruction.hh"

class SteppingAction : public G4UserSteppingAction {
    public:
        SteppingAction(DetectorConstruction*);
        ~SteppingAction();

        void UserSteppingAction(const G4Step*) override;

    private: 
        G4LogicalVolume* calorVolume = nullptr;
        DetectorConstruction* fDetector;

};

#endif