import processing.serial.*; //Library for communication between Mac and Arduiono Uno
import processing.sound.*;  //Library to play sound 

Serial myPort;      // Create object from Serial class
String val;         // Data received from the serial port
String portName;    //Name of Port where Board is connected to
int prevSound = 1;  //Prev Scene that played sound

SoundFile[] soundFiles = new SoundFile[6];    //All soundfile Objects
String[] filesName = new String[6];          //Names of the soundfiles

void setup()
{
  String portName = Serial.list()[8]; //Port 8 is connected via USB to the board
  println(portName);                  //Just to make sure the rigth port is connected
  myPort = new Serial(this, portName, 9600);  //start conversation via Port, create new serial object

  //save all available soundtracks in the data folder into soundFile objects to be ready for playback
  for (int i = 1; i <= 4; i++) {
    filesName[i] = "SoundScene" + i + ".wav";   //writes the name of the files into the filesName Array
    soundFiles[i] = new SoundFile(this, filesName[i], false);  //create soundFiles objects
    //from files that are found be the name in the filesName array
  }
}

void draw() {
  if ( myPort.available() > 0) // If data is available
  {
    val = trim(myPort.readStringUntil('\n'));         // read it and store it in val
  }

  for (int i = 1; i <= 4; i++) {              
    println(val);
    String currentScene = "activeScene_" + i; //creates String "activeScene_1", "activeScene_2", ... like it is transferred via serial

    //check if String from board is not null and if it is write "NoScene". To prevend program to stop because of error.
    try {
      val.equals(currentScene);
    }
    catch (NullPointerException e) {
      val = "NoScene";
    }

    //comapare String from Serial with the Scene Number from Array above (i)
    if (val.equals(currentScene)) {  //if true, program knows which scene active
      println("Scene Active");
      if (soundFiles[i].isPlaying() == false) {  //if sound from scene isn't currently playing
        println("Play");
        soundFiles[prevSound].stop();  //stop other sound from other scene
        soundFiles[i].play();  //start play sound from current scene
        prevSound = i;          //save which scene is the current scene
      }
    }
  }
}
