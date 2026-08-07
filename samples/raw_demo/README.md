## System Logic

The application performs the following tasks:

- **Sends raw (unformatted) uplink data:**
	- Switches the uplink to raw mode with `setUlDataMode(UL_MODE_RAW)`.
	- Cycles through the payloads in `raw_payloads[]`, sending one every `MAILER_LOOP_TIME`.
	- Each payload is passed to `setRawData()` with the upload flag set, so it goes out immediately.

- **Enabled GNSS, obtains GNSS Fix, calls callback function**
