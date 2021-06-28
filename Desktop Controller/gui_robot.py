
from tkinter import *
import bluetooth

control_byte = 0x00
bool_up = True
bool_down = True
bool_left = True
bool_right = True
bool_buz = True


def up_pressed():
    global bool_up
    if bool_up:
        global control_byte
        control_byte |= 0x40
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_up = False
        print("{0:b}".format(control_byte))


def up_released():
    global bool_up
    if not bool_up:
        global control_byte
        control_byte &= 0xBF
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_up = True
        print("{0:b}".format(control_byte))


def down_pressed():
    global bool_down
    if bool_down:
        global control_byte
        control_byte |= 0x20
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_down = False
        print("{0:b}".format(control_byte))


def down_released():
    global bool_down
    if not bool_down:
        global control_byte
        control_byte &= 0xDF
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_down = True
        print("{0:b}".format(control_byte))


def left_pressed():
    global bool_left
    if bool_left:
        global control_byte
        control_byte |= 0x10
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_left = False
        print("{0:b}".format(control_byte))


def left_released():
    global bool_left
    if not bool_left:
        global control_byte
        control_byte &= 0xEF
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_left = True
        print("{0:b}".format(control_byte))


def right_pressed():
    global bool_right
    if bool_right:
        global control_byte
        control_byte |= 0x08
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_right = False
        print("{0:b}".format(control_byte))


def right_released():
    global bool_right
    if not bool_right:
        global control_byte
        control_byte &= 0xF7
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_right = True
        print("{0:b}".format(control_byte))


def buz_pressed():
    global bool_buz
    if bool_buz:
        global control_byte
        control_byte |= 0x80
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_buz = False
        print("{0:b}".format(control_byte))


def buz_released():
    global bool_buz
    if not bool_buz:
        global control_byte
        control_byte &= 0x7F
        sock.send(control_byte.to_bytes(1, 'big'))
        bool_buz = True
        print("{0:b}".format(control_byte))


# configure workspace
ws = Tk()
ws.title("SBL controller")
ws.geometry('300x300')
ws.configure(bg="gray")

# button definition
upBtn = Button(ws, text="W", bg='white')
downBtn = Button(ws, text="S", bg='white')
leftBtn = Button(ws, text="A", bg='white')
rightBtn = Button(ws, text="D", bg='white')
buzBtn = Button(ws, text="Horn", bg='white')

# Buttons binding
upBtn.bind('<ButtonRelease>', lambda event: up_released())
upBtn.bind('<ButtonPress>', lambda event: up_pressed())
downBtn.bind('<ButtonRelease>', lambda event: down_released())
downBtn.bind('<ButtonPress>', lambda event: down_pressed())
leftBtn.bind('<ButtonRelease>', lambda event: left_released())
leftBtn.bind('<ButtonPress>', lambda event: left_pressed())
rightBtn.bind('<ButtonRelease>', lambda event: right_released())
rightBtn.bind('<ButtonPress>', lambda event: right_pressed())
buzBtn.bind('<ButtonRelease>', lambda event: buz_released())
buzBtn.bind('<ButtonPress>', lambda event: buz_pressed())

# Keyboard binding
ws.bind('<KeyRelease-w>', lambda event: up_released())
ws.bind('<KeyPress-w>', lambda event: up_pressed())
ws.bind('<KeyRelease-s>', lambda event: down_released())
ws.bind('<KeyPress-s>', lambda event: down_pressed())
ws.bind('<KeyRelease-a>', lambda event: left_released())
ws.bind('<KeyPress-a>', lambda event: left_pressed())
ws.bind('<KeyRelease-d>', lambda event: right_released())
ws.bind('<KeyPress-d>', lambda event: right_pressed())
ws.bind('<KeyRelease-space>', lambda event: buz_released())
ws.bind('<KeyPress-space>', lambda event: buz_pressed())

upBtn.grid(row=0, column=1, sticky=NSEW, padx=1, pady=1)
downBtn.grid(row=2, column=1, sticky=NSEW, padx=1, pady=1)
leftBtn.grid(row=1, column=0, sticky=NSEW, padx=1, pady=1)
rightBtn.grid(row=1, column=2, sticky=NSEW, padx=1, pady=1)
buzBtn.grid(row=1, column=1, sticky=NSEW, padx=1, pady=1)

for x in range(3):
  ws.columnconfigure(x, weight=1)
  ws.rowconfigure(x, weight=1)

# Socket connection
# target_name = "SBLGruppo2"
# targetBluetoothMacAddress = None
# devices = bluetooth.discover_devices()
# print(devices)
# for bdaddr in devices:
#     print(bluetooth.lookup_name(bdaddr))
#     if target_name == bluetooth.lookup_name(bdaddr):
#         targetBluetoothMacAddress = bdaddr
#         print(targetBluetoothMacAddress)
#         break

targetBluetoothMacAddress = '24:6F:28:B4:F0:6E'
if targetBluetoothMacAddress is not None:
    port = 1
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    sock.connect((targetBluetoothMacAddress, port))
else:
    exit(2)

# infinite loop
ws.mainloop()
