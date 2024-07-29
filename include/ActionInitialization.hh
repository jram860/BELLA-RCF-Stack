#ifndef ActionInitialization_hh
#define ActionInitialization_hh

#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

class ActionInitialization : public G4VUserActionInitialization
{
    public:
        ActionInitialization(DetectorConstruction*,PrimaryGeneratorAction*);
        virtual ~ActionInitialization();

        virtual void Build() const;
        virtual void BuildForMaster() const;

    private:
        DetectorConstruction* fDetector;
        PrimaryGeneratorAction* fPrimary;
};
#endif