#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class Cache {
    private:
        int cacheSize;
        int cacheAssociativity;
        int *cacheArray;
        int *mostRecent;
        int modVal;

        void initializeCache(int *cArray, int cSize) {
            for (int i = 0; i < cSize; i++) {
                *(cArray + i) = -1;
            }
        }

        bool cacheFull() {
            bool cFull = true;

            for (int i = 0; i < cacheSize; i++) {
                if (*(cacheArray + i) == -1) {
                    cFull = false;
                    return cFull;
                }
            }

            return cFull;
        }

    public:
        Cache(int s, int a) {
            cacheSize = s;
            cacheAssociativity = a;
            modVal = cacheSize/cacheAssociativity;
            cacheArray = new int[cacheSize];
            mostRecent = new int[modVal];
            initializeCache(cacheArray, cacheSize);
            initializeCache(mostRecent, modVal);
        }

        string updateCache(int r) {
            string missType = "";
            *(mostRecent + (r % modVal)) = (*(mostRecent + (r % modVal)) + 1) % cacheAssociativity;
            int baseAddress = r % modVal;
            int nextSet = *(mostRecent + (r % modVal));
            int offset = (baseAddress + (cacheAssociativity * nextSet));

            if (*(cacheArray + offset) == -1) {
                missType = "COMPULSORY";
            }

            *(cacheArray + offset) = r;

            if (cacheFull()) {
                missType = "CAPACITY";
            }
            else if(missType != "COMPULSORY") {
                missType = "CONFLICT";
            }

            return missType;
        }

        bool hit(int r) {
            bool valuePresent = false;
            int startAddress = r % modVal;

            for (int i = startAddress; i < cacheSize; i = i + cacheAssociativity) {
                if (*(cacheArray + i) == r) {
                    valuePresent = true;
                    break;
                }
            }

            return valuePresent;
        }
};

int main(int argc, char *argv[]) {
    int cacheSize = atoi(argv[1]);
    int cacheAssoc = atoi(argv[2]);
    int refVal;
    string missType;
    string fileInput = argv[3];
    Cache myCache(cacheSize, cacheAssoc);
    ifstream refFile(fileInput);
    ofstream outFile("cache_sim_output.txt");

    if (!refFile) {
        cerr << "Reference file not found!" << endl;
        return 1;
    }

    while (refFile >> refVal) {
        if(myCache.hit(refVal)) {
            outFile << refVal << " : HIT" << endl;
        }
        else {
            missType = myCache.updateCache(refVal);
            outFile << refVal << " : MISS (" << missType << ")" << endl;
        }
    }

    cout << "Program complete" << endl;

    outFile.close();


    return 0;

}
