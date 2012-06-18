
#include <lifev/ecm2/navier_stokes/SingleInflowBloodFlowNSApplication.hpp>

using namespace LifeV;

// Do not edit
int main(int argc, char **argv)
{
	using namespace LifeV;
#ifdef HAVE_MPI
	MPI_Init(&argc, &argv);
	std::cout<< "MPI Initialization\n";
#endif

    boost::scoped_ptr<SingleInflowBloodFlowNSApplication> singleInflowNSApp(
                    new SingleInflowBloodFlowNSApplication( argc, argv ) );
    singleInflowNSApp->run();
    singleInflowNSApp.reset();

#ifdef HAVE_MPI
    MPI_Finalize();
    std::cout<< "MPI Finalization \n";
#endif

	return EXIT_SUCCESS;
}
