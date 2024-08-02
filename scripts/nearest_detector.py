#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from scipy.signal import find_peaks

plt.rcParams.update({
    'axes.titlesize': 20,
    'axes.labelsize': 16,  # Size of axis labels
    'axes.labelweight': 'regular',  # Weight of axis labels
    'xtick.labelsize': 14,  # Size of x-tick labels
    'ytick.labelsize': 14  # Size of y-tick labels
})

def parse_dose_file(filename):
    df  = np.genfromtxt(filename)
    z = df[0,1:]
    E_init = df[1:,0]
    dose = df[1:,1:]
    return z,E_init,dose


def parse_geometry_file(filename):
    layers = []
    materials = []

    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('#') or not line.strip():
                continue  # Skip comments and empty lines
            parts = line.strip().split()
            layer_type, material,thickness = parts
            layers.append((layer_type.strip(),material.strip(), float(thickness.strip())))
            materials.append(material)

    return layers, materials

def find_last_peak(dose):
    peaks = []
    for row in dose:
        peak_indices, _ = find_peaks(row)
        peaks.append(peak_indices[-1])

    return peaks

def calculate_detector_parameters(layers):
    detector_index = []
    detector_midpoints = []
    z = 0
    hdv2_ebt3_counter = 1 
    for i, (layer_type, material, thickness) in enumerate(layers):
        z += thickness / 2  # Move to the midpoint of the current layer
        if material == 'HDv2' or material == 'EBT3':  # Check if the current layer is a detector
            detector_index.append(hdv2_ebt3_counter)
            detector_midpoints.append(z)
            hdv2_ebt3_counter += 1
        z += thickness / 2  # Move to the end of the current layer

    return detector_index,detector_midpoints

def find_nearest_detector(peaks,z,detector_midpoints,max_distance):
    closest_detectors = []
    for peak in peaks:
        if peak is not None:
            peak_position = z[peak]
            closest_detector = None
            min_distance = float('inf')
            for i, midpoint in enumerate(detector_midpoints):
                distance = abs(midpoint - peak_position)
                if distance < min_distance:
                    min_distance = distance
                    closest_detector = i
            if min_distance <= max_distance:
                closest_detectors.append(closest_detector)
            else:
                closest_detectors.append(None)
        else:
            closest_detectors.append(None)
    return closest_detectors



z,E_init,dose = parse_dose_file('bragg_curveLivermore.txt')
print(z)
layers, materials = parse_geometry_file('geomConfig.txt')

peaks = find_last_peak(dose)
detector_index,detector_midpoints = calculate_detector_parameters(layers)
closest_detectors = find_nearest_detector(peaks,z,detector_midpoints,1)
print(closest_detectors)
for i, detector in enumerate(closest_detectors):
    if detector is not None:
        print(f"Initial Energy: {E_init[i]}, Layer Number: {detector_index[detector]}")

E_plot = []
Layer_no = []
for i, detector in enumerate(closest_detectors):
    if detector is not None:
        E_plot.append(E_init[i])
        Layer_no.append(detector_index[detector])


fig,ax = plt.subplots()

ax.scatter(Layer_no,E_init)
ax.grid(True,linestyle=':')
ax.set_xlabel('Detector Number')
ax.set_ylabel('Incident Energy')
ax.set_title('RCF Stack Sensitivity')
plt.xticks(detector_index)
plt.yticks(E_plot)
plt.show()