import socket
import time
import sys
import machine
import ubinascii

ENERGY = 100
NPIN = 34
PLAYER = 'nachillo'
PATH = '/hit'
PORT = 8000
HOST = f'192.168.1.10'

WIFI_SSID = 'toniToni'
WIFI_PWD = 'cocoloco'

def connect_wifi():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect(WIFI_SSID, WIFI_PWD)
        global MAC
        MAC = ubinascii.hexlify(wlan.config("mac")).decode()
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())


def send_hit(energy):
    payload = f"energy,player={PLAYER} value={energy}"
    data = f'POST {PATH} HTTP/1.1\r\nHost: {HOST}:{PORT}\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: {len(payload)}\r\n\r\n{payload}'
    addr = socket.getaddrinfo(HOST, PORT)[0][-1]
    s = socket.socket()
    s.connect(addr)
    print(data)
    s.send(bytes(data, 'utf8'))
    while True:
        data = s.recv(100)
        if data:
            print(str(data, 'utf8'), end='')
        else:
            break
    s.close()


connect_wifi()
pin = machine.Pin(NPIN, machine.Pin.IN)
print(f"Energy: {ENERGY}")
while True:
    hit = not pin.value()
    if hit:
        ENERGY -= 1
        print('HIT!')
        print(f"Energy: {ENERGY}")
        try:
            send_hit(ENERGY)
        except Exception as e:
            print('Bad HTTP response.')
            print(e)

    if ENERGY <= 0:
        print('Die!!!')
        sys.exit()
    time.sleep(0.1)
