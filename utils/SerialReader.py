import serial
from io import StringIO
from time import sleep

import numpy as np
import pandas as pd


class SerialReader:
	"""
	Connect to the Arduino and read CRLF-terminated, fixed-width formatted data.
	Is a context manager for the connection.
	"""
	def __init__(self, port: str, baud_rate: int = 9600):
		"""
		Connect to the Arduino.
		:param port: Serial port device
		:param baud_rate: Baud rate
		"""
		self.serial = serial.Serial(port, baud_rate)
		self.serial.setDTR(False)
		sleep(1)
		self.serial.reset_input_buffer()
		self.serial.setDTR(True)

		# The Arduino will reset if we open the serial port, so we wait for it to boot and signal to be ready
		rec = self.serial.readline()
		print(rec)

		# Indicate that we're ready
		self.serial.write(b'RDY\r\n')
		print('Connected.')

		# Receive header
		self.header = self.serial.readline().decode().split()
		print(self.header)

	def __enter__(self):
		return self

	def __exit__(self, exc_type, exc_val, exc_tb):
		self.serial.close()

	def readline(self) -> np.ndarray:
		"""
		Read a line
		:return: A 1D ndarray
		"""
		line = self.serial.readline()
		print(line)

		return np.array(line.decode().split(), dtype=float)

	def available(self) -> bool:
		"""
		:return: Return true if buffer is not empty.
		"""
		return self.serial.in_waiting > 0