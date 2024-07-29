#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det, PrimaryGeneratorAction* prim)
: G4VUserActionInitialization(),fDetector(det), fPrimary(prim) 
{}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction(fDetector,fPrimary));
}

void ActionInitialization::Build() const 
{
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction(fDetector,fPrimary));
    SetUserAction(new SteppingAction(fDetector));
}

