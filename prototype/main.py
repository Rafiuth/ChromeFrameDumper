from pathlib import Path
import sys
import frida
import psutil

def find_target_process():
    return 2128
    for proc in psutil.process_iter(attrs=["name", "cmdline"]):
        try:
            #if (proc.name() == "chrome.exe" and "--service-sandbox-type=cdm" in proc.cmdline()):
            if (proc.name() == "chrome.exe" and "--type=renderer" in proc.cmdline()):
                return proc.pid
        except:
            pass

def on_message(message, data):
    print("[on_message] message:", message, "data:", data)

def main():
    pid = find_target_process()
    if pid == None:
        print("CDM process not found")
        return
    
    print("Attaching into", pid)

    session = frida.attach(pid)

    source = Path("agent.js").read_text()
    script = session.create_script(source)
    script.on("message", on_message)
    script.load()

    sys.stdin.read()
    
main()