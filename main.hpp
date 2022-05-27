#include "stb_image.h"
#include "geneticbase.h"
#include "probabilistic.h"
#include "lib/cromodistribution.h"
#include "lib/individual.h"
#include "Socket_C/socketWin.cpp"
#include <iostream>
#include <ctime>
#include <time.h>
#include <bits/stdc++.h>
#include <vector>

#define TOTAL_PIXELS  10.0 //Number of pixels that I want to search for.
#define QUADRANT_QUANTITY 1600 //40 x 40
#define QUADRANT 40 // Length that each quadrant will have
#define CONTROLROW 27 //Quadrants x row
#define NUMBER_OF_QUADRANT 702 //Total quadrants 27*26
#define IMAGE_NAME "persona_3.jpg"
#define CONTROL_DARK_GRAY 180
#define SAMPLERATE 0.15
#define CONTROL_PORCENTAGE_CROMO 0.50
#define DISTRIBUTION_SIZE 702 