import sys
sys.path.append("..")

import process
import numpy as np
import pathlib
import time

maxParallelProcesses = 1
submitJobs = True

def main():
    nParticles = 1e3
    particle = np.array(["alpha"])
    energy = np.array([3,4,5,6]) # MeV

    # energy = np.array([0.05*(1+x) for x in range(60)]) # MeV

    widths = np.array([20]) # um
    alWidths = np.array([10]) # um

    proc = process.Process(r"/data2/user_data/atlas/vostrel/UFP/YAG_scintillator/build/alfa")

    for w in widths:
        for alW in alWidths:
            for e in energy:
                for p in particle:
                    dataDir = pathlib.Path("data/")
                    
                    def func_fPath():
                        fPath = dataDir.joinpath("w_"+str(w).replace(".", "_"))
                        fPath = fPath.joinpath("AlW_"+str(alW).replace(".", "_"))
                        fPath = fPath.joinpath(str(p))
                        fPath = fPath.joinpath("E_"+str(e).replace(".", "_"))
                        
                        return fPath.resolve()
                               
                    fPath = func_fPath()                  
                    settings = {'fPath': str(fPath),
                                "/output/file": str(fPath.joinpath("output.root")),
                                "/construction/width": w,
                                "/construction/AlWidth": alW,
                                "/particleGun/particleName": p,
                                "/particleGun/energy": e,
                                "/run/reinitializeGeometry": " ",
                                "/run/beamOn": nParticles}
                    
                    idStr = ("w_" + str(w) + "_alW_" + str(alW) + "_p_"+str(p) + "_e_" + str(e)).replace('.', '_')
                    proc.addSimulation(idStr, settings)

    proc.runPBS()


if __name__ == '__main__':
    main()
