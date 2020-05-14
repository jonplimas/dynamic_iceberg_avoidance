///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
  // Solve the iceberg avoiding problem for the given grid, using an exhaustive
  // optimization algorithm.
  //
  // This algorithm is expected to run in exponential time, so the grid's
  // width+height must be small enough to fit in a 64-bit int; this is enforced
  // with an assertion.
  //
  // The grid must be non-empty.
  unsigned int iceberg_avoiding_exhaustive(const grid& setting) {

    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

    // Compute the path length, and check that it is legal.
    const size_t steps = setting.rows() + setting.columns() - 2;
    assert(steps < 64);

    unsigned int count_paths = 0;
    bool valid_step; // used to check validity of each step
    step_direction direction; // used to indicate direction of each step

    // First for loop: to generate every possible path
    for (unsigned int bits = 0; bits <= pow(2,steps)-1; bits++) {
      // candidate path initialization
      path candidate(setting);
      // Second for loop: to iterate through every step of a path
      for(unsigned int k = 0; k <= steps-1; k++) {
        // bit to be parsed from the current bit-string path
        auto bit = (bits>>k) & 1;
        
        // bit determines the direction of the step
        if(bit == 1) {
          direction = STEP_DIRECTION_RIGHT;
        } else {
          direction = STEP_DIRECTION_DOWN;
        }

        // check if step is valid and if so then increment count_paths
        if(candidate.is_step_valid(direction)) {
          candidate.add_step(direction);
          valid_step = true;
        } else {
          valid = false;
          break;
        }
      }
      // if every step in this path is valid, then add path.
      if(valid_step == true) {
        count_paths++;
      }
    }
    return count_paths;
  }

  // Solve the iceberg avoiding problem for the given grid, using a dynamic
  // programming algorithm.
  //
  // The grid must be non-empty.
  unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);


    const int DIM=100;
    std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));

    A[0][0] = 1;

    unsigned int none = 0;
    unsigned int myAbove;
    unsigned int myLeft;

    // Nested for loop to check validity of coordinates
    // First For loop: Rows
    for(unsigned int i = 0; i <= setting.rows()-1; i++) {
      // Second For loop: Columns
      for(unsigned int j = 0; j <= setting.columns()-1; j++) {
        // Check coordinate validity at every step,
        // Set array A to none & move on
          if(setting.may_step(i,j) == false ) {
            A[i][j] = none;
            continue;
          }
          myAbove = none;
          myLeft = none;

          // skip it if both indices are equal to 0
          if(i== 0 && j == 0) {
            continue;
          }
          // otherwise continue
          else {
            if(i > 0 && A[i-1][j] != none) {
              myAbove = A[i-1][j];
            }
            if(j > 0 && A[i][j-1] != none) {
              myLeft = A[i][j-1];
            }
            
            A[i][j] = (myAbove + myLeft);

            if(myAbove == none && myLeft == none) {
              A[i][j] = none;
            }
          }
        }
      }
      //return the ending coordinate of the valid path.
      return A[setting.rows()-1][setting.columns()-1];
    }
}
