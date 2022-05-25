#include <iostream>
#include <string>
#include <memory>

struct Manipulator {
    double xPos = 0;
    double yPos = 0;
    double zPos = 0;

    Manipulator(double setXPos, double setYPos, double setZPos) : xPos(setXPos), yPos(setYPos), zPos(setZPos) {}

    Manipulator() {};

    void moveX(double xChange) {
        xPos += xChange;
    }
    void moveY(double yChange) {
        yPos += yChange;
    }
    void moveZ(double zChange) {
        zPos += zChange;
    }

    void outCoords() {
        std::cout << xPos << " " << yPos << " " << zPos << "\n";
    }
};

bool parseFragment(std::string& codeFragment, Manipulator& manip) {
    std::string xCode = "";
    std::string yCode = "";
    std::string zCode = "";

    std::string* currentCode = new std::string("");
    char codeSymb;

    for (int coordinate = 0; coordinate < 3; coordinate++) {

        switch (coordinate) {
        case 0:
            currentCode = &xCode;
            codeSymb = 'x';
            break;
        case 1:
            currentCode = &yCode;
            codeSymb = 'y';
            break;
        case 2:
            currentCode = &zCode;
            codeSymb = 'z';
            break;
        default:
            break;
        }
        


        for (int i = 0; i < codeFragment.size(); i++) {
            if (codeFragment[i] == codeSymb || codeFragment[i] == int(codeSymb) - 32) {
                for (int x = i + 1; x < codeFragment.size(); x++) {
                    if (codeFragment[x] != ' ' && codeFragment[x] != ';') {
                        currentCode->push_back(codeFragment[x]);
                        continue;
                    }
                    break;
                }
            }
            if (std::string("0123456789.-; xXyYzZ").find(codeFragment.at(i)) == std::string::npos) {
                manip.outCoords();
                std::cout << "ERROR SCRIPT\n";
                return 1;
            }
        }
        
        if (*currentCode == "") {
            continue;
        }

        double coordMove = 0;
        bool isNegative = 0;

        for (int i = 0; i < currentCode->size(); i++) {
            if (currentCode->at(i) == '-') {
                if (isNegative == 1) {
                    manip.outCoords();
                    std::cout << "ERROR SCRIPT\n";
                    return 1;
                }
                isNegative = 1;
            }
            
            else if (std::string("0123456789").find(currentCode->at(i)) != std::string::npos) {
                coordMove = coordMove * 10 + (currentCode->at(i) - 48);
            }

            else if (currentCode->at(i) == '.') {
                for (int x = i + 1; x < currentCode->size(); x++) {
                    if (std::string("0123456789").find(currentCode->at(x)) != std::string::npos) {
                        coordMove = coordMove + static_cast<double>(currentCode->at(x) - 48) / 10;
                        continue;
                    }
                    manip.outCoords();
                    std::cout << "ERROR SCRIPT\n";
                    return 1;
                }
            }

            else {
                manip.outCoords();
                std::cout << "ERROR SCRIPT\n";
                return 1;
            }
        }

        if (coordMove == 0) {
            continue;
        }

        if (isNegative == 1) {
            coordMove *= -1;
        }

        switch (coordinate) {
        case 0:
            manip.moveX(coordMove);
            break;
        case 1:
            manip.moveY(coordMove);
            break;
        case 2:
            manip.moveZ(coordMove);
            break;
        default:
            break;
        }
    }

    return 0;
}

void parseString(std::string& codeString, Manipulator& manip) {
    std::string stringFragment = "";
    for (int i = 0; i <= codeString.size(); i++) {
        if (codeString[i] != '\n' && i != codeString.size()) {
            stringFragment.push_back(codeString[i]);
        }
        else {
            if (parseFragment(stringFragment, manip) == 1) {
                return;
            }
            stringFragment = "";
        }
    }
    manip.outCoords();
}

int main()
{
    Manipulator manip;
    std::cin >> manip.xPos >> manip.yPos >> manip.zPos;

    std::string codeString{ "X10;Y20;Z30;\nX40;Y-20;Z20;\nX-10;Y-20;Z-10;"}; //{"X10;Y20;Z30;\nX40;Y-20;<Z20;\nX-10;>20;Z-10;"}
    parseString(codeString, manip);
}