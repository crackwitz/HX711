#include <Arduino.h>
#include <HX711.h>

HX711::HX711(byte dout, byte pd_sck, byte gain) {
	PD_SCK 	= pd_sck;
	DOUT 	= dout;
	
	pinMode(PD_SCK, OUTPUT);
	pinMode(DOUT, INPUT);

	set_gain(gain);
}

HX711::~HX711() {

}

bool HX711::is_ready() {
	return digitalRead(DOUT) == LOW;
}

void HX711::set_gain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}

	digitalWrite(PD_SCK, LOW);
	read();
}

int32_t HX711::read() {
	// wait for the chip to become ready
	while (!is_ready());

	byte data[4] = {0,0,0,0};

	// pulse the clock pin 24 times to read the data
	for (byte j = 3; j--;) {
		for (char i = 8; i--;) {
			digitalWrite(PD_SCK, HIGH);
			bitWrite(data[j], i, digitalRead(DOUT));
			digitalWrite(PD_SCK, LOW);
		}
	}

	// set the channel and the gain factor for the next reading using the clock pin
	for (byte i = 0; i < GAIN; i++) {
		digitalWrite(PD_SCK, HIGH);
		digitalWrite(PD_SCK, LOW);
	}

	data[2] ^= 0x80;

	return *(int32_t*)data;
	//return *(int32_t*)data - 0x800000;
}

float HX711::read_average(byte times) {
	int32_t sum = 0;
	for (byte i = 0; i < times; i++) {
		sum += read();
	}
	return (float)sum / times;
}

float HX711::get_value(byte times) {
	return read_average(times) - OFFSET;
}

float HX711::get_units(byte times) {
	return get_value(times) * SCALE;
}

void HX711::tare(byte times) {
	float zero = read_average(times);
	set_offset(zero);
}

void HX711::tare_ref(float reference, byte times) {
	set_scale(reference / get_value(times));
}

void HX711::set_scale(float scale) {
	SCALE = scale;
}

void HX711::set_offset(float offset) {
	OFFSET = offset;
}

void HX711::power_down() {
	digitalWrite(PD_SCK, LOW);
	digitalWrite(PD_SCK, HIGH);	
}

void HX711::power_up() {
	digitalWrite(PD_SCK, LOW);	
}
