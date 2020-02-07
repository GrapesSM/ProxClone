if (Serial.available() > 1) {
    int val = Serial.parseInt();
    c.in = !c.in;
    c.flag = false;
}

if (c.in == false) {
    if (c.flag == false) {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
    }
} 
else {
    if (c.flag == false) {
    c.flag = true;
    c.speaker.addToPlay(SOUND_POWER_UP);
    }
}