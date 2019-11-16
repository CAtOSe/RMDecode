void remote_read() {
  unsigned int timer_value;

  switch(_nec_state) {
  	case 0:
      //Serial.println("Rcv begin");
  		// 9 ms pulse start
      TCCR2B = 7; // Set clock divider to 1024 (16.32ms)
      TCNT2 = 0; // Reset timer to start counting 9ms
      _overflow_count = 0;
      _nec_state = 1;
      Serial.println("START 9");

      return;

    case 1:
      // 9 ms pulse end
      timer_value = TCNT2;
      TCNT2 = 0;

      if (timer_value >= 133 && timer_value <= 147) {
        // Good timing, move on to next stage
        TCNT2 = 0;
        Serial.println("PASS 9");
        _nec_state = 2;
      } else {
        // Bad timing, disable & reset
        Serial.println(timer_value);
        TCCR2B = 0;
        _nec_state = 0;
      }
      return;

    case 2:
      // 4.5ms space end & start of 1st bit
      timer_value = TCNT2;
      TCNT2 = 0;

      if (timer_value >= 63 && timer_value <= 77) {
        TCCR2B = 6; // Change prescaler to 256 (4.08ms)
        TCNT2 = 0;
        Serial.println("PASS 4.5");
        _i = 0;
        _nec_state = 3;
      } else {
        // Serial.println("B2");
        // Bad timing, disable & reset
        TCCR2B = 0;
        _nec_state = 0;
      }
      return;

    case 3:
      // end of logic pulse bit (should be 560us)
      timer_value = TCNT2;

      if (timer_value >= 28 && timer_value <= 42) {
        _nec_state = 4;
      } else {
        // Serial.println("B3");
        // Bad timing, disable & reset
        TCCR2B = 0;
        _nec_state = 0;
      }
      return;

    case 4:
      // end of logic pulse (space and bit)
      // 2.25ms for '1'
      // 1.12ms for '0'
      timer_value = TCNT2;
      TCNT2 = 0;

      if (timer_value >= 133 && timer_value <= 147) {
        // Logic '1'
        bitSet(_nec_code, (31 - _i));
        // Serial.print("End of ");
        // Serial.println(_i);
        _i++;
        _nec_state = 3;
      } else if (timer_value >= 63 && timer_value <= 77) {
        // Logic '0'
        bitClear(_nec_code, (31 - _i));
        // Serial.print("End of ");
        // Serial.println(_i);
        _i++;
        _nec_state = 3;
      } else {
        // Serial.println("B4");
        // Bad timing, disable & reset
        TCCR2B = 0;
        _nec_state = 0;
      }

      if (_i > 31) {
        TCCR2B = 0;
        TCNT2 = 0;

        detachInterrupt(IR_INT);
        delay(2);

        //Serial.println("DONE");
        Serial.println(_nec_code, HEX);
        _nec_ok = true;
        _nec_state = 0;

      }
      return;

    default:
      // ???
      TCCR2B = 0;
      _nec_state = 0;
  }
}
