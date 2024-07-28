import tkinter as tk
from tkinter import ttk  # Import ttk for themed widgets
import requests
from datetime import datetime, timezone
from collections import defaultdict
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import re  # Import re for regular expressions

def login():
    username = username_entry.get()
    password = password_entry.get()
    
    correct_username = "ashish"
    correct_password = "ashish"
    
    if not username:
        error_label.config(text="Please enter your username.")
    elif not password:
        error_label.config(text="Please enter your password.")
    elif username != correct_username and password == correct_password:
        error_label.config(text="Enter correct username.")
    elif username == correct_username and password != correct_password:
        error_label.config(text="Enter correct password.")
    elif username == correct_username and password == correct_password:
        error_label.config(text="")
        login_frame.pack_forget()
        weather_frame.pack(fill=tk.BOTH, expand=1)
    else:
        error_label.config(text="Invalid credentials, please try again.")

def validate_city_name(city):
    # City name should only contain alphabetic characters and spaces
    if re.match(r"^[A-Za-z\s]+$", city):
        return True
    else:
        return False

def get_weather(city):
    api_key = '9b4885f831598c0e469a5ef0dc97b745'  # Your actual API key
    weather_url = f'http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=metric'
    
    try:
        weather_response = requests.get(weather_url)
        weather_response.raise_for_status()
        weather_data = weather_response.json()

        if weather_data['cod'] == 200:
            city_label.config(text=f"City: {weather_data['name']}")
            temp_label.config(text=f"Temperature: {weather_data['main']['temp']}°C")
            desc_label.config(text=f"Description: {weather_data['weather'][0]['description']}")
            humid = weather_data['main']['humidity']
            humidity_label.config(text=f"Humidity: {humid * 1}%")
            
            lat = weather_data['coord']['lat']
            lon = weather_data['coord']['lon']
            get_air_quality(lat, lon)
            get_5_day_forecast(city, lat, lon)
        else:
            display_error("City not found")
    except requests.exceptions.RequestException as e:
        display_error("Error retrieving data")
        print(e)

def get_air_quality(lat, lon):
    api_key = '9b4885f831598c0e469a5ef0dc97b745'  
    air_quality_url = f'http://api.openweathermap.org/data/2.5/air_pollution?lat={lat}&lon={lon}&appid={api_key}'
    
    try:
        air_quality_response = requests.get(air_quality_url)
        air_quality_response.raise_for_status()
        air_quality_data = air_quality_response.json()
        
        if 'list' in air_quality_data:
            aqi = air_quality_data['list'][0]['main']['aqi']
            Air = aqi
            if Air == 1:
               air_quality_label.config(text=f"Air Quality Index: A")
            elif Air == 2:
                air_quality_label.config(text=f"Air Quality Index: B")
            elif Air == 3:
                air_quality_label.config(text=f"Air Quality Index: C")
            elif Air == 4:
                air_quality_label.config(text=f"Air Quality Index: D")
            elif Air == 5:
                air_quality_label.config(text=f"Air Quality Index: E")
            elif Air == 6:
                air_quality_label.config(text=f"Air Quality Index: F")
            elif Air == 7:
                air_quality_label.config(text=f"Air Quality Index: G")
            elif Air == 8:
                air_quality_label.config(text=f"Air Quality Index: H")
            elif Air == 9:
                air_quality_label.config(text=f"Air Quality Index: J")
        else:
            air_quality_label.config(text="Air quality data not available")
    except requests.exceptions.RequestException as e:
        air_quality_label.config(text="Error retrieving air quality data")
        print(e)

def get_5_day_forecast(city, lat, lon):
    API_key = '18be179ae6d147727c673ff307ceec25'  
    forecast_url = f'http://api.openweathermap.org/data/2.5/forecast?lat={lat}&lon={lon}&appid={API_key}&units=metric'
    
    try:
        forecast_response = requests.get(forecast_url)
        forecast_response.raise_for_status()
        forecast_data = forecast_response.json()
        
        if 'list' in forecast_data:
            forecast_text.delete('1.0', tk.END)
            daily_forecasts = defaultdict(list)

            for forecast in forecast_data['list']:
                date = datetime.fromtimestamp(forecast['dt'], tz=timezone.utc).strftime('%Y-%m-%d')
                temp = forecast['main']['temp']
                description = forecast['weather'][0]['description']
                humidity = forecast['main']['humidity']
                daily_forecasts[date].append((temp, description, humidity))

            forecast_text.insert(tk.END, f"City: {city}\n\n")
            dates = []
            temps = []
            for date, values in daily_forecasts.items():
                avg_temp = sum(temp for temp, _, _ in values) / len(values)
                avg_humidity = sum(hum for _, _, hum in values) / len(values)
                descriptions = ', '.join(set(desc for _, desc, _ in values))
                forecast_text.insert(tk.END, f"Date: {date}\n")
                forecast_text.insert(tk.END, f"Average Temperature: {avg_temp:.2f}°C\n")
                forecast_text.insert(tk.END, f"Average Humidity: {avg_humidity * 2:.2f}%\n")
                forecast_text.insert(tk.END, f"Weather: {descriptions}\n\n")
                
                dates.append(date)
                temps.append(avg_temp)

            plot_forecast(dates, temps)
        else:
            forecast_text.insert(tk.END, "Error fetching forecast data\n")
    except requests.exceptions.RequestException as e:
        forecast_text.insert(tk.END, "Error retrieving forecast data\n")
        print(e)

def display_error(message):
    city_label.config(text=message)
    temp_label.config(text="")
    desc_label.config(text="")
    humidity_label.config(text="")
    air_quality_label.config(text="")

def on_submit():
    city = city_entry.get()
    display_error("")
    if validate_city_name(city):
        get_weather(city)
    else:
        display_error("Invalid city name. Please enter a valid city name.")

def plot_forecast(dates, temps):
    fig, ax = plt.subplots(figsize=(8, 4))
    ax.plot(dates, temps, marker='o', linestyle='-', color='b')
    ax.set_title('5-Day Weather Forecast')
    ax.set_xlabel('Date')
    ax.set_ylabel('Average Temperature (°C)')
    ax.grid(True)
    plt.xticks(rotation=45)
    canvas = FigureCanvasTkAgg(fig, master=scrollable_frame)
    canvas.draw()
    canvas.get_tk_widget().grid(row=9, column=0, columnspan=2, pady=10)

# GUI setup
root = tk.Tk()
root.title("Weather and Air Quality App")
root.geometry("800x800")
root.config(bg="#ADD8E6")

# Login Frame
login_frame = tk.Frame(root, bg="#ADD8E6")
login_frame.pack(pady=20)

tk.Label(login_frame, text="Username:", font=('Arial', 14, 'bold'), bg="#ADD8E6").grid(row=0, column=0, pady=5, sticky='e')
username_entry = tk.Entry(login_frame, font=('Arial', 14))
username_entry.grid(row=0, column=1, pady=5)

tk.Label(login_frame, text="Password:", font=('Arial', 14, 'bold'), bg="#ADD8E6").grid(row=1, column=0, pady=5, sticky='e')
password_entry = tk.Entry(login_frame, show='*', font=('Arial', 14))
password_entry.grid(row=1, column=1, pady=5)

login_button = tk.Button(login_frame, text="Login", command=login, font=('Arial', 14, 'bold'), bg="#0000FF", fg="#FFFFFF", padx=10, pady=5)
login_button.grid(row=2, column=0, columnspan=2, pady=10)

error_label = tk.Label(login_frame, text="", font=('Arial', 14, 'bold'), fg="red", bg="#ADD8E6")
error_label.grid(row=3, column=0, columnspan=2)

# Weather Frame with Scrollbar
weather_frame = tk.Frame(root, bg="#ADD8E6")

canvas = tk.Canvas(weather_frame, bg="#ADD8E6")
canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)

scrollbar = ttk.Scrollbar(weather_frame, orient=tk.VERTICAL, command=canvas.yview)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

canvas.configure(yscrollcommand=scrollbar.set)

scrollable_frame = ttk.Frame(canvas)
scrollable_frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")

# Adding content to the scrollable frame
l1 = tk.Label(scrollable_frame, text="WEATHER FORECAST", font=("Arial", 23, "bold"), bg="#ADD8E6")
l1.grid(row=0, columnspan=10, pady=10)

city_label = tk.Label(scrollable_frame, font=('Arial', 18, 'bold'), bg="#ADD8E6")
city_label.grid(row=1, column=0, columnspan=2)

temp_label = tk.Label(scrollable_frame, font=('Arial', 18, 'bold'), bg="#ADD8E6")
temp_label.grid(row=2, column=0, columnspan=2)

desc_label = tk.Label(scrollable_frame, font=('Arial', 18, 'bold'), bg="#ADD8E6")
desc_label.grid(row=3, column=0, columnspan=2)

humidity_label = tk.Label(scrollable_frame, font=('Arial', 18, 'bold'), bg="#ADD8E6")
humidity_label.grid(row=4, column=0, columnspan=2)

air_quality_label = tk.Label(scrollable_frame, font=('Arial', 18, 'bold'), bg="#ADD8E6")
air_quality_label.grid(row=5, column=0, columnspan=2)

tk.Label(scrollable_frame, text="Enter city name:", font=('Arial', 18, 'bold'), bg="#ADD8E6").grid(row=6, column=0, pady=10, sticky='e')
city_entry = tk.Entry(scrollable_frame, font=('Arial', 18))
city_entry.grid(row=6, column=1, pady=10)

submit_button = tk.Button(scrollable_frame, text="Submit", command=on_submit, font=('Arial', 18, 'bold'), bg="#0000FF", fg="#FFFFFF")
submit_button.grid(row=7, column=0, columnspan=2, pady=10)

forecast_text = tk.Text(scrollable_frame, height=10, width=80, font=('Arial', 14))
forecast_text.grid(row=8, column=0, columnspan=2, pady=10)

# Start the GUI main loop
root.mainloop()