#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from scipy.signal import find_peaks

def parse_dose_file(filename):
    df  = np.genfromtxt(filename)
    return df

def parse_geometry_file(filename):
    layers = []
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('#') or not line.strip():
                continue  # Skip comments and empty lines
            parts = line.strip().split()
            if len(parts) == 3:
                layer_type, material,thickness = parts
                layers.append((layer_type.strip(), float(thickness.strip())))

    return layers

def plot_dose(df,layers):
    z = df[0,1:]
    E_init = df[1:,0]
    dose = df[1:,1:]
    # Initialize a list to store the peaks
    peaks = []

    current_y = 0
    fig, ax = plt.subplots()

    for layer_type, thickness in layers:
        if layer_type == 'Detector':
            rect = patches.Rectangle((0, current_y), E_init.max(), thickness, color='red', alpha=0.5)
            ax.add_patch(rect)
        current_y += thickness

    # Loop through each row in the 2D array
    for row in dose:
        peak_indices, _ = find_peaks(row)
        peaks.append(peak_indices[-1])

    ax.scatter(E_init,z[peaks])

    ax.set_ylim(0, current_y)
    ax.set_xlabel('Incident Energy [MeV]')
    ax.set_ylabel('Depth into Detector [mm]')
    ax.set_title('Energy deposition profile')
    ax.grid()
    ax.minorticks_on()
    ax.grid(True,linestyle=':')
    fig.tight_layout()

    plt.show()

df = parse_dose_file('bragg_curve.txt')
layers = parse_geometry_file('geomConfig.txt')

plot_dose(df,layers)