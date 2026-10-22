//https://en.wikipedia.org/wiki/Discrete_Laplace_operator 2D filter section (non-seperable convolution)
//actually nvm it might be for the 5 point convolution

#define height 20
#define width 20
#define mediumradius 8

#include <stdio.h>
#include <stdlib.h>

void XConvolve(double *t, double *b, double *d){ // horizontal convolution [1,-2,1]
	for(int y = 0; y < height; y++){
		t[y*width] = d[y*width]*(b[y*width + 1] - b[y*width]);// guess as to what to do at a boundary
		t[y*width + width - 1] = d[y*width + width - 1]*(b[y*width + width - 2] - b[y*width + width - 1]);// guess as to what to do at a boundary
		for(int x = 1; x < width-1; x++){
			t[y*width + x] = d[y*width + x]*(b[y*width + x - 1] + b[y*width + x + 1] - 2.0*b[y*width + x]);
		}
	}
}

void YConvolve(double *t, double *b, double *d){ // horizontal convolution [1,-2,1]
	for(int x = 0; x < width; x++){
		t[x] = d[x]*(b[x + width] - b[x]);
		t[x + (height - 1)*width] = d[x + (height - 1)*width]*(b[x + (height - 2)*width] - b[x + (height - 1)*width]);
		for(int y = 1; y < height-1; y++){
			t[y*width + x] = d[y*width + x]*(b[y*width + x - width] + b[y*width + x + width] - 2.0*b[y*width + x]);
		}
	}
}

int main(int argc, char **argv){
	double *temperatureArray = calloc(width*height, sizeof(double));
	double *temperatureBackArray = calloc(width*height, sizeof(double)); // back buffer
	double *DArray = calloc(width*height, sizeof(double)); // array of thermal diffusivities
	// construct circle of a different medium
	for(int i = 0; i < width*height; i++){
		int x = i % width - width/2;
		int y = i / width - height/2;
		printf("%d %d\n", x, y);
		if(x*x + y*y < mediumradius*mediumradius){
			DArray[i] = 0.09;
		} else {
			DArray[i] = 0.03;
		}
	}
	//display this
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			printf("%0.1f ", DArray[x+y*width]);
		}
		printf("\n");
	}
	//set a test temperature
	temperatureArray[width*height/2 + width/2] = 10.0;
	//perform a step (horizontal convolution)
	XConvolve(temperatureBackArray, temperatureArray, DArray);
	//perform a step (Vertical convolution)
	YConvolve(temperatureArray, temperatureBackArray, DArray);
	/*//swap buffers
	double *temp = temperatureArray;
	temperatureArray = temperatureBackArray;
	temperatureBackArray = temp;*/
	// no need to swap buffers since the two convolutions already do that
	//print temperatures
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			printf("%0.1f ", temperatureArray[x+y*width]);
		}
		printf("\n");
	}
	// continue this
	while(1){
		XConvolve(temperatureBackArray, temperatureArray, DArray);
		YConvolve(temperatureArray, temperatureBackArray, DArray);
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				printf("%1.0f ", temperatureArray[x+y*width]);
			}
			printf("\n");
		}
		getchar();
	}
	return 0;
}

