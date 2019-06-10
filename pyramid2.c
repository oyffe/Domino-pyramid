#include <stdio.h>

#define NUM_BRICKS 6
#define BIT_ON(b, i) ((i) & (1 << (b)))

void swap(char *loc1, char *loc2) {
    int temp = *loc1;
    *loc1 = *loc2;
    *loc2 = temp;
}

void generateSwapedDominosBySwapIdx(int idx, int* swapedDominos) {
    for (int brick = 0; brick < NUM_BRICKS; ++brick) {
        if (BIT_ON(brick, idx)) {
            // swap in a cool way just for fun.
            //this will work only if we are shore we are not performing swap of an element on itself
            // which is true in this case..
            swapedDominos[brick * 2] ^= swapedDominos[brick * 2 + 1];
            swapedDominos[brick * 2 + 1] ^= swapedDominos[brick * 2];
            swapedDominos[brick * 2] ^= swapedDominos[brick * 2 + 1];
        }
    }
}

void generateDominosArrByConfig(int* destArr, const int* dominos, char *config) {
    for (int brick = 0; brick < NUM_BRICKS; ++brick) {
        int newBrickIdx = (config[brick] - 'a') * 2;
        destArr[newBrickIdx] = dominos[brick * 2];
        destArr[newBrickIdx + 1] = dominos[brick * 2 + 1];
    }
}

int checkSwapedPyramid(int *pyramid) {
    if (pyramid[1] == pyramid[6] && pyramid[2] == pyramid[7] && pyramid[7] == pyramid[10]
        && pyramid[11] == pyramid[8] && pyramid[8] == pyramid[3] && pyramid[9] == pyramid[4]) {
            return 1;
        }
        return 0;
}

int checkConfig(const int* dominos, char *config) {
    // there are 2^6 = 64 ways to turn the bricks 180 deg.
    // a number between 0 and 63 can be represented by 6 bits
    // each bit number i represents whether we should turn brick i 180 deg.
    for (int idx = 0; idx < 64; ++idx) {
        int swapedDominos[NUM_BRICKS * 2] = {};
        // Place all bricks according to the config
        // after this stage the first brick in the arr will be in loc 'a'
        // the second in loc 'b' and so on.
        generateDominosArrByConfig(&swapedDominos[0], dominos, config);
        // swap some of the bricks (turn them 180 degrees)
        generateSwapedDominosBySwapIdx(idx, &swapedDominos[0]);
        //check if this pyramid is correct
        if (1 == checkSwapedPyramid(&swapedDominos[0])) {
            return 1;
        }
    }
    
    return 0;
}

int findIdxNext(char *g_config, int curIdx) {
    char curMin = 'z';
    int curMinIdx = -1;
    
    for (int idx = curIdx + 1; idx < NUM_BRICKS; ++idx) {
        if (curMin > g_config[idx] && g_config[idx] > g_config[curIdx]) {
            curMin = g_config[idx];
            curMinIdx = idx;
        }
    }
    return curMinIdx;
}

// just naive bubbleSort
void sort(char* g_config, int curIdx) {
    int i, j;
    int n = NUM_BRICKS - curIdx;
    for (i = 0; i < n-1; i++) {       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (g_config[j + curIdx] > g_config[j + 1 + curIdx]) 
              swap(&g_config[j + curIdx], &g_config[j + 1 + curIdx]); 
    }
}

// Recursive function to check all permitations of possible brick locations in the pyramid
// We will check all permitations from curIdx onwards,
// fixing the locations of the bricks before curIdx.
int tryAllPerm(const int* dominos, char* g_config, int curIdx) {
    // stop conditon, idx is in the end of the configuration.
    // meaning that all locations are fixed
    if (curIdx == NUM_BRICKS - 1) {
        return 0;
    }
    
    int nextIdx = 0;
    // every iteration we will look for the next possible place
    // to put the brick at curIdx
    while (nextIdx != -1) {
        
        // before making any changes to the config check if it solves the problem
        if (1 == checkConfig(dominos, g_config)) {
                return 1;
        }
        
        // fix the first curIdx bricks locations and try all possible config.
        if (1 == tryAllPerm(dominos, g_config, curIdx + 1)) {
            return 1;
        }
        
        // search for the next locations (in abc order) we can put the curIdx brick in
        // if we tried all such locations the function will return -1
        nextIdx = findIdxNext(g_config, curIdx);
        if (nextIdx != -1) {
            swap(&g_config[curIdx], &g_config[nextIdx]);
            // Sort is nesseary. Since we are using alphabetical order,
            // we want to ensure the the recursive call
            // will start with the same start condition as the root call.
            sort(g_config, curIdx + 1);
            nextIdx = findIdxNext(g_config, curIdx);
        }
    }
    
    // if we got here we tried all possible locations for brick numbered curIdx
    // except the last one..
    if (1 == tryAllPerm(dominos, g_config, curIdx + 1)) {
            return 1;
    }
    
    // after we messed up all the config string we are left with the last one to check.
    // we could put this check in the stop condition as well
    if (1 == checkConfig(dominos, g_config)) {
                return 1;
    }
    
    return 0;
}

char* isPyramid(const int* dominos) {
    // the config represents a way to place the bricks in the pyramid 
    // the config is ignoring the possibility of swaping the bricks.
    // every char in the string represents one of six possible locations in the pyramid like so:
    //    |f|
    //  |d||e|
    // |a||b||c|
    char startConfig[NUM_BRICKS + 1] = "abcdef";
    if (1 == tryAllPerm(dominos, &startConfig[0], 0)) {
        return "YES";
    }
    return "NO";
}

int main()
{
    const int dominos[NUM_BRICKS * 2 + 1] = {2,2 , 3,2 , 3,4 , 5,4 , 3,4 , 6,6};
    printf("%s\n", isPyramid(&dominos[0]));
    
    const int dominos2[NUM_BRICKS * 2 + 1] = {1,2 , 1,2 , 1,1 , 1,1 , 1,1 , 1,1};
    printf("%s\n", isPyramid(&dominos2[0]));

    return 0;
}