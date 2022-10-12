# ArduinoIntruderSystem

- TinkerCad

##### Arduino coursework 2 - Intruder Detection

##### Report of Development

#### Description

My tool is a intruder alarm system which will detect motion within a range that is specified by the user. The Tool is designed to be intuitive and has a light system which turns on red only when its dark outside in order to make it clear to the intruder that they should not be there. The alarm can be toggled on and off as required. The idea is that it can be used in multiple environments to protect objects or even entire entrances to locations you want to secure.

#### Step 1 - Register the distance of the intruder

For this section I used an ultrasonic distance sensor and sent out a wave to the object and then calculated the distance using the time travelled by the sound wave back and forth from the echo and trigger pins, this is used via PulseIn with the echo pin which reads the incoming wave that was sent out when trigger pin was set to High. This was then multiplied by the 0.01723 in order to calculate the distance in cm. This will then be passed to the alarm checking system to see whether to turn the alarm on. Pros of this method is that it can quickly locate the distance of the person present to be able register the alarm. Cons of this method is that the distance is not completely accurate and takes a little time to calculate which can slightly throttle the rest of the system. 

#### Step 2 - Light detection sensor For toggling Lights

This is just a quick addition that toggles the lights that signal on and off of the alarm as during the daytime, they would not would be required. This will just read the value from the LDR circuit and map this value from 1-310 to 1-100 which is more intuitive to work with for a human for when its displayed on the screen and when working with it in the code. For the actual lights toggling on and off, when light goes below 5% it will toggle lights on and display a short message to signal the lights turning on and vice versa for above 5%. This value will be used within the alarm function when determining whether to flash the lights. Pros is that its automatic and quick to update with no issues, Cons is that this is not customisable by the user which I would add in future version as this would enable the ability to specify whether they want this feature of course. This light value is updated within each Loop()

#### Step 3 - Alarm triggering function

Within Loop() this will consistently be checking that the distance calculated by the system  is lower than the specified distance threshold, and if so, run the alarm if it has been set to be activated which can be set by the user using the RHS pushbutton. If the alarm is active , the screen will display intruder in all caps , the piezo will create a loud fluctuating sound to imitate a proper alarm and if toggle lights is true the red light can turn on. This will loop endlessly until the user presses the disarm button (In practical use the screen and button will be seperate from the motion detection so the intruder cant just disarm it). Pros are that the alarm is very loud and clear red light signals the danger. Cons is the slight delay between registering the distance below a certain threshold and the red light and piezo cannot execute at the same time therefore the ability to flash the red lights does not work as nicely and the light isn't bright enough therefore just triggering a red alarm seems more suitable in design view.

#### Step 4 - Piezo Alarm Noise

For the actual alarm noise i decided to research about to use sine wave in order to create a sort of rising and falling tone for the piezo which creates a more realistic alarm system that could be used to generate a much clearer loud sound than just a monotone alarm.

#### Step 5 - Toggle Alarm On/off

I attached an interrupt to a pushbutton which when pressed will switch the value of the alarm. This is a disarming system however can be used to turn the alarm on and off whenever the user wishes. Pro is that this can be triggered no what section of the sequence via an interrupt sequence, Con is it can stutter the screen a bit however its very slight issue that cannot really affect the overall use of the system.

#### Step 6 - Shift distance threshold

A second interrupt I used will essentially increment the value by 20 each time and when it reaches 300cm it will loop back to start at 20cm, this will be quickly updated as the Loop() updates the display values. An error i noticed was after 100cm it starts incrementing 22cm which I could not figure out why however it should not really be a big deal overall. 

#### Step 7 - Update Display

Within the loop the screen is used to display data of : If the alarm is active? What is the light percentage ? The current Distance threshold? Whether lights are toggled on?. This is concatenated into 2 seperate lines which are then passed to my scroll line function which will on each loop cycle through the char array in order to display all the data which cannot fit within the standard 16 slots specified by the LCD array. The data is dynamically update quickly as its within each loop so a good pro is that the distance/ light/status' are all shown quickly as the lines are scrolled. A clear con is that you have to scroll anyway , I was considering changing it so that the names are shortened however for usability reasons i decided to keep these values proper lengths. This display  can of course be turned off with the potentiometer. 

##### Circuit

<img src="images/image-20201207181132677.png" alt="image-20201207181132677" style="zoom:50%;" />

##### Schematic

<img src="images/image-20201207192929714.png" alt="image-20201207192929714" style="zoom:67%;" />
