#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "G4VisExecutive.hh"
#include "QBBC.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include <iostream>

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;
    if (argc == 1){
        ui = new G4UIExecutive(argc,argv);
    }

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

	DetectorConstruction* detector = new DetectorConstruction();
	runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new PhysicsList());
	PrimaryGeneratorAction* primary = new PrimaryGeneratorAction();
    runManager->SetUserInitialization(new ActionInitialization(detector,primary));
    
    G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Run macro or start UI
	if (!ui) {
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	else {
		// run visualization
		UImanager->ApplyCommand("/control/execute vis.mac");

		// use UI
		ui->SessionStart();
		delete ui;
	}

	// clean up
	delete visManager;
	delete runManager;


	return 0;
}