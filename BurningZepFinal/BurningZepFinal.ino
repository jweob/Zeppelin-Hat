
/*******************************************************************************
 Burning Zeppelin Costume Control Electronics
 --------------------------------------------
 
 Controls a tricolour LED, a relay and a servo motor
 When the costume is in the unexploded state tricolour LED stays blue, servo motor is in up position and relay is off
 When the board receives a HIGH signal on SWITCH_PIN it will move servo motor to down position, turn on relay and cause the tricolour
 LED to flicker randomly as it moves between yellow and red and back again (flame effect)
 Written by jweob Nov 2014, based on the tricolour LED sketch in the Sparkfun Inventor's Kit and on the blink without delay example at http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 More information on the project at http://jweoblog.com/?p=381

 This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 
 Unported License (CC BY-SA 3.0) http://creativecommons.org/licenses/by-sa/3.0/
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
*******************************************************************************/


#include <Servo.h>  // servo library
Servo servo1;  // servo control object

// Set up constants
const int RED_PIN = 6; //For tricolour LED
const int GREEN_PIN = 5; //For tricolour LED
const int BLUE_PIN = 3; //For tricolour LED
const int SWITCH_PIN = 2; //Input - when it is high the board goes into explode mode
const int RELAY_PIN = 7; //For relay
const unsigned long changeFlicker = 1000; //How often board changes the frequency of the LED flickering in ms
unsigned long previousMillis = 0; //stores last update time for LED
int unsigned long flicker = 5; //Initial value of flicker in ms
bool rising = true; //Used to cycle between red and yellow on tricolour LED
int i = 0; //For loops
int explode = LOW; //Variable that says whether in explode mode or not

void setup()
{
  // Configure pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  servo1.attach(9);
  digitalWrite(RELAY_PIN, LOW);
 }


void loop()
{

  // millis() is number of milliseconds since uno switched on
  unsigned long currentMillis = millis();

  // if the SWITCH_PIN is high go into explode mode
  explode = digitalRead(SWITCH_PIN);

  // If in explode mode then perform explode actions
  if (explode == HIGH) {
    //To avoid continuously writing to servo only update it every 100 ms
    if(currentMillis % 100 == 0){
      servo1.write(130);
      digitalWrite(RELAY_PIN, HIGH);
    }
    
    //Changes flickering frequency
    if (currentMillis - previousMillis > changeFlicker) {
      flicker = random(20, 1000);
    }
    
    //Cycles yellow value for tricolour LED between 0 and 255
    if (currentMillis - previousMillis > flicker) {
      previousMillis = currentMillis;
      if (rising == true) {
        if (i == 255) {
          rising = false;
        }
        else {
          i++;
        }
      }
      else {
        if (i == 0) {
          rising = true;
        }
        else {
          i--;
        }
      }
    }

    //Flickers the LED to make it more flamelike.
    if (i % flicker == 0) {
      showRGB(255, i, 0);
    }
    else {
      showRGB(100, 0, 0);
    }
  }
  else{
    //Actions if not in explode mode. make LED blue
    showRGB(0,0,255);
    previousMillis = currentMillis;
    if(currentMillis % 100 == 0){
      servo1.write(40);
      digitalWrite(RELAY_PIN, LOW);
    }
    
  }
  






}


void showRGB(int redIntensity, int greenIntensity, int blueIntensity)
{
  analogWrite(RED_PIN, redIntensity);
  analogWrite(GREEN_PIN, greenIntensity);
  analogWrite(BLUE_PIN, blueIntensity);

}

