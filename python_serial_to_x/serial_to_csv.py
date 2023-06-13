"""
Serial Data Reader and Plotter

Reads the serial output of an Arduino for a specified duration, saves the data to CSV files,
and generates plots using Matplotlib.

Usage: python script.py <duration>

Arguments:
    <duration>  Duration in seconds to read serial data

Requirements:
    - pyserial
    - matplotlib
"""

import sys
import os
import serial
import csv
import time
import matplotlib.pyplot as plt

# Arduino Serial Port Configuration
port = '/dev/cu.usbmodem14301'  # Replace with your Arduino's serial port
baud_rate = 9600

# Number of samples for averaging
average_samples = 60

# Output files
dir = "data/"
csv_average_file = 'data_average'
csv_direct_file = 'data_direct'
plot_average_file = 'plot_average'
plot_direct_file = 'plot_direct'

# Function to save data to CSV file
def save_data_to_csv(data,dir, filename, start_time, end_time):
    filename = f'{dir}{filename}_{start_time}_{end_time}.csv'
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Index', 'Value'])
        for i, value in enumerate(data):
            writer.writerow([i, value])

# Read serial output and collect data
def read_serial_data(duration):
    arduino = serial.Serial(port, baud_rate)
    data_direct = []
    data_average = []
    averaged_values = []
    
    start_time = time.time()
    end_time = start_time + duration

    while time.time() < end_time:
        # Read a line from serial port
        line = arduino.readline().decode().strip()

        # Convert line to float (assuming the output is numeric)
        value = float(line)

        # Append direct value to data list
        data_direct.append(value)

        # Append value to data list for averaging
        data_average.append(value)

        # Perform averaging when enough samples are collected
        if len(data_average) >= average_samples:
            # Calculate average
            average_value = sum(data_average) / average_samples

            # Append average value to averaged values list
            averaged_values.append(average_value)

            # Clear the list for the next set of samples
            data_average = []

    # Close the serial port
    arduino.close()

    return data_direct, averaged_values, start_time, end_time

# Plot data
def plot_data(data_direct,dir, averaged_values, start_time, end_time):
    plt.figure(1)
    plt.subplot(211)
    plt.plot(data_direct, 'b-')
    plt.xlabel('Sample')
    plt.ylabel('Direct Value')

    plt.subplot(212)
    plt.plot(averaged_values, 'r-')
    plt.xlabel('Sample')
    plt.ylabel('Average Value')

    plt.tight_layout()
    plt.savefig(fname=f'{dir}plot_{start_time}_{end_time}.png')
    plt.show()

# Format time as string
def format_time(unformatted_time):
    s = time.gmtime(unformatted_time)
    formatted_time = time.strftime("%Y-%m-%d_%H:%M:%S", s)
    return formatted_time

# Main script
if __name__ == '__main__':
    # Parse command line arguments
    if len(sys.argv) != 2:
        print('Usage: python script.py <duration>')
        sys.exit(1)

    duration = int(sys.argv[1])
    os.makedirs(dir, exist_ok = True)

    # Read serial data
    data_direct, averaged_values, start_time, end_time = read_serial_data(duration)

    formatted_start = format_time(start_time)
    formatted_end = format_time(end_time)

    # Save data to CSV files
    save_data_to_csv(averaged_values,dir, csv_average_file, formatted_start, formatted_end)
    save_data_to_csv(data_direct, dir, csv_direct_file, formatted_start, formatted_end)

    # Plot data
    plot_data(data_direct, dir, averaged_values, formatted_start, formatted_end)
