## System Logic

The application performs the following tasks:

- **Read potentiometer position using ADC:**
	- Measures the position of a knob (potentiometer) as a percentage (0% to 100%).
	- Sets the PWM (Pulse Width Modulation) output based on the potentiometer position.
	- Sends the potentiometer position (in %) to a remote server.

- **Read LIS3DH accelerometer:**
	- Continuously reads acceleration data.
	- Tracks and saves the maximum acceleration measured during the last period.

- **Read BMP390 sensor:**
	- Reads temperature (in °C) and atmospheric pressure (in Pascals).

- **Data packaging and transmission:**
	- Packs all sensor data (potentiometer %, max acceleration, temperature, pressure) into a message.
	- Sends the message to a remote server for monitoring and logging.

