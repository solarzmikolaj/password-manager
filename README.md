# 🔐 Password Manager

> 💻 **A simple and functional password manager written in C++** — generates secure passwords, stores credentials locally, and provides an intuitive console-based menu system.

![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/Build-CMake-darkgreen?logo=cmake)
![Platform](https://img.shields.io/badge/Platform-CLI-lightgrey?logo=windows-terminal)
![Status](https://img.shields.io/badge/Status-Active-success)
![License](https://img.shields.io/badge/License-MIT-orange)

---

## 📝 Overview

This project is a lightweight **console-based password manager** built in C++.  
It allows users to quickly generate strong passwords, save them to local storage, and manage them through a clean, user-friendly terminal interface.  
The application is modular, easy to extend, and designed with clarity and simplicity in mind.

---

## ⚙️ Features

### 🧠 **Password Generator** (`PasswordGenerator`)
- Creates strong, randomized passwords  
- Supports multiple character sets  
- Allows custom password length  
- Designed to ensure high entropy and security  

### 💾 **File Storage & Database Operations**  
`OperationsOnFile`, `OperationOnDatabase`
- Handles reading and writing credential data to files  
- Ensures consistent file formatting and safe operations  
- Stores passwords locally without external dependencies  
- Simple structure that can be extended with encryption later  

### 📋 **Record Management** (`Record`)
- Represents stored credentials (service, username, password)  
- Makes adding, editing, and listing entries straightforward  
- Clear data structure, easy to expand with new fields  

### 🧭 **Console Menu System** (`Menus`)
- Intuitive CLI interface for interacting with the application  
- Menu options for:
  - Generating a new password  
  - Adding or viewing saved entries  
  - Removing stored records  
  - Managing the database  
- Beginner-friendly, readable, and well-organized  

---

## 🧱 Project Structure

```
├── src/
│   ├── password/          # Password generation
│   ├── database/          # File & database operations
│   ├── ui/                # Console menus
│   └── models/            # Record structure
├── include/               # Header files
├── CMakeLists.txt         # Build configuration
└── README.md
```

---

## 🎯 Purpose of the Project

This Password Manager serves as both a **useful tool** and a **learning project**, aiming to:
- Practice modern C++ and OOP principles  
- Work with file I/O in a safe and structured way  
- Build modular, maintainable code  
- Provide a small but practical application  
