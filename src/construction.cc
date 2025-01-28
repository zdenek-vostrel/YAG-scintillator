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
    messenger->DeclareProperty("width", width, "Foil width in um. Default = 1.5");
    messenger->DeclareProperty("temperature", temperature, "Temperature in degrees celsius. Default = 20");
    messenger->DeclareProperty("pressure", pressure, "Pressure in Pa. Default = 1e-6");

    DefineMaterial();
}

Construction::~Construction() = default;

G4VPhysicalVolume *Construction::Construct() {
    CreateAir();

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

    // Active detector 1 construction
    solidActiveDetector1 = new G4Box("activeDetector1", 0.5 * foil_sizeXY, 0.5 * foil_sizeXY, 1*um);
    logicActiveDetector1 = new G4LogicalVolume(solidActiveDetector1, worldMaterial, "ActiveDetector");

    activeDetectorVolume1 = new G4PVPlacement(nullptr,
                                             G4ThreeVector(0, 0,  -0.5 *world_sizeZ + 2*cm + 1 *cm),
                                              logicActiveDetector1,
                                             "ActiveDetector",
                                             logicWorld,
                                             true,
                                             0,
                                             true);

    // Active detector 2 construction
    solidActiveDetector2 = new G4Box("activeDetector1", 0.5 * foil_sizeXY, 0.5 * foil_sizeXY, 1*um);
    logicActiveDetector2 = new G4LogicalVolume(solidActiveDetector2, worldMaterial, "ActiveDetector");

    activeDetectorVolume2 = new G4PVPlacement(nullptr,
                                              G4ThreeVector(0, 0,  0.5 *world_sizeZ - 1 *cm),
                                              logicActiveDetector2,
                                              "ActiveDetector",
                                              logicWorld,
                                              true,
                                              0,
                                              true);

    return physWorldVolume;
}

void Construction::DefineMaterial() {
    G4NistManager *nist = G4NistManager::Instance();
//    worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
    foilMaterial = nist->FindOrBuildMaterial("G4_Al");

}

void Construction::CreateAir() {
    G4NistManager* nist = G4NistManager::Instance();

    // Define the molar mass of air (kg/mol)
    G4double molarMassAir = 0.029; // kg/mol

    // Convert temperature to Kelvin
    G4double tempK = temperature + 273.15; // Celsius to Kelvin

    // Calculate density using the ideal gas law
    G4double density = (pressure * molarMassAir) / (8.314 * tempK); // kg/m³

    // Create the air material with the calculated density
    worldMaterial = new G4Material("CustomAir", density * kg / m3, 3);

    // Define the composition of air
    G4Element* N = nist->FindOrBuildElement("N");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* Ar = nist->FindOrBuildElement("Ar");

    worldMaterial->AddElement(N, 0.78);
    worldMaterial->AddElement(O, 0.21);
    worldMaterial->AddElement(Ar, 0.01);
}

void Construction::ConstructSDandField() {
    G4VSensitiveDetector *activeDetector1 = new ActiveDetector("ActiveDetector1", 0);
    logicActiveDetector1->SetSensitiveDetector(activeDetector1);

    G4VSensitiveDetector *activeDetector2 = new ActiveDetector("ActiveDetector2", 1);
    logicActiveDetector2->SetSensitiveDetector(activeDetector2);

}