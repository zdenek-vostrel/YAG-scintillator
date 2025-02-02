import sys
sys.path.append("..")

import process
import numpy as np
import pathlib
import time

maxParallelProcesses = 1
submitJobs = True

def main():
    nParticles = 2e2
    particle = np.array(["proton"])
    energy = np.array([0.05*(1+x) for x in range(60)]) # MeV
    widths = np.array([20]) # mm

    proc = process.Process(r"/data2/user_data/atlas/vostrel/UFP/YAG_scintillator/build/alfa")

    for w in widths:
        for e in energy:
            for p in particle:
                dataDir = pathlib.Path("data/")
                fPath = dataDir.joinpath("w_"+str(w).replace(".", "_")).joinpath(str(p)).joinpath("E_"+str(e).replace(".", "_")).resolve()
                settings = {'fPath': str(fPath),
                            "/output/file": str(fPath.joinpath("output.root")),
                            "/construction/width": w,
                            "/particleGun/particleName": p,
                            "/particleGun/energy": e,
                            "/run/reinitializeGeometry": " ",
                            "/run/beamOn": nParticles}
                idStr = ("w_" + str(w) + "_p_"+str(p) + "_e_" + str(e)).replace('.', '_')
                proc.addSimulation(idStr, settings)

    proc.runPBS()


if __name__ == '__main__':
    main()
