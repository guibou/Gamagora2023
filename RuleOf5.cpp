#include<string>
#include <iostream>
#include<vector>
#include<memory>

struct Foo
{
    // Constructeur
    Foo(){
        theInt = 20;
        thePointer = std::make_shared<int>(10);
    }

    /*
    // Destructeur
    ~Foo()
    {
        delete thePointer;
    }

    // Constructeur par copie
    Foo(Foo &otherFoo)
    {
        theInt = otherFoo.theInt;
        thePointer = new int(*otherFoo.thePointer);
    }

    Foo& operator=(Foo &otherFoo)
    {
        theInt = otherFoo.theInt;
        thePointer = new int(*otherFoo.thePointer);

        return *this;
    }
    */

    int theInt;
    std::shared_ptr<int> thePointer;
};

struct Sphere
{
    float cx, cy, cz, r;
};



int main()
{
    Foo a;
    {
       Foo b(a);

       std::cout << "yo" << std::endl;
    }

    Foo c;

    a = c;

    std::cout << *a.thePointer << std::endl;

    std::cout << "plait" << std::endl;
    return 0;
}

// TODO: unique_ptr / shared_ptr

/*
int main()
{
    char arr[8];
    int i = 0;

    // I est forcement 1 <+ i < 9
    while(i <= 8)
    {
        // Compilo: JE *SAIS*
        // que 0 <= i < 8
        arr[i] = 0;
        i++;
        // Compilo: JE *SAIS*
        // que 1 <= i < 9
    }
}
*/

/*
 *
 * RULE OF 3:
 *
 * Vous devez implementer:
 *   - Le constructeur par copie
 *   - Le destructeur
 *   - L'operateur =
 *
 * Si vous implementez l'un.
 */

// Valable jusqu'en 2011
//
// 2011: rule of 5
//
// Il faut aussi implementer:
//   - Le constructeur par deplacement
//   - L'operateur = par déplacement

// RULE OF 0:
// On implement AUCUN de ces trucs.


{
    ...

    delete a;
    ...

    *a;


    ...

    ...

    if(a)
    {
         ...
    }
}
