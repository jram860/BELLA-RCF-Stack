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
        void ConstructCalorimeter(G4LogicalVolume* physWorld);
        void DefineMaterials();
        void PrintCalorParameters();

        // Initialize some calorimeter geometry parameters
        G4LogicalVolume* calorVolume = nullptr;
        G4double calorSizeXY = 10*cm;
        G4double calorSizeZ;
        G4double worldSizeZ;
        G4Material* worldMaterial; // need this to access in ConstructCalorimeter()
        
        // Store the readout from the geometry file
        std::vector<G4String> filterMaterials;
        std::vector<G4double> filterThicknesses;
        std::vector<G4String> detectorMaterials;
        std::vector<G4double> detectorThicknesses;
};
#endif