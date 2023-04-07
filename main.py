import time
import sys
import machine

ENERGY = 100
PLAYER = 'nachillo'
PATH = '/hit/'
PORT = 8000
HOST = 'http://192.68.5.182:{PORT}{PATH}'
MAC = 'a1b2c3'

def connect_wifi():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('Indaleccius_14C9_Exterior', 'AADJXFFTFRCO')
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())


def send_hit(energy):
    import socket
    _, _, host, path = url.split('/', 3)
    addr = socket.getaddrinfo(host, port)[0][-1]
    s = socket.socket()
    s.connect(addr)
    s.send(bytes(f'GET {PATH}?player={PLAYER}&mac={MAC}&energy={ENERGY} HTTP/1.0\r\nHost: {HOST}\r\n\r\n', 'utf8'))
    while True:
        data = s.recv(100)
        if data:
            print(str(data, 'utf8'), end='')
        else:
            break
    s.close()


connect_wifi()
pin = machine.Pin(34, machine.Pin.IN)
print(f"Energy: {ENERGY}")
while True:
    hit = not pin.value()
    if hit:
        ENERGY -= 1
        print('HIT!')
        print(f"Energy: {ENERGY}")
        send_hit(ENERGY)

    if ENERGY <= 0:
        print('Die!!!')
        sys.exit()
    time.sleep(0.1)
