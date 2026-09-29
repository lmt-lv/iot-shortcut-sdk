## 2.4.1 (2026--09-29)
### Bug Fixes
* GNSS work queue allocated in application build
* Allow set LTE preference in LTE-M and NB-IoT only modes
* Read device serial number and allow sending custom modem commands before modem initialization

## 2.4.0 (2026--09-22)
### Feature
* NTN (satellite NB-IoT) support, with modem hardware and firmware capability checks
* GNSS and terrestrial network services can be enabled or disabled independently
* Mailer start can be suspended and resumed; next mailer loop can be run without packing
* Modem can be kept powered on across a disconnect (keep_modem_on)
* PSM: separate requested and network-assigned TAU and active time, full encoding range
* Thread and work queue objects are allocated by the application build, so their Zephyr structure sizes always match the application's configuration rather than the library's
* Library and application are checked for structure size compatibility at startup, with a warning is logged before the threads start

### Bug Fixes
* Modem is powered off with CFUN=0 before the library shuts down, so resets count as graceful
* No needless NVM writes: the functional mode is read before a power-off is issued
* Reconnect no longer re-initialises a modem that is still registered
* Socket descriptor invalidated on close and on failed create; descriptor 0 no longer read as "none"
* EVENT_NETWORK_DOWN emitted on every registration loss
* Cell ID validity compared against LTE_LC_CELL_EUTRAN_ID_INVALID
* Signal quality is not queried while the modem is disconnected
* PSM RAT unit table corrected; unassigned PSM values reported as negative
* System mode command sent from the assembled buffer; askModem argument order corrected
* LTE event handler updated for NCS 3.2.4, with the modem domain events reported
* setNtnFlag stores a normalised flag instead of the raw argument
* Event handlers take const void *p_data

## 2.3.0 (2026-09-08)
### Feature

* User can creat custom event
* Custom event usage sample

## 2.2.0 (2026-08-07)
### Feature

* Raw data mode sample project

### Bug Fixes

* PSM current decreased - external flash in DPD when idle. Serial log output severity changable
* Network connection timeout settable. Fixed re-attach timeout logic
* Weak coap_queue and coap_queue_size definition in library

## 2.1.0 (2026-07-16)
### Feature
*  Sending of GPS data to cloud

### Bug Fixes

* GNSS sample additions. Cell ID sending to cloud and GNSS setting edits.
* Improved net_active_time use
* FOTA fail detect

## 2.0.6 (2026-05-28)


### Bug Fixes

* SIM disable detect