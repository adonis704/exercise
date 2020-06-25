#include <math.h>
#include <stdbool.h>
#include "APDS_9960.h"
#include "delay.h"
#include "myiic.h"
#include "LED.h"
#include "usart.h"

extern int abs(int __x);
void Wire_begin_(void);


/* Container for gesture data */
typedef struct gesture_data_type {
	uint8_t u_data[32];
	uint8_t d_data[32];
	uint8_t l_data[32];
	uint8_t r_data[32];
	uint8_t index;
	uint8_t total_gestures;
	uint8_t in_threshold;
	uint8_t out_threshold;
} gesture_data_type;
gesture_data_type gesture_data_;

int gesture_ud_delta_;
int gesture_lr_delta_;
int gesture_ud_count_;
int gesture_lr_count_;
int gesture_near_count_;
int gesture_far_count_;
int gesture_state_;
int gesture_motion_;


/*Instantiates SparkFun_APDS9960 object*/
void SparkFun_APDS9960(void)
{
	gesture_ud_delta_ = 0;
	gesture_lr_delta_ = 0;

	gesture_ud_count_ = 0;
	gesture_lr_count_ = 0;

	gesture_near_count_ = 0;
	gesture_far_count_ = 0;

	gesture_state_ = 0;
	gesture_motion_ = DIR_NONE;
}

bool SparkFun_APDS9960_init(void)
{
	uint8_t id, *pid;

	/*Settings*/
	Wire_begin_();

	if (!wireReadDataByte(APDS9960_ID, pid))
	{
		return false;
	}

	id = *pid;

	if (!((id == APDS9960_ID_1 || id == APDS9960_ID_2)))
	{
		return false;
	}

	if (!setMode(ALL, OFF))
	{
		return false;
	}


	/* Set default values for gesture sense registers */
	if (!setGestureEnterThresh(DEFAULT_GPENTH)) {
		return false;
	}
	if (!setGestureExitThresh(DEFAULT_GEXTH)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GCONF1, DEFAULT_GCONF1)) {
		return false;
	}
	if (!setGestureGain(DEFAULT_GGAIN)) {
		return false;
	}
	if (!setGestureLEDDrive(DEFAULT_GLDRIVE)) {
		return false;
	}
	if (!setGestureWaitTime(DEFAULT_GWTIME)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GOFFSET_U, DEFAULT_GOFFSET)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GOFFSET_D, DEFAULT_GOFFSET)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GOFFSET_L, DEFAULT_GOFFSET)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GOFFSET_R, DEFAULT_GOFFSET)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GPULSE, DEFAULT_GPULSE)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_GCONF3, DEFAULT_GCONF3)) {
		return false;
	}
	if (!setGestureIntEnable(DEFAULT_GIEN)) {
		return false;
	}


	return true;


}

/*Get contents of the ENABLE register*/
uint8_t getMode(void)
{
	uint8_t enable_value;

	/* Read current ENABLE register */
	if (!wireReadDataByte(APDS9960_ENABLE, &enable_value)) {
		return ERROR;
	}

	return enable_value;
}

/*Set mode*/
bool setMode(int8_t mode, uint8_t enable)
{
	uint8_t reg_val;

	reg_val = getMode();
	if (reg_val == ERROR) {
		return false;
	}

	enable = enable & 0x01;
	if ((mode >= 0) && (mode <= 6)) {
		if (enable) {
			reg_val |= (1 << mode);
		}
		else {
			reg_val &= ~(1 << mode);
		}
	}
	else if (mode == ALL) {
		if (enable) {
			reg_val = 0x7F;
		}
		else {
			reg_val = 0x00;
		}
	}

	/* Write value back to ENABLE register */
	if (!wireWriteDataByte(APDS9960_ENABLE, reg_val)) {
		return false;
	}

	return true;
}


/*Enable sensor*/
bool enableGestureSensor(bool interrupts)
{
	resetGestureParameters();
	if (!wireWriteDataByte(APDS9960_WTIME, 0xFF)) {
		return false;
	}
	if (!wireWriteDataByte(APDS9960_PPULSE, DEFAULT_GESTURE_PPULSE)) {
		return false;
	}
	if (!setLEDBoost(LED_BOOST_300)) {
		return false;
	}
	if (interrupts) {
		if (!setGestureIntEnable(1)) {
			return false;
		}
	}
	else {
		if (!setGestureIntEnable(0)) {
			return false;
		}
	}
	if (!setGestureMode(1)) {
		return false;
	}
	if (!enablePower()) {
		return false;
	}
	if (!setMode(WAIT, 1)) {
		return false;
	}
	if (!setMode(PROXIMITY, 1)) {
		return false;
	}
	if (!setMode(GESTURE, 1)) {
		return false;
	}

	return true;
}


/*Determines if there is a gesture available for reading*/
bool isGestureAvailable(void)
{
	uint8_t val;

	if (!wireReadDataByte(APDS9960_GSTATUS, &val))
	{
		return ERROR;
	}

	val &= APDS9960_GVALID;

	if (val == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*Processes a gesture event and returns best guessed gesture*/
int readGesture(void)
{
	uint8_t fifo_level = 0;
	int8_t bytes_read = 0;
	uint8_t fifo_data[128];
	uint8_t gstatus;
	int motion;
	int i;


	/* Make sure that power and gesture is on and data is valid */
	if (!isGestureAvailable() || !(getMode() & 01000001)) {
		return DIR_NONE;
	}

	/* Keep looping as long as gesture data is valid */
	while (1)
	{

		/* Wait some time to collect next batch of FIFO data */
		delay_ms(FIFO_PAUSE_TIME);

		/* Get the contents of the STATUS register. Is data still valid? */
		if (!wireReadDataByte(APDS9960_GSTATUS, &gstatus))
			return ERROR;

		/* If we have valid data, read in FIFO */
		if ((gstatus & APDS9960_GVALID) == APDS9960_GVALID)
		{

			/* Read the current FIFO level */
			if (!wireReadDataByte(APDS9960_GFLVL, &fifo_level))
				return ERROR;

			/* If there's stuff in the FIFO, read it into our data block */
			if (fifo_level > 0) {
				bytes_read = wireReadDataBlock(APDS9960_GFIFO_U,
					(uint8_t*)fifo_data,
					(fifo_level * 4));
				if (bytes_read == -1) {
					return ERROR;
				}

				/* If at least 1 set of data, sort the data into U/D/L/R */
				if (bytes_read >= 4) {
					for (i = 0; i < bytes_read; i += 4) {

						gesture_data_.u_data[gesture_data_.index] = \
							fifo_data[i + 0];
						gesture_data_.d_data[gesture_data_.index] = \
							fifo_data[i + 1];
						gesture_data_.l_data[gesture_data_.index] = \
							fifo_data[i + 2];
						gesture_data_.r_data[gesture_data_.index] = \
							fifo_data[i + 3];
						gesture_data_.index++;
						gesture_data_.total_gestures++;
					}


					/* Filter and process gesture data. Decode near/far state */
					if (processGestureData()) {
						if (decodeGesture()) {
							//***TODO: U-Turn Gestures
						}
					}

					/* Reset data */
					gesture_data_.index = 0;
					gesture_data_.total_gestures = 0;
				}
			}
		}
		else
		{

			/* Determine best guessed gesture and clean up */
			delay_ms(FIFO_PAUSE_TIME);
			decodeGesture();
			motion = gesture_motion_;
			resetGestureParameters();

			return motion;
		}
	}
}

/*Turn the APDS-9960 on*/
bool enablePower(void)
{
	if (!setMode(POWER, 1)) {
		return false;
	}

	return true;
}

/*Turn the APDS-9960 off*/
bool disablePower(void)
{
	if (!setMode(POWER, 0)) {
		return false;
	}

	return true;
}


/*Resets all the parameters */
void resetGestureParameters(void)
{
	gesture_data_.index = 0;
	gesture_data_.total_gestures = 0;

	gesture_ud_delta_ = 0;
	gesture_lr_delta_ = 0;

	gesture_ud_count_ = 0;
	gesture_lr_count_ = 0;

	gesture_near_count_ = 0;
	gesture_far_count_ = 0;

	gesture_state_ = 0;
	gesture_motion_ = DIR_NONE;
}

/*Processes the raw gesture data*/
bool processGestureData(void)
{

	uint8_t u_first = 0;
	uint8_t d_first = 0;
	uint8_t l_first = 0;
	uint8_t r_first = 0;
	uint8_t u_last = 0;
	uint8_t d_last = 0;
	uint8_t l_last = 0;
	uint8_t r_last = 0;
	int ud_ratio_first;
	int lr_ratio_first;
	int ud_ratio_last;
	int lr_ratio_last;
	int ud_delta;
	int lr_delta;
	int i;

	/* Check to make sure our data isn't out of bounds */
	if ((gesture_data_.total_gestures <= 32) && \
		(gesture_data_.total_gestures > 0)) {

		/* Find the first value in U/D/L/R above the threshold */
		for (i = 0; i < gesture_data_.total_gestures; i++) {
			if ((gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
				(gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
				(gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
				(gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT)) {

				u_first = gesture_data_.u_data[i];
				d_first = gesture_data_.d_data[i];
				l_first = gesture_data_.l_data[i];
				r_first = gesture_data_.r_data[i];
				break;
			}
		}

		/* If one of the _first values is 0, then there is no good data */
		if ((u_first == 0) || (d_first == 0) || \
			(l_first == 0) || (r_first == 0)) {

			return false;
		}

		/* Find the last value in U/D/L/R above the threshold */
		for (i = gesture_data_.total_gestures - 1; i >= 0; i--) {

			if ((gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
				(gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
				(gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
				(gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT)) {

				u_last = gesture_data_.u_data[i];
				d_last = gesture_data_.d_data[i];
				l_last = gesture_data_.l_data[i];
				r_last = gesture_data_.r_data[i];

				break;
			}
		}
	}

	/* Calculate the first vs. last ratio of up/down and left/right */
	ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
	lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
	ud_ratio_last = ((u_last - d_last) * 100) / (u_last + d_last);
	lr_ratio_last = ((l_last - r_last) * 100) / (l_last + r_last);


	/* Determine the difference between the first and last ratios */
	ud_delta = ud_ratio_last - ud_ratio_first;
	lr_delta = lr_ratio_last - lr_ratio_first;


	/* Accumulate the UD and LR delta values */
	gesture_ud_delta_ += ud_delta;
	gesture_lr_delta_ += lr_delta;


	/* Determine U/D gesture */
	if (gesture_ud_delta_ >= GESTURE_SENSITIVITY_1) {
		gesture_ud_count_ = 1;
	}
	else if (gesture_ud_delta_ <= -GESTURE_SENSITIVITY_1) {
		gesture_ud_count_ = -1;
	}
	else {
		gesture_ud_count_ = 0;
	}

	/* Determine L/R gesture */
	if (gesture_lr_delta_ >= GESTURE_SENSITIVITY_1) {
		gesture_lr_count_ = 1;
	}
	else if (gesture_lr_delta_ <= -GESTURE_SENSITIVITY_1) {
		gesture_lr_count_ = -1;
	}
	else {
		gesture_lr_count_ = 0;
	}

	/* Determine Near/Far gesture */
	if ((gesture_ud_count_ == 0) && (gesture_lr_count_ == 0)) {
		if ((abs(ud_delta) < GESTURE_SENSITIVITY_2) && (abs(lr_delta) < GESTURE_SENSITIVITY_2))
		{

			if ((ud_delta == 0) && (lr_delta == 0)) {
				gesture_near_count_++;
			}
			else if ((ud_delta != 0) || (lr_delta != 0)) {
				gesture_far_count_++;
			}

			if ((gesture_near_count_ >= 10) && (gesture_far_count_ >= 2)) {
				if ((ud_delta == 0) && (lr_delta == 0)) {
					gesture_state_ = NEAR_STATE;
				}
				else if ((ud_delta != 0) && (lr_delta != 0)) {
					gesture_state_ = FAR_STATE;
				}
				return true;
			}
		}
	}
	else {
		if ((abs((int)ud_delta) < GESTURE_SENSITIVITY_2) && (abs((int)lr_delta) < GESTURE_SENSITIVITY_2)) {

			if ((ud_delta == 0) && (lr_delta == 0)) {
				gesture_near_count_++;
			}

			if (gesture_near_count_ >= 10) {
				gesture_ud_count_ = 0;
				gesture_lr_count_ = 0;
				gesture_ud_delta_ = 0;
				gesture_lr_delta_ = 0;
			}
		}
	}

	return false;
}

/*Determines swipe direction or near/far state*/
bool decodeGesture(void)
{
	/* Return if near or far event is detected */
	if (gesture_state_ == NEAR_STATE) {
		gesture_motion_ = DIR_NEAR;
		return true;
	}
	else if (gesture_state_ == FAR_STATE) {
		gesture_motion_ = DIR_FAR;
		return true;
	}

	/* Determine swipe direction */
	if ((gesture_ud_count_ == -1) && (gesture_lr_count_ == 0)) {
		gesture_motion_ = DIR_UP;
	}
	else if ((gesture_ud_count_ == 1) && (gesture_lr_count_ == 0)) {
		gesture_motion_ = DIR_DOWN;
	}
	else if ((gesture_ud_count_ == 0) && (gesture_lr_count_ == 1)) {
		gesture_motion_ = DIR_RIGHT;
	}
	else if ((gesture_ud_count_ == 0) && (gesture_lr_count_ == -1)) {
		gesture_motion_ = DIR_LEFT;
	}
	else if ((gesture_ud_count_ == -1) && (gesture_lr_count_ == 1)) {
		if (abs(gesture_ud_delta_) > abs(gesture_lr_delta_)) {
			gesture_motion_ = DIR_UP;
		}
		else {
			gesture_motion_ = DIR_RIGHT;
		}
	}
	else if ((gesture_ud_count_ == 1) && (gesture_lr_count_ == -1)) {
		if (abs(gesture_ud_delta_) > abs(gesture_lr_delta_)) {
			gesture_motion_ = DIR_DOWN;
		}
		else {
			gesture_motion_ = DIR_LEFT;
		}
	}
	else if ((gesture_ud_count_ == -1) && (gesture_lr_count_ == -1)) {
		if (abs(gesture_ud_delta_) > abs(gesture_lr_delta_)) {
			gesture_motion_ = DIR_UP;
		}
		else {
			gesture_motion_ = DIR_LEFT;
		}
	}
	else if ((gesture_ud_count_ == 1) && (gesture_lr_count_ == 1)) {
		if (abs(gesture_ud_delta_) > abs(gesture_lr_delta_)) {
			gesture_motion_ = DIR_DOWN;
		}
		else {
			gesture_motion_ = DIR_RIGHT;
		}
	}
	else {

		return false;
	}

	return true;
}


/*Sets the LED current boost value*/
bool setLEDBoost(uint8_t boost)
{
	uint8_t val;

	/* Read value from CONFIG2 register */
	if (!wireReadDataByte(APDS9960_CONFIG2, &val)) {
		return false;
	}

	/* Set bits in register to given value */
	boost &= 00000011;
	boost = boost << 4;
	val &= 11001111;
	val |= boost;

	/* Write register value back into CONFIG2 register */
	if (!wireWriteDataByte(APDS9960_CONFIG2, val)) {
		return false;
	}

	return true;
}





/*Sets the entry proximity threshold for gesture sensing*/
bool setGestureEnterThresh(uint8_t threshold)
{
	if (!wireWriteDataByte(APDS9960_GPENTH, threshold)) {
		return false;
	}

	return true;
}


/*Sets the exit proximity threshold for gesture sensing*/
bool setGestureExitThresh(uint8_t threshold)
{
	if (!wireWriteDataByte(APDS9960_GEXTH, threshold)) {
		return false;
	}

	return true;
}



/*Sets the gain of the photodiode during gesture mode*/
bool setGestureGain(uint8_t gain)
{
	uint8_t val;

	/* Read value from GCONF2 register */
	if (!wireReadDataByte(APDS9960_GCONF2, &val)) {
		return false;
	}

	/* Set bits in register to given value */
	gain &= 00000011;
	gain = gain << 5;
	val &= 10011111;
	val |= gain;

	/* Write register value back into GCONF2 register */
	if (!wireWriteDataByte(APDS9960_GCONF2, val)) {
		return false;
	}

	return true;
}



/*Sets the LED drive current during gesture mode*/
bool setGestureLEDDrive(uint8_t drive)
{
	uint8_t val;

	/* Read value from GCONF2 register */
	if (!wireReadDataByte(APDS9960_GCONF2, &val)) {
		return false;
	}

	/* Set bits in register to given value */
	drive &= 00000011;
	drive = drive << 3;
	val &= 11100111;
	val |= drive;

	/* Write register value back into GCONF2 register */
	if (!wireWriteDataByte(APDS9960_GCONF2, val)) {
		return false;
	}

	return true;
}



/*Sets the time in low power mode between gesture detections*/
bool setGestureWaitTime(uint8_t time)
{
	uint8_t val;

	/* Read value from GCONF2 register */
	if (!wireReadDataByte(APDS9960_GCONF2, &val)) {
		return false;
	}


	/* Set bits in register to given value */
	time &= 00000111;
	val &= 11111000;
	val |= time;

	/* Write register value back into GCONF2 register */
	if (!wireWriteDataByte(APDS9960_GCONF2, val)) {
		return false;
	}

	return true;
}




/*Turns gesture-related interrupts on or off*/
bool setGestureIntEnable(uint8_t enable)
{
	uint8_t val;

	/* Read value from GCONF4 register */
	if (!wireReadDataByte(APDS9960_GCONF4, &val)) {
		return false;
	}

	/* Set bits in register to given value */
	enable &= 00000001;
	enable = enable << 1;
	val &= 11111101;
	val |= enable;

	/* Write register value back into GCONF4 register */
	if (!wireWriteDataByte(APDS9960_GCONF4, val)) {
		return false;
	}

	return true;
}




/*Tells the state machine to either enter or exit gesture state machine*/
bool setGestureMode(uint8_t mode)
{
	uint8_t val;

	/* Read value from GCONF4 register */
	if (!wireReadDataByte(APDS9960_GCONF4, &val)) {
		return false;
	}

	/* Set bits in register to given value */
	mode &= 00000001;
	val &= 11111110;
	val |= mode;

	/* Write register value back into GCONF4 register */
	if (!wireWriteDataByte(APDS9960_GCONF4, val)) {
		return false;
	}

	return true;
}


/*Writes a single byte to the I2C device and specified register*/
bool wireWriteDataByte(uint8_t reg, uint8_t val)
{
	IIC_Start();
	IIC_Send_Byte((APDS9960_I2C_ADDR << 1) | 0);
	if (IIC_Wait_Ack())
	{
		IIC_Stop();
		return false;
	}
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Send_Byte(val);
	if (IIC_Wait_Ack())
	{
		IIC_Stop();
		return false;
	}
	IIC_Stop();
	return true;

}



/*Reads a single byte from the I2C device and specified register*/
bool wireReadDataByte(uint8_t reg, uint8_t *val)
{

	IIC_Start();
	IIC_Send_Byte((APDS9960_I2C_ADDR << 1) | 0X00);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((APDS9960_I2C_ADDR << 1) | 0x01);
	IIC_Wait_Ack();
	*val = IIC_Read_Byte(0);
	IIC_Stop();
	return true;
}

/*Reads a block (array) of bytes from the I2C device and register*/
int wireReadDataBlock(uint8_t reg, uint8_t *val, unsigned int len)
{
	unsigned char i = 0;
	IIC_Start();
	IIC_Send_Byte((APDS9960_I2C_ADDR << 1) | 0);
	if (IIC_Wait_Ack())
	{
		IIC_Stop();
		return -1;
	}
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((APDS9960_I2C_ADDR << 1) | 1);
	IIC_Wait_Ack();
	while (len)
	{
		if (len == 1) val[i] = IIC_Read_Byte(0);
		else val[i] = IIC_Read_Byte(1);
		i++;
		len--;
	}
	IIC_Stop();
	return i;
}



void Wire_begin_(void)
{
	IIC_Init();

}



