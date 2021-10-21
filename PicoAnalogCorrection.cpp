/*
	Arduino Pico Analog Correction
	https://github.com/Phoenix1747/Arduino-Pico-Analog-Correction/
*/

#include "PicoAnalogCorrection.h"


PicoAnalogCorrection::PicoAnalogCorrection(int gnd_val, int vcc_val) {
	_gnd_offset = gnd_val;
	_vcc_offset = vcc_val;
}


void PicoAnalogCorrection::calibrateAdc(uint8_t gnd_pin, uint8_t vcc_pin, size_t avg_size) {
	float gnd_value = .0;
	
	for(size_t i = 0; i < avg_size; i++) {
		gnd_value += float(analogRead(gnd_pin));
	}
	_gnd_offset = gnd_value/avg_size;
	
	float vcc_value = .0;
	
	for(size_t i = 0; i < avg_size; i++) {
		vcc_value += float(analogRead(vcc_pin));
	}
	_vcc_offset = vcc_value/avg_size;
	
	return;
}


void PicoAnalogCorrection::returnCalibrationValues() {
	Serial.println("(" + String(_gnd_offset) + ", " + String(_vcc_offset) + ")");
	return;
}


int PicoAnalogCorrection::analogRead(uint8_t pin) {
	digitalWrite(PS_PIN, HIGH); // Disable power-saving
	delayMicroseconds(2); // Cooldown, maybe useless?
	
	int value = ::analogRead(pin); // Use normal Arduino analogRead func
	
	digitalWrite(PS_PIN, LOW); // Re-enable power-saving
	
	return value;
}


int PicoAnalogCorrection::analogCRead(uint8_t pin, size_t avg_size) {
	float value = .0;
	
	for(size_t i = 0; i < avg_size; i++) {
		value += float( map(analogRead(pin), _gnd_offset, _vcc_offset, 0, 4095) );
	}
	
	return round(value/avg_size);
}

