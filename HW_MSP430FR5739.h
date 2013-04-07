// *** Hardwarespecific functions ***
void UTFT::_hw_special_init()
{
}

void UTFT::LCD_Writ_Bus(char VH,char VL, byte mode)
{   
	switch (mode)
	{
	case 1:
		if (display_serial_mode==SERIAL_4PIN)
		{
		if (VH==1)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		}
		else
		{
		if (VH==1)
			sbi(P_RS, B_RS);
		else
			cbi(P_RS, B_RS);
		}

		if (VL & 0x80)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x40)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x20)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x10)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x08)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x04)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x02)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x01)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		break;
	case 8:
		// Set 8-bit parallel interface pins
		// D0 J.1
		// D1 J.0
		// D2 1.3
		// D3 3.3
		// D4 3.2
		// D5 3.1
		// D6 3.0
		// D7 1.2
		PJOUT &= ~0x03;
		PJOUT |= ((VH & 0x01) << 1) | (VH & 0x02)>>1; // set D0, D1
		P1OUT &= ~0x08;
		P1OUT |= (VH & 0x04) << 1; // set D2
		P3OUT &= ~0x0F;
		P3OUT |= (VH & 0x08) | ((VH & 0x10) >> 2) | ((VH & 0x20) >> 4) | ((VH & 0x40) >> 6); // set D3, D4, D5, D6
		P1OUT &= ~0x04;
		P1OUT |= (VH & 0x80) >> 5; // set D7
		pulse_low(P_WR, B_WR);
		PJOUT &= ~0x03;
		PJOUT |= ((VL & 0x01) << 1) | (VL & 0x02)>>1; // set D0, D1
		P1OUT &= ~0x08;
		P1OUT |= (VL & 0x04) << 1; // set D2
		P3OUT &= ~0x0F;
		P3OUT |= (VL & 0x08) | ((VL & 0x10) >> 2) | ((VL & 0x20) >> 4) | ((VL & 0x40) >> 6); // set D3, D4, D5, D6
		P1OUT &= ~0x04;
		P1OUT |= (VL & 0x80) >> 5; // set D7
		pulse_low(P_WR, B_WR);
		break;
	case 16:
		#pragma message("LCD_Writ_Bus mode 16 pins not defined!")
		break;
	case LATCHED_16:
		asm("nop");		// Mode is unsupported
		break;
	}
}

void UTFT::_set_direction_registers(byte mode)
{
	if (mode!=LATCHED_16)
	{
		PJDIR |= 0x03;
		P1DIR |= 0x0C;
		P3DIR |= 0x0F;
		if (mode==16) {
			#pragma message("_set_direction_registers mode 16 pins not defined!")
		}
	}
	else
	{
		asm("nop");		// Mode is unsupported
	}
}

void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
	long blocks;
	#pragma message("_fast_fill_16 pins not defined!")

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);
		}
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;
	// Set 8-bit parallel interface pins
	// D0 J.1
	// D1 J.0
	// D2 1.3
	// D3 3.3
	// D4 3.2
	// D5 3.1
	// D6 3.0
	// D7 1.2
	PJOUT &= ~0x03;
	PJOUT |= ((ch & 0x01) << 1) | (ch & 0x02)>>1; // set D0, D1
	P1OUT &= ~0x08;
	P1OUT |= (ch & 0x04) << 1; // set D2
	P3OUT &= ~0x0F;
	P3OUT |= (ch & 0x08) | ((ch & 0x10) >> 2) | ((ch & 0x20) >> 4) | ((ch & 0x40) >> 6); // set D3, D4, D5, D6
	P1OUT &= ~0x04;
	P1OUT |= (ch & 0x80) >> 5; // set D7

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}
