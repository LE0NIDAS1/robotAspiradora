/*
 * I2C.c
 *
 * Created: 18/10/2019 1:04:50
 *  Author: leoeg
 */ 

#include "I2C.h"

void startI2c()
{
	TWBR = ((F_CPU / 400000) - 16) / 2;
}

char scanI2c (const uint8_t DEVICE)
{
	uint8_t n = 0;
	uint8_t check = 0;
	uint8_t error = 0;
	char foundDevice = false;

	while (n == 0 && error < 16)
	{
		//start condition
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

		for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
		{
		}

		//check start is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x08 && check != 0x10)
			{
				n = 0;
			}
		}

		//send device address
		if (n != 0)
		{
			TWDR = (DEVICE << 1);
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send device address is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x18 && check != 0x40)
			{
				n = 0;
			}
		}

		//stop condition
		if (n != 0)
		{
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWSTO))) == true && n > 0; n--)
			{
			}
		}

		//error count (max error = 16)
		if (n == 0)
		{
			TWCR = 0;
			error++;
		}
	}

	//device found or not
	if (error < 16)
	{
		foundDevice = true;
	}

	return foundDevice;
}

void writeI2c (const uint8_t DEVICE, const uint8_t ADDRESS, const uint8_t DATA)
{
	uint8_t n = 0;
	uint8_t check = 0;

	while (n == 0)
	{
		TWCR = 0;

		//start condition
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

		for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
		{
		}

		//check start is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x08 && check != 0x10)
			{
				n = 0;
			}
		}

		//send device address
		if (n != 0)
		{
			TWDR = (DEVICE << 1);
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send device address is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x18 && check != 0x40)
			{
				n = 0;
			}
		}

		//send address
		if (n != 0)
		{
			TWDR = ADDRESS;
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send address is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x28)
			{
				n = 0;
			}
		}

		//send data
		if (n != 0)
		{
			TWDR = DATA;
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send data is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x28)
			{
				n = 0;
			}
		}

		//stop condition
		if (n != 0)
		{
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWSTO))) == true && n > 0; n--)
			{
			}
		}
	}
}

int16_t readI2c (const uint8_t DEVICE, const uint8_t ADDRESS)
{
	uint8_t n = 0;
	uint8_t check = 0;
	uint8_t error = 0;
	int16_t data = 0;

	while (n == 0 && error < 16)
	{
		//start condition
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

		for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
		{
		}

		//check start is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x08 && check != 0x10)
			{
				n = 0;
			}
		}

		//send device address
		if (n != 0)
		{
			TWDR = (DEVICE << 1);
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send device address is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x18 && check != 0x40)
			{
				n = 0;
			}
		}

		//send address
		if (n != 0)
		{
			TWDR = ADDRESS;
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send address is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x28)
			{
				n = 0;
			}
		}

		//start condition
		if (n != 0)
		{
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check start is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x08 && check != 0x10)
			{
				n = 0;
			}
		}

		//send device address
		if (n != 0)
		{
			TWDR = (DEVICE << 1) + 1;
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}
		}

		//check send device address is ok
		if (n != 0)
		{
			check = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3)) & 0xf8;

			if (check != 0x18 && check != 0x40)
			{
				n = 0;
			}
		}

		//read ack
		if (n != 0)
		{
			TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}

			data = TWDR << 8;
		}

		//read nak
		if (n != 0)
		{
			TWCR = (1 << TWINT) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWINT))) == true && n > 0; n--)
			{
			}

			data |= TWDR;
		}

		//stop condition
		if (n != 0)
		{
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

			for (n = 255; (!(TWCR & (1 << TWSTO))) == true && n > 0; n--)
			{
			}
		}

		//error count (max error = 16)
		if (n == 0)
		{
			TWCR = 0;
			data = 0;
			error++;
		}
	}

	return data;
}