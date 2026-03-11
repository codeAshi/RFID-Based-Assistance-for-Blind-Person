import serial
import subprocess

ser = serial.Serial('COM4',115200, timeout=1)

print("Listening for tag data...")

while True:
    line = ser.readline().decode(errors='ignore').strip()

    # ignore ESP32 boot messages
    if (
        line == "" or
        line.startswith("ets") or
        line.startswith("rst") or
        line.startswith("config") or
        line.startswith("clk") or
        line.startswith("mode") or
        line.startswith("load") or
        line.startswith("entry") or
        line.startswith("ho") or
        line == "Scan tag..."
    ):
        continue

    print("Tag:", line)

    # speak using Windows built-in speech
    subprocess.run([
        "powershell",
        "-Command",
        f"Add-Type –AssemblyName System.Speech; "
        f"(New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('{line}')"
    ])