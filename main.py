def do_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('Indaleccius_14C9_Exterior', 'AADJXFFTFRCO')
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())



import time
import machine
# do_connect()
pin = machine.Pin(34, machine.Pin.IN)
while True:
    print("Lectura del laser: ", pin.value())
    time.sleep(0.5)
