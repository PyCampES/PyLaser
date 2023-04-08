import _thread
import network
import socket
import time
import sys
import machine
import ubinascii

# Modificar estas variables para cada jugador
PLAYER = 'nachillo'
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


def send_hit(energy):
    payload = f"energy,player={PLAYER} value={energy}"
    data = f'POST {PATH} HTTP/1.1\r\nHost: {HOST}:{PORT}\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: {len(payload)}\r\n\r\n{payload}'
    addr = socket.getaddrinfo(HOST, PORT)[0][-1]
    s = socket.socket()
    s.connect(addr)
    s.send(bytes(data, 'utf8'))
    while True:
        data = s.recv(100)
        if not data:
            break
    s.close()



def main():
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
            _thread.start_new_thread(send_hit, [ENERGY])

        if ENERGY <= 0:
            print('Die!!!')
            sys.exit()
        time.sleep(0.1)


def display():
    pass

connect_wifi()
_thread.start_new_thread(main, [])
_thread.start_new_thread(display, [])
