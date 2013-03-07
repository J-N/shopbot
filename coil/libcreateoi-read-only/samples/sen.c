/** drive.c
 *
 *  A simple program to drive the create using the keyboard. 
 *
 *  Author: Nathan Sprague
 *
 * 
 *  This file is part of COIL.
 *
 *  COIL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  COIL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with COIL.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX(a,b)	(a > b? a : b)
#define MIN(a,b)	(a < b? a : b)
#define SONG_LENGTH 8


/* Plays some random notes on the create's speakers. */
void horn() {
  char song[SONG_LENGTH * 2];
  int i;
  for (i =0; i< SONG_LENGTH; i++) {
    song[i*2] = 31 + floor(drand48() * (127-31));
    song[i*2 +1] = 8 + floor(drand48() * 10);
  }
  writeSong(0, SONG_LENGTH, song);
  playSong (0);
}

void printSensors()
{
        int* sensors = getAllSensors();
        int i;

        for (i = 0; i < 36; i++)
        {
                printf ("Sensor %d:\t%d\n", i, sensors[i]);
        }
        free (sensors);
}


int main(int argv, char* argc[])
{
	int key;
	int not_done = 1;


	int speed = 0;//store user input.
	int turn = 0;
	
	int velocity = 0; //values sent to the create
	int radius = 0;

	int charge;

	if (argv < 2) {
	  fprintf(stderr, "Usage: drive DEVICE (e.g. /dev/ttyUSB0)\n");
	  exit(1);
	}
	
	 startOI_MT (argc[1]);	
	//drive(velocity, radius);
        int left, right, scans, ii;
	scans = 20;
	for (ii=0; ii<scans; ii++)
	{
	//printSensors();		
	int* sensors = getAllSensors();
	//printf("front left cliff sensor %d\n",sensors[33]);
	//printf("front right cliff sensor %d\n",sensors[34]);
	
	usleep(1000000);
	}
}
