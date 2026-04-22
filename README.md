# 📌 RFID-Based Audio Assistance System (ESP32)

## 📖 Overview
This project is designed to help users (especially visually impaired individuals) by playing **audio messages when an RFID tag is scanned**.

When an RFID tag is detected:
- ESP32 reads the tag UID
- Matches it with predefined data
- Plays a corresponding audio message using a speaker

---

## 🚀 Features
- 📡 RFID tag detection
- 🔊 Audio playback for each tag
- ⚡ Compact and battery-powered system
- 🎯 Useful for smart navigation & assistance

---

## 🧰 Components Required
- ESP32
- RC522 RFID Module
- MAX98357A I2S Audio Amplifier
- Speaker (4Ω / 3W recommended)
- Jumper wires
- 18650 Battery (3.7V) + Charging Module (TP4056)
- Boost Converter (if needed for stable 5V)

---

## 🔌 Wiring Connections

### 📡 RC522 RFID → ESP32
| RC522 | ESP32 |
|------|------|
| SDA  | GPIO 5 |
| SCK  | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| RST  | GPIO 22 |
| GND  | GND |
| 3.3V | 3.3V |

---

### 🔊 MAX98357A → ESP32
| MAX98357A | ESP32 |
|----------|-------|
| VIN | 5V / VIN |
| GND | GND |
| BCLK | GPIO 26 |
| LRC | GPIO 25 |
| DIN | GPIO 27 |

---

### 🔈 Speaker → MAX98357A
| Speaker | MAX98357A |
|--------|----------|
| + | SPK+ |
| - | SPK- |

---

## ⚙️ How It Works
1. Power ON the system  
2. RFID module scans nearby tag  
3. ESP32 reads UID  
4. Code checks UID in database  
5. Corresponding audio is played  

---

## 💻 Sample Logic
```cpp
if (uid == "TAG1") {
   playAudio("Location 1");
}
else if (uid == "TAG2") {
   playAudio("Location 2");
}
