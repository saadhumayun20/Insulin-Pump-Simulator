#ifndef CARTRIDGE_H
#define CARTRIDGE_H

class Cartridge{
public:
    explicit Cartridge(float capacity = 300.0f);

    void refill();
    //void primeInfusionSet();

    //getter
    float getCurrentLevel() const;

private:
    const float capacity;
    float currentLevel;
};

#endif // CARTRIDGE_H
