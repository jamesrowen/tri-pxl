import processing.serial.*;
import java.io.*;
import java.util.Arrays;

boolean doRead = true;
Serial serial;

String filename = "/Users/jamers/Desktop/projects/tri-pxl/programs/program.txt";
RandomAccessFile file = null;
String text = null;

void setup(){
  serial = new Serial(this, Serial.list()[2], 9600);
  serial.bufferUntil('\n');
  
  try {
    file = new RandomAccessFile(new File(filename), "r");
  } catch(FileNotFoundException e) {
    e.printStackTrace();
  }
}

void draw() {
  if (doRead) {
    doRead = false;
    text = getLine(file);
    // end of file - loop back to start
    if (text == null) {
      try {
        file.seek(0);
      } catch (IOException e) {
        e.printStackTrace();
      }
      text = getLine(file);
    }
  }
  
  if (serial.available() > 0) {
    String msg = serial.readStringUntil('\n');
    if (msg == null)
      return;
      
    if (msg.equals("get\r\n")) {
      serial.write(text + "\n");
      doRead = true;
    }
    else {
      print("arduino: ");
      print(msg);
    }
  }
}

String getLine(RandomAccessFile file) {
  String result = null;
  try {
    result = file.readLine();
    // skip comments
    while (result != null && result.charAt(0) == '/' && result.charAt(1) == '/') {
      result = file.readLine();
    }
  } catch(IOException e) {
    e.printStackTrace();
  }
  return result;
}