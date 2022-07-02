#include <array>
#include <vector>
#include <iostream>
#include <omp.h>
#include "constants.hpp"
#include "pngwriter.h"

using namespace std;

// TODO make mandelbrot device function
#pragma omp declare target
int kernel(int xi, int yi);
#pragma omp end declare target

int main() {

  std::vector<int> image(width * height);
  int num_blocks = 8;
  int block_size = (height / num_blocks) * width;
  int y_block_size = height / num_blocks;

  double st = omp_get_wtime();

  // TODO start: offload the calculation according to assignment

  #pragma omp target enter data map(to:image[0:width*height]) map(to:num_blocks,y_block_size)

  #pragma omp target update from(image)
  for(int block = 0; block < num_blocks; block++ ) {
    int y_start = block * y_block_size;
    int y_end = y_start + y_block_size;

    #pragma omp target teams distribute
    for (int y = y_start; y < y_end; y++) {
      #pragma omp parallel for
      for (int x = 0; x < width; x++) {
        int ind = y * width + x;
        image[ind] = kernel(x, y);
      }
    }


  #pragma omp target exit data map(from:image[0:width*height])

  }

  // TODO end

  double et = omp_get_wtime();

  cout << "Time: " << (et - st) << " seconds" << endl;
  int *image_ptr = image.data();
  save_png(image_ptr, width, height, "mandelbrot.png");
}
