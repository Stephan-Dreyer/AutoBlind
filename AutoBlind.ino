#include <WiFi.h>
#include <WebServer.h>
#include "interface.h" // contains html css for webserver interface
#include <NTPClient.h>
#include <WiFiUdp.h>

// define pins
int motor_speed= 18 ;// pins for motor control
int control_1=17;
int control_2= 5;
int encoder =15;
int green= 33;
int red =32;
int blue =25;
int led_sw =3;
int blind_sw =21;
int up_sw =22;
int down_sw =23;
int led =2;

// Define Global Variables
//variables for timing
int period = 10000;
unsigned long time_now = 0;
int current_time;
int tracker = 100;
int alarm_hour = 9;
int alarm_min = 0;
//struct tm timeinfo; possibly redundant
String alarm_str = "9:00";

// counters to measure blind position
int required_ticks = 310;
int partial_ticks = floor((double)required_ticks * 0.1);
int ticks = required_ticks;
int tracker_destination = 0;

// boolean control variables
bool is_active = LOW;
bool up_toggle = LOW;
bool down_toggle = LOW;
bool up_toggle_full = LOW;
bool down_toggle_full = LOW;
bool fade_toggle = LOW;
bool alarm_toggle = HIGH;
bool morn_alarm_trig = LOW;
bool night_alarm_trig = LOW;
bool rave_toggle = LOW;
bool raid_toggle = LOW;
bool switch_used = LOW;

//html interface
String HTML = ControlCenter;

//Led states
uint8_t dim_factor = 3;
int R = 0;
int G = 0;
int B = 0;
int color_state[] = {0, 0, 0}; // for remembering the light color when they turn off during blind usage

//define wifi particulars
const char *ssid = "";
const char *password = "";
WebServer server(80);
WiFiClient client;


// initialise timers 
hw_timer_t *timing = NULL;
hw_timer_t *debounce_time = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE debounceMux = portMUX_INITIALIZER_UNLOCKED;
//ISR for  encoder switch
void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&timerMux);
  switch_click();
  is_active = HIGH;
  portEXIT_CRITICAL_ISR(&timerMux);
}
// ISR for debouncing switchbox switches
void IRAM_ATTR onDebounce()
{
  portENTER_CRITICAL_ISR(&debounceMux);
  debounce_sw();
  portEXIT_CRITICAL_ISR(&debounceMux);
}
// start debounce timer
void start_debounce()
{
  debounce_time = timerBegin(1, 80, true);
  timerAttachInterrupt(debounce_time, &onDebounce, true); 
  timerAlarmWrite(debounce_time, 10000, true);
  timerAlarmEnable(debounce_time);
}
// start timer for blind encoder switch debouncing
// needs to be started and stoped each time blind movese
void startTimer()
{
  timing = timerBegin(0, 80, true);             
  timerAttachInterrupt(timing, &onTimer, true); 
  timerAlarmWrite(timing, 1000, true);
  timerAlarmEnable(timing);
}
// stops the deboucning timer
void endTimer()
{
  timerEnd(timing);
  timing = NULL;
}

// Debounce routine for encoder switch isr.
uint8_t switch_click(void)
{
  static uint16_t btndbc = 0, lastb = 0;
  btndbc = (btndbc << 1) | digitalRead(15) | 0xe000;
  lastb = btndbc;
  if (btndbc == 0xf000)
  {
    if (up_toggle || up_toggle_full)
      ticks--;
    if (down_toggle || down_toggle_full)
      ticks++;
  }
}
// debounce routine for ISR 
void debounce_sw()
{
  static int up_counter = 0;
  static int down_counter = 0;
  static int prev_state = 0;
  int input = 0;
  if (digitalRead(up_sw) == LOW)
  {
    up_counter++;
  }
  else
    up_counter = 0;

  if (digitalRead(down_sw) == LOW)
  {
    down_counter++;
  }
  else
    down_counter = 0;

  if (up_counter >= 8 || down_counter >= 8)
    input = 1;

  if (prev_state != input)
  {
    prev_state = input;
    if (input)
    {
      switch_used = 1;// set switch as activated
    }
  }
}
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 36000, 60000);
void setup()
{
  //begin Serial for debugging
  Serial.begin(125200);
  // setup pinmodes
  pinMode(motor_speed, OUTPUT);
  pinMode(control_1, OUTPUT);
  pinMode(control_2, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(led_sw, INPUT);
  pinMode(blind_sw, INPUT);
  pinMode(up_sw, INPUT);
  pinMode(down_sw, INPUT);
  pinMode(led, OUTPUT);
  pinMode(encoder, INPUT);
  // pull pins reading switches high
  digitalWrite(led_sw, HIGH);
  digitalWrite(blind_sw, HIGH);
  digitalWrite(up_sw, HIGH);
  digitalWrite(down_sw, HIGH);
  // pull motor control pins high by default
  //digitalWrite(motor_speed,HIGH);
  digitalWrite(control_1, HIGH);
  digitalWrite(control_2, HIGH);
  // setup pwm to control motor speed, needs to be implemented in hardware first
  ledcSetup(4, 4000, 8);
  ledcAttachPin(motor_speed, 4);

  delay(10);
  //connect to wifi
  WifiConnect();
  // assign functions to handle to diffrent actions on the webserver interface
  server.on("/", handle_root);
  server.on("/down", handle_down);
  server.on("/up", handle_up);
  server.on("/down_FULL", handle_down_full);
  server.on("/up_FULL", handle_up_full);
  server.on("/reset0", handle_reset0);
  server.on("/reset100", handle_reset100);
  server.on("/indicator", handle_indicator);
  server.on("/alarm", handle_alarm);
  server.on("/off", handle_off);
  server.on("/white", handle_white);
  server.on("/fade", handle_fade);
  server.on("/rave", handle_rave);
  server.on("/raid", handle_raid);
  server.on("/red", handle_red);
  server.on("/green", handle_green);
  server.on("/blue", handle_blue);
  server.on("/orange", handle_orange);
  server.on("/cyan", handle_cyan);
  server.on("/magenta", handle_magenta);
  server.on("/yellow", handle_yellow);
  server.on("/brightup", handle_bright_up);
  server.on("/brightdown", handle_bright_down);
  server.on("/sleep", handle_sleep);
  server.on("/630", handle_630);
  server.on("/7", handle_7);
  server.on("/8", handle_8);
  server.on("/830", handle_830);
  server.on("/9", handle_9);
  server.begin();     //begin webserver
  timeClient.begin(); //begin ntp time
  start_debounce();   // begin timer debounce for switches
  delay(1000);
}
// configure bilnd to go down
void wind_down()
{
  digitalWrite(motor_speed, HIGH);
  digitalWrite(control_1, LOW);
  digitalWrite(control_2, HIGH);
  if (ticks >= 300)
    ledcWrite(4, 100);
}
// configure blind to go up
void wind_up()
{
  digitalWrite(motor_speed, HIGH);
  digitalWrite(control_1, HIGH);
  digitalWrite(control_2, LOW);
}
// stop the blind
void wind_stop()
{
  digitalWrite(motor_speed, LOW);
  digitalWrite(control_1, HIGH);
  digitalWrite(control_2, HIGH);
  //check if morning alarm was triggered and reset
  if (morn_alarm_trig)
  {
    morn_alarm_trig = LOW;
    alarm_min = 0;
    alarm_hour = 9;
    alarm_str = "9:00";
    //reset any previously stored colorstates
    for (int i = 0; i < 3; i++)
      color_state[i] = 0;
  }
  //turn lights back on after blind has stoped
  color(color_state[0], color_state[1], color_state[2], dim_factor);
  // reset night alarm and turn lights on
  if (night_alarm_trig)
  {
    night_alarm_trig = LOW;
    color(255, 255, 255, dim_factor);
  }
}
// wind blind up 10%
int wind_up_partial()
{
  // check if tracker not at destination
  if (tracker > 1 && tracker != tracker_destination)
  {
    tracker = floor(((double)ticks / (double)required_ticks) * 100); // calculate the position of the blind in percentage
    wind_up();
    return tracker;
  }

  else
  {
    // blind must be at desination so stop and reset
    wind_stop();
    up_toggle = LOW;
    endTimer();
 
    return tracker;
  }
}
// wind blind down 10%
int wind_down_partial()
{
  // check if not at destination
  if (tracker + 1 <= 100 && tracker != tracker_destination)
  {
    tracker = floor(((double)ticks / (double)required_ticks) * 100); // calculate the position of the blind in percentage
    wind_down();
    return tracker;
  }

  else
  {
    // blind must be at desination so stop and reset
    wind_stop();
    down_toggle = LOW;
    endTimer();
    return tracker;
  }
}
// wind blind down fully
int wind_down_full()
{
  // check if not at the bottom
  if (tracker + 1 <= 100)
  {
    Serial.println(tracker);
    wind_down();
    tracker = floor(((double)ticks / (double)required_ticks) * 100);// calculate position of the blind
    return tracker;
  }
  else
  {
    // blind must be at desination so stop and reset
    wind_stop();
    down_toggle_full = LOW;
    endTimer();
    return tracker;
  }
}
// wind blind up fully
int wind_up_full()
{
  //check if not at the top
  if (tracker + 1 > 1)
  {
    Serial.println(tracker);
    wind_up();
    tracker = floor(((double)ticks / (double)required_ticks) * 100);// calculate position of the blind
    return tracker;
  }
  else
  {
    // blind must be at desination so stop and reset
    wind_stop();
    up_toggle_full = LOW;
    endTimer();
  }
}
// sets the current colour of the lights
void color(int r, int g, int b, uint8_t dim_factor)
{
  // set global variables for use in other functions
  R = r;
  B = b;
  G = g;
  double precent = ((double)dim_factor / 3);
  //write red
  ledcSetup(1, 4000, 8);
  ledcAttachPin(red, 1);
  ledcWrite(1, floor(r * precent));

  //write green
  ledcSetup(2, 4000, 8);
  ledcAttachPin(green, 2);
  ledcWrite(2, floor(g * precent));

  //write blue
  ledcSetup(3, 4000, 8);
  ledcAttachPin(blue, 3);
  ledcWrite(3, floor(b * precent));
}
// changes the brighness of the lights
uint8_t dim(bool up)
{
  // check if dim factor is increasing or decreasing
  if (up)
    dim_factor += 1;
  else
    dim_factor -= 1;
  // cap values between 3 and 0
  if (dim_factor >= 3)
    dim_factor = 3;
  if (dim_factor <= 0)
    dim_factor = 0;
  color(R, G, B, dim_factor);

  return dim_factor;
}
// fades through a range of colours
void fade()
{
  // initialise static varaibles
  static int R = 255;
  static int G = 0;
  static int B = 0;
  static unsigned long prev_time = millis();
  unsigned long current_time = millis();
// update every 50ms- can be changed to speed up/ slow down
  if (current_time - prev_time >= 50)
  {
    // esentialy counts throught all the colours in a cycle
    if (R == 255 || (R + G == 255 && R != 0))
    {
      R -= 5;
      G += 5;
      prev_time = current_time;
      color(R, G, B, dim_factor);
    }

    if (G == 255 || (G + B == 255 && G != 0))
    {
      G -= 5;
      B += 5;
      color(R, G, B, dim_factor);
      prev_time = current_time;
    }

    if (B == 255 || (B + R == 255 && B != 0))
    {
      B -= 5;
      R += 5;
      color(R, G, B, dim_factor);
      prev_time = current_time;
    }
  }
}
 //connects to wifi using details provided
void WifiConnect()
{
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// turns lights off
void light_off()
{
  color(0, 0, 0, dim_factor);
  reset_lights();
}
// strobes lights very quickly
void rave()
{
  static unsigned long prev_time = millis();
  static bool turn_on = false;

  unsigned long time_now = millis();
  // toggles lights every 20 ms
  if ((time_now - prev_time) >= 20)
  {
    if (turn_on)
    {
      turn_on = 0;
      color(255, 255, 255, dim_factor);
    }
    else if (!turn_on)
    {
      turn_on = 1;
      color(0, 0, 0, dim_factor);
    }
    prev_time = time_now;
  }
}
//pulses lights red as if in a raid bunker
void raid()
{
  static int R = 0;
  static bool up = 1;
  static unsigned long prev_time = micros();
  // check if lights need to go up or down
  if (R == 255)
    up = 0;
  if (R == 0 && !up)
  {
    unsigned long time_now = micros();
    // define period to go stay turned off before fading up  again
    if ((time_now - prev_time) >= 750000)
    {
      up = true;
      prev_time = time_now;
    }
  }
  // fade lights up incrementally
  else if (up)
  {
    unsigned long time_now = micros();
    if ((time_now - prev_time) >= 1000)
    {
      R++;
      color(R, 0, 0, dim_factor);
      prev_time = time_now;
    }
  }
  //fade lights down incrementally
  else if (!up)
  {
    unsigned long time_now = micros();
    if ((time_now - prev_time) >= 1000)
    {
      R--;
      color(R, 0, 0, dim_factor);
      prev_time = time_now;
    }
  }
}
// cancel any curernt blind activity and reset to default state
void blind_cancel()
{
  if (up_toggle || down_toggle || up_toggle_full || down_toggle_full)
  {
    endTimer();
  }
  up_toggle = LOW;
  down_toggle = LOW;
  up_toggle_full = LOW;
  down_toggle_full = LOW;
}
// handles actions and logic for the hardware switch controls
void manual_switch()
{
  bool led_manual = 0;
  bool blind_manual = 0;
  // determine wheter switch is in blind or led mode other switch performs diffrent fucntions depending on the config
  if (digitalRead(led_sw) == LOW)
    led_manual = 1;
  else
    led_manual = 0;
  if (digitalRead(blind_sw) == LOW)
    blind_manual = 1;
  else
    blind_manual = 0;

  if (blind_manual)
  {

    for (int i = 0; i < 3; i++)
      color_state[i] = ledcRead(i + 1);// save current light config so it can be restored
    // move blind based on action of other swich
    if (digitalRead(up_sw) == LOW)
    {
      blind_cancel();
      up_toggle_full = HIGH;
      startTimer();
    }
    if (digitalRead(down_sw) == LOW)
    {
      blind_cancel();
      down_toggle_full = HIGH;
      startTimer();
    }
    switch_used = 0;// reset switch useage
  }
  else if (led_manual)
  {
    // cycle thorugh values in colour matrix
    static int i = 0;
    if (digitalRead(up_sw) == LOW)
      i++;
    if (digitalRead(down_sw) == LOW)
      i--;
    int red[8] = {0, 1, 0, 0, 1, 1, 0, 1};
    int blue[8] = {0, 0, 1, 0, 0, 1, 1, 1};
    int green[8] = {0, 0, 0, 1, 1, 0, 1, 1};
    if (i > 7)
      i = 0;
    if (i < 0)
      i = 7;
    color(red[i] * 255, green[i] * 255, blue[i] * 255, dim_factor);
    switch_used = 0;// reset switch usage
  }
  else
  {
    // actions if no mode is slected
    // cancle all actions
    if (digitalRead(up_sw) == LOW)
    {
      blind_cancel();
      light_off();
      wind_stop();
    }
    if (digitalRead(down_sw) == LOW)
    {
      // toggles alarm
      if (alarm_toggle)
      {
        alarm_str = "Zzz";
        alarm_toggle = LOW;
        digitalWrite(led, LOW);
      }
      else if (!alarm_toggle)
      {
        alarm_toggle = HIGH;
        alarm_str = "9:00";
        digitalWrite(led, HIGH);
      }
    }
    switch_used = 0;// reset switch usage
  }
}
// main loop
void loop()
{
  // refresh time every period miliseconds
  if (millis() >= time_now + period)
  {
    digitalWrite(led, LOW);
    time_now += period;
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());
    int hh = timeClient.getHours();
    int mm = timeClient.getMinutes();
    int ss = timeClient.getSeconds();
  // check for morning alarm
    if (hh == alarm_hour && mm == alarm_min && ss > 30 && alarm_toggle)
    {
      if (!up_toggle_full)
      {
        blind_cancel();
        up_toggle_full = HIGH;
        morn_alarm_trig = HIGH;
        startTimer();
      }
    }
// check for night alarm
    if (hh == 17 && mm == 30 && ss > 30)
    {
      alarm_toggle = HIGH;
      if (!down_toggle_full)
      {
        blind_cancel();
        down_toggle_full = HIGH;
        night_alarm_trig = HIGH;
        startTimer();
      }
    }
  }
  server.handleClient();
  // check if blind activity flag is active
  if (is_active)
  {
    light_off();// turn off light to avoid reducing performance of blind due to powerdraw constraints
    // check what function was activated
    if (up_toggle)
      tracker = wind_up_partial();
    if (down_toggle)
      tracker = wind_down_partial();
    if (up_toggle_full)
      tracker = wind_up_full();
    if (down_toggle_full)
      tracker = wind_down_full();
  }
  // handle lighting effects and switch box
  if (switch_used)
    manual_switch();
  if (rave_toggle)
    rave();
  if (raid_toggle)
    raid();
  if (fade_toggle)
    fade();
// reset blind activity flag
  is_active = LOW;
}
//web handles- essentialy these handle the requests of the webserver
// sends html page to new devices that connect
void handle_root()
{
  server.send(200, "text/html", HTML);
}
// pulls blind down 10%
void handle_down()
{
  blind_cancel();
  server.send(200, "text/html", HTML);
  for (int i = 0; i < 3; i++)
    color_state[i] = ledcRead(i + 1);
  light_off();
  tracker_destination = floor((((double)(ticks + partial_ticks)) / (double)(required_ticks)) * 100);
  startTimer();
  down_toggle = HIGH;
}
//pulls blind up 10%
void handle_up()
{
  blind_cancel();
  up_toggle = HIGH;
  for (int i = 0; i < 3; i++)
    color_state[i] = ledcRead(i + 1);
  light_off();
  tracker_destination = floor((((double)(ticks - partial_ticks)) / (double)(required_ticks)) * 100);
  startTimer();
  server.send(200, "text/html", HTML);
}
// pulls the blind fully up
void handle_up_full()
{
  blind_cancel();
  up_toggle_full = HIGH;
  for (int i = 0; i < 3; i++)
    color_state[i] = ledcRead(i + 1);
  light_off();
  startTimer();
  server.send(200, "text/html", HTML);
}
//resets the position the blind defines as its fully down position
void handle_reset0()
{
  ticks = 0;
  tracker = floor(((double)ticks / (double)required_ticks) * 100);
  server.send(200, "text/html", HTML);
}
//resets the position the blind defines as its fully down position
void handle_reset100()
{
  ticks = required_ticks;
  tracker = floor(((double)ticks / (double)required_ticks) * 100);
  server.send(200, "text/html", HTML);
}
//pulls blind fully down
void handle_down_full()
{
  blind_cancel();
  for (int i = 0; i < 3; i++)
    color_state[i] = ledcRead(i + 1);
  light_off();
  down_toggle_full = HIGH;
  startTimer();
  server.send(200, "text/html", HTML);
}
//sets current position of the blind to the webserver interface
void handle_indicator()
{
  server.send(200, "text/plane", (String(tracker) + "%"));
}
// sends the current alarm time to the webserver interface
void handle_alarm()
{
  server.send(200, "text/plane", alarm_str);
}
// resets lights completely to off
void handle_off()
{
  reset_lights();
  light_off();
  server.send(200, "text/html", HTML);
}
// turn lights white
void handle_white()
{
  reset_lights();
  color(255, 255, 255, dim_factor);
  server.send(200, "text/html", HTML);
}
// makes lights fade from color to color
void handle_fade()
{
  rave_toggle = LOW;
  raid_toggle = LOW;
  if (fade_toggle)
  {
    fade_toggle = LOW;
  }
  else
  {
    fade_toggle = HIGH;
    server.send(200, "text/html", HTML);
  }
}
// make lights strobe very quickly, just fast enought that it doesnt hurt your eyes
void handle_rave()
{
  raid_toggle = LOW;
  fade_toggle = LOW;
  if (rave_toggle)
  {
    rave_toggle = LOW;
  }
  else
  {
    rave_toggle = HIGH;
    server.send(200, "text/html", HTML);
  }
}
// enable raid mode for  lights pulse red (as if in a bunker under emergency power) i thought this would be cool but was kind of underwelming
void handle_raid()
{
  fade_toggle = LOW;
  rave_toggle = LOW;
  if (raid_toggle)
  {
    raid_toggle = LOW;
  }
  else
  {
    raid_toggle = HIGH;
    server.send(200, "text/html", HTML);
  }
}
// turn lights red
void handle_red()
{
  reset_lights();
  color(255, 0, 0, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights orange
void handle_orange()
{
  reset_lights();
  color(255, 128, 0, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights green
void handle_green()
{
  reset_lights();
  color(0, 255, 0, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights blue
void handle_blue()
{
  reset_lights();
  color(0, 0, 255, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights cyan
void handle_cyan()
{
  reset_lights();
  color(0, 255, 255, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights magenta
void handle_magenta()
{
  reset_lights();
  color(255, 0, 255, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights yellow
void handle_yellow()
{
  reset_lights();
  color(255, 255, 0, dim_factor);
  server.send(200, "text/html", HTML);
}
// turn lights brightness up
void handle_bright_up()
{
  dim_factor = dim(1);
  server.send(200, "text/html", HTML);
}
// turn lights brightness down
void handle_bright_down()
{
  dim_factor = dim(0);
  server.send(200, "text/html", HTML);
}
// disable alarm for one morning if sleepin is desired
void handle_sleep()
{
  alarm_toggle = LOW;
  alarm_str = "Zzz";
  server.send(200, "text/html", HTML);
}
// set alarm for 6:30 am
void handle_630()
{
  alarm_hour = 6;
  alarm_min = 30;
  alarm_str = "6:30";
  server.send(200, "text/html", HTML);
}
// set alarm for 7am
void handle_7()
{
  alarm_hour = 7;
  alarm_min = 0;
  alarm_str = "7:00";
  server.send(200, "text/html", HTML);
}
//set alarm for 8am
void handle_8()
{
  alarm_hour = 8;
  alarm_min = 0;
  alarm_str = "8:00";
  server.send(200, "text/html", HTML);
}
//set alarm for 8:30 am
void handle_830()
{
  alarm_hour = 8;
  alarm_min = 30;
  alarm_str = "8:30";
  server.send(200, "text/html", HTML);
}
//set alarm for 9am
void handle_9()
{
  alarm_hour = 9;
  alarm_min = 0;
  alarm_str = "9:00";
  server.send(200, "text/html", HTML);
}
// reset all light modes
void reset_lights()
{
  fade_toggle = LOW;
  rave_toggle = LOW;
  raid_toggle = LOW;
}
