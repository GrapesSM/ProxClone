if (Serial.available() > 1)
{
  int val = Serial.parseInt();
  c.in = !c.in;
  c.flag = false;
}

if (c.in == false)
{
  if (c.flag == false)
  {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
  }
}
else
{
  if (c.flag == false)
  {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
  }
}

///////////////////////////////////////

if (c.powerSwitch.isSwitchOff())
{
  if (c.state == ON)
    c.flag = false;
  c.state = OFF;
  if (c.flag == false)
  {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
  }
}
else
{
  if (c.state == OFF)
    c.flag = false;
  c.state = ON;
  if (c.flag == false)
  {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
  }
}

////////////////////////////////////////////////

if (c.powerSwitch.isSwitchOff())
{
  if (c.state == ON)
    c.flag = false;
  c.state = OFF;
  if (c.flag == false)
  {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
  }
}

if (c.powerSwitch.isSwitchOn())
{
  // if (c.state == OFF) c.flag = false;
  c.state = ON;
  if (c.flag == false)
  {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
  }
}

////////////////////////////////////////

if (c.timer.start == 0)
{
  c.timer.start = millis();
}

if (c.timer.start > 0)
{
  if ((c.timer.current - c.timer.start) > 3000)
  {
    Serial.println("Period 1: Speak speech 1");
  }

  if ((c.timer.current - c.timer.start) > (3000 + 3000))
  {
    Serial.println("Period 2: Speak speech 2");
  }

  if ((c.timer.current - c.timer.start) > (3000 + 3000 + 3000))
  {
    Serial.println("Period 3: Speak speech 3");
  }
}