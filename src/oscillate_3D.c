#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH  5
#define HEIGHT 5
#define MAX_LED_HEIGHT 64

//3rd dimension comes from integers values of 2D matrix
int matrix_1D[WIDTH];
int final_mtx[WIDTH][HEIGHT];
int pointer_mtx[WIDTH][HEIGHT]; //points to index of matrix_1D

int test_mtx[WIDTH][HEIGHT];

double angle_mtx[WIDTH/2][HEIGHT/2];

void printMtx(int matrix[WIDTH][HEIGHT]);

int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}

int getCenterIndexRotated() {
  return pointer_mtx[WIDTH/2][HEIGHT/2];
}

void calc_phi() {
  printf("\n");
  printf("-----------------------------ANGLE MATRIX------------------------------\n\n");
  for (int x = 0; x < WIDTH/2; x++) {
    for (int y = 0; y < HEIGHT/2; y++) {
      double atan_in = (double)(x)/(double)(y);
      angle_mtx[x][y] = atan(atan_in);//*180.0/3.14;
    }
  }
  angle_mtx[0][0] = 0.0;
  for (int y = HEIGHT/2-1; y >= 0; y--) {
    for (int x = 0; x < WIDTH/2; x++) {
      printf("angle[%d][%d]: %f | ", x, y, angle_mtx[x][y]*180/3.14); //print in degrees
    }
    printf("\n");
  }
  printf("-----------------------------------------------------------------------\n");

}

double trace_rotation(int x, int y, double phi) {
  double index = (double)(x)*cos(phi) + (double)(y)*sin(phi);

  return index;
}

// update 3D pointer mtx to correspond to 2D index, only do Q1 and then call copy_quadrants to reduce computation time
int cast_3D_pointers() {
  for (int y = HEIGHT-1; y >= 0; y--) {
    for (int x = 0; x < WIDTH; x++) {
      int test_idx = (int)trace_rotation(x, y, angle_mtx[x][y]);
      printf("IDX: %d ", test_idx);
      pointer_mtx[x][y] = test_idx;
    }
    printf("\n");
  }
}

int update_1D_height() {
  int midpoint = getCenterIndexRotated();
  for (int i = midpoint-1; i >= 0; i--) {
    if (abs(matrix_1D[i+1] - matrix_1D[i]) == 1) {
       matrix_1D[i] =  matrix_1D[i+1];
    } else {
      matrix_1D[i] = matrix_1D[i] + ((matrix_1D[i+1] - matrix_1D[i]) / 2);
    }
  }
  for (int i = midpoint+1; i <= WIDTH; i++) {
    if (abs(matrix_1D[i-1] - matrix_1D[i]) == 1) {
       matrix_1D[i] =  matrix_1D[i-1];
    } else {
      matrix_1D[i] = matrix_1D[i] + ((matrix_1D[i-1] - matrix_1D[i]) / 2);
    }
  }
}

void copy_quadrants() {
  //Q4
  for (int x = 0; x < WIDTH/2; x++) {
    for (int y = 0; y < HEIGHT/2; y++) {
      pointer_mtx[WIDTH-1-x][y] = pointer_mtx[x][y];
    }
  }

  //Q0 & Q1
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT/2; y++) {
      pointer_mtx[x][HEIGHT-1-y] = pointer_mtx[x][y];
    }
  }

  printf("---------------------ROTATION_COPY-----------------------\n");
  printMtx(pointer_mtx);
  printf("---------------------END_COPY-----------------------------\n\n\n");
}

void printMtx(int matrix[WIDTH][HEIGHT]) {
  for (int y = HEIGHT-1; y >= 0; y--) {
    for (int x = 0; x < WIDTH; x++) {
      //printf("[%2d][%2d]:" x, y);
      printf("%2d ", matrix[x][y]);
    }
    printf("\n");
  }
  printf("\n");
}

void update_final_matrix() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      final_mtx[x][y] = matrix_1D[pointer_mtx[x][y]];
    }
  }
}

int main() {
// initialize before infinite loop
  calc_phi();
  cast_3D_pointers();
  copy_quadrants();

  int midpoint_1D = getCenterIndexRotated();

  printf("MIDPOINT 1D: %d\n\n", midpoint_1D);
  for (int i = 0; i < WIDTH; i++) {
    matrix_1D[i] = 0;
  }
  matrix_1D[midpoint_1D] = MAX_LED_HEIGHT;

  update_1D_height();

  printf("\nPOINTERS\n");
  printMtx(pointer_mtx);
  int iter_count = 0;
  int direction = 1;
  //matrix loop
  while(1) {
    msleep(50);
    iter_count +=1;
    update_1D_height();
    update_final_matrix();
    printMtx(final_mtx);

    if (iter_count == 20) {
      if (direction == 1) {
        direction = 0;
        matrix_1D[midpoint_1D] = 0;
      } else {
        direction = 1;
        matrix_1D[midpoint_1D] = MAX_LED_HEIGHT;
      }

      iter_count = 0;
    }
  }
}

main();
