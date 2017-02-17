**proof of concept - don't rely on this code to work**

arduino hourglass. 


video of hourglass in action: https://www.youtube.com/watch?v=eZwY0fYrKhk
pic of how it's all hooked up: http://i.imgur.com/rAksFuN.jpg?1

the code drives two 8x8 LEDs through four shift-registers to simulate an hourglass.

the drop-speed of the sand can be regulated with a potentiometer. the orientation of the hourglass is derived from two tilt swiches (so one axis only).

short description of classes:

 * LedControl set/get 8x8 LEDs of one display
 * HourGlass simulates the sand dots
 * DelayLoop track time to executed something delayed without sleeping

