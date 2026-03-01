#include <cstring>

bool hasNearbyPixel(int r, int c, bool pixels[7][7]) {
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
      int nr = r + dr;
      int nc = c + dc;

      if (nr >= 0 && nr < 7 && nc >= 0 && nc < 7) {
        if (pixels[nr][nc] == 1) return true;
      }
    }
  }
  return false;
}


int nearbyScore(int r, int c, bool pixels[7][7]) {
    // Perfect match
    if (pixels[r][c] == 1) return 3;

    // Check neighbors (excluding center)
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;  // skip center

            int nr = r + dr;
            int nc = c + dc;

            if (nr >= 0 && nr < 7 && nc >= 0 && nc < 7) {
                if (pixels[nr][nc] == 1) return 2; // nearby match
            }
        }
    }

    // No match at all
    return 0;
}

void cleanNoise(bool pixels[7][7]) {
  bool temp[7][7] = {0};

  for (int r = 0; r < 7; r++) {
    for (int c = 0; c < 7; c++) {

      int neighbors = 0;

      for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
          int nr = r + dr;
          int nc = c + dc;

          if (nr>=0 && nr<7 && nc>=0 && nc<7) {
            if (pixels[nr][nc]) neighbors++;
          }
        }


      if (pixels[r][c] && neighbors >= 3) {
        temp[r][c] = 1;
      }
    }
  }

  memcpy(pixels, temp, sizeof(pixels));
}
