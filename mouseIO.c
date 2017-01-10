/* Atari Mouse GPIO
  Andrew Armstrong January 2017
  v0.1

  Changenotes:

  Sorry for the mess, I hacked this together using nano over puTTY!
  I was messing around with adding a velocity sensing thing, but 
  have technically disabled it by overriding its calcs.

  Im using the WiringPi GPIO library to simplify things.

*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#define MOUSEFILE "/dev/input/mouse0"
//
int main()
{
    int fd;
    int i;
    struct input_event ie;
    int ourDelayX=1000;
    int ourDelayY=1000;
    int velocityX=3;
    int velocityY=3;
    //
    wiringPiSetup () ;
    pinMode (29, OUTPUT) ;
    pinMode (28, OUTPUT) ;
    pinMode (26, OUTPUT) ;
    pinMode (25, OUTPUT) ;
    pinMode (24, OUTPUT) ;
    pinMode (23, OUTPUT) ;

    //
    unsigned char button,bLeft,bMiddle,bRight;
    signed char x,y;
    signed char oldx, oldy;
    int diffX, diffY;
    unsigned char oldL, oldR;

    if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
        printf("Device open ERROR\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Device open OK\n");
    }

    while(read(fd, &ie, sizeof(struct input_event)))
    {
        unsigned char *ptr = (unsigned char*)&ie;

        button=ptr[0];
        bLeft = button & 0x1;
        bMiddle = ( button & 0x4 ) > 0;
        bRight = ( button & 0x2 ) > 0;
        x=(signed char) ptr[1];y=(signed char) ptr[2];

	diffX = abs((unsigned char) x - (unsigned char) oldx);
	diffY = abs((unsigned char) y - (unsigned char) oldy);

	if  (diffX > 15 && diffX < 250)
	{
		velocityX = 8;
		ourDelayX = 130;		
	}
	else
	{
		velocityX = 1;
		ourDelayX = 200;
	}

        if  (diffY > 15 && diffX < 250)
        {
                velocityY = 8;
                ourDelayY = 130;
        }
        else
        {
                velocityY = 1;
                ourDelayY = 200;
        }

//Overriding all the velocity stuff for testing!
velocityX = 1;
velocityY = 1;
ourDelayX = 2800;
ourDelayY = 2800;

	if (bLeft != oldL)
	{
		if (bLeft == 1)
		{	
			digitalWrite(29, LOW);
		}
		else
		{	
			digitalWrite(29, HIGH);
		}
	}
	oldL = bLeft;

	if ( bRight != oldR)
	{
	        if (bRight == 1)
	        {
	                digitalWrite(28, LOW);
	        }
	        else
	        {
	                digitalWrite(28, HIGH);
	        }
	}
	oldR = bRight;

	if (abs(y) > 1)
	{

		for (i=0; i<velocityY; i++)
		{

		if ( y < 0 )
		{
			digitalWrite (26, HIGH);		
			delayMicroseconds(ourDelayY);
    			digitalWrite (23, HIGH);
 			delayMicroseconds(ourDelayY);
			digitalWrite (26,  LOW);
 			delayMicroseconds(ourDelayY);
			digitalWrite (23,  LOW);
                        delayMicroseconds(ourDelayY);
		}
		else
		{
                        digitalWrite (23, HIGH);
			delayMicroseconds(ourDelayY);
                        digitalWrite (26, HIGH);
 			delayMicroseconds(ourDelayY);
                        digitalWrite (23,  LOW);
 			delayMicroseconds(ourDelayY);
                        digitalWrite (26,  LOW);
                        delayMicroseconds(ourDelayY);
		}
		}
	}

        if (abs(x) > 1)
        {
                for (i=0; i<velocityX; i++)
                {

                if ( x < 0 )
                {
                        digitalWrite (25, HIGH);
 			delayMicroseconds(ourDelayX);
                        digitalWrite (24, HIGH);
 			delayMicroseconds(ourDelayX);
                        digitalWrite (25,  LOW); 
			delayMicroseconds(ourDelayX);
                        digitalWrite (24,  LOW);
                        delayMicroseconds(ourDelayX);
                }
                else
                {
                        digitalWrite (24, HIGH);
 			delayMicroseconds(ourDelayX);
                        digitalWrite (25, HIGH);
 			delayMicroseconds(ourDelayX);
                        digitalWrite (24,  LOW);
 			delayMicroseconds(ourDelayX);
                        digitalWrite (25,  LOW);
			delayMicroseconds(ourDelayX);
                }
		}
        }

	oldx=x;
	oldy=y;
    }

return 0;
}
