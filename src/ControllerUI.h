#ifndef HUMIDISTAT_CONTROLLERUI_H
#define HUMIDISTAT_CONTROLLERUI_H

#include "LiquidCrystal.h"
#include "ButtonReader.h"
#include "Humidistat.h"

/// User interface (display and input) for humidistat for Ks0256 LCD keypad shield.
/// Holds references to a LiquidCrystal instance for writing to display, a ButtonReader for keypad input, and Humidistat
/// for updating the humidity setpoint.
/// Displays current humidity, setpoint and temperature on the first line of the display. Use keypad to adjust setpoint
/// (UP/DOWN for fine, LEFT/RIGHT for coarse).
class ControllerUI {
private:
	LiquidCrystal &liquidCrystal;
	const ButtonReader &buttonReader;
	Humidistat &humidistat;
	unsigned long lastTime = 0;              //!< Last time buttons were polled (in millis)
	const unsigned long inputInterval = 200; //!< Polling interval for reading buttons (in millis)
	const unsigned long blinkInterval = 500; //!< Interval for blinking displays (in millis)
	uint8_t tolerance = 1;                   //!< Tolerance in difference between process variable and setpoint outside
	                                         //!< which the setpoint blinks (in percentage points)

	/// Update the values displayed on the LCD.
	void updateDisplay();

	/// Handle input.
	void input();

	/// Read buttons and adjust a variable.
	void adjustValue(uint8_t &value, uint8_t min, uint8_t max);

	/// Print blinking text.
	void blink(uint8_t col, uint8_t row, char* buf);

public:
	/// Constructor.
	/// \param liquidCrystal Pointer to a LiquidCrystal instance
	/// \param buttonReader Pointer to a ButtonReader instance
	/// \param humidistat Pointer to a Humidistat instance
	explicit ControllerUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, Humidistat *humidistat);

	/// Update the display and handle input: set Humidistat's setpoint
	void update();
};

#endif //HUMIDISTAT_CONTROLLERUI_H
