#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Timer.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

class RunAction : public G4UserRunAction {
    public:
        RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim);
        virtual ~RunAction();

        void BeginOfRunAction(const G4Run* aRun) override;
        void EndOfRunAction(const G4Run* aRun) override;
    
    private:
        DetectorConstruction* fDetector;
        PrimaryGeneratorAction* fPrimary;
        G4Timer fTimer;
        void PrintTime();
};
#endif