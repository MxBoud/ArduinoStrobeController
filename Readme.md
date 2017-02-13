## Arduino Strobe controller
Arduino Strobe controller is a project to build a low cost LED strobe light, mainly for cool physics demonstrations. It uses two Arduino [one master (Arduino Uno with a [LCD Keypad Shield][1]) and one slave (nano)]. The master arduino is used to define the pulse frequency and its duty cycle. It is manually controlled from the LCD Keypad Shield. These settings are then sent to the slave Arduino using serial communication (RX and TX pins). 

The slave Arduino is using a timmer interrupt from the Timer1 library to generate a pulse of adjustable pulse in digital pin 9 (PWM). The current circuit is using two Arduinos sinces I haven't figured a way to setup a timer interupt without messing up with the LCD on the main arduino. If ever somebody know how to do that, I would really apreciate to learn from you!

### Installation
Upload the [LCD_Strob.ino][master] script to the master Arduino and the [PWM.ino][slave] stript to the slave Arduino. 
### Circuit
![Schematics][Schematics]
### First working prototype
For now, I'm only driving a LED, as seen in the picture... but it works! :)
![First prototype][firstProto]
### Library requirement
[Timer1][2]
### Specific hardware requirement 
[LCD Keypad Shield][1]

[1]: https://www.aliexpress.com/wholesale?ltype=wholesale&d=y&origin=y&isViewCP=y&catId=400401&initiative_id=AS_20170212163641&SearchText=lcd+keypad+shield&blanktest=0&tc=af
[2]: http://playground.arduino.cc/Code/Timer1
[Schematics]: /Ressources/Schematics/Schematics.png
[firstProto]: /Ressources/Schematics/firstProto.png
[master]: /LCD_Strob/LCD_Strob.ino
[slave]: /PWM/PWM.ino
