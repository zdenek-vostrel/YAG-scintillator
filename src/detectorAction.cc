//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#include "../include/detectorAction.hh"

#include "G4AnalysisManager.hh"

DetectorAction::DetectorAction(){
    messenger = new G4GenericMessenger(this, "/output/", "Output file settings.");
    messenger->DeclareProperty("file", outFile, "Output file");
}

DetectorAction::~DetectorAction() = default;

void DetectorAction::BeginOfRunAction(const G4Run *) {
    auto *man = G4AnalysisManager::Instance();

    man->OpenFile(outFile);

    man->CreateNtuple("Hits1", "Hits1");
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleIColumn("Particle");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->FinishNtuple(0);
}

void DetectorAction::EndOfRunAction(const G4Run *) {
    auto *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile(outFile);
    G4cout << "Results saved to: " << outFile << G4endl;
}