import sensor, image, time, pyb,math
uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
tmp = ""
sensor.reset() # Initialize the camera sensor.
sensor.set_hmirror(True)
sensor.set_vflip(True)
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
clock = time.clock() # Tracks FPS.

enable_pin = pyb.Pin("P0", pyb.Pin.IN)
mode_pin = pyb.Pin("P1", pyb.Pin.IN)
#p.value() # Returns 0 or 1.
def degrees(radians):
    return (180 * radians) / math.pi
def trace_apriltag():
    clock.tick()
    img = sensor.snapshot()
    tags = img.find_apriltags()
    for tag in tags:
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        #img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        print_args = (tag.x_translation(), tag.y_translation(), tag.z_translation(), \
            degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation()))
        #print("Tx: %f, Ty %f, Tz %f, Rx %f, Ry %f, Rz %f" % print_args)
        #print("%.2f" %degrees(tag.y_rotation()))
        #print(tag)
        y_deg = degrees(tag.y_rotation())
        if y_deg > 180:
            y_deg = 360 - y_deg
        pixel_len = (tag.w()+tag.h())/2
        K = 47 # the pixel len measured in 10cm
        # print('pixel_len =  ', pixel_len)
        distance = K/pixel_len *10
        print('distance = ', distance)
        if distance < 19:
            uart.write("/stop/run \n".encode())
            if distance > 17:
                uart.write("/goStraight/run 50 \n".encode())
            elif distance < 14:
                uart.write("/goStraight/run -50 \n".encode())

        if distance > 16:
            if tag.cx()>100:
                print('turn right')
                uart.write("/turn/run 50 -0.3 \n".encode())
            elif tag.cx() <80:
                print('turn left')
                uart.write("/turn/run 50 0.3 \n".encode())
            else:
                print('go straight')
                uart.write("/goStraight/run 50 \n".encode())
        elif distance < 13:
                print('go back')
                uart.write("/goStraight/run -50 \n".encode())
        else:
            if tag.cx()>100:
                print('turn right')
                uart.write("/turn/run 50 -0.3 \n".encode())
            elif tag.cx() <80:
                print('turn left')
                uart.write("/turn/run 50 0.3 \n".encode())
        if distance < 16 and distance > 14:
                print('stop')
                command = '/enable_pin/write 0 \n'
                uart.write(command.encode())
                time.sleep(1)
                command = '/mode_pin/write 1 \n'
                uart.write(command.encode())
                time.sleep(1)
                print('parking......')
                uart.write("/goParking/run 40 60 1 \n".encode())
                time.sleep(5)
                print('done')
    if not tags:
        #print('no tag')
        uart.write("/stop/run \n".encode())
    #print(clock.fps())

    #print(clock.fps())
green_threshold   = (6,46,-10,9,-27,-6)
size_threshold = 2000
def find_max(blobs):
    max_size=0
    max_blob = blobs[0]
    for blob in blobs:
        y_bot = blob.y() + blob.h()
        if blob[2]*blob[3] > max_size :
            if y_bot > 50:
                max_blob=blob
                max_size = blob[2]*blob[3]
    return max_blob
def trace_line():
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.

    blobs = img.find_blobs([green_threshold])
    if blobs:
        max_blob = find_max(blobs)
        img.draw_rectangle(max_blob[0:4]) # rect
        img.draw_cross(max_blob[5], max_blob[6]) # cx, cy
        #print(max_blob)
        mean_x = max_blob.x() + max_blob.w()/2
        diff = mean_x - 80
        turn_thershold = 40
        if abs(diff) >turn_thershold:
            if diff < 0:
                uart.write("/turn/run 50 0.3 \n".encode())
                print('turn left')
            else:
                uart.write("/turn/run 50 -0.3 \n".encode())
                print('turn right')
        else:
            uart.write("/goStraight/run 100 \n".encode())
            print('go straight')
        #time.sleep(0.3)
        #print(mean_x)
        #uart.write(("%.2f \r\n" %mean_x).encode())
    else:
        uart.write("/stop/run \n".encode())

while(True):
    print(mode_pin.value(), enable_pin.value())
    #if enable_pin.value():
        #if mode_pin.value():
            #while enable_pin.value() and mode_pin.value():
                #trace_apriltag()
                #print('tracing_apriltag mode')
        #else:
            #while enable_pin.value() and not mode_pin.value():
                #trace_line()
                #print('tracing line mode')
    #else:
        #if not mode_pin.value():
            #print('Idle mode')
        #else:

    while enable_pin.value() and mode_pin.value():
        trace_apriltag()
        print(mode_pin.value(), enable_pin.value())
        #time.sleep(0.5)
    #print('parking......')
    #uart.write("/goParking/run 0 30 30 \n".encode())
    #time.sleep(5)
    #print('done')
    #command = '\n\r/enable_pin/write 1 \n'
    #uart.write(command.encode())
    #time.sleep(0.5)
    #command = '\n\r/mode_pin/write 0 \n'
    #uart.write(command.encode())
    time.sleep(0.5)
    while not enable_pin.value() and mode_pin.value():
    #while 1:
        trace_line()
        print('lineininin')
    time.sleep(1)
    print('end')
