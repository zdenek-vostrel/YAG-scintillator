#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4EmLowEPPhysics.hh>
#include <G4hIonisation.hh>
#include <G4SystemOfUnits.hh>
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4StepLimiter.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4eMultipleScattering.hh"

#include "G4BosonConstructor.hh"

#include "../include/physics.hh"

Physics::Physics() {
    RegisterPhysics (new G4EmPenelopePhysics());
    RegisterPhysics (new G4StepLimiterPhysics());

    // RegisterPhysics (new G4EmStandardPhysics());
    // RegisterPhysics (new G4EmLowEPPhysics());
    // RegisterPhysics(new G4DecayPhysics());
    // // Decay Physics is always defined
    // fDecayPhysicsList = new G4DecayPhysics();

    // // EM physics
    // fEmPhysicsList = new G4EmLowEPPhysics();
}

void Physics::ConstructParticle() {
    // Register the standard particles
    G4VModularPhysicsList::ConstructParticle();
}

void Physics::ConstructProcess(){
    G4VModularPhysicsList::ConstructProcess();
}

// void Physics::ConstructProcess() {
//     AddTransportation();

//     // This will register physics for standard particles
//     fEmPhysicsList->ConstructProcess();
//     fDecayPhysicsList->ConstructProcess();
// }