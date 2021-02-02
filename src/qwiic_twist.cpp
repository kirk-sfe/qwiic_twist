/*
  This is a library written for the SparkFun Qwiic Twist Encoder
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/15083

  Written by Nathan Seidle @ SparkFun Electronics, November 25th, 2018

  The Qwiic Twist is a I2C controlled encoder

  https://github.com/sparkfun/SparkFun_Qwiic_Twist_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.7

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include "qwiic_i2c.h"
#include "qwiic_twist.h"

Qwiic_I2C qwiic;

//Constructor
TWIST::TWIST(uint8_t deviceAddress)
{
    _deviceAddress = deviceAddress;
}

//Initializes the sensor with basic settings
//Returns false if sensor is not detected
bool TWIST::begin()
{

    // init I2C.

    if(!qwiic.init()){
        printf("Error: I2C subsystem failed to initialize.");
        return false;
    }

    return isConnected();

}

//Returns true if I2C device ack's
bool TWIST::isConnected()
{
    return  qwiic.pingDevice(_deviceAddress);
}

//Change the I2C address of this address to newAddress
bool TWIST::changeAddress(uint8_t newAddress)
{

    bool rc = qwiic.writeRegister(_deviceAddress, TWIST_CHANGE_ADDRESS, newAddress);

    //Once the address is changed, we need to change it in the library
    if(rc)
        _deviceAddress = newAddress;

    return rc;
}

//Clears the moved, clicked, and pressed bits
void TWIST::clearInterrupts()
{
    qwiic.writeRegister(_deviceAddress, TWIST_STATUS, 0); //Clear the moved, clicked, and pressed bits
}

//Returns the number of indents the user has twisted the knob
int16_t TWIST::getCount()
{

  uint16_t value;

  int rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_COUNT, &value);

  return (rc != QWIIC_ERROR_GENERIC ? value : -1);

}

//Set the encoder count to a specific amount
bool TWIST::setCount(int16_t amount)
{

  return qwiic.writeRegisterInt16(_deviceAddress, TWIST_COUNT, amount);
}

//Returns the limit of allowed counts before wrapping
//0 is disabled
uint16_t TWIST::getLimit()
{
    uint16_t value;

    int rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_LIMIT, &value);

    return (rc != QWIIC_ERROR_GENERIC ? value : -1);

}

//Set the encoder count limit to a specific amount
bool TWIST::setLimit(uint16_t amount)
{
    return qwiic.writeRegisterInt16(_deviceAddress, TWIST_LIMIT, amount);  

}

//Returns the number of ticks since last check
int16_t TWIST::getDiff(bool clearValue)
{
    uint16_t difference;

    int rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_DIFFERENCE, &difference);

    if(rc == QWIIC_ERROR_GENERIC) // error
        return 0;

    //Clear the current value if requested
    if (clearValue == true)
      qwiic.writeRegisterInt16(_deviceAddress, TWIST_DIFFERENCE, 0);

    return difference;
}

//Returns true if button is currently being pressed
bool TWIST::isPressed()
{
    uint8_t status = qwiic.readRegister(_deviceAddress, TWIST_STATUS);

    bool pressed = status & (1 << statusButtonPressedBit);

    qwiic.writeRegister(_deviceAddress, TWIST_STATUS, status & ~(1 << statusButtonPressedBit)); //We've read this status bit, now clear it

    return pressed;
}

//Returns true if a click event has occurred
bool TWIST::isClicked()
{
    uint8_t status = qwiic.readRegister(_deviceAddress, TWIST_STATUS);
    bool clicked = status & (1 << statusButtonClickedBit);

    qwiic.writeRegister(_deviceAddress, TWIST_STATUS, status & ~(1 << statusButtonClickedBit)); //We've read this status bit, now clear it

    return clicked;
}

//Returns true if knob has been twisted
bool TWIST::isMoved()
{
    uint8_t status = qwiic.readRegister(_deviceAddress, TWIST_STATUS);
    bool moved = status & (1 << statusEncoderMovedBit);

    qwiic.writeRegister(_deviceAddress, TWIST_STATUS, status & ~(1 << statusEncoderMovedBit)); //We've read this status bit, now clear it

    return moved;
}

//Returns the number of milliseconds since the last encoder movement
//By default, clear the current value
uint16_t TWIST::timeSinceLastMovement(bool clearValue)
{
    int rc;

    uint16_t timeElapsed;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_LAST_ENCODER_EVENT, &timeElapsed);

    if(rc == QWIIC_ERROR_GENERIC)
       return -1; // eeror

    //Clear the current value if requested
    if (clearValue == true)
        qwiic.writeRegisterInt16(_deviceAddress, TWIST_LAST_ENCODER_EVENT, 0);

    return timeElapsed;
}

//Returns the number of milliseconds since the last button event (press and release)
uint16_t TWIST::timeSinceLastPress(bool clearValue)
{

    int rc;

    uint16_t timeElapsed;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_LAST_BUTTON_EVENT, &timeElapsed);

    if(rc == QWIIC_ERROR_GENERIC)
        return -1; // eeror

    //Clear the current value if requested
    if (clearValue == true)
        qwiic.writeRegisterInt16(_deviceAddress, TWIST_LAST_BUTTON_EVENT, 0);

    return timeElapsed;
}

//Sets the color of the encoder LEDs
bool TWIST::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    int rc;
    uint8_t color[3]  = {blue, green, red};

    rc = qwiic.writeRegisterRegion(_deviceAddress, TWIST_RED, color, 3);

    return (rc != QWIIC_ERROR_GENERIC);

}

//Sets the color of a specific color
bool TWIST::setRed(uint8_t red)
{
    return qwiic.writeRegister(_deviceAddress, TWIST_RED, red);
}

bool TWIST::setGreen(uint8_t green)
{
    return qwiic.writeRegister(_deviceAddress, TWIST_GREEN, green);
}

bool TWIST::setBlue(uint8_t blue)
{
    return qwiic.writeRegister(_deviceAddress, TWIST_BLUE, blue);
}

//Returns the current value of a color
uint8_t TWIST::getRed()
{
    return qwiic.readRegister(_deviceAddress, TWIST_RED);
}
uint8_t TWIST::getGreen()
{
    return qwiic.readRegister(_deviceAddress, TWIST_GREEN);
}
uint8_t TWIST::getBlue()
{
     return qwiic.readRegister(_deviceAddress, TWIST_BLUE);
}

//Returns a two byte Major/Minor version number
uint16_t TWIST::getVersion()
{
    int rc;
    uint16_t version;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_VERSION, &version);

    return (rc == QWIIC_ERROR_GENERIC ? 0 : version);

}

//Sets the relation between each color and the twisting of the knob
//Connect the LED so it changes [amount] with each encoder tick
//Negative numbers are allowed (so LED gets brighter the more you turn the encoder down)
bool TWIST::connectColor(int16_t red, int16_t green, int16_t blue)
{
    uint8_t buffer[6] = {(uint8_t)(red >> 8), (uint8_t)(red & 0xFF), 
                    (uint8_t)(green >> 8), (uint8_t)(green & 0xFF), 
                    (uint8_t)(blue >> 8), (uint8_t)(blue & 0xFF)};

    int rc;

    rc = qwiic.writeRegisterRegion(_deviceAddress, TWIST_CONNECT_RED, buffer, sizeof(buffer));

    return (rc != QWIIC_ERROR_GENERIC);
  
}

bool TWIST::connectRed(int16_t red)
{

    return (qwiic.writeRegisterInt16(_deviceAddress, TWIST_CONNECT_RED, red) != QWIIC_ERROR_GENERIC);

}

bool TWIST::connectGreen(int16_t green)
{
    return (qwiic.writeRegisterInt16(_deviceAddress, TWIST_CONNECT_GREEN, green) != QWIIC_ERROR_GENERIC);

}
bool TWIST::connectBlue(int16_t blue)
{
    return (qwiic.writeRegisterInt16(_deviceAddress, TWIST_CONNECT_BLUE, blue) != QWIIC_ERROR_GENERIC);

}

//Get the connect value for each color
int16_t TWIST::getRedConnect()
{
    int16_t value;

    int rc;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_CONNECT_RED, (uint16_t*)&value);

    return (rc != QWIIC_ERROR_GENERIC ? value : 0);

}
int16_t TWIST::getGreenConnect()
{
    int16_t value;

    int rc;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_CONNECT_GREEN, (uint16_t*)&value);

    return (rc != QWIIC_ERROR_GENERIC ? value : 0);

}
int16_t TWIST::getBlueConnect()
{
    int16_t value;

    int rc;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_CONNECT_BLUE, (uint16_t*)&value);

    return (rc != QWIIC_ERROR_GENERIC ? value : 0);

}

//Get number of milliseconds that elapse between end of knob turning and interrupt firing
uint16_t TWIST::getIntTimeout()
{
    uint16_t value;

    int rc;

    rc = qwiic.readRegisterInt16(_deviceAddress, TWIST_TURN_INT_TIMEOUT, &value);

    return (rc != QWIIC_ERROR_GENERIC ? value : 0);

}

//Set number of milliseconds that elapse between end of knob turning and interrupt firing
bool TWIST::setIntTimeout(uint16_t timeout)
{

    int rc;
    rc = qwiic.writeRegisterInt16(_deviceAddress, TWIST_TURN_INT_TIMEOUT, timeout);

    return (rc != QWIIC_ERROR_GENERIC);

}
