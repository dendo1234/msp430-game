# msp430 game
Developed for the MSP-EXP430F5529LP, this is an atempt to make a game for such low power microcontroler. I wrote all of the combrosome firmware code to interact with the peripherals, but got lazy at the gameplay code so there is no game here. Even thought, I had to implement partial rendering frame technics for this to even be possible so it might be interesting.

## results
trust me bro: worst case, this gets a frame time of 11,1 ms at 16 MiHz MCLK and 8 MiHz SMCLK. 11,1 is lower then 16,6 so we get 60 fps.

## how is the rendering done
### math
Before we begin, a bit of math: a 256x240 pixel display has 61440 pixels. The pixel format used is the RGB565, which uses 2 bytes per pixels or 122880 bytes per frame. If we want 60 fps, we gotta have a bandwidth of 7.372.800 bytes/second or 58.982.400 bits/seconds or 59 Mbit/s. For the unitiated on serial comunication, that means we would need at least a 59 MHz clock. A pretty fast clock. Maybe not impossible to achive with this MCU, but defenetly out of recomended specs. For reference, the main system clock MCLK, which is not the same clock as the serial comunication clock, althougth similar, can only be up to 25 MHz.

Also, this processor has only 8 kiB of RAM, which is 114 kB less than the necessary to have a frame buffer. pretty nice.

It is impossible not to make comparissons with the nintendo entrenteriment system (NES) which is a much older system with worse specs, and it renders 2D just fine. The thing is that the NES had dedicated hardware for rendering and also it used a color pallet feature making each pixel just 2 bits, not bytes. 

### tecnics
So, because of this bandwidth constrain, it became imperative to implement partial rendering, which is all about rendering just the pixels that changed between each frame, and since most pixels don't change, it can save a ton of bandwidth. It was achived using two main tecnincs: display scrolling and dirty squares.

On display scrolling: what happens when the camera move? Essentialy every pixel can change, so we have to re render every pixel. not very nice. But, to our advantage, the camera only moves horizontaly. That way, it is possible to make use of the ILI9341 scrolling feature. It will shifts how every memory pixel maps to every physical pixel, mimicing camera movement. Because this changes the maping, I had to spent quite some time thiking of an elaborate coordinate systems to acount to the scroll amount. The scrolling also added a "memory border" that moved as the camera moved - the last memory position could be maped to the midle of the screen, making pixels that are contiguous in physical space be not contiguous in memory space, requiring two draw calls as ILI9341 is not capable of going over the memory border to the first memory position. This also made rendering column by column much more convinient than row by row, as the memory border can only block when going horizontaly. This display scrolling feature is what is used for the tilemap renderer.

On dirty squares: as I don't have enought memory to keep track of every pixel on screen, intead, I keep track of every 8x8 square on screen. If a given sprite moves to a pixel that ocupy a new 8x8, it marks the entire square as needing to render. So the renderer, after every sprite is aconted for, renders and updates just the dirty 8x8 sections. This is how the sprites are rendered.

## how is music done
its just pwm signals, very fun. Two timers are used to make the square waves and another one is used to interrupt when it is time to get a new note. I am pretty bad at eletrical circuits so I just used 2 buzzers instead of figuring out if it is possible for just a single one to be driven by the two timers.

## how to run it
Wow, if someone actualy find this interesting to the point of wanting to run it, plese consider sending me an message, I would be delighted to hear what you are up to!

The project was developed using Code Composer Studio version 20.5.0.28__1.11.0

If you want everything working, you will need the following:
- MSP-EXP430F5529LP
- ILI9341 LCD 
- potentiometer joystick
- 2 buttons
- 2 passive buzzers
- \+ jumpers, resistors, transistors

### pins
- ILI9341
  + P3.0 MOSI
  + P3.1 MISO
  + P3.2 SPICLK
  + P2.6 CS
  + P8.1 RESET
  + P8.2 DC
- joystick
  + VCC 3.3 V
  + GND GND
  + P6.0 VRx
  + P6.1 VRy
- button 1 (connected to GND)
  + P1.2
- button 2 (connected to GND)
  + P1.3
- buzzer 1 (consider driving the current with an transistor)
  + P2.0
- buzzer 2 (consider driving the current with an transistor)
  + P2.4


