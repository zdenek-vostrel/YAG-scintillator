import sys
sys.path.append("..")

import process
import numpy as np
import pathlib
import time

maxParallelProcesses = 1
submitJobs = True

def main():
    nParticles = 1e7
    widths = (np.array([0.75, 0.75, 0.75, 0.8, 0.8, 0.85, 0.9, 1.0, 1.0, 1.1, 1.1, 1.1, 1.2, 1.2, 1.3, 1.4, 1.4, 1.5, 1.5, 1.6, 1.6,0])[::-1]).cumsum()
    temperature = np.array([20, 25])
    pressure = np.array([1e-6, 1e-4, 1e-3])

    proc = process.Process(r"/Users/zdenekvostrel/Programming/UFP/AlfaCastice/build/alfa")

    for w in widths:
        for t in temperature:
            for p in pressure:
                dataDir = pathlib.Path("data/")
                fPath = dataDir.joinpath("t_" + str(t)).joinpath("p_"+str(p)).joinpath("w_"+str(w).replace(".", "_")).resolve()
                settings = {'fPath': str(fPath),
                            "/output/file": str(fPath.joinpath("output.root")),
                            "/construction/width": w,
                            "/construction/temperature": t,
                            "/construction/pressure": p,
                            "/run/reinitializeGeometry": " ",
                            "/run/beamOn": nParticles}
                idStr = ("t_" + str(t) + "_p_"+str(p) + "_w_" + str(w)).replace('.', '_')
                proc.addSimulation(idStr, settings)

    proc.runPBS()


if __name__ == '__main__':
    main()
