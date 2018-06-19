import serial
import json 
import subprocess
import time

states = { 15: False, 16: False, 17: False, 18: False, 19: False, 22: False } 

files = { 15: "Vibes 1.m4a", 16: "Wood Blocks.m4a", 17: "Marimba.m4a", 18: "Taiko Drums.m4a", 19: "Glockenspiel 1.m4a", 22: "Tympani.m4a" }

procs = { 15: False, 16: False, 17: False, 18: False, 19: False, 22: False }

def playSound(button):
    procs[button] = subprocess.Popen(["afplay","/Users/eimeister/Documents/Music/"+ files[button] ])



with serial.Serial('/dev/cu.usbmodem4012401', 38400, timeout=1) as ser:
    while True:
        line = ser.readline().rstrip()
        try: 
            buttons = json.loads(line) 
        except ValueError:
            print("Error reading button status")
            continue
        loopStart = str(time.time()).split('.')[1][0]
        for key in states:
            if key in buttons:
                if states[key] is not True:
                     if loopStart == "0":
                         states[key] = True
                         playSound(key)
                else:
                    procs[key].poll()
                    if procs[key].returncode is not None:
                        playSound(key)
            else:
                states[key] = False
                if procs[key]:
                    procs[key].terminate()
                


