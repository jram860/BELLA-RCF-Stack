#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"

#include <fstream>
#include <sstream>

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    ReadGeometryFile("geomConfig.txt");
    // Construct the World
    G4NistManager* nist = G4NistManager::Instance();
    G4double worldSizeXY = 2*calorSizeXY;
    if (calorSizeZ<=1*m) {worldSizeZ = 2*m;} else {worldSizeZ =2.5*calorSizeZ;}
    worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
    auto solidWorld = new G4Box("World", worldSizeXY/2,worldSizeXY/2,worldSizeZ/2);
    auto logicWorld = new G4LogicalVolume(solidWorld,worldMaterial,"World");
    auto physiWorld = new G4PVPlacement(nullptr,G4ThreeVector(),logicWorld,"World",nullptr,false,0);

    DefineMaterials();
    ConstructCalorimeter(logicWorld);
    PrintCalorParameters();

    return physiWorld;
}


void DetectorConstruction::ReadGeometryFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open()) {
        G4cerr << "Error opening file: " << fileName << G4endl;
        return;
    }

    while (std::getline(file,line)) {
        if (line.empty() || line[0] == '#') {continue;}

        std::istringstream iss(line);
        G4String volType;
        G4String volMaterial;
        G4double volThickness;
        
        iss >> volType >> volMaterial >> volThickness;

        if (iss.fail()) {
            G4cerr << "Error reading line: " << line << G4endl;
            continue;
        }

        calorSizeZ += volThickness*mm;

        if (volType == "Filter") {
            filterMaterials.push_back(volMaterial);
            filterThicknesses.push_back(volThickness*mm);
        } else if (volType == "Detector") {
            detectorMaterials.push_back(volMaterial);
            detectorThicknesses.push_back(volThickness*mm);
        }
        
    }
}

void DetectorConstruction::DefineMaterials() {
    G4NistManager *nist = G4NistManager::Instance();
    // Define Custom Materials
    G4int ncomponents;
    
    // Gafchromic Film substrate... good enough approximation for the real composite material
    G4double substrateDensity = 1.2595*g/cm3;
    G4Material *HDv2 = new G4Material("Gafchromic_substrate", substrateDensity,ncomponents=6);
    HDv2->AddElement(nist->FindOrBuildElement("H"),38.8*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Li"),0.1*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("C"),43.4*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("O"),17.5*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Na"),0.1*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Cl"),0.1*perCent);
}

void DetectorConstruction::ConstructCalorimeter(G4LogicalVolume* logicWorld) {

    // Set some visualization attributes:
    G4VisAttributes* calorVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //Calorimeter is blue
    calorVisAtt->SetVisibility(true);
    
    G4VisAttributes* filterVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); // Filters are red
    filterVisAtt->SetVisibility(true);    
    
    G4VisAttributes* detectorVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0)); // Detectors are green
    detectorVisAtt->SetVisibility(true);

    // Construct the Calorimeter
    G4NistManager *nist = G4NistManager::Instance();

    auto solidCalor = new G4Box("Calorimeter", calorSizeXY/2,calorSizeXY/2,calorSizeZ/2);
    auto logicCalor = new G4LogicalVolume(solidCalor,worldMaterial,"Calorimeter");
    //Assign the calorimeter volume to the logical volume. This makes some the logic easier to interpret in other classes. 
    calorVolume = logicCalor;
    logicCalor->SetVisAttributes(calorVisAtt);
    new G4PVPlacement(0,G4ThreeVector(0,0,calorSizeZ/2),logicCalor,"Calorimeter",logicWorld,false,0,false);

    // The initial position of the layers in the calorimeter.
    // The front face of calorimeter is at origin. 
    G4double z = -calorSizeZ/2;

    // Iterate over number of filters so that each filter/detector pair have same copy number
    for (size_t i = 0; i < filterMaterials.size(); ++i) {
        //Construct the filter
        G4Material* filterMat = nist->FindOrBuildMaterial("G4_" + filterMaterials[i]);
            if (!filterMat) {G4Material* filterMat = G4Material::GetMaterial(filterMaterials[i]);}
        
        G4Box* solidFilter = new G4Box("Filter", calorSizeXY/2, calorSizeXY/2, filterThicknesses[i]/2);
        G4LogicalVolume* logicFilter = new G4LogicalVolume(solidFilter,filterMat,"logicFilter");
        logicFilter->SetVisAttributes(filterVisAtt);
        new G4PVPlacement(0,G4ThreeVector(0,0,z+filterThicknesses[i]/2),logicFilter,"Filter",logicCalor,false,i,true);
        
        z += filterThicknesses[i];

        G4Material* detectorMat = nist->FindOrBuildMaterial("G4_" + detectorMaterials[i]);
            if (!detectorMat) {G4Material* detectorMat = G4Material::GetMaterial(detectorMaterials[i]);}

        G4Box* solidDetector = new G4Box("Detector",calorSizeXY/2,calorSizeXY/2,detectorThicknesses[i]/2);
        G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector,detectorMat,"logicDetector");
        logicDetector->SetVisAttributes(detectorVisAtt);
        new G4PVPlacement(0,G4ThreeVector(0,0,z+detectorThicknesses[i]/2),logicDetector,"Detector",logicCalor,false,i,true);

        z += detectorThicknesses[i];
    }
}

void DetectorConstruction::PrintCalorParameters() {
    G4int nbLayers= filterMaterials.size() + detectorMaterials.size();
    //Print out Calorimeter geometry information
    G4cout << std::string(80, '-') << G4endl;
    G4cout << "---> Calorimeter is constructed" << G4endl;
    G4cout << "\t---> Number of Layers: " << nbLayers << G4endl;
    G4cout << "\t---> Total Thickness: " << calorSizeZ/mm << "mm" << G4endl;
    G4cout << "\t--->  Transverse Size: " << calorSizeXY/mm << "mm" << G4endl;
    G4cout << std::string(80, '-') << G4endl;
}