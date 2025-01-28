//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#ifndef ALFA_DETECTORACTION_HH
#define ALFA_DETECTORACTION_HH

#include "G4UserRunAction.hh"
#include "G4GenericMessenger.hh"

class DetectorAction : public G4UserRunAction {
public:
    DetectorAction();
    ~DetectorAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    G4GenericMessenger *messenger;
    G4String outFile = "output.root";
};

#endif //ALFA_DETECTORACTION_HH
