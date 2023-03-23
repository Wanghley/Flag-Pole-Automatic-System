-- Create Users table
CREATE TABLE Users (
    user_id INT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL
);

-- Create Devices table
CREATE TABLE Devices (
    mac_address INT PRIMARY KEY,
    device_name VARCHAR(255) NOT NULL,
    ip_address VARCHAR(255) NOT NULL,
    status_id INT NOT NULL,
);

-- Create Flags table
CREATE TABLE Flags (
    flag_id INT PRIMARY KEY,
    flag_name VARCHAR(255) NOT NULL,
    device_id INT NOT NULL,
    flagpole_id INT NOT NULL,
    FOREIGN KEY (device_id) REFERENCES Devices(device_id),
    FOREIGN KEY (flagpole_id) REFERENCES Flagpoles(flagpole_id)
);

-- Create Flagpoles table
CREATE TABLE Flagpoles (
    flagpole_id INT PRIMARY KEY,
    flagpole_name VARCHAR(255) NOT NULL,
    building_name VARCHAR(255) NOT NULL,
    latitude FLOAT NOT NULL,
    longitude FLOAT NOT NULL
);
