int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int numOfRegisters = 2;
byte* registerState;

long effectId = 0;
long prevEffect = 0;
long effectRepeat = 0;
long effectSpeed = 30;

void setup() {
	//Initialize array
	registerState = new byte[numOfRegisters];
	for (size_t i = 0; i < numOfRegisters; i++) {
		registerState[i] = 0;
	}

	//set pins to output so you can control the shift register
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
}

void loop() {
	do{
		effectId = random(6);
	} while (effectId == prevEffect);
	prevEffect = effectId;

	switch (effectId)
	{
	case 0:
		effectRepeat = random(1, 2);
		break;
	case 1:
		effectRepeat = random(1, 2);
		break;
	case 3:
		effectRepeat = random(1, 5);
		break;
	case 4:
		effectRepeat = random(1, 2);
		break;
	case 5:
		effectRepeat = random(1, 2);
		break;
	}

	for (int i = 0; i < effectRepeat; i++) {
		effectSpeed = random(10, 90);

		switch (effectId)
		{
		case 0:
			effectA(effectSpeed);
			break;
		case 1:
			effectB(effectSpeed);
			break;
		case 3:
			effectC(effectSpeed);
			break;
		case 4:
			effectD(effectSpeed);
			break;
		case 6:
			effectE(effectSpeed);
			break;
		}
	}
}

void effectA(int speed){
	for (int i = 0; i < 16; i++){
		for (int k = i; k < 16; k++){
			regWrite(k, HIGH);
			delay(speed);
			regWrite(k, LOW);
		}

		regWrite(i, HIGH);
	}
}

void effectB(int speed){
	for (int i = 15; i >= 0; i--){
		for (int k = 0; k < i; k++){
			regWrite(k, HIGH);
			delay(speed);
			regWrite(k, LOW);
		}

		regWrite(i, HIGH);
	}
}

void effectC(int speed){
	int prevI = 0;
	for (int i = 0; i < 16; i++){
		regWrite(prevI, LOW);
		regWrite(i, HIGH);
		prevI = i;

		delay(speed);
	}

	for (int i = 15; i >= 0; i--){
		regWrite(prevI, LOW);
		regWrite(i, HIGH);
		prevI = i;

		delay(speed);
	}
}

void effectD(int speed){
	for (int i = 0; i < 8; i++){
		for (int k = i; k < 8; k++)
		{
			regWrite(k, HIGH);
			regWrite(15 - k, HIGH);
			delay(speed);
			regWrite(k, LOW);
			regWrite(15 - k, LOW);
		}

		regWrite(i, HIGH);
		regWrite(15 - i, HIGH);
	}
}

void effectE(int speed){
	for (int i = 7; i >= 0; i--){
		for (int k = 0; k <= i; k++)
		{
			regWrite(k, HIGH);
			regWrite(15 - k, HIGH);
			delay(speed);
			regWrite(k, LOW);
			regWrite(15 - k, LOW);
		}

		regWrite(i, HIGH);
		regWrite(15 - i, HIGH);
	}
}

void regWrite(int pin, bool state){
	//Determines register
	int reg = pin / 8;
	//Determines pin for actual register
	int actualPin = pin - (8 * reg);

	//Begin session
	digitalWrite(latchPin, LOW);

	for (int i = 0; i < numOfRegisters; i++){
		//Get actual states for register
		byte* states = &registerState[i];

		//Update state
		if (i == reg){
			bitWrite(*states, actualPin, state);
		}

		//Write
		shiftOut(dataPin, clockPin, MSBFIRST, *states);
	}

	//End session
	digitalWrite(latchPin, HIGH);
}