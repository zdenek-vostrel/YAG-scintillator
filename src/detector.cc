//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#include "../include/detector.hh"

#include <G4AnalysisManager.hh>
#include "G4RunManager.hh"

ActiveDetector::ActiveDetector(G4String name, G4int id) : G4VSensitiveDetector(name) {
    detectorID = id;
}

ActiveDetector::~ActiveDetector() = default;

G4bool ActiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    const G4Track* track = aStep->GetTrack();
    auto particle = track->GetParticleDefinition();
    G4String particleName = particle->GetParticleName();
      
    // Check if the particle is an alpha particle or a proton
    if (particleName == "alpha" || particleName == "proton") {
        G4double energy = track->GetKineticEnergy();
        
        // Check if the energy is below the threshold
        if (energy < threshold) { // Define a threshold for stopping
            G4ThreeVector position = track->GetPosition();
            G4cout << "Alpha particle stopped at: " << position << G4endl;

            G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            G4int particleTypeID = particle->GetPDGEncoding();

            auto *man = G4AnalysisManager::Instance();
            man->FillNtupleIColumn(detectorID, 0, evt); // event
            man->FillNtupleIColumn(detectorID, 1, particleTypeID); // Particle (particle type)
            man->FillNtupleDColumn(detectorID, 2, position[0]); // X
            man->FillNtupleDColumn(detectorID, 3, position[1]); // Y
            man->FillNtupleDColumn(detectorID, 4, position[2]); // Z
        }
    }
    
    return true; 
}