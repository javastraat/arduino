/*

  SendCommand() sends commands in the format that the SRF sensors expect

 */

void sendCommand (int address, int command) {

  // start I2C transmission:

  Wire.beginTransmission(address);

  // send command:

  Wire.send(0x00);

  Wire.send(command);

  // end I2C transmission:

  Wire.endTransmission();

}

/*

  setRegister() tells the SRF sensor to change the address pointer position

 */

void setRegister(int address, int thisRegister) {

  // start I2C transmission:

  Wire.beginTransmission(address);

  // send address to read from:

  Wire.send(thisRegister);

  // end I2C transmission:

  Wire.endTransmission();

}

/*

readData() returns a result from the SRF sensor

 */

int readData(int address, int numBytes) {

  int result = 0;        // the result is two bytes long

// send I2C request for data:

  Wire.requestFrom(address, numBytes);

  // wait for two bytes to return:

  while (Wire.available() < 2 )   {

    // wait for result

  }

  // read the two bytes, and combine them into one int:

  result = Wire.receive() * 256;

  result = result + Wire.receive();

  // return the result:

  return result;

}
