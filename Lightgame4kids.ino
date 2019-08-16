#include "stdlib.h"
#include "pitches.h"
#include "StopWatch.h"

#define MAX_PATTERN_SIZE 20

// initialize a stopwatch
StopWatch MySW;

// initialize the timers for each color
// the timers will keep the sum of the time it took
// to press correctly the given color
int yellow_timer=0;
int red_timer=0;
int green_timer=0;

// the rgb led pins
int buzzer = 7;


int divisionResult;

// the rgb led pins
int pin_rgb_red = 11;
int pin_rgb_green = 10;
int pin_rgb_blue = 9;

// the rgb led pins
int button_red = 5;
int button_green = 6;
int button_yellow = 4;

// stage_game values :
//		0 - wait for serial command to begin the game
//		1 - start the game
//		2 - ending the game
// at the ending stage, it returns to 0 value, waiting again for serial command
int stage_game = 0;

// initialize the string we will use
// for the pattern
String pattern;

// reaction_times[] array will keep the reaction time
// for each color.
// if the wrong color is pressed it will have a 0 value
int reaction_times[MAX_PATTERN_SIZE];

// initialize the current turn to 0
// the turn will be used as index in the pattern() array
// to display the correct color each time
int turn=0;

// the pace of music
int noteDuration = 115; 

//5 notes
int melody_correct[] = {NOTE_AS6,NOTE_F7,NOTE_AS7,NOTE_DS8,NOTE_D8};

//5 notes
int melody_fault[] = {NOTE_E6,NOTE_A5,NOTE_AS5,NOTE_E5,NOTE_F5};

//8 notes
int melody_tension[] = {NOTE_G6,NOTE_DS7,NOTE_B6,NOTE_G6,NOTE_DS6,NOTE_C6,NOTE_A5,NOTE_G5};

//64 notes
int melody_outro[] = { 
  NOTE_G5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_G4,NO_SOUND,NOTE_G5,NO_SOUND,
  NOTE_D5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_D4,NO_SOUND,NOTE_D5,NO_SOUND,
  NOTE_E5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_E4,NO_SOUND,NOTE_E5,NO_SOUND,
  NOTE_B4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_B3,NO_SOUND,NOTE_B4,NO_SOUND,
  NOTE_G5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C4,NO_SOUND,NOTE_G5,NO_SOUND,
  NOTE_G4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_G3,NO_SOUND,NOTE_G4,NO_SOUND,
  NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C4,NO_SOUND,NOTE_C5,NO_SOUND,
  NOTE_D5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_D4,NO_SOUND,NOTE_D5,NO_SOUND
};

// 320 notes
int melody_intro[] = {
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6,NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6,
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6, NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,
  
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,
  
  NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,
  NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,
  NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,
  NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,
  
  NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,
  NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,
  NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,
  NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,
  NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,
  NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,
  NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,
  NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,
  
  NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,
  NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,
  
};
  

void setup()
{
	
	// declare buzzer pin as output
	pinMode(buzzer, OUTPUT);
	
	// declare the rgb pins as outputs
	pinMode(pin_rgb_red, OUTPUT);
	pinMode(pin_rgb_blue, OUTPUT);
	pinMode(pin_rgb_green, OUTPUT);
	
	// declare the buttons pins as inputs
	pinMode(button_red, INPUT);
	pinMode(button_green, INPUT);
	pinMode(button_yellow, INPUT);
	
	//initialize reaction_times array
	for(int i; i<20; i++){
		reaction_times[i]=0;
	}
	
	//turn on serial communication at 9600
	Serial.begin(9600);
}

void loop()
{
	// stage_game has initial value 0
	// at this stage it will get commands and data
	// through the serial
	//
	// when the command to start the game is given
	// it will get the value of 1 and ignore the serial
	// communication untill the game ends or reset button is pressed
	//
	// when the game ends it will take the value of 2
	if(stage_game == 0){

		// When there is something sent by serial
		// reach each character and save it in c
		// add c to the string readString
		//
		// at the end of the command, readString will contain the
		// given command as a string
		String readString;
		while (Serial.available()) {
	    	delay(3); 
	    	if (Serial.available() >0) {
	      		char c = Serial.read();  
	      		readString += c;
	    	} 
	  	}
	  
	    // compare the command give through serial
	    // to the available commands
	    //
	    // if it doesn't equal any of them
	    // treat it as raw data
	  	if ( readString.length() > 0) {
        	    	if(readString == "start"){
        	    		if(pattern.length() > 0){
        	    			// start game, play the intro music
        	    			stage_game=1;
        	    			play_music(5); 
        	    		}
        	    	}else{
        	    		// do something with the raw data 
            			pattern=readString; 
        	    	}
	  	}
	  	
	  	
	}else if(stage_game == 1){
		
		// small delay between rounds ( 2 sec)
		delay(2000);
		
		// button_pressed values :
		// 0 - no button is pressed
		// 1 - button is pressed
		int button_pressed = 0;
		
		// play a little sound before the led lights
		play_music(3);
		
		// display the correct color on the rgb led
		// based on the pattern given by the doctor
		// right after that, the timer begins
		if(pattern[turn]=='y'){						// yellow
			digitalWrite(pin_rgb_blue, LOW);
		    analogWrite(pin_rgb_red, 180);
		    digitalWrite(pin_rgb_green, HIGH);
		}else if(pattern[turn]=='r'){				// red
			digitalWrite(pin_rgb_blue, LOW);
		    digitalWrite(pin_rgb_red, HIGH);
		    digitalWrite(pin_rgb_green, LOW);
		}else if(pattern[turn]=='g'){				// green
			digitalWrite(pin_rgb_blue, LOW);
		    digitalWrite(pin_rgb_red, LOW);
		    digitalWrite(pin_rgb_green, HIGH);
		}
		MySW.reset();
	 	MySW.start();
		
		// wait for a button to be pressed
		//
		// when a button is pressed exit the loop
		// play the correct music
		// increase the correct counter if necessary
		// and add the reaction time for that color to it's sum
		// then, stop the timer
		while( button_pressed == 0){
			if( digitalRead(button_red)== HIGH){
				button_pressed=1;
				if(pattern[turn]=='r'){
					play_music(1);
					reaction_times[turn]=MySW.elapsed();
				}else{
					play_music(2);
				}
			}
			if( digitalRead(button_green)== HIGH){
				button_pressed=1;
				if(pattern[turn]=='g'){
					play_music(1);
					reaction_times[turn]=MySW.elapsed();
				}else{
					play_music(2);
				}
			}
			if( digitalRead(button_yellow)== HIGH){
				button_pressed=1;
				if(pattern[turn]=='y'){
					play_music(1);
					reaction_times[turn]=MySW.elapsed();
				}else{
					play_music(2);
				}
			}
		}
		MySW.stop();
		
		// turn will increase by 1 to point the next index of the pattern
		// if it's equal to the pattern's length, we have display all the colors
		turn++;
		if(turn == pattern.length()){
			// game over stage
			stage_game = 2;
		}
		
	}else{
		delay(1000);
		// send back the statistics
		for(int i =0; i<pattern.length(); i++){
			Serial.write(reaction_times[i]);
                        divisionResult = reaction_times[i] / 255;
                        Serial.write(divisionResult);
                        reaction_times[i] = 0;
		}
                
		// game is over, play the ending music
		// and wait again for serial commands
		play_music(4);
		stage_game = 0;
                turn=0;
                pattern="";
        }
}


// function play_music will play the music based on the value
// of menu_music
// menu_music values :
// 1 - correct button pressed
// 2 - wrong button pressed
// 3 - into for each turn just before the rgb led lights
// 4 - game over
// 5 - game begins
void play_music(int menu_music){
	
	// turn rgb led off after a button is pressed
	digitalWrite(pin_rgb_blue, LOW);
    digitalWrite(pin_rgb_red, LOW);
    digitalWrite(pin_rgb_green, LOW);
 
 	// play the music
 	// we play each note from the corresponding array with the melody 
 	// for <noteDuration> seconds ( note duration )
 	// and wait for <noteDuration*1.2> seconds ( music pace )
	if(menu_music==1){
	  for (int Note = 0; Note <5; Note++) {
	    tone(buzzer, melody_correct[Note],noteDuration); //Play note
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	}else if(menu_music==2){
	  for (int Note = 0; Note <5; Note++) {
	    tone(buzzer, melody_fault[Note],noteDuration); //Play note
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	}else if(menu_music==3){
	  for (int Note = 0; Note <8; Note++) {
	    tone(buzzer, melody_tension[Note],noteDuration); //Play note
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	}else if(menu_music==4){
	  for (int Note = 0; Note <64; Note++) {
	    tone(buzzer, melody_outro[Note],noteDuration); //Play note
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	}else if(menu_music==5){
	  for (int Note = 0; Note <64; Note++) {
	    tone(buzzer, melody_intro[Note],noteDuration); //Play note
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	}
	
}
