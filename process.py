import pathlib
import subprocess
import yaml
import time


def getLogDir(dataDir : pathlib.Path) -> pathlib.Path:
    dataDir = pathlib.Path(dataDir)
    logDir = dataDir.joinpath('logs')
    logDir.mkdir(parents=True, exist_ok=True)
    return logDir

class Simulation(object):
    def __init__(self, dataDir: pathlib.Path | str, runSettings: dict, idStr: str):
        self.dataDir: pathlib.Path = pathlib.Path(dataDir)
        self.runSettings: dict = runSettings
        self.idStr: str = idStr


class Process(object):
    def __init__(self, runFile: pathlib.Path | str):
        self.runFile = pathlib.Path(runFile)

        self._loadedSimulations = {}

    def addSimulation(self, idStr: str, runSettings: dict) -> None:
        self._loadedSimulations[idStr] = runSettings

    def createYamlFile(self, idStr: str, runSettings: dict) -> pathlib.Path:
        logDir = getLogDir(runSettings["fPath"])

        yamlFile = logDir.joinpath(f'{idStr}_settings.yaml')

        with open(yamlFile, 'w') as f:
            yaml.dump(runSettings, f)

        return yamlFile

    def runLocally(self, nParallel: int) -> None:
        waitingProcesses = len(self._loadedSimulations)
        runningProcesses = []

        def queueCheck():
            for name, p, process_start_time in runningProcesses:
                if p.poll() is not None:  # True if process finished
                    runningProcesses.remove([name, p, process_start_time])  # remove from running processes
                    # update scan properties
                    duration = time.gmtime(time.time() - process_start_time)
                    print(f"Process {name} finished after {time.strftime('%H:%M:%S', duration)})")

        for idStr, runSettings in self._loadedSimulations.items():
            while nParallel <= len(runningProcesses):
                time.sleep(10)
                queueCheck()
            runningProcesses += [[idStr, self._submitLocally(idStr, runSettings), time.time()]]
            waitingProcesses -= 1

        while len(runningProcesses) > 0:
            time.sleep(10)
            queueCheck()

        return

    def runPBS(self):
        for idStr, runSettings in self._loadedSimulations.items():
            self._submitPBS(idStr, runSettings)

    def _submitLocally(self, idStr: str, runSettings: dict) -> subprocess:
        print("Running process!")

        yamlFile = self.createYamlFile(idStr, runSettings)

        p = subprocess.Popen(['python', self.runFile, '-r', yamlFile])
        return p

    def _createMacFile(self, idStr, runSettings):
        logDir = getLogDir(runSettings["fPath"])
        macFile = logDir.joinpath(f"{idStr}.mac")

        useInt = ["/run/beamOn"]
        
        with open(macFile, "w") as f:
            for k, v in runSettings.items():
                if k[0] == "/":
                    f.write(str(k) + " " + str(v if k not in useInt else int(v)) + "\n")

        return macFile

    def _submitPBS(self, idStr: str,  runSettings: dict) -> None:
        macFile = self._createMacFile(idStr, runSettings)

        logDir = getLogDir(runSettings["fPath"])
        errorFile = logDir.joinpath(f"error-{idStr}.txt")
        outputFile = logDir.joinpath(f"output-{idStr}.txt")

        shellScript = "#! /bin/bash\n"
        shellScript += f"#PBS -e '{errorFile.resolve()}'\n"
        shellScript += f"#PBS -o '{outputFile.resolve()}'\n"
        shellScript += f"#PBS -q normal\n" # max 24 hours
        shellScript += "\n"
        shellScript += "source /data2/user_data/atlas/vostrel/UFP/YAG_scintillator/G4_setup.sh\n"
        shellScript += "export QT_DEBUG_PLUGINS=1\n"
        shellScript += "export QT_QPA_PLATFORM='minimal'\n"
        shellScript += "export DISPLAY=localhost:0.0\n"
        shellScript += f"{str(self.runFile.resolve())} {str(macFile.resolve())}\n"

        shellScriptFile = logDir.joinpath(f'{idStr}_submit.sh')
        with open(shellScriptFile, 'w') as f:
            f.write(shellScript)

        subprocess.Popen(['qsub', str(shellScriptFile.resolve())])
