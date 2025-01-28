//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#ifndef ALFA_CONSTRUCTION_HH
#define ALFA_CONSTRUCTION_HH

#include <G4Box.hh>
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4GenericMessenger.hh"

class Construction : public G4VUserDetectorConstruction {
public:
    Construction(G4double length);
    ~Construction() override;

    G4VPhysicalVolume *Construct() override;
private:
    void ConstructSDandField();
    void DefineMaterial();
    void CreateAir();

    G4double len;

    G4Box *solidWorld, *solidFoil, *solidActiveDetector1, *solidActiveDetector2;
    G4LogicalVolume *logicWorld, *logicFoil, *logicActiveDetector1, *logicActiveDetector2;
    G4VPhysicalVolume *physWorldVolume, *physFoilVolume, *activeDetectorVolume1, *activeDetectorVolume2;
    G4Material *worldMaterial, *foilMaterial;

    G4GenericMessenger *messenger;
    G4double width = 1.5; // um
    G4double temperature = 20; // degree celsius
    G4double pressure = 1e-6; // Pa
};

#endif //ALFA_CONSTRUCTION_HH
