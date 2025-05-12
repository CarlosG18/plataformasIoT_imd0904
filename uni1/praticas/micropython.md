# 🚀 Gravando o Firmware MicroPython no ESP32

Siga este guia para gravar o firmware MicroPython no seu ESP32 usando o `esptool`.

🔗 **Download do firmware**: [Clique aqui para baixar](https://micropython.org/download/ESP32_GENERIC/)

---

## 🛠️ Pré-requisitos

* 🔧 Ferramenta `esptool` instalada ([veja aqui como instalar](https://github.com/espressif/esptool))
* 📦 Firmware `.bin` do MicroPython para ESP32 baixado
* 🧰 Cabo USB e driver instalados corretamente
* 💻 Porta serial identificada (ex: `COM14` no Windows)

---

## 1️⃣ Apagar a memória flash do ESP32

> ⚠️ **Importante**: Pressione e segure o botão `BOOT` durante este processo.

```cmd
esptool.exe --port <porta serial ao qual o esp esta conectado> erase_flash
```

### ✅ Saída esperada:

```cmd
esptool.py v4.6
Serial port COM14
Connecting.........
Detecting chip type... Unsupported detection protocol, switching and trying again...
Connecting......
Detecting chip type... ESP32
Chip is ESP32-D0WDQ6 (revision v1.0)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: 80:7d:3a:d5:21:b8
Uploading stub...
Running stub...
Stub running...
Erasing flash (this may take a while)...
Chip erase completed successfully in 4.1s
Hard resetting via RTS pin...
```

---

## 2️⃣ Gravando o firmware no ESP32

```cmd
esptool.exe --port <porta serial ao qual o esp esta conectado> --baud 460800 write_flash 0x1000 <caminho do arquivo binario do firmware>
```

### ✅ Saída esperada:

```cmd
esptool.py v4.6
Serial port COM14
Connecting.........
Detecting chip type... Unsupported detection protocol, switching and trying again...
Connecting.................
Detecting chip type... ESP32
Chip is ESP32-D0WDQ6 (revision v1.0)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: 80:7d:3a:d5:21:b8
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00001000 to 0x001a0fff...
Compressed 1702240 bytes to 1117021...
Wrote 1702240 bytes (1117021 compressed) at 0x00001000 in 30.5 seconds (effective 446.8 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```

---

## ✅ Pronto!

Agora seu ESP32 está com o MicroPython instalado! Você pode conectar com o [Thonny IDE](https://thonny.org/) ou outro terminal serial para começar a programar em Python! 🐍✨
