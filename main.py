import _thread
import socket
import network
import socket
import time
import sys
import machine
import ubinascii

# Modificar estas variables para cada jugador
PLAYER = 'humitos'
HOST = '192.168.1.10'

ENERGY = 100
NPINS = (
    34,
    35,
    32
)
PATH = '/write?db=pylaser'
PORT = 8086

WIFI_SSID = 'toniToni'
WIFI_PWD = 'cocoloco'

def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        wlan.connect(WIFI_SSID, WIFI_PWD)
        while not wlan.isconnected():
            pass


def send_hit(energy, player, path, host, port):
    payload = f"energy,player={player} value={energy}"
    data = f'POST {path} HTTP/1.1\r\nHost: {host}:{port}\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: {len(payload)}\r\n\r\n{payload}'
    addr = socket.getaddrinfo(host, port)[0][-1]
    s = socket.socket()
    s.connect(addr)
    s.send(bytes(data, 'utf8'))
    while True:
        data = s.recv(100)
        if not data:
            break
    s.close()


def energy_update():
    global ENERGY, PLAYER, PATH, HOST, PORT
    while True:
        time.sleep(0.3)
        try:
            send_hit(ENERGY, PLAYER, PATH, HOST, PORT)
        except Exception:
            pass

def main():
    global ENERGY, PLAYER, PATH, HOST, PORT
    while True:

        hit = False
        for npin in NPINS:
            pin = machine.Pin(npin, machine.Pin.IN)
            hit = not pin.value()
            if hit:
                break

        if hit:
            ENERGY -= 1
            print('HIT!')
            print(f"Energy: {ENERGY}")

        if ENERGY <= 0:
            print('Die!!!')
            sys.exit()
        time.sleep(0.1)


def display():
    pass

connect_wifi()
_thread.start_new_thread(main, [])
_thread.start_new_thread(energy_update, [])
_thread.start_new_thread(display, [])
