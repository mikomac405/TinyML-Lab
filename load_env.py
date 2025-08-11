Import("env")

from dotenv import load_dotenv
import os
load_dotenv()

# Create a .env file with these informations
# REMEMBER ABOUT STRINGIFICATION: https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_flags.html?utm_source=vscode&utm_medium=completion#stringification
env.Append(CPPDEFINES=[
  ("WIFI_SSID", os.getenv("WIFI_SSID")),
  ("WIFI_PASSWORD", os.getenv("WIFI_PASSWORD")),
  ("MQTT_HOST", os.getenv("MQTT_HOST")),
  ("MQTT_PORT", os.getenv("MQTT_PORT")),
])