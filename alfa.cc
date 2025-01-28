#include "iostream"
#include <fstream>
#include <string>
#include <sstream>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UImanager.hh"
#include <G4SystemOfUnits.hh>

#include "QBBC.hh"

#include "include/construction.hh"
#include "include/action.hh"

int main(int argc, char** argv){
    auto *runManager = new G4RunManager();

    G4double length = 2.5 * m;

    runManager->SetUserInitialization(new Construction(length));
    runManager->SetUserInitialization(new QBBC());
    runManager->SetUserInitialization(new Action(length));

    runManager->Initialize();

    G4UIExecutive *ui = 0;
    if (argc == 1){
        ui = new G4UIExecutive(argc, argv);
    }

//    auto *ui = new G4UIExecutive(argc, argv);
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager =  G4UImanager::GetUIpointer();

    if(ui){
//        UImanager->ApplyCommand("/control/execute positronProduction.mac");
        UImanager->ApplyCommand("/vis/open OGL");
        UImanager->ApplyCommand("/vis/drawVolume");
        UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
        UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
        ui->SessionStart();
    }
    else{
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }
    delete runManager;

    return 0;
}