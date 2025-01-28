//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#ifndef ALFA_PARTICLEGENERATOR_HH
#define ALFA_PARTICLEGENERATOR_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>

class particleGenerator : public G4VUserPrimaryGeneratorAction {
public:
    particleGenerator(G4double length);
    ~particleGenerator();

    virtual void GeneratePrimaries(G4Event *anEvent);
private:
    G4ParticleGun *fParticleGun;
    G4double len;
};

#endif //ALFA_PARTICLEGENERATOR_HH
