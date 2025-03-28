#ifndef INFUSIONSET_H
#define INFUSIONSET_H

class InfusionSet{
public:
    InfusionSet();

    bool checkOcclusion();

    //getter
    bool getIsOcculuded() const;

private:
    bool isOccluded;
};

#endif // INFUSIONSET_H
