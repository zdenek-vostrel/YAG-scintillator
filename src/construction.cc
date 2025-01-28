#include <G4NistManager.hh>
#include "../include/construction.hh"
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VPhysicalVolume.hh>
#include "../include/detector.hh"

Construction::Construction(G4double length) {
    len = length;

    messenger = new G4GenericMessenger(this, "/construction/", "Parameters for constructions.");
    messenger->DeclareProperty("width", width, "Foil width in mm. Default = 10 mm");

    DefineMaterial();
}

Construction::~Construction() = default;

G4VPhysicalVolume *Construction::Construct() {

    double world_sizeZ = len;
    double world_sizeXY = 24*cm;

    double foil_sizeZ = width * um;
    double foil_sizeXY = world_sizeXY;

    // World construction
    solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 *world_sizeZ);
    logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World");

    physWorldVolume = new G4PVPlacement(nullptr,
                                        G4ThreeVector(0., 0., 0),
                                        logicWorld,
                                        "World",
                                        nullptr,
                                        false,
                                        0,
                                        true
    );

    // Foil construction
    solidFoil = new G4Box("Foil", 0.5 * foil_sizeXY, 0.5 * foil_sizeXY, 0.5 * foil_sizeZ);
    logicFoil = new G4LogicalVolume(solidFoil, foilMaterial, "Foil");

    physFoilVolume = new G4PVPlacement(nullptr,
                                       G4ThreeVector(0., 0.,  -0.5 *world_sizeZ + 2*cm),
                                       logicFoil,
                                       "Foil",
                                       logicWorld,
                                       false,
                                       0,
                                       true);

    return physWorldVolume;
}

void Construction::DefineMaterial() {
    G4NistManager *nist = G4NistManager::Instance();
    worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");

    // Define YAG (Y3Al5O12)
    G4double density = 4.56 * g / cm3; // Density of YAG
    G4Material* YAG = new G4Material("YAG", density, 3);
    
    // Define the elements
    G4Element* Y = nist->FindOrBuildElement("Y");
    G4Element* Al = nist->FindOrBuildElement("Al");
    G4Element* O = nist->FindOrBuildElement("O");

    // Define the composition
    YAG->AddElement(Y, 3);
    YAG->AddElement(Al, 5);
    YAG->AddElement(O, 12);

    foilMaterial = YAG;

}

void Construction::ConstructSDandField() {
    G4VSensitiveDetector *activeDetector1 = new ActiveDetector("ActiveDetector", 0);
    logicFoil->SetSensitiveDetector(activeDetector1);
}