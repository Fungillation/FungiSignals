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
import os 
import sys
import serial
import csv
import time
import matplotlib.pyplot as plt

# Arduino Serial Port Configuration
port = '/dev/cu.usbmodem14301'  # Replace with your Arduino's serial port
baud_rate = 115200

# Number of samples for averaging
average_samples = 600

# Output files
dir = "data_multisensor/"
csv_average_file = 'data_average.csv'
csv_direct_file = 'data_direct.csv'
plot_average_file = 'plot_average.png'
plot_direct_file = 'plot_direct.png'

# Sensor value indices
HUMIDITY_IDX = 0
TEMPERATURE_IDX = 1
VISIBILITY_IDX = 2
INFRARED_IDX = 3
UV_IDX = 4
MUSHROOM_IDX = 5

# Format time as string
def format_time(unformatted_time):
    s = time.gmtime(unformatted_time)
    formatted_time = time.strftime("%Y-%m-%d_%H:%M:%S", s)
    return formatted_time

# Function to save data to CSV file
def save_data_to_csv(data, filename, start_time, end_time):
    filename = f'{dir}{filename}_{start_time}_{end_time}.csv'
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file)
        header = ['Elapsed Time (s)', 'Humidity', 'Temperature (C)', 'Visibility', 'Infrared', 'UV', 'Mushroom Readings']
        writer.writerow(header)
        for i, values in enumerate(data):
            elapsed_time = i * sampling_interval
            row = [elapsed_time] + values
            writer.writerow(row)

# Read serial output and collect data
def read_serial_data(duration):
    arduino = serial.Serial(port, baud_rate)
    data_direct = [[] for _ in range(6)]
    data_average = []
    averaged_values = []
    start_time = time.time()
    end_time = start_time + duration

    while time.time() < end_time:
        # Read a line from serial port
        line = arduino.readline().decode().strip()

        # Split the line into separate values
        values = line.split(',')
        print(values)

        # Convert values to float (assuming the output is numeric)
        values = [float(value) for value in values]

        # Append direct values to data list
        for i, value in enumerate(values):
            data_direct[i].append(value)

        # Append values to data list for averaging
        data_average.append(values[MUSHROOM_IDX])

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
def plot_data(data_direct, averaged_values, start_time, end_time):
    elapsed_time_direct = [i for i in range(len(data_direct[0]))]
    elapsed_time_avg = [i * average_samples for i in range(len(averaged_values))]

    plt.figure(figsize=(10, 12))

    plt.subplot(611)
    plt.plot(elapsed_time_direct, data_direct[HUMIDITY_IDX], 'b-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('Humidity')
    plt.xlim(0, duration)

    plt.subplot(612)
    plt.plot(elapsed_time_direct, data_direct[TEMPERATURE_IDX], 'r-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('Temperature (C)')
    plt.xlim(0, duration)

    plt.subplot(613)
    plt.plot(elapsed_time_direct, data_direct[VISIBILITY_IDX], 'g-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('Visibility')
    plt.xlim(0, duration)

    plt.subplot(614)
    plt.plot(elapsed_time_direct, data_direct[INFRARED_IDX], 'm-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('Infrared')
    plt.xlim(0, duration)

    plt.subplot(615)
    plt.plot(elapsed_time_direct, data_direct[UV_IDX], 'c-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('UV')
    plt.xlim(0, duration)

    plt.subplot(616)
    plt.plot(elapsed_time_direct, data_direct[MUSHROOM_IDX], 'k-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('Mushroom Readings')
    plt.xlim(0, duration)

    plt.tight_layout()
    plt.savefig(fname=f'{dir}plot_direct{start_time}_{end_time}.png')
    plt.show()

    plt.figure()
    plt.plot(elapsed_time_avg, averaged_values, 'r-')
    plt.xlabel('Elapsed Time (s)')
    plt.ylabel('Average Mushroom Readings')
    plt.xlim(0, duration)
    plt.savefig(fname=f'{dir}plot_average{start_time}_{end_time}.png')
    plt.show()

# Main script
if __name__ == '__main__':
    # Parse command line arguments
    if len(sys.argv) != 2:
        print('Usage: python script.py <duration>')
        sys.exit(1)

    duration = int(sys.argv[1])
    os.makedirs(dir, exist_ok = True)

    # Sampling interval for elapsed time calculation
    sampling_interval = duration / average_samples

    # Read serial data
    data_direct, averaged_values, start_time, end_time = read_serial_data(duration)

    formatted_start = format_time(start_time)
    formatted_end = format_time(end_time)

    # Save data to CSV files
    save_data_to_csv(data_direct, csv_direct_file, formatted_start, formatted_end)
    save_data_to_csv([[avg] for avg in averaged_values], csv_average_file, formatted_start, formatted_end)

    # Plot data
    plot_data(data_direct, averaged_values, formatted_start, formatted_end)
