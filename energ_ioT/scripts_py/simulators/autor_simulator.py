# Simulação automática com dados randômicos
import serial, time, random

def main():
    with serial.Serial('/dev/ttyACM0', 9600) as ser:
        while True:
            data = f"PAPP:{random.randint(800, 2500)}\r\n"
            data += f"HCHC:{random.randint(5000, 15000)}\r\n"
            data += f"HPHC:{random.randint(2000, 10000)}\r\n"
            
            ser.write(data.encode())
            print(f"📤 Enviado: {data.strip().replace('\r\n', ' | ')}")
            time.sleep(15)

if __name__ == "__main__":
    main()