## Future Improvements

- **More intuitive function and file names:**
	- Improve naming throughout the codebase to make it even easier to understand and maintain.

- **COAP.json key management:**
	- Some sensitive keys may be hidden or managed within the SDK or hardware in the future, as expected by platform developers. This approach is still need to be discussed.

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

