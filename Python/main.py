import tkinter as tk
import tkinter.font as tkFont
from tkinter import ttk
from firebase import firebase

firebaseBd = firebase.FirebaseApplication("https://esp8266-iot-947ea-default-rtdb.firebaseio.com/", None)

WINDOW_DIM = "450x400"
WINDOW_TITLE = "IOT-ESP8266 BY M.S v1.0"

def make_label(window, text, x, y, fontSize=10):
    fontStyle = tkFont.Font(size=fontSize)
    l = tk.Label(window, text=text, font=fontStyle)
    l.place(x=x, y=y)

def make_button(window, x, y, text, command):
    b = tk.Button(window, text=text, command=command)
    b.place(x=x, y=y)

def make_bar(window, x, y, from_, to, command, variable):
    slider = ttk.Scale(window, from_=from_, to=to, orient='horizontal', command=command, variable=variable)
    slider.place(x=x, y=y)
    return slider

def set_led_off():
    firebaseBd.put(url = "", name = 'led', data = 0)

def set_led_rain():
    firebaseBd.put(url = "", name = 'led', data = 1)

def set_led_red():
    firebaseBd.put(url = "", name = 'led', data = 2)

def set_led_green():
    firebaseBd.put(url = "", name = 'led', data = 3)

def set_led_blue():
    firebaseBd.put(url = "", name = 'led', data = 4)

def set_led_bar():
    firebaseBd.put(url = "", name = 'led', data = 9)

def set_rele_on():
    firebaseBd.put(url = "", name = 'led', data = 5)

def set_rele_off():
    firebaseBd.put(url = "", name = 'led', data = 6)


def make_window():
    window = tk.Tk()
    window.geometry(WINDOW_DIM)
    window.title(WINDOW_TITLE)
    temp = tk.StringVar()
    current_value = tk.IntVar()

    def get_temp():
        tempFb = str(firebaseBd.get("/Temperatura", "")) + " ºC"
        temp.set(tempFb)
        make_label(window, temp.get(), 150, 250, 20)

    def slider_changed(event):  
        firebaseBd.put(url = "", name = 'barra', data = int(slider.get()))

    # LED 
    make_label(window, "LED", 20, 20, 15)
    make_label(window, "Colours: ", 30, 50)
    make_button(window, 90, 50, "RED", set_led_red)
    make_button(window, 140, 50, "GREEN", set_led_green)
    make_button(window, 210, 50, "BLUE", set_led_blue)
    make_button(window, 270, 50, "OFF", set_led_off)
    make_label(window, "Patterns: ", 30, 90)
    make_button(window, 90, 90, "Rainbow", set_led_rain)
    make_label(window, "Bar: ", 30, 130)
    make_button(window, 70, 130, "Start", set_led_bar)
    # Bar
    slider = make_bar(window, 130, 130, 0, 1023, slider_changed, current_value)

    # RELE
    make_label(window, "RELE", 20, 170, 15)
    make_label(window, "Rele 1: ", 30, 200)
    make_button(window, 90, 200, "ON", set_rele_on)
    make_button(window, 130, 200, "OFF", set_rele_off)

    # TEMP
    make_label(window, "Temperature: ", 20, 250, 15)
    make_label(window, "Press -->", 150, 250, 12)
    make_button(window, 250, 250, "Update", get_temp)

    window.mainloop()


make_window()