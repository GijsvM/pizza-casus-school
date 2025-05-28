# Pizza Casus - School Project

This repository contains the Arduino code for the **Pizza Casus** school project, which consists of three interconnected embedded systems designed to streamline and monitor the pizza preparation process. Each system receives commands via HTTP requests and plays a specific role in the pizza-making workflow.

> ⚠️ The WinForms control application and central server responsible for sending the HTTP requests are **not included** in this repository.

## Project Overview

The project is divided into three main hardware modules:

1. **Buzzer System**
2. **Servo System (Ingredient Boxes)**
3. **Oven System**

Each module runs independently and is controlled over HTTP via a centralized system.

---

## 🔴🟠🟢 Buzzer System

### Function
Indicates the status of a pizza order using LEDs:

- 🔴 **Red** – Order received
- 🟠 **Orange** – Pizza in progress
- 🟢 **Green** – Pizza ready

### Communication
- Controlled by HTTP requests sent from the WinForms desktop application.
- Listens for status updates and changes LED color accordingly.

---

## 🍅🥓🧀 Servo System (Ingredient Boxes)

### Function
Controls access to ingredient compartments to minimize mistakes:

- Ingredient boxes are locked by default.
- Only boxes corresponding to ingredients required for the specific pizza order will open.

### Communication
- Controlled via HTTP requests from the central server.
- Each request specifies which ingredients are required for the current pizza.

---

## 🔥 Oven System

### Function
Manages the heating process for cooking the pizza:

- Can be manually controlled using physical buttons.
- Can also be triggered remotely via HTTP commands from the server.

### Features
- Adjustable temperature and bake time.
- Safety features to prevent overheating.
- Dual-mode operation: manual and automated.

---

## Installation

1. Clone or download this repository.
2. Flash the relevant Arduino sketch (`buzzer`, `servo`, or `oven`) to the appropriate microcontroller.
3. Connect the components as described in each file's comments.
4. Set up your network so that the microcontrollers can receive HTTP requests from the control server (not included).

---

## Usage

- Ensure all microcontrollers are powered and connected to the same network as the central server.
- Send HTTP requests to each module to:
  - Update order status (buzzer).
  - Open relevant ingredient compartments (servo).
  - Start/stop oven heating process (oven).

---

## Note

- This repository only contains the embedded system code.
- Server logic and WinForms application for sending HTTP requests are **not included**.
- The project was developed as part of a school assignment focusing on automation and IoT in food preparation.

---
