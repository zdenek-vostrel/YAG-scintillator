import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import pathlib

class Analysis(object):
    def __init__(self, f, outDir, total_width):
        root_file = uproot.open(f)
        tree = root_file['Hits1']

        self.df = tree.arrays(library='pd')
        self.outDir = pathlib.Path(outDir)
        self.outDir.mkdir(parents=True, exist_ok=True)
        self.total_width = total_width
        
        self._analyse()
        
    def _analyse(self):
        self.df["Zceil"] = np.ceil(self.df["Z"]*1000)/1000
        self.Zreached = self.df.groupby("Event")["Z"].max().reset_index()
        
        dff_temp = pd.DataFrame(index=pd.MultiIndex.from_product([self.df["Event"].unique(),self.df["Zceil"].unique() ], names=["Event", "Zceil"])).reset_index()
        self.df_full = pd.merge(self.df, dff_temp, on=["Event", "Zceil"], how="right").fillna(0)
    
    def plot(self):
        self._plot_energy_loss()
    
    def _plot_energy_loss(self):
        # Sample data: heights of each bin and their right edges
        results = self.df_full.groupby("Zceil")["EnergyDeposit"].mean().reset_index()
        results_errors = self.df_full.groupby("Zceil")["EnergyDeposit"].std().reset_index().fillna(0)
        heights = results["EnergyDeposit"] # Heights of the bins
        right_edges = np.arange(self.total_width)+0.5  # Right edges of the bins

        # Create the histogram with outlines only
        plt.bar(right_edges, np.append(np.array(heights), np.zeros(20-len(heights))), width=1, edgecolor='black', fill=False)
        plt.errorbar(right_edges[:len(heights)], heights, yerr=results_errors["EnergyDeposit"], fmt='none', color='black', capsize=5)

        def percentile(n, color):
            percentile_n = np.percentile(np.array(self.Zreached["Z"]*1000), n)
            plt.axvline(x=percentile_n, color=color, linestyle='--', label=f'{n} percentile: {percentile_n:.1f} um')

        percentile(99, "red")
        percentile(90, "orange")
        percentile(50, "green")

        # Set labels and title
        plt.ylabel('Energy deposit [MeV]')
        plt.xlabel('Depth [um]')
        
        plt.ylim(0)

        plt.legend()

        # Show the plot
        plt.savefig(self.outDir.joinpath("enegy_deposition.png"), dpi=300, bbox_inches='tight')
        plt.close()


def main():
    directory = pathlib.Path(r'/data2/user_data/atlas/vostrel/UFP/YAG_scintillator/scripts/data/w_20')
    
    files_to_analyse = list(directory.rglob('*.root'))
    
    def extractName(p):
        # Split the path into parts
        parts = str(p).split('/')
    
        # Extract the desired parts
        # Assuming the structure is consistent, we can access the parts directly
        w_value = parts[9]  # This corresponds to 'w_20'
        alpha_value = parts[10]  # This corresponds to 'alpha'
        e_value = parts[11] # This corresponds to 'E_5', split to get '5'
        
        return w_value+"_"+alpha_value+"_"+e_value
    
    for f in files_to_analyse:
        print(f"Plotting for: {f}")
        a = Analysis(f, pathlib.Path.cwd().joinpath("data").joinpath(extractName(f)), 20)
        a.plot()
    
if __name__ == '__main__':
    main()
