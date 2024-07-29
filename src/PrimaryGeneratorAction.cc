#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    G4int nParticles = 1;
    fParticleGun = new G4ParticleGun(nParticles);

    const G4String& particleName = "proton";

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

    fParticleGun->SetParticleDefinition(particle);
    G4ThreeVector momentumDir = G4ThreeVector(0,0,1);
    fParticleGun->SetParticleMomentumDirection(momentumDir);

    G4double energy = 500*MeV;
    fParticleGun->SetParticleEnergy(energy);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    G4ThreeVector position = G4ThreeVector(0,0,-5* cm);
    fParticleGun->SetParticlePosition(position);
    fParticleGun->GeneratePrimaryVertex(event);
}