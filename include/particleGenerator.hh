//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#ifndef ALFA_PARTICLEGENERATOR_HH
#define ALFA_PARTICLEGENERATOR_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include "G4GenericMessenger.hh"
#include <G4SystemOfUnits.hh>
#include <G4ParticleTable.hh>

class particleGenerator : public G4VUserPrimaryGeneratorAction {
public:
    particleGenerator(G4double length);
    ~particleGenerator();

    virtual void GeneratePrimaries(G4Event *anEvent);
private:
    G4ParticleGun *fParticleGun;
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4double len;
    G4GenericMessenger *messenger;
    G4String particleName = "alpha"; 
    G4float energy = 1 * MeV; 
};

#endif //ALFA_PARTICLEGENERATOR_HH
