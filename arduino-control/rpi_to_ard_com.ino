#include <Servo.h>

Servo jointA, jointB, jointC;
int jointAPin = 3;
int jointBPin = 5;
int jointCPin = 6;
int jogPin = 8;
int jogPlusPin = 9;
int jogMinusPin = 10;
int jogSpeedPin = A0;
bool jogModeEnabled = false;

//takes an array of ints and writes them
//to the servo motors
void command_joints(int positions[3]) {
  jointA.write(positions[0]);
  jointB.write(positions[1]);
  jointC.write(positions[2]);
}

//reentrant function to read commands
//make *read...
int read_joint_commands() {
  static int commands[3];
  for(int i = 0; i < 3; i++) {
    commands[i] = Serial.parseInt();
  }
  return commands;
}

void move_to_setpoint() {
  if(Serial.available()) {
    int positions = read_joint_commands();
    command_joints(positions);
  }
}

void disable_jog_mode() {
  if (jogModeEnabled == true) {
    while(Serial.available()) {
      int positions = read_joint_commands();
    }
    jogModeEnabled = false;
  }
}

void enable_jog_mode() {
  if (jogModeEnabled == false) {
    jogModeEnabled = true;
  }
}

bool jog_pin_enabled() {
  if (digitalRead(jogPin) == HIGH) {
    return false;
  } else {
    return true;
  }
}

int read_jog_speed() {
  int jogSpeed = 1024 - analogRead(jogSpeedPin);
  return jogSpeed/25;
}

//make generic jog joint function
void jog_joint(Servo joint) {
  int current_position = joint.read();
  if (digitalRead(jogPlusPin) == LOW) {
    current_position += 1;
  } else if (digitalRead(jogMinusPin) == LOW) {
    current_position -= 1;
  }
  joint.write(current_position);
  delay(read_jog_speed());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  jointA.attach(jointAPin);
  jointB.attach(jointBPin);
  jointC.attach(jointCPin);
  pinMode(jogPin, INPUT_PULLUP);
  pinMode(jogPlusPin, INPUT_PULLUP);
  pinMode(jogMinusPin, INPUT_PULLUP);
}

void loop() {
  if (!jog_pin_enabled()) {
    disable_jog_mode();
    move_to_setpoint();
  } else {
    enable_jog_mode();
    //read jog speed potentiometer
    jog_joint(jointA);
    jog_joint(jointB);
    jog_joint(jointC);
  }
}
