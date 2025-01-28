//
// Created by Zdeněk Vostřel on 12.11.2024.
//

#ifndef ALFA_ACTION_HH
#define ALFA_ACTION_HH

#include <G4VUserActionInitialization.hh>
#include "G4GenericMessenger.hh"

class Action : public G4VUserActionInitialization {
public:
    Action(G4double length);
    ~Action();
    virtual void Build() const;
private:
    G4double len;
};


#endif //ALFA_ACTION_HH
