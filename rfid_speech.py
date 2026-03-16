import serial
import win32com.client
import time

speaker = win32com.client.Dispatch("SAPI.SpVoice")

ser = serial.Serial('COM4',115200, timeout=1)

print("Listening for tag data...")
prev = ""
prev_time = 0
while True:
    try : 
        line = ser.readline().decode(errors='ignore').strip()
        ts = time.time()
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
        if(prev_time < ts - 2 and line != 'Tag Record not found'): 
            print("Tag:", line)
            # speak immediately
            speaker.Speak(line)
        elif (prev_time < ts - 3 and line == prev):
            speaker.Speak("Tag Record not found")

        elif(line == prev) : 
            # print(line , prev)
            # speaker.Speak("Wait 2 seconds")
            # speaker.Speak("Tag Record not found")
            continue

            
        prev_time = ts
        prev = line
    except (e):
        print("Failed")