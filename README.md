<h1>AutoBlind</h1>

<h3>Automated IKEA: FRIDANS blind, using ESP32 32 webserver</h3>
<p>Blind was Automated to be controlled through a Webserver hosted on an ESP32. Its programmed to  automatically open and close at set times of day the opeing time can be set within the hosted interface to act as a alarm in the morning. All aspects of the Blind can be controlled through the web interface from any device connected to the same wifi network.A set of hardware controls were also implemented in the form of a set of a swithc pannel but the funcitonlaity is more limited. An RGB LED Driver was also developed to control an RGB LED strip to provide aditional lighting. these LEDs can be contolled using both the software and hardware controls.</p>

![ezgif com-gif-maker](https://user-images.githubusercontent.com/63902709/153567206-497ebd32-34aa-4b36-82b5-bed5925055f1.gif)
Components used:
<ul>
<li>1x ESP32 Developer Board
<li>Various Resistors and Capacitors (Pulldown and filtering, values not critical)
<li>1x LM7805 Linear 5V regulator
<li>1x L298D Dual H-bridge
<li>Vairous Headers and Terminals
<li>3x IRLZ34N N-channel MOSFETS
<li>1x DPDT Momentary toggle switch
<li>1x SPDT Toggle Switch
<li>1x RGB led Diode
<l>1x Momentary Switch
<li>1x YG2732 Gear motor
 <li>3D printed gears and mounting brackets
</ul>
  
 To Do:
  <ul>
  <li>Store Blind position In EEPROM to avoid posiiton loss when device is reset 
    <li>Investigate and develop better encoding system
      <li> implement neccesary connections in Hardware to allow motor speed control
        <li> Upgrade motor to brushless to reduce noise (would need to replace Hbridge with ECS)
  </ul>
  
  

