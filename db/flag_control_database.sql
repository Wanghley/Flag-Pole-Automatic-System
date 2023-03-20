-- Create Users table
CREATE TABLE Users (
    user_id INT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL
);

-- Create Devices table
CREATE TABLE Devices (
    device_id INT PRIMARY KEY,
    device_name VARCHAR(255) NOT NULL,
    ip_address VARCHAR(255) NOT NULL,
    user_id INT NOT NULL,
    flagpole_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    FOREIGN KEY (flagpole_id) REFERENCES Flagpoles(flagpole_id)
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

-- Create Status table
CREATE TABLE Status (
    status_id INT PRIMARY KEY,
    flag_id INT NOT NULL,
    status VARCHAR(255) NOT NULL,
    FOREIGN KEY (flag_id) REFERENCES Flags(flag_id)
);

-- Create Flagpoles table
CREATE TABLE Flagpoles (
    flagpole_id INT PRIMARY KEY,
    flagpole_name VARCHAR(255) NOT NULL,
    building_name VARCHAR(255) NOT NULL
);
