
#include "AneuriskNavierStokes.hpp"

using namespace LifeV;

// Do not edit
int main(int argc, char **argv)
{
	using namespace LifeV;
#ifdef HAVE_MPI
	MPI_Init(&argc, &argv);
	std::cout<< "MPI Initialization\n";
#endif

    boost::scoped_ptr<AneuriskNavierStokes> aneurisk( new AneuriskNavierStokes( argc, argv ) );
    aneurisk->run();
    aneurisk.reset();

#ifdef HAVE_MPI
    MPI_Finalize();
    std::cout<< "MPI Finalization \n";
#endif

	return EXIT_SUCCESS;
}
