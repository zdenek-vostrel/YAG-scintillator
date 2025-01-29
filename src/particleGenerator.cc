//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#include "../include/particleGenerator.hh"
#include <G4SystemOfUnits.hh>

particleGenerator::particleGenerator(G4double length){
    messenger = new G4GenericMessenger(this, "/particleGun/", "Parameters for constructions.");
    messenger->DeclareProperty("particleName", particleName, "Name of the particle to be generated. Default alpha.");
    messenger->DeclareProperty("energy", energy, "Kinetic energy of the generated particle [MeV], default 1 MeV.");

    len = length;
    fParticleGun = new G4ParticleGun(1);

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

particleGenerator::~particleGenerator(){
    delete fParticleGun;
}

void particleGenerator::GeneratePrimaries(G4Event *anEvent) {
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -0.5*len + 1 * mm));
    G4ParticleDefinition* particle
            = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(energy * MeV);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}