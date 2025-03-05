# Interface interativa para entrada manual
import serial, sys, readline

class SimulatorCLI:
    def __init__(self, port):
        self.ser = serial.Serial(port, 9600)
        
    def start(self):
        print("💻 Modo de simulação manual (CTRL+C para sair)")
        while True:
            try:
                cmd = input("▶️ Comando (PAPP/HCHC/HPHC): ").strip().upper()
                val = input("🔢 Valor: ")
                self.ser.write(f"{cmd}:{val}\r\n".encode())
            except KeyboardInterrupt:
                print("\n🚫 Conexão encerrada")
                break

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python cli_injector.py [PORT]")
        sys.exit(1)
    SimulatorCLI(sys.argv[1]).start()