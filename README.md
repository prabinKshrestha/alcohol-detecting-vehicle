# Alcohol Detecting Vehicle

An **Alcohol Detecting Vehicle** is an embedded system project designed to prevent drunk driving accidents. This system uses an **MQ3 gas sensor** to detect the presence of alcohol in the driver’s breath and automatically stops the vehicle from operating if alcohol is detected. The aim is to promote road safety using a proactive approach.

## 🧠 Project Summary

This project leverages embedded systems and sensors to detect alcohol and control the vehicle accordingly. The main components include:

* **MQ3 Alcohol Sensor**
* **Arduino Uno**
* **Relay Module**
* **DC Motor**
* **Buzzer & LED Indicators**
* **12V Battery**

Once the alcohol concentration exceeds a threshold, the system disables the motor, emits a buzzer alert, and lights up an LED.

---

## 🔧 Features

* Real-time alcohol detection via MQ3 sensor
* Automatic vehicle lock on alcohol detection
* Audible and visual alerts (buzzer + LED)
* Fail-safe embedded circuit design
* Easy to implement with low-cost hardware

---

## ⚙️ How It Works

1. The **MQ3 sensor** continuously monitors the air near the driver for alcohol vapor.
2. If alcohol is detected:

   * The **buzzer** and **LED** are activated.
   * The **relay module** cuts power to the **DC motor**, stopping the vehicle.
3. If no alcohol is detected, the vehicle operates normally.

---

## 📂 Project Structure

```bash
alcohol-detecting-vehicle/
│
├── minor_project_alcohol_detector.pdf   # Full project documentation
├── alcDet_GSM_GPS                       # Source code
└── README.md                            # Project summary and setup instructions
```

## 📄 Documentation

For full project explanation, circuit diagrams, and analysis, refer to:

📄 [minor\_project\_alcohol\_detector.pdf](https://github.com/prabinKshrestha/alcohol-detecting-vehicle/blob/main/minor_project_alcohol_detector.pdf)

---

## 👥 Authors

* **Prabin Kshrestha**
* Team Members as listed in PDF
