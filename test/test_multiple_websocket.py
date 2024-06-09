
import subprocess
for i in range(5):
    subprocess.Popen(["python", "test_websocket.py"])
    print(f"Test {i+1} started")