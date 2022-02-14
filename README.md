<h1>AutoBlind</h1>

<h3>Automated IKEA: FRIDANS blind, using ESP32 webserver</h3>
<p>This is a roller style blind and was automated to be controlled through a webserver hosted on an ESP32. It's programmed to automatically open and close at set times of day & the opeing time can be set within the hosted interface to act as a alarm in the morning. The position of the blind is measured using a limit switch based  encoding system. As the the motor rotates, the gear attached clicks the switch, thus this can be used to measure how much the motor has rotated, and by extention the blind, has moved. All aspects of the Blind can be controlled through the web interface from any device connected to the same wifi network. A set of hardware controls were also implemented in the form of a set of a switch pannel but the funcitonality is more limited. A RGB LED Driver was also developed to control an RGB LED strip, with 8 bit colour resolution,to provide aditional lighting. These LEDs can be contolled using both the software and hardware controls.</p>

![gif](https://user-images.githubusercontent.com/63902709/153567206-497ebd32-34aa-4b36-82b5-bed5925055f1.gif)

  
 <h2>Gallery</h2>
  <h4>Switch Pannel</h4>
  
![Switch Pannel](https://user-images.githubusercontent.com/63902709/153574412-1ebd0453-ae7a-40d1-9f71-58f43c55932d.jpg)
  <h4>3D printed Gear assembly and Mounting Bracket</h4>
  
![Gear assembly](https://user-images.githubusercontent.com/63902709/153574429-0ef62fb2-d96e-4c0e-be0a-a9071d46ffed.jpg)
 <h4>Rotary Encoder System</h4>
 
![Encoder System](https://user-images.githubusercontent.com/63902709/153575889-3329070a-2dd6-481e-ad99-ed70bb910bb9.jpg)  
  <h3>Components used:</h3>
<ul>
<li>1x ESP32 Developer Board
<li>Various Resistors and Capacitors (Pulldown and filtering, values not critical)
<li>1x LM7805 Linear 5V regulator
<li>1x L298D Dual H-bridge
<li>Vairous Headers and Terminals
<li>3x IRLZ34N N-channel MOSFETS
<li>1x DPDT Momentary toggle switch
<li>1x SPDT Toggle Switch
<li>1x RGB LED 
<li>1x Momentary Switch
<li>1x YG2732 Gear motor
 <li>3D printed gears and mounting brackets
</ul>
 
  
 <h4>To Do:<h4>
  <ul>
  <li>Store Blind position In EEPROM to avoid posiiton loss when device is reset 
    <li>Investigate and develop better encoding system
      <li> implement neccesary connections in Hardware to allow motor speed control
        <li> Upgrade motor to brushless to reduce noise (would need to replace H-bridge with ESC)
  </ul>
  
  

