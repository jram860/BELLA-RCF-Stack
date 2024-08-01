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

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    DefineMaterials();
    ReadGeometryFile("geomConfig.txt");
    // Construct the World
    G4NistManager* nist = G4NistManager::Instance();
    G4double worldSizeXY = 2*calorSizeXY;
    if (calorSizeZ<=1*m) {worldSizeZ = 2*m;} else {worldSizeZ =2.5*calorSizeZ;}
    worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
    auto solidWorld = new G4Box("World", worldSizeXY/2,worldSizeXY/2,worldSizeZ/2);
    auto logicWorld = new G4LogicalVolume(solidWorld,worldMaterial,"World");
    auto physiWorld = new G4PVPlacement(nullptr,G4ThreeVector(),logicWorld,"World",nullptr,false,0);

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
        
        iss >> volType >> volMaterial >> volThickness;

        if (iss.fail()) {
            G4cerr << "Error reading line: " << line << G4endl;
            continue;
        }

        calorSizeZ += volThickness*mm;

        layerType.push_back(volType);
        layerMaterial.push_back(volMaterial);
        layerThickness.push_back(volThickness*mm);
    }
}

void DetectorConstruction::DefineMaterials() {
    G4NistManager *nist = G4NistManager::Instance();
    // Define Custom Materials
    G4int ncomponents;

    // Gafchromic film substrate
    // G4double substrateDensity = 1.35*g/cm3;
    // G4Material *GS = new G4Material("GS", substrateDensity,ncomponents=3);
    // GS->AddElement(nist->FindOrBuildElement("H"), 36.4*perCent);
    // GS->AddElement(nist->FindOrBuildElement("C"), 45.5*perCent);
    // GS->AddElement(nist->FindOrBuildElement("O"), 18.2*perCent);

    G4double substrateDensity = 1.2595*g/cm3;
    G4Material *GS = new G4Material("GS", substrateDensity,ncomponents=6);
    GS->AddElement(nist->FindOrBuildElement("H"),38.8*perCent);
    GS->AddElement(nist->FindOrBuildElement("Li"),0.1*perCent);
    GS->AddElement(nist->FindOrBuildElement("C"),43.4*perCent);
    GS->AddElement(nist->FindOrBuildElement("O"),17.5*perCent);
    GS->AddElement(nist->FindOrBuildElement("Na"),0.1*perCent);
    GS->AddElement(nist->FindOrBuildElement("Cl"),0.1*perCent);
    //HD-V2 Active Layer
    G4double HDv2_Density = 1.2595*g/cm3;
    G4Material *HDv2 = new G4Material("HDv2", HDv2_Density,ncomponents=9);
    HDv2->AddElement(nist->FindOrBuildElement("H"), 58.1*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Li"), 0.6*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("C"), 27.7*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("N"), 0.4*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("O"),11.7*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Na"), 0.5*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Al"), 0.3*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("S"), 0.1*perCent);
    HDv2->AddElement(nist->FindOrBuildElement("Cl"), 0.6*perCent);

    //EBT3 Active Layer
    G4double EBT3_density = 1.20*g/cm3;
    G4Material *EBT3 = new G4Material("EBT3", EBT3_density, ncomponents = 5);
    EBT3->AddElement(nist->FindOrBuildElement("H"), 56.8*perCent);
    EBT3->AddElement(nist->FindOrBuildElement("Li"), 0.6*perCent);
    EBT3->AddElement(nist->FindOrBuildElement("C"), 27.6*perCent);
    EBT3->AddElement(nist->FindOrBuildElement("O"), 13.3*perCent);
    EBT3->AddElement(nist->FindOrBuildElement("Al"), 1.7*perCent);

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

    for (size_t i = 0; i < layerType.size(); ++i) {

        G4Material* layerMat = nist->FindOrBuildMaterial("G4_" + layerMaterial[i]);
        if (!layerMat) {layerMat = G4Material::GetMaterial(layerMaterial[i]);}

        if(layerType[i] == "Filter"){
            G4Box* solidFilter = new G4Box("Filter", calorSizeXY/2, calorSizeXY/2, layerThickness[i]/2);
            G4LogicalVolume* logicFilter = new G4LogicalVolume(solidFilter,layerMat,"logicFilter");
            logicFilter->SetVisAttributes(filterVisAtt);
            new G4PVPlacement(0,G4ThreeVector(0,0,z+layerThickness[i]/2),logicFilter,"Filter",logicCalor,false,i,true);
            
            z += layerThickness[i];
        }

        if (layerType[i] == "Detector") {
            G4Box* solidDetector = new G4Box("Detector",calorSizeXY/2,calorSizeXY/2,layerThickness[i]/2);
            G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector,layerMat,"logicDetector");
            logicDetector->SetVisAttributes(detectorVisAtt);
            new G4PVPlacement(0,G4ThreeVector(0,0,z+layerThickness[i]/2),logicDetector,"Detector",logicCalor,false,i,true);

            z += layerThickness[i];
        }
    }
}

void DetectorConstruction::PrintCalorParameters() {
    //Print out Calorimeter geometry information
    G4int nbLayers = layerType.size();
    G4cout << std::string(80, '-') << G4endl;
    G4cout << "---> Calorimeter is constructed" << G4endl;
    G4cout << "\t---> Number of Layers: " << nbLayers << G4endl;
    G4cout << "\t---> Total Thickness: " << calorSizeZ/mm << "mm" << G4endl;
    G4cout << "\t--->  Transverse Size: " << calorSizeXY/mm << "mm" << G4endl;
    G4cout << std::string(80, '-') << G4endl;
}