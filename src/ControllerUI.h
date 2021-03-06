#ifndef HUMIDISTAT_CONTROLLERUI_H
#define HUMIDISTAT_CONTROLLERUI_H

#include <stdint.h>
#include <Array.h>

#include "LiquidCrystal.h"
#include "ButtonReader.h"
#include "Humidistat.h"
#include "ThermistorReader.h"

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
	Array<ThermistorReader*, 4> trs;
	unsigned long lastRefreshed = 0;           //!< Last time display was updated (in millis)
	unsigned long lastPressed = 0;             //!< Last time a button was pressed (in millis)
	const uint16_t RefreshInterval = 100;      //!< Interval for updating the display (in millis)
	const uint16_t inputInterval = 200;        //!< Polling interval for reading buttons (in millis)
	const uint16_t blinkInterval = 500;        //!< Interval for blinking displays (in millis)
	const uint16_t splashDuration = 1000;      //!< Duration for which to show the splash screen (in millis)
	const uint16_t infoDuration = 3000;        //!< Duration for which to show the info screen (in millis)
	const uint8_t tolerance = 1;               //!< Tolerance in difference between process variable and setpoint
	                                           //!< outside which the setpoint blinks (in percentage points)
	const uint8_t adjustStep = 5;              //!< Step size by which to in-/de-crement for coarse adjustment

	bool splashDrawn = false;
	bool infoDrawn = false;
	bool screenCleared = false;

	/// Update the values displayed on the LCD.
	void updateDisplay();

	/// Handle input.
	void input();

	/// Read buttons and adjust a variable.
	/// \param value Value to adjust
	/// \param min Lower limit
	/// \param max Upper limit
	/// \return 1 if button was pressed, 0 if not
	bool adjustValue(uint8_t &value, uint8_t min, uint8_t max);

	/// Print blinking text.
	/// \param col LCD column
	/// \param row LCD row
	/// \param buf Buffer of text to blink
	void blink(uint8_t col, uint8_t row, char *buf);

	/// Print temperature read from thermistors. Handles NaN values as 0
	/// \param col LCD column
	/// \param row LCD row
	/// \param i ThermistorReader index
	void printNTC(uint8_t col, uint8_t row, uint8_t i);

public:
	/// Constructor.
	/// \param liquidCrystal Pointer to a LiquidCrystal instance
	/// \param buttonReader Pointer to a ButtonReader instance
	/// \param humidistat Pointer to a Humidistat instance
	/// \param trs Pointer to array of 4 ThermistorReader instances
	explicit ControllerUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, Humidistat *humidistat,
	                      Array<ThermistorReader*, 4> trs);

	/// Update the display and handle input: set Humidistat's setpoint
	void update();

	/// Show the splash screen.
	void splash();

	/// Show the info screen.
	void info();
};

#endif //HUMIDISTAT_CONTROLLERUI_H
