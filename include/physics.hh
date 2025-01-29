#ifndef ALFA_PHYSICS_HH
#define ALFA_PHYSICS_HH

#include <G4VModularPhysicsList.hh>
#include "G4GenericMessenger.hh"
#include <G4SystemOfUnits.hh>

class Physics : public G4VModularPhysicsList{
public:
    Physics();
    ~Physics() = default;

    G4VPhysicsConstructor* fEmPhysicsList;
    G4VPhysicsConstructor* fDecayPhysicsList;

    void ConstructParticle() override;
    void ConstructProcess() override;
};

#endif //ALFA_PHYSICS_HH