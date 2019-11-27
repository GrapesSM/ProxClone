from gtts import gTTS
import pygame
import os

import json    

with open('data.txt') as json_file:
    data = json.load(json_file)
    for t in data['texts']:
        print('Reading: ' + t['value'])
        tts = gTTS(t['value'])
        tts.save(t['filename'])
        i = 0
        while i < t['repeat-number']:
			print(' repeat: ' + str(i))
			pygame.mixer.init()
			pygame.mixer.music.load(t['filename'])
			pygame.mixer.music.set_volume(t['volume'])
			pygame.mixer.music.play()
			i+=1	
			while pygame.mixer.music.get_busy() == True:
				continue
			




