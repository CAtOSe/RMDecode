void remote_read() {
  unsigned int timer_value;
  if(_nec_state != 0){
    timer_value = TCNT1;                         // Store Timer1 value
    TCNT1 = 0;                                   // Reset Timer1
  }
  
  switch(_nec_state) {
	case 0:
		// 9 ms pulse start
		TCNT2 = 0; // Reset timer to start counting 9ms
		TCCR2B = 
  }
  
  /*
  switch(_nec_state){
   case 0 :                                      // Start receiving IR data (we're at the beginning of 9ms pulse)
    TCNT1  = 0;                                  // Reset Timer1
    TCCR1B = 2;                                  // Enable Timer1 module with 1/8 prescaler ( 2 ticks every 1 us)
    _nec_state = 1;                               // Next state: end of 9ms pulse (start of 4.5ms space)
    _i = 0;
    return;
   case 1 :                                      // End of 9ms pulse
    if((timer_value > 19000) || (timer_value < 17000)){         // Invalid interval ==> stop decoding and reset
      _nec_state = 0;                             // Reset decoding process
      TCCR1B = 0;                                // Disable Timer1 module
    }
    else
      _nec_state = 2;                             // Next state: end of 4.5ms space (start of 562µs pulse)
    return;
   case 2 :                                      // End of 4.5ms space
    if((timer_value > 10000) || (timer_value < 8000)){
      _nec_state = 0;                             // Reset decoding process
      TCCR1B = 0;                                // Disable Timer1 module
    }
    else
      _nec_state = 3;                             // Next state: end of 562µs pulse (start of 562µs or 1687µs space)
    return;
   case 3 :                                      // End of 562µs pulse
    if((timer_value > 1400) || (timer_value < 800)){           // Invalid interval ==> stop decoding and reset
      TCCR1B = 0;                                // Disable Timer1 module
      _nec_state = 0;                             // Reset decoding process
    }
    else
      _nec_state = 4;                             // Next state: end of 562µs or 1687µs space
    return;
   case 4 :                                      // End of 562µs or 1687µs space
    if((timer_value > 3600) || (timer_value < 800)){           // Time interval invalid ==> stop decoding
      TCCR1B = 0;                                // Disable Timer1 module
      _nec_state = 0;                             // Reset decoding process
      return;
    }
    if( timer_value > 2000)                      // If space width > 1ms (short space)
      bitSet(_nec_code, (31 - _i));                // Write 1 to bit (31 - i)
    else                                         // If space width < 1ms (long space)
      bitClear(_nec_code, (31 - _i));              // Write 0 to bit (31 - i)
    _i++;
    if(_i > 31){                                  // If all bits are received
      _nec_ok = 1;                                // Decoding process OK
      detachInterrupt(IR_INT);                        // Disable external interrupt (INT0)
      return;
    }
    _nec_state = 3;                               // Next state: end of 562µs pulse (start of 562µs or 1687µs space)
  }
  */
}
