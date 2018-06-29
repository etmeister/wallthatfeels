import serial
import json 
import subprocess
import time

states = { 15: False, 16: False, 17: False, 18: False, 19: False, 22: False } 
files = {
15:    "Celesta.m4a",
16:    "Cello 1.m4a",
17:    "Taiko Drums.m4a",
18:    "Tubular Bells.m4a",
19:    "Vibes High.m4a",
22:    "Violin 5.m4a"
}


#files = {
#15:    "Celesta.m4a",
#16:    "Cello 1.m4a",
#17:    "Glockenspiel 1.m4a",
#18:    "Gongs.m4a",
#19:    "Log Drums.m4a",
#22:    "Marimba.m4a" }
#files = { 15: "Vibes 1.m4a", 16: "Wood Blocks.m4a", 17: "Marimba.m4a", 18: "Taiko Drums.m4a", 19: "Cello.m4a", 22: "Violin 1.m4a" }

procs = { 15: False, 16: False, 17: False, 18: False, 19: False, 22: False }

def playSound(button):
    proc = subprocess.Popen(["afplay","/Users/eimeister/Documents/installation/Music/"+ files[button] ])
    endtime = time.time() + 2.00
    if procs[button] is False:
        procs[button] = [[  proc, endtime ]]
    else:
        procs[button].append([proc, endtime])


with serial.Serial('/dev/cu.usbmodem4012401', 38400, timeout=1) as ser:
    while True:
        line = ser.readline().rstrip()
        print(line)
        try: 
            buttons = json.loads(line) 
        except ValueError:
            print("Error reading button status")
            continue
        loopStart = str(time.time()).split('.')[1][0]
        for key in states:
            if key in buttons:
                if states[key] is not True:
                     if loopStart == "0":# or loopStart == "5":
                         states[key] = True
                         playSound(key)
                else:
                    for proc in procs[key]:
                        proc[0].poll()
                        if proc[0].returncode is not None:
                            procs[key].remove(proc)
                            playSound(key)
            else:
                states[key] = False
                if procs[key]:
                    for proc in procs[key]:
                        if time.time() > proc[1]:
                            procs[key].remove(proc)
                            proc[0].terminate()
                            


