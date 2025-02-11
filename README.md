# Realtime Receiver Viewer

This plugin aims to extend the receiver screen capabilities by adding logging options to store and share different kinds of data obtained from a simulator and a receiver (usually the one connected to the simulator).

## Functionalities

- **Whiteboard**: Displays NMEA messages from the connected receiver in real-time.
- **Serial Panel**:
    - Configure and connect a receiver.
    - Configure and enable receiver messages **Network** (UDP) and **File** logging.
- **Observer Config**:
    - Configure logging capabilities for simulation observers (simulated position and receiver position).
    - Allows UDP and file logging capabilities for each observer.
    - Logs are formatted in CSV format.
- **Summary Panel**:
    - Shows real-time parsed information extracted from the connected receiver (Time, Position, Altitude MSL, Altitude Ellipsoid, HDOP, PDOP, VDOP, and Fix type).
    - Shows real-time observer positions in ECEF format.

## Guide of Use

At initialization, the plugin will display the initial screen with the messages panel on the left and the serial configuration panel on the right.

![initial-screen](images/initial-screen.png)

In this screen, you can set the necessary parameters to connect to the serial receiver. By default, the screen will show all the parameters initialized with default values:

- **Port**: This is the name of the serial port where the serial receiver is connected. By default, it will show the first serial port connected and will allow you to select all the available ports in `/dev`.
- **Baud rate**: Default value is 9600. The list will show the default parameters for a GNSS receiver, but with "Other" you can specify a custom baud rate.
- **Log Path**: The path where the `rvv.log` logging file will be stored. This file contains all the messages transmitted by the receiver. Default: your `$HOME` directory.
- **Open folder...**: This button opens a file dialog that allows you to select a directory for the log file.
- **Log Address**: This is the address of the server that will receive the UDP messages from the network logger. This will receive all the messages transmitted by the receiver. Default: 127.0.0.1
- **Log Port**: This is the port where the server will receive the UDP messages from the network logger. Default: 8080
- **File Logging**: This option determines if the receiver messages will be logged to a file.
- **Network Logging**: This option determines if the receiver messages will be sent to a UDP server.
- **Connect**: This button connects the receiver following the previous parameters. This changes to Disconnect if it successfully connects to a receiver.

Once all the configurations are established, you can connect to the receiver.

![Receiver Connected](images/connected-receiver.png)

If you activate the Network Logging option and open a UDP socket on the specified port, you can see the messages on the UDP server.

![UDP server reading serial information](images/serialUDP.png)

In the Observer Config, you can access the configuration of the receivers and simulator observers that will show the ECEF coordinates in real-time during the simulation.

![Observer config panel](images/observerPanel.png)

The possible parameters are the following:
- **Simulator observer configuration**:
    - **Log Path**: The path where the `simulation_position_observer_output.csv` logging file will be stored. This file contains all the registered positions generated by the simulator. Default: your `$HOME` directory.
    - **Open folder...**: This button opens a file dialog that allows you to select a directory for the log file.
    - **Log Address**: This is the address of the server that will receive the UDP messages from the simulator observer network logger. This will receive all ECEF positions generated by the simulator. Default: 127.0.0.1
    - **Log Port**: This is the port where the server will receive the UDP messages from the simulator observer network logger. Default: 8081
    - **File Logging**: This option determines if the simulator observer positions will be logged to a file.
    - **Network Logging**: This option determines if the simulator observer positions will be sent to a UDP server.
- **Receiver configuration**:
    - **Log Path**: The path where the `receiver_position_output.csv` logging file will be stored. This file contains all the registered positions received from the receiver. Default: your `$HOME` directory.
    - **Open folder...**: This button opens a file dialog that allows you to select a directory for the log file.
    - **Log Address**: This is the address of the server that will receive the UDP messages from the receiver network logger. This will receive all ECEF positions received from the receiver. Default: 127.0.0.1
    - **Log Port**: This is the port where the server will receive the UDP messages from the receiver network logger. Default: 8081
    - **File Logging**: This option determines if the receiver positions will be logged to a file.
    - **Network Logging**: This option determines if the receiver positions will be sent to a UDP server.

If you activate the Network Logging option for both the simulator and the receiver, and configure them to send data to the same host and port, you can see the positions of both observers on the same UDP server.

![Both observers sending information to UDP server](images/observersUDP.png)

### Summary Panel

The Summary panel shows all the information obtained and processed by the serial receiver and the observers. This panel provides:

- **Receiver Information**:
    - **UTC**: Time
    - **Position**: Position in Lat/Lon format.
    - **Altitude (MSL)**: Altitude above mean sea level.
    - **Altitude (Ellipsoid)**: Altitude above the ellipsoid level.
    - **HDOP**: Horizontal Dilution of Precision.
    - **PDOP**: Position Dilution of Precision.
    - **VDOP**: Vertical Dilution of Precision.
    - **Fix**: Fix type.

- **Observers**:
    - **Position Receiver**: Position provided by the receiver based on GGA messages in ECEF format.
    - **Position Simulator**: Position simulated by the Skydel simulator.

![Summary panel](images/summary.png)

## Additional Information

For more details on how to use the plugin and its features, please refer to the user manual or the help section within the application.