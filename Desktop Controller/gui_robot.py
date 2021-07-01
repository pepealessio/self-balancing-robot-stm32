"""
    GUI for the control of the Self Balancing Robot
"""

from tkinter import *
from tkinter import messagebox, font

import bluetooth

control_byte = 0x00
bool_up = True
bool_down = True
bool_left = True
bool_right = True
# bool_up_right = True
# bool_down_right = True
# bool_up_left = True
# bool_down_left = True
bool_buz = True


def up_pressed():
    """ Sends control byte with the up Bit set through the Bluetooth Socket  """
    upBtn['background'] = 'LightGoldenrod3'
    global bool_up
    if bool_up:
        try:
            global control_byte
            control_byte |= 0x40  # 0100 0000
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_up = False
        except (OSError, AttributeError):
            disconnect_handle()


def up_released():
    """ Sends control byte with the up Bit unset through the Bluetooth Socket  """
    upBtn['background'] = 'white'
    global bool_up
    if not bool_up:
        try:
            global control_byte
            control_byte &= 0xBF    # 1011 1111
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_up = True
        except (OSError, AttributeError):
            disconnect_handle()


def down_pressed():
    """ Sends control byte with the down Bit set through the Bluetooth Socket  """
    downBtn['background'] = 'LightGoldenrod3'
    global bool_down
    if bool_down:
        try:
            global control_byte
            control_byte |= 0x20    # 0010 0000
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_down = False
        except (OSError, AttributeError):
            disconnect_handle()


def down_released():
    """ Sends control byte with the down Bit unset through the Bluetooth Socket  """
    downBtn['background'] = 'white'
    global bool_down
    if not bool_down:
        try:
            global control_byte
            control_byte &= 0xDF    # 1101 1111
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_down = True
        except (OSError, AttributeError):
            disconnect_handle()


def left_pressed():
    """ Sends control byte with the left bit set through the Bluetooth Socket  """
    leftBtn['background'] = 'LightGoldenrod3'
    global bool_left
    if bool_left:
        try:
            global control_byte
            control_byte |= 0x10    # 0001 0000
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_left = False
        except (OSError, AttributeError):
            disconnect_handle()


def left_released():
    """ Sends control byte with the left Bit unset through the Bluetooth Socket  """
    leftBtn['background'] = 'white'
    global bool_left
    if not bool_left:
        try:
            global control_byte
            control_byte &= 0xEF    # 1110 1111
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_left = True
        except (OSError, AttributeError):
            disconnect_handle()


def right_pressed():
    """ Sends control byte with the right Bit set through the Bluetooth Socket  """
    rightBtn['background'] = 'LightGoldenrod3'
    global bool_right
    if bool_right:
        try:
            global control_byte
            control_byte |= 0x08    # 0000 1000
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_right = False
        except (OSError, AttributeError):
            disconnect_handle()


def right_released():
    """ Sends control byte with the right Bit unset through the Bluetooth Socket  """
    rightBtn['background'] = 'white'
    global bool_right
    if not bool_right:
        try:
            global control_byte
            control_byte &= 0xF7    # 1111 0111
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_right = True
        except (OSError, AttributeError):
            disconnect_handle()

#
# def up_left_pressed():
#     """ Sends control byte with the up and left bits set through the Bluetooth Socket  """
#     upLeftBtn['background'] = 'LightGoldenrod3'
#     global bool_up_left
#     if bool_up_left:
#         try:
#             global control_byte
#             control_byte |= 0x50    # 0101 0000
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_up_left = False
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def up_left_released():
#     """ Sends control byte with the up and left Bits unset through the Bluetooth Socket  """
#     upLeftBtn['background'] = 'white'
#     global bool_up_left
#     if not bool_up_left:
#         try:
#             global control_byte
#             control_byte &= 0xAF    # 1010 1111
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_up_left = True
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def up_right_pressed():
#     """ Sends control byte with the up and right bits set through the Bluetooth Socket  """
#     upRightBtn['background'] = 'LightGoldenrod3'
#     global bool_up_right
#     if bool_up_right:
#         try:
#             global control_byte
#             control_byte |= 0x48    # 0100 1000
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_up_right = False
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def up_right_released():
#     """ Sends control byte with the up and right Bits unset through the Bluetooth Socket  """
#     upRightBtn['background'] = 'white'
#     global bool_up_right
#     if not bool_up_right:
#         try:
#             global control_byte
#             control_byte &= 0xB7    # 1011 0111
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_up_right = True
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def down_left_pressed():
#     """ Sends control byte with the down and left bits set through the Bluetooth Socket  """
#     downLeftBtn['background'] = 'LightGoldenrod3'
#     global bool_down_left
#     if bool_down_left:
#         try:
#             global control_byte
#             control_byte |= 0x30    # 0011 0000
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_down_left = False
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def down_left_released():
#     """ Sends control byte with the down and left Bits unset through the Bluetooth Socket  """
#     downLeftBtn['background'] = 'white'
#     global bool_down_left
#     if not bool_down_left:
#         try:
#             global control_byte
#             control_byte &= 0xCF    # 1100 1111
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_down_left = True
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def down_right_pressed():
#     """ Sends control byte with the down and right bits set through the Bluetooth Socket  """
#     downRightBtn['background'] = 'LightGoldenrod3'
#     global bool_down_right
#     if bool_down_right:
#         try:
#             global control_byte
#             control_byte |= 0x28    # 0010 1000
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_down_right = False
#         except (OSError, AttributeError):
#             disconnect_handle()
#
#
# def down_right_released():
#     """ Sends control byte with the down and right Bits unset through the Bluetooth Socket  """
#     downRightBtn['background'] = 'white'
#     global bool_down_right
#     if not bool_down_right:
#         try:
#             global control_byte
#             control_byte &= 0xD7    # 1101 0111
#             sock.send(control_byte.to_bytes(1, 'big'))
#             bool_down_right = True
#         except (OSError, AttributeError):
#             disconnect_handle()
#

def buz_pressed():
    """ Sends control byte with the buz bit set through the Bluetooth Socket  """
    buzBtn['background'] = 'LightGoldenrod3'
    global bool_buz
    if bool_buz:
        try:
            global control_byte
            control_byte |= 0x80    # 1000 0000
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_buz = False
        except (OSError, AttributeError):
            disconnect_handle()


def buz_released():
    """ Sends control byte with the buzzer Bit unset through the Bluetooth Socket  """
    buzBtn['background'] = 'white'
    global bool_buz
    if not bool_buz:
        try:
            global control_byte
            control_byte &= 0x7F    # 0111 1111
            sock.send(control_byte.to_bytes(1, 'big'))
            bool_buz = True
        except (OSError, AttributeError):
            disconnect_handle()


def connect():
    """ Starts a BluetoothSocket with the ESP32"""
    connectBtn["state"] = "disabled"
    try:
        global sock
        if sock is not None:
            sock.close()
        port = 1
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((targetBluetoothMacAddress, port))
        global control_byte
        control_byte = 0x00
        connectedLbl.config(text="Connected", bg='green')
        active_all()
    except OSError as e:
        messagebox.showwarning(title=OSError, message=str(e))
        disable_all()
        connectedLbl.config(text="NOT connected!", bg='red')
    finally:
        connectBtn["state"] = "active"


def disable_all():
    """ Disables all the buttons of the GUI (except the connection one)"""
    key_unbind()
    upBtn["state"] = "disabled"
    downBtn["state"] = "disabled"
    leftBtn["state"] = "disabled"
    rightBtn["state"] = "disabled"
    # upLeftBtn["state"] = "disabled"
    # downLeftBtn["state"] = "disabled"
    # upRightBtn["state"] = "disabled"
    # downRightBtn["state"] = "disabled"
    buzBtn["state"] = "disabled"


def active_all():
    """ Enables all the buttons of the GUI"""
    key_bind()
    upBtn["state"] = "normal"
    downBtn["state"] = "normal"
    leftBtn["state"] = "normal"
    rightBtn["state"] = "normal"
    # upLeftBtn["state"] = "normal"
    # downLeftBtn["state"] = "normal"
    # upRightBtn["state"] = "normal"
    # downRightBtn["state"] = "normal"
    buzBtn["state"] = "normal"


def disconnect_handle():
    """ Shows a warning dialog in case of error"""
    messagebox.showwarning(title='You are disconnected', message='Please connect to the esp32')
    disable_all()
    connectedLbl.config(text="NOT connected!", bg='red')


# Keyboard binding
def key_bind():
    """ Binds each action to a key of the keyboard"""
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


def key_unbind():
    """ Unbinds the actions from the keys of the keyboard"""
    ws.unbind('<KeyPress-w>')
    ws.unbind('<KeyRelease-s>')
    ws.unbind('<KeyPress-s>')
    ws.unbind('<KeyRelease-a>')
    ws.unbind('<KeyPress-a>')
    ws.unbind('<KeyRelease-d>')
    ws.unbind('<KeyPress-d>')
    ws.unbind('<KeyRelease-space>')
    ws.unbind('<KeyPress-space>')


def on_enter(btn):
    btn['background'] = 'LightGoldenrod1'


def on_leave(btn):
    btn['background'] = 'white'


# configure workspace
ws = Tk()
ws.title("SBR controller")
ws.geometry('470x300')
ws.configure(bg="lightgray")

# button definition
buttonFont = font.Font(family='Tahoma', size=16)
upBtn = Button(ws, text="🡡 (W)", bg='white', font=buttonFont)
downBtn = Button(ws, text="🡣 (S)", bg='white', font=buttonFont)
leftBtn = Button(ws, text="🡠 (A)", bg='white', font=buttonFont)
rightBtn = Button(ws, text="🡢 (D)", bg='white', font=buttonFont)
# upLeftBtn = Button(ws, text="🡤 (WA)", bg='white', font=buttonFont)
# downLeftBtn = Button(ws, text="🡧 (SA)", bg='white', font=buttonFont)
# upRightBtn = Button(ws, text="🡥 (WD)", bg='white', font=buttonFont)
# downRightBtn = Button(ws, text="🡦 (SD)", bg='white', font=buttonFont)
buzBtn = Button(ws, text="Horn (ws)", bg='white', font=buttonFont)
connectBtn = Button(ws, text="Connect", bg='gray', fg="white", command=connect, width=10, font=buttonFont)
connectedLbl = Label(ws, text="NOT connected!", bg='red', fg='white', font=buttonFont)

# Buttons binding
upBtn.bind('<ButtonRelease>', lambda event: up_released())
upBtn.bind('<ButtonPress>', lambda event: up_pressed())
upBtn.bind('<Enter>', lambda event: on_enter(upBtn))
upBtn.bind('<Leave>', lambda event: on_leave(upBtn))

downBtn.bind('<ButtonRelease>', lambda event: down_released())
downBtn.bind('<ButtonPress>', lambda event: down_pressed())
downBtn.bind('<Enter>', lambda event: on_enter(downBtn))
downBtn.bind('<Leave>', lambda event: on_leave(downBtn))

leftBtn.bind('<ButtonRelease>', lambda event: left_released())
leftBtn.bind('<ButtonPress>', lambda event: left_pressed())
leftBtn.bind('<Enter>', lambda event: on_enter(leftBtn))
leftBtn.bind('<Leave>', lambda event: on_leave(leftBtn))

rightBtn.bind('<ButtonRelease>', lambda event: right_released())
rightBtn.bind('<ButtonPress>', lambda event: right_pressed())
rightBtn.bind('<Enter>', lambda event: on_enter(rightBtn))
rightBtn.bind('<Leave>', lambda event: on_leave(rightBtn))
#
# upLeftBtn.bind('<ButtonRelease>', lambda event: up_left_released())
# upLeftBtn.bind('<ButtonPress>', lambda event: up_left_pressed())
# upLeftBtn.bind('<Enter>', lambda event: on_enter(upLeftBtn))
# upLeftBtn.bind('<Leave>', lambda event: on_leave(upLeftBtn))
#
# upRightBtn.bind('<ButtonRelease>', lambda event: up_right_released())
# upRightBtn.bind('<ButtonPress>', lambda event: up_right_pressed())
# upRightBtn.bind('<Enter>', lambda event: on_enter(upRightBtn))
# upRightBtn.bind('<Leave>', lambda event: on_leave(upRightBtn))
#
# downLeftBtn.bind('<ButtonRelease>', lambda event: down_left_released())
# downLeftBtn.bind('<ButtonPress>', lambda event: down_left_pressed())
# downLeftBtn.bind('<Enter>', lambda event: on_enter(downLeftBtn))
# downLeftBtn.bind('<Leave>', lambda event: on_leave(downLeftBtn))
#
# downRightBtn.bind('<ButtonRelease>', lambda event: down_right_released())
# downRightBtn.bind('<ButtonPress>', lambda event: down_right_pressed())
# downRightBtn.bind('<Enter>', lambda event: on_enter(downRightBtn))
# downRightBtn.bind('<Leave>', lambda event: on_leave(downRightBtn))

buzBtn.bind('<ButtonRelease>', lambda event: buz_released())
buzBtn.bind('<ButtonPress>', lambda event: buz_pressed())
buzBtn.bind('<Enter>', lambda event: on_enter(buzBtn))
buzBtn.bind('<Leave>', lambda event: on_leave(buzBtn))


# Buttons show
upBtn.grid(row=0, column=1, sticky=NSEW, padx=1, pady=1)
downBtn.grid(row=2, column=1, sticky=NSEW, padx=1, pady=1)
leftBtn.grid(row=1, column=0, sticky=NSEW, padx=1, pady=1)
rightBtn.grid(row=1, column=2, sticky=NSEW, padx=1, pady=1)
# upLeftBtn.grid(row=0, column=0, sticky=NSEW, padx=1, pady=1)
# upRightBtn.grid(row=0, column=2, sticky=NSEW, padx=1, pady=1)
# downLeftBtn.grid(row=2, column=0, sticky=NSEW, padx=1, pady=1)
# downRightBtn.grid(row=2, column=2, sticky=NSEW, padx=1, pady=1)
buzBtn.grid(row=1, column=1, sticky=NSEW, padx=1, pady=1)
connectBtn.grid(row=0, column=3, sticky=NSEW, padx=1, pady=1)
connectedLbl.grid(row=1, column=3, rowspan=2, sticky=NSEW, padx=1, pady=1)

for x in range(3):
    ws.columnconfigure(x, weight=1)
    ws.rowconfigure(x, weight=1)

# Socket connection
target_name = "SBLGruppo2"
targetBluetoothMacAddress = None
devices = bluetooth.discover_devices()
for bdaddr in devices:
    if target_name == bluetooth.lookup_name(bdaddr):
        targetBluetoothMacAddress = bdaddr
        # print(targetBluetoothMacAddress)
        break

targetBluetoothMacAddress = '24:6F:28:B4:F0:6E'
if targetBluetoothMacAddress is None:
    messagebox.showerror(title='SBR not found', message='Turn on the SBR and restart the application')
    exit(2)

disable_all()
sock = None

# infinite loop
ws.mainloop()
