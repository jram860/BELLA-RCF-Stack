#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"

#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4LossTableManager.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"

// Different options to test
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsGS.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    //RegisterPhysics(new G4EmStandardPhysics());
    //RegisterPhysics(new G4EmStandardPhysics_option3());
    RegisterPhysics(new G4EmLivermorePhysics());
}

PhysicsList::~PhysicsList() {}

void PhysicsList::ConstructParticles() {
    G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess() {
    G4VModularPhysicsList::ConstructProcess();
}