#ifndef Physics_List_hh
#define Physics_List_hh

#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
    virtual ~PhysicsList();

    virtual void ConstructParticles();
    virtual void ConstructProcess();  
}; 

#endif