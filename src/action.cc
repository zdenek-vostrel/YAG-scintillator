//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#include "../include/action.hh"
#include "../include/detectorAction.hh"
#include "../include/particleGenerator.hh"

Action::Action(G4double length){
    len = length;
}

Action::~Action() = default;

void Action::Build() const {
    auto generator = new particleGenerator(len);
    SetUserAction(generator);

    auto *detectorAction = new DetectorAction();
    SetUserAction(detectorAction);
}