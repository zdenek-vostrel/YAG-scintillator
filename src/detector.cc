//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#include "../include/detector.hh"

#include <G4AnalysisManager.hh>
#include "G4RunManager.hh"

ActiveDetector::ActiveDetector(G4String name, G4int id) : G4VSensitiveDetector(name) {
    detectorID = id;
};

ActiveDetector::~ActiveDetector() = default;

G4bool ActiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    G4Track *track = aStep->GetTrack();
    auto preStepPoint = aStep->GetPreStepPoint();

    auto particle = track->GetParticleDefinition();
    G4String particleName = particle->GetParticleName();
    G4int particleTypeID = particle->GetPDGEncoding();
//    G4cout << "PARTICLE NAME: " << particleName << G4endl;
//    std::cout << "PARTICLE NAME: " << particleName << " ENERGY: " << preStepPoint->GetKineticEnergy() << " MASS: "
//              << particle->GetPDGMass() << std::endl;
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4ThreeVector position = preStepPoint->GetPosition();
    G4ThreeVector momentum = preStepPoint->GetMomentum();
    G4double energy = preStepPoint->GetKineticEnergy();

    auto *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(detectorID, 0, evt); // event
    man->FillNtupleIColumn(detectorID, 1, particleTypeID); // Particle (particle type)
    man->FillNtupleDColumn(detectorID, 2, energy); // energy
    man->FillNtupleDColumn(detectorID, 3, position[0]); // X
    man->FillNtupleDColumn(detectorID, 4, position[1]); // Y
    man->FillNtupleDColumn(detectorID, 5, position[2]); // Z
    man->FillNtupleDColumn(detectorID, 6, momentum[0]); // X
    man->FillNtupleDColumn(detectorID, 7, momentum[1]); // Y
    man->FillNtupleDColumn(detectorID, 8, momentum[2]); // Z
    man->AddNtupleRow(detectorID);

    return true;
}