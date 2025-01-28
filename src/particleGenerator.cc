//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#include "../include/particleGenerator.hh"
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>

particleGenerator::particleGenerator(G4double length){
    len = length;
    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle
            = particleTable->FindParticle("alpha");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticleEnergy(5.5 * MeV);

}

particleGenerator::~particleGenerator(){
    delete fParticleGun;
}

void particleGenerator::GeneratePrimaries(G4Event *anEvent) {
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -0.5*len + 1 * mm));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}