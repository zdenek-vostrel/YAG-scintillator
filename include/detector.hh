//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#ifndef ALFA_DETECTOR_HH
#define ALFA_DETECTOR_HH

#include <G4VSensitiveDetector.hh>

class ActiveDetector : public G4VSensitiveDetector {
public:
    ActiveDetector(G4String name, G4int id);
    ~ActiveDetector();
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
private:
    G4int detectorID;
    G4float threshold = 10 * keV;
};

#endif //ALFA_DETECTOR_HH
