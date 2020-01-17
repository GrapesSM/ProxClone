import time
timer=0
counter=0
while True:
    if (time.time() - timer) < 2.0000:
        continue

    if counter > 10:
        break
    counter = counter + 1
    timer = time.time()
    print(timer)