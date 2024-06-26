#include "asaed/constants.hpp"

// It's 64 fps (frame per second)
// Why 64 fps and not another number?
// 30 is quite good but not enough, 60 is enough good but not beautiful
// So 64 is best option (2^6)
const float LOGICAL_FPS = 64.0f;

// Do not change!!!
// I only use this size for all floor
const float BLOCK_SIZE = 16.0f;

// SHIFT_DELTA (??). It exists because sometimes collision shouldn't exist
const float SHIFT_DELTA = 2.0f;

// A small value to help collision detection
const float EPSILON = 0.005f;