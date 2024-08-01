#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction();
        virtual ~DetectorConstruction();
    
        virtual G4VPhysicalVolume* Construct();

        G4LogicalVolume* GetCalorVolume() const {return calorVolume; }
        G4double GetCalorSizeZ() const {return calorSizeZ;}

    private:
        void ReadGeometryFile(const std::string& fileName);
        void ConstructCalorimeter(G4LogicalVolume* logicWorld);
        void DefineMaterials();
        void PrintCalorParameters();

        // Initialize some calorimeter geometry parameters
        G4LogicalVolume* calorVolume = nullptr;
        G4double calorSizeXY = 10*cm;
        G4double calorSizeZ;
        G4double worldSizeZ;
        G4Material* worldMaterial; // need this to access in ConstructCalorimeter()
        
        // Store the readout from the geometry file
        G4String volType;
        G4String volMaterial;
        G4double volThickness;
        std::vector<G4String> layerType;
        std::vector<G4String> layerMaterial;
        std::vector<G4double> layerThickness;
};
#endif