import serial
import time

print("Opening ttyUSB0...")

esp32 = serial.Serial(
    port="/dev/ttyUSB0",
    baudrate=115200,
    timeout=1,
    dsrdtr=False,
    rtscts=False
)

esp32.dtr = False
esp32.rts = False

print("Connected to ESP32")

time.sleep(3)

print("-------------------")
print("Waiting for TEST_SIGNAL...")
print("-------------------")

pass_count = 0
fail_count = 0

while pass_count < 5:

    data = esp32.readline().decode(
        "utf-8",
        errors="ignore"
    ).strip()

    if data == "":
        continue

    print("Received:", data)

    if data == "TEST_SIGNAL":
        pass_count += 1
        print("Test Result: PASS")
        print("Test Count:", pass_count)

    else:
        print("Startup/Other Message - Ignored")

esp32.close()

print("-------------------")
print("Test completed.")
print("Total PASS:", pass_count)
print("Total FAIL:", fail_count)
print("-------------------")
